;;;; Sonic CD's D.A. Garden
;;;; Powered by Common Lisp, SDL2, CFFI and OpenGL
;;;;
;;;; Written by Lucas Vieira <lucas.samuel2002@gmail.com>
;;;; under the MIT License
;;;;
;;;; NOTE: Sonic The Hedgehog and all graphics are Copyright
;;;; SEGA and SONICTEAM.
;;;; I do not own any of this, and this software was written
;;;; with the sole purpose of STUDY.
;;;;
;;;; THIS SHOULD NOT BE SOLD.

;;; Controls:
;;; WASD  -> Move Little Planet
;;; Q/E   -> Change rotation
;;; Esc   -> Quit
;;; Start -> TO-DO
;;; Bcksp -> TO-DO


(eval-when (:compile-toplevel :load-toplevel :execute)
  (format t "===== Loading Quicklisp Libs~%")
  (finish-output)
  (ql:quickload "sdl2")
  (ql:quickload "sdl2-image")
  (ql:quickload "cl-opengl")
  (ql:quickload "bordeaux-threads")
  (format t "===== Loading symbols~%")
  (finish-output)
  (require 'sdl2)
  (require 'cl-opengl)
  (require 'bordeaux-threads))




;; Hooks for thread-related stuff
(defvar *next-frame-hook-mutex* (bt:make-lock "frame-hook-lock"))
(defparameter *next-frame-hook* nil)
 ;;; Helper macros
(defmacro restartable (&body body)
  `(restart-case
       (progn ,@body)
     (continue () :report "Continue")))
(defmacro next-frame (&body body)
  `(bt:with-lock-held (*next-frame-hook-mutex*)
     (progn (push (lambda () ,@body) *next-frame-hook*))))


;; SDL2 window parameters
(defparameter *window* nil)
(defparameter *gl-context* nil)
(defparameter *window-width* 960)
(defparameter *window-height* 540)
(defparameter *game-controllers* nil)
(defparameter *running* t)


;; Input-related
(defstruct input-values
  (up nil :type boolean)
  (down nil :type boolean)
  (left nil :type boolean)
  (right nil :type boolean)
  (start nil :type boolean)
  (back nil :type boolean)
  (rot-l nil :type boolean)
  (rot-r nil :type boolean))

;; Input info
(defparameter *input-state* (make-input-values))
(defparameter *old-input-state* (make-input-values))

(defun has-pressed (button)
  (case button
    (:btn-up
     (and (input-values-up *input-state*)
	  (not (input-values-up *old-input-state*))))
    (:btn-down
     (and (input-values-down *input-state*)
	  (not (input-values-down *old-input-state*))))
    (:btn-left
     (and (input-values-left *input-state*)
	  (not (input-values-left *old-input-state*))))
    (:btn-right
     (and (input-values-right *input-state*)
	  (not (input-values-right *old-input-state*))))
    (:btn-start
     (and (input-values-start *input-state*)
	  (not (input-values-start *old-input-state*))))
    (:btn-back
     (and (input-values-back *input-state*)
	  (not (input-values-back *old-input-state*))))
    (:btn-rot-l
     (and (input-values-rot-l *input-state*)
	  (not (input-values-rot-l *old-input-state*))))
    (:btn-rot-r
     (and (input-values-rot-r *input-state*)
	  (not (input-values-rot-r *old-input-state*))))))




;; Framerate Stuff
(defparameter *frame-count* 0)
(defparameter *last-fps-message-time* 0)
(defparameter *last-fps-message-frame-count* 0)
(defparameter *fps-message-interval* 2000)
(defparameter *fps* 0)

(defmacro set-append (item mylist)
  `(setf ,mylist (append ,mylist (list ,item))))

;; Vec2
(defstruct vec2
  (x 0.0 :type short-float)
  (y 0.0 :type short-float))

;; Texture
(defstruct texture
  (name nil)
  (size (make-vec2) :type vec2))


(defun load-texture (filename)
  (let* ((surf (sdl2-image:load-image filename))
       	 (gltex (car (gl:gen-textures 1))))
    (gl:enable :texture-2d)
    (gl:bind-texture :texture-2d gltex)
    (gl:tex-parameter :texture-2d :texture-wrap-s :clamp-to-edge)
    (gl:tex-parameter :texture-2d :texture-wrap-t :clamp-to-edge)
    (gl:tex-parameter :texture-2d :texture-min-filter :nearest)
    (gl:tex-parameter :texture-2d :texture-mag-filter :nearest)
    (gl:tex-image-2d :texture-2d
		     0
		     :rgba
		     (sdl2:surface-width surf)
		     (sdl2:surface-height surf)
		     0
		     :rgba
		     :unsigned-byte
		     (sdl2:surface-pixels surf))
    (gl:finish)	     
    (gl:disable :texture-2d)
    
    (make-texture
     :name gltex
     :size (make-vec2
	    :x (float (sdl2:surface-width surf))
	    :y (float (sdl2:surface-height surf))))))


;;; DA Garden Stuff

(defclass planet ()
  ((tex :initform nil :accessor tex)
   (pos :initform (make-vec2
		   :x (/ *window-width* 2.0)
		   :y (/ *window-height* 2.0))
	:accessor pos)
   (angle :initform 0.0 :accessor angle)
   (rotation :initform -20.0
	     :accessor rotation)))


(defgeneric load-planet (planet)
  (:documentation "Loads assets for rendering the planet."))

(defgeneric unload-planet (planet)
  (:documentation "Unloads assets for the planet."))

(defgeneric update-planet (planet delta-t)
  (:documentation "Updates logic and input for the planet."))

(defgeneric draw-planet (planet)
  (:documentation "Draws the planet texture as needed."))


(defmethod load-planet ((theplanet planet))
  (setf (tex theplanet)
	(load-texture "./sprites/littleplanet.png")))

(defmethod unload-planet ((theplanet planet))
  (gl:delete-textures (list (texture-name (tex theplanet)))))

(defmethod update-planet ((theplanet planet) delta-t)
  ;; Rotate planet accordingly
  (incf (angle theplanet) (* (rotation theplanet)
			     (/ delta-t 1000.0)))
  ;; Clamp angle
  (cond ((< (angle theplanet) 0.0)
	 (incf (angle theplanet) 360.0))
	((> (angle theplanet) 360.0)
	 (decf (angle theplanet) 360.0)))

  ;; Move planet
  (when (input-values-up *input-state*)
    (decf (vec2-y (pos theplanet)) (* 300.0
				      (/ delta-t 1000.0))))
  (when (input-values-down *input-state*)
    (incf (vec2-y (pos theplanet)) (* 300.0
				      (/ delta-t 1000.0))))
  (when (input-values-left *input-state*)
    (decf (vec2-x (pos theplanet)) (* 300.0
				      (/ delta-t 1000.0))))
  (when (input-values-right *input-state*)
    (incf (vec2-x (pos theplanet)) (* 300.0
				      (/ delta-t 1000.0))))
  ;; Change rotation speed
  (when (has-pressed :btn-rot-l)
    (decf (rotation theplanet) 20.0))
  (when (has-pressed :btn-rot-r)
    (incf (rotation theplanet) 20.0)))

(defmethod draw-planet ((theplanet planet))
  (gl:enable :texture-2d)
  (gl:bind-texture :texture-2d (texture-name (tex theplanet)))
  (let ((h-w (/ (vec2-x (texture-size (tex theplanet))) 2.0))
	(h-h (/ (vec2-y (texture-size (tex theplanet))) 2.0)))
    (gl:with-pushed-matrix
      (gl:translate (vec2-x (pos theplanet))
		    (vec2-y (pos theplanet))
		    0.0)
      (gl:rotate (angle theplanet) 0.0 0.0 1.0)
      (gl:with-primitive :quads
	(gl:tex-coord 0.0 0.0)
	(gl:vertex (- h-w) (- h-h))

	(gl:tex-coord 1.0 0.0)
	(gl:vertex h-w (- h-h))

	(gl:tex-coord 1.0 1.0)
	(gl:vertex h-w h-h)

	(gl:tex-coord 0.0 1.0)
	(gl:vertex (- h-w) h-h))))
  (gl:bind-texture :texture-2d 0)
  (gl:disable :texture-2d))


(defparameter *littleplanet* (make-instance 'planet))
(defparameter *t-background* nil)
(defparameter *m-dagarden* nil)


(defun setup-view ()
  ;; Disable VSync
  ;;(sdl2:gl-set-swap-interval 0)

  ;; Setup viewport and ortographic view
  (gl:viewport 0 0 *window-width* *window-height*)
  (gl:matrix-mode :projection)
  (gl:ortho 0 *window-width* *window-height* 0 0 1)
  (gl:matrix-mode :modelview)
  (gl:load-identity)
  (gl:clear-color 0.0 0.0 0.0 1.0)
  (gl:enable :blend)
  (gl:blend-func :src-alpha :one-minus-src-alpha))


(defun init ()
  (setf *frame-count* 0)
  (setf *last-fps-message-time* (sdl2:get-ticks))

  ;; Init planet
  (load-planet *littleplanet*)
  (setf *t-background* (load-texture "./sprites/background.png"))

  

  ;; Init game controllers
  (loop for i from 0 upto (- (sdl2:joystick-count) 1)
     do (when (sdl2:game-controller-p i)
	  (let ((controller (sdl2:game-controller-open i)))
	    (setf *game-controllers* (acons i controller *game-controllers*))))))

(defun unload ()
  (unload-planet *littleplanet*)
  (gl:delete-textures (list (texture-name *t-background*))))

(defun update-fps ()
  (incf *frame-count*)
  (when (< (sdl2:get-ticks) *last-fps-message-time*)
    (setf *last-fps-message-time* (sdl2:get-ticks)))
  (when (>= (sdl2:get-ticks)
	    (+ *last-fps-message-time* *fps-message-interval*))
    (let ((frames (- *frame-count* *last-fps-message-frame-count*))
	  (seconds (/ (- (sdl2:get-ticks) *last-fps-message-time*) 1000.0)))
      (setf *fps*
	    (if (zerop seconds) -1.0 (/ frames seconds)))
      (format t "~s seconds: ~s fps, ~s ms/frame~%"
	      seconds
	      *fps*
	      (if (zerop frames) "<infinite>" (* 1000 (/ seconds frames)))))
    (setf *last-fps-message-time* (sdl2:get-ticks))
    (setf *last-fps-message-frame-count* *frame-count*)))


(defun update-input ()
  ;; Pause game
  (when (has-pressed :btn-start)
    (setf *running* (not *running*))
    (if (not *running*)
	(format t "Game Paused~%")
	(format t "Continuing game~%")))

  ;; Quit game (Esc or Back)
  (when (has-pressed :btn-back)
    (sdl2:push-quit-event))
  
  ;; Update old state after processing
  (setf *old-input-state* (copy-structure *input-state*)))


(defun update (delta-t)
  (when *running*
    (update-planet *littleplanet* delta-t))
  (update-fps)
  (update-input))


(defun draw()
  (gl:clear :color-buffer)
  ;; Draw background
  (gl:with-pushed-matrix
    (gl:translate (/ *window-width* 2.0)
		  (/ *window-height* 2.0)
		  0.0)
    (gl:enable :texture-2d)
    (gl:bind-texture :texture-2d (texture-name *t-background*))
    (gl:with-primitive :quads
      (let ((h-w (/ (vec2-x (texture-size *t-background*)) 2.0))
	    (h-h (/ (vec2-y (texture-size *t-background*)) 2.0)))
	(gl:tex-coord 0.0 0.0)
	(gl:vertex (- h-w) (- h-h))
	(gl:tex-coord 1.0 0.0)
	(gl:vertex h-w (- h-h))
	(gl:tex-coord 1.0 1.0)
	(gl:vertex h-w h-h)
	(gl:tex-coord 0.0 1.0)
	(gl:vertex (- h-w) h-h)))
    (gl:bind-texture :texture-2d 0)
    (gl:disable :texture-2d))
  
  (draw-planet *littleplanet*)
  (sdl2:gl-swap-window *window*))


(defun main-loop ()
  (sdl2:with-init (:video :joystick)
    (setf *window* (sdl2:create-window
		    :title "D.A. Garden (Lisp/SDL2/GL)"
		    :x (- 1820 (round (* *window-width* 1)))
		    :w *window-width* :h *window-height*
		    :flags '(:shown :opengl)))
    (setf *gl-context* (sdl2:gl-create-context *window*))
    (sdl2:gl-make-current *window* *gl-context*)
    (setf cl-opengl-bindings:*gl-get-proc-address*
	  #'sdl2:gl-get-proc-address)

    (let ((previous-tick (sdl2:get-ticks)))
      (setup-view)

      ;; Windows fix!
      ;; SBCL holds a "hide" window state on Windows
      ;; so the inferior-lisp doesn't show. So hiding
      ;; and showing SDL2's window will do the trick
      (sdl2:hide-window *window*)
      (sdl2:show-window *window*)

      ;; Initialize SDL2_Image
      (sdl2-image:init '(:png))

      (init)
      (sdl2:with-event-loop (:method :poll)
	(:quit () t)
	(:keyup (:keysym keysym)
		;; Arrows
		(when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-up)
		    (setf (input-values-up *input-state*) nil))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-down)
		    (setf (input-values-down *input-state*) nil))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-left)
		    (setf (input-values-left *input-state*) nil))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-right)
		    (setf (input-values-right *input-state*) nil))
		  ;; WASD
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-w)
		    (setf (input-values-up *input-state*) nil))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-s)
		    (setf (input-values-down *input-state*) nil))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-a)
		    (setf (input-values-left *input-state*) nil))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-d)
		    (setf (input-values-right *input-state*) nil))
		  ;; Enter, Esc
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-return)
		    (setf (input-values-start *input-state*) nil))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-escape)
		    (setf (input-values-back *input-state*) nil))
		  ;; Rotation
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-e)
		    (setf (input-values-rot-r *input-state*) nil))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-q)
		    (setf (input-values-rot-l *input-state*) nil)))
	
	(:keydown (:keysym keysym)
		  ;; Arrows
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-up)
		    (setf (input-values-up *input-state*) t))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-down)
		    (setf (input-values-down *input-state*) t))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-left)
		    (setf (input-values-left *input-state*) t))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-right)
		    (setf (input-values-right *input-state*) t))
		  ;; WASD
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-w)
		    (setf (input-values-up *input-state*) t))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-s)
		    (setf (input-values-down *input-state*) t))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-a)
		    (setf (input-values-left *input-state*) t))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-d)
		    (setf (input-values-right *input-state*) t))
		  ;; Enter, Esc
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-return)
		    (setf (input-values-start *input-state*) t))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-escape)
		    (setf (input-values-back *input-state*) t))
		  ;; Rotation
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-e)
		    (setf (input-values-rot-r *input-state*) t))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-q)
		    (setf (input-values-rot-l *input-state*) t)))
	
	(:joyaxismotion (:which controller-id :axis axis-id :value value)
			(when (= controller-id 0)
			  (cond ((= axis-id 0) ; Horizontal axis
				 (setf (input-values-left *input-state*) nil)
				 (setf (input-values-right *input-state*) nil)
				 (cond
				   ((<= value -32767)
				    (setf (input-values-left *input-state*) t))
				   ((>= value 32767)
				    (setf (input-values-right *input-state*) t))))
				
				((= axis-id 1) ; Vertical axis
				 (setf (input-values-up *input-state*) nil)
				 (setf (input-values-down *input-state*) nil)
				 (cond
				   ((<= value -32767)
				    (setf (input-values-up *input-state*) t))
				   ((>= value 32767)
				    (setf (input-values-down *input-state*) t)))))))


	;; todo: joypad buttons for rotation
	(:joybuttondown (:which controller-id :button button-id)
			;;(format t "Button: ~a~%" button-id)
			(when (= controller-id 0)
			  (when (= button-id 13)
			    (setf (input-values-up *input-state*) t))
			  (when (= button-id 14)
			    (setf (input-values-down *input-state*) t))
			  (when (= button-id 11)
			    (setf (input-values-left *input-state*) t))
			  (when (= button-id 12)
			    (setf (input-values-right *input-state*) t))
			  (when (= button-id 7)
			    (setf (input-values-start *input-state*) t))
			  (when (= button-id 6)
			    (setf (input-values-back *input-state*) t))))

	
	(:joybuttonup (:which controller-id :button button-id)
			(when (= controller-id 0)
			  (when (= button-id 13)
			    (setf (input-values-up *input-state*) nil))
			  (when (= button-id 14)
			    (setf (input-values-down *input-state*) nil))
			  (when (= button-id 11)
			    (setf (input-values-left *input-state*) nil))
			  (when (= button-id 12)
			    (setf (input-values-right *input-state*) nil))
			  (when (= button-id 7)
			    (setf (input-values-start *input-state*) nil))
			  (when (= button-id 6)
			    (setf (input-values-back *input-state*) nil))))

	
	(:idle ()
	       #+(and sbcl (not sb-thread))
	       (restartable (sb-sys:serve-all-events 0))

	       (let ((delta-t (- (sdl2:get-ticks) previous-tick)))
		 (setf previous-tick (sdl2:get-ticks))
		 (restartable (update (if (minusp delta-t) 0 delta-t))))
	       (restartable (draw))
	       (bt:with-lock-held (*next-frame-hook-mutex*)
		 (loop for i in *next-frame-hook*
		    do (funcall i))
		 (setf *next-frame-hook* nil))))

      (unload)

      
      ;; Delete game controllers
      (loop for (i . controller) in *game-controllers*
	 do (sdl2:game-controller-close controller))

      ;; Quit SDL2_Image
      (sdl2-image:quit)

      
      ;; Delete context and window
      (sdl2:gl-delete-context *gl-context*)
      (sdl2:destroy-window *window*))))

(main-loop)
