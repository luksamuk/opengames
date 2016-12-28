(eval-when (:compile-toplevel :load-toplevel :execute)
  (finish-output)
  (ql:quickload "sdl2")
  (ql:quickload "sdl2-image")
  (ql:quickload "cl-opengl")
  (ql:quickload "bordeaux-threads")
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
(defparameter *window-width* 500)
(defparameter *window-height* 520)
(defparameter *game-controllers* nil)
(defparameter *running* t)


;; Input-related
(defstruct input-values
  (up nil :type boolean)
  (down nil :type boolean)
  (left nil :type boolean)
  (right nil :type boolean)
  (start nil :type boolean)
  (back nil :type boolean))

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
	  (not (input-values-back *old-input-state*))))))




;; Framerate Stuff
(defparameter *frame-count* 0)
(defparameter *last-fps-message-time* 0)
(defparameter *last-fps-message-frame-count* 0)
(defparameter *fps-message-interval* 2000)
(defparameter *fps* 0)

;; Vec2
(defstruct vec2
  (x 0.0 :type short-float)
  (y 0.0 :type short-float))







;;; Actual game stuff

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

(defclass numfont ()
  ((texture
    :initform nil
    :accessor texture)
   (charsize
    :initform (make-vec2 :x 6.0 :y 11.0)
    :accessor charsize)))

(defgeneric draw-string (numfont string position color mag)
  (:documentation "Draws a string at a specific place"))

(defmethod draw-string ((thefont numfont) string position color mag)
  (gl:enable :texture-2d)
  (gl:with-pushed-matrix
    (gl:translate (vec2-x position) (vec2-y position) 0.0)
    (gl:with-pushed-matrix
      (let ((rel-pos (* (- (length string) 1) (* (vec2-x (charsize thefont)) mag))))
	(gl:translate (- (/ rel-pos 2.0)) 0.0 0.0))
      (gl:color (car color) (cadr color) (caddr color))

      (gl:bind-texture :texture-2d (texture-name (texture thefont)))
      (gl:tex-env :texture-env :texture-env-mode :modulate)
      
      (loop for c across string
	 do (let ((hw (* (/ (vec2-x (charsize thefont)) 2.0) mag))
		  (hh (* (/ (vec2-y (charsize thefont)) 2.0) mag))
		  (texelw 0.1)
		  (charpos 0.0)
		  (position 0.0))
	      
	      (setf position (- (char-code c) (char-code #\0)))
	      (setf charpos (* (float position) texelw))
	      (gl:with-primitive :quads
		(gl:tex-coord charpos 0.0)
		(gl:vertex (- hw) (- hh))
		(gl:tex-coord (+ charpos texelw) 0.0)
		(gl:vertex hw (- hh))
		(gl:tex-coord (+ charpos texelw) 1.0)
		(gl:vertex hw hh)
		(gl:tex-coord charpos 1.0)
		(gl:vertex (- hw) hh))
	      
	      (gl:translate (* hw 2.0) 0.0 0.0))))
    (gl:disable :texture-2d)))
  



(defparameter *game-array* nil)
(defvar *game-square-bg-sz* 120)
(defparameter *game-font* (make-instance 'numfont))
(defparameter *game-score* 0)
;;;(defparameter *game-squares* nil)

(defun spawn-new-block ()
  (let ((chosen nil)
	  (current nil))
      (loop while (not chosen)
	 do (setf current
		  (list (random 4)
			(random 4)))
	   (if (eq (aref *game-array*
			 (car current)
			 (cadr current))
		   nil)
	       (progn
		 (setf (aref *game-array*
			     (car current)
			     (cadr current))
		       2)
		 ;;(let ((new-sq (make-instance 'walking-square)))
		   ;; todo
		 (setf chosen t))))))

(defun init-array ()
  (setf *game-array*
	(make-array '(4 4) :initial-element nil))
  ;;(setf *game-squares* nil)
  (dotimes (n 2)
    (spawn-new-block)))

(defun draw-square (position)
  (gl:with-pushed-matrix
    (gl:translate (vec2-x position)
		  (vec2-y position)
		  0.0)
    (gl:with-primitive :quads
      (let ((hs (/ *game-square-bg-sz* 2.0)))
	(gl:vertex (- hs) (- hs))
	(gl:vertex hs (- hs))
	(gl:vertex hs hs)
	(gl:vertex (- hs) hs)))))

(defun set-square-color (value)
  (let ((initial '(1.0 0.7 0.5))
	(final   '(0.55 0.07 0.86))
	(percent (/ (- (log value 2) 1) 10.0))
	(current nil))
    (setf current (list
		   (+ (* (- (car final)
			    (car initial))
			 percent)
		      (car initial))
		   (+ (* (- (cadr final)
			    (cadr initial))
			 percent)
		      (cadr initial))
		   (+ (* (- (caddr final)
			    (caddr initial))
			 percent)
		      (caddr initial))))
    ;;(format t "Value = ~a, Percent = ~a, Color: ~a~%" value percent current)
    (gl:color (car current)
	      (cadr current)
	      (caddr current)
	      0.85)))


#|

;;; This here is commented-out because I didn't want to throw
;;; the code in the trash. But it is untested.

(defclass walking-square ()
  ((pos
    :initform (make-vec2 :x 0.0 :y 0.0)
    :accessor pos)
   (dest
    :initform (make-vec2 :x 0.0 :y 0.0)
    :accessor dest)
   (velocity
    :initform (make-vec2 :x 0.0 :y 0.0)
    :accessor velocity)
   (towards
    :initform nil
    :accessor towards)
   (moving
    :initform nil
    :accessor moving)
   (coord
    :initform '(0 0)
    :accessor coord)))

(defgeneric update-square (walking-square dt)
  (:documentation "Updates the position of a walking-square."))


(defmethod update-square ((thesquare walking-square) dt)
  ;; Update position
  (if (and (not (equal (pos thesquare)
			 (dest thesquare)))
	     (not (towards thesquare)))
      ;; Define our "Towards"
      (cond
	((< (vec2-x (pos thesquare)) (vec2-x (dest thesquare)))
	 (setf (towards thesquare) 0)) ; 0 = right
	((> (vec2-x (pos thesquare)) (vec2-x (dest thesquare)))
	 (setf (towards thesquare) 1)) ; 1 = left
	((< (vec2-y (pos thesquare)) (vec2-y (dest thesquare)))
	 (setf (towards thesquare) 2)) ; 2 = down
	((> (vec2-y (pos thesquare)) (vec2-y (dest thesquare)))
	 (setf (towards thesquare) 3))) ; 3 = up
      ;; Follow our "Towards"
      (progn
	;; First, ALWAYS transform our position
	(incf (vec2-x (pos thesquare)) (vec2-x (velocity thesquare)))
	(incf (vec2-y (pos thesquare)) (vec2-y (velocity thesquare)))
	;; Now increment our accel
	(let ((accel-mod 12.0)
	      (accel (make-vec2 :x 0.0 :y 0.0)))
	  (case (towards thesquare)
	    (1 (setf (vec2-x accel) (- accel-mod)))
	    (0 (setf (vec2-x accel) accel-mod))
	    (3 (setf (vec2-y accel) (- accel-mod)))
	    (2 (setf (vec2-y accel) accel-mod)))
	  (incf (vec2-x (velocity thesquare)) (* (vec2-x accel) dt))
	  (incf (vec2-y (velocity thesquare)) (* (vec2-y accel) dt)))
	;; Now check if we're past our destination
	(when (or (and (< (vec2-x (pos thesquare))
			  (vec2-x (dest thesquare)))
		       (eq (towards thesquare) 1))
		  (and (> (vec2-x (pos thesquare))
			  (vec2-x (dest thesquare)))
		       (eq (towards thesquare) 0))
		  (and (< (vec2-y (pos thesquare))
			  (vec2-y (dest thesquare)))
		       (eq (towards thesquare) 3))
		  (and (> (vec2-y (pos thesquare))
			  (vec2-y (dest thesquare)))
		       (eq (towards thesquare) 2)))
	  ;; Whoops we're way past where we're supposed to be
	  (setf (pos thesquare) (make-vec2 :x (vec2-x (dest thesquare))
					   :y (vec2-y (dest thesquare))))
	  (setf (towards thesquare) nil)))))
|#

(defun check-board ()
  (let ((board-condition :game-over))
    (dotimes (m 4)
      (dotimes (n 4)
	;; If scored 2048 somewhere, you won
	(when (and (not (eq (aref *game-array* m n) nil))
		   (= (aref *game-array* m n) 2048))
	  (setf board-condition :win))
	;; If you didn't win yet and there is at
	;; least one nil square, you can PU-RAY
	(when (and (not (eq board-condition :win))
		   (not (aref *game-array* m n)))
	  (setf board-condition :playing))))
    ;; However, if there are NO nil squares left, you're
    ;; out of space for spawning... so you lost the game!
    (when (not board-condition)
      (setf board-condition :game-over))
    ;; Return the condition
    board-condition))

(defun update-board ()
  ;; Check state of board
  (case (check-board)
    (:game-over
     (progn
       (format t "You lost the game!~%"))
     )
    (:win
     (progn
       (format t "You won the game!~%"))
     )
    (otherwise
     ;; Understand UP and DOWN by checking RIGHT and LEFT ok?
     (let ((spawn-flag nil))
       (when (has-pressed :btn-up)
	 (setf spawn-flag t)
	 (dotimes (n 4)
	   (loop for m from 0 to 3
	      do (when (not (eq (aref *game-array* m n) nil))
		   (loop for m2 from (+ m 1) to 3
		      do
			(when (not (eq (aref *game-array* m2 n) nil))
			  (when (= (aref *game-array* m n)
				   (aref *game-array* m2 n))
			    (incf *game-score* (* 10 (aref *game-array* m n)))
			    (setf (aref *game-array* m2 n) nil)
			    (setf (aref *game-array* m n)
				  (* (aref *game-array* m n) 2)))
			  ;;(setf m m2)
			  (setf m2 3)
			  ;;(return)
			  ))))
	   (loop for m from 0 to 3
	      do (when (eq (aref *game-array* m n) nil)
		   (loop for m2 from (+ m 1) to 3
		      do (when (not (eq (aref *game-array* m2 n) nil))
			   (format t "Swapping ~ax~a with ~ax~a~%"
				   m n
				   m2 n)
			   (setf (aref *game-array* m n)
				 (aref *game-array* m2 n))
			   (setf (aref *game-array* m2 n) nil)
			   (setf m2 3)))))))
       (when (has-pressed :btn-down)
	 (setf spawn-flag t)
	 (dotimes (n 4)
	   (loop for m from 3 downto 0
	      do (when (not (eq (aref *game-array* m n) nil))
		   (loop for m2 from (- m 1) downto 0
		      do
			(when (not (eq (aref *game-array* m2 n) nil))
			  (when (= (aref *game-array* m n)
				   (aref *game-array* m2 n))
			    (incf *game-score* (* 10 (aref *game-array* m n)))
			    (setf (aref *game-array* m2 n) nil)
			    (setf (aref *game-array* m n)
				  (* (aref *game-array* m n) 2)))
			  ;;(setf m m2)
			  (setf m2 0)
			  ;;(return)
			  ))))
	   (loop for m from 3 downto 0
	      do (when (eq (aref *game-array* m n) nil)
		   (loop for m2 from (- m 1) downto 0
		      do (when (not (eq (aref *game-array* m2 n) nil))
			   (format t "Swapping ~ax~a with ~ax~a~%"
				   m n
				   m2 n)
			   (setf (aref *game-array* m n)
				 (aref *game-array* m2 n))
			   (setf (aref *game-array* m2 n) nil)
			   (setf m2 0)))))))
       (when (has-pressed :btn-left)
	 (setf spawn-flag t)
	 ;; For each line
	 (dotimes (m 4)
	   ;; Get squares from left to right
	   (loop for n from 0 to 3
	      do (when (not (eq (aref *game-array* m n) nil))
		   ;; We found a non-nil element! Now check the next
		   (loop for n2 from (+ n 1) to 3
		      do
			(when (not (eq (aref *game-array* m n2) nil))
			  ;; We found another non-nil element to its left!
			  (when (= (aref *game-array* m n)
				   (aref *game-array* m n2))
			    ;; Add points
			    (incf *game-score* (* 10 (aref *game-array* m n)))
			    ;; Delete the current element; duplicate the one
			    ;; to the left
			    (setf (aref *game-array* m n2) nil)
			    (setf (aref *game-array* m n)
				  (* (aref *game-array* m n) 2)))
			  ;; Regardless of result move iterator to our current
			  ;; position and force break
			  ;;(setf n n2)
			  (setf n2 3)
			  ;;(return)
			  ))))
	   ;; Regardless, move everyone to the left using a bubble
	   (loop for n from 0 to 3
	      do (when (eq (aref *game-array* m n) nil)
		   (loop for n2 from (+ n 1) to 3
		      do (when (not (eq (aref *game-array* m n2) nil))
			   (format t "Swapping ~ax~a with ~ax~a~%"
				   m n
				   m n2)
			   (setf (aref *game-array* m n)
				 (aref *game-array* m n2))
			   (setf (aref *game-array* m n2) nil)
			   (setf n2 3)))))))
       (when (has-pressed :btn-right)
	 (setf spawn-flag t)
	 ;; For each line
	 (dotimes (m 4)
	   ;; Get squares from right to left
	   (loop for n from 3 downto 0
	      do (when (not (eq (aref *game-array* m n) nil))
		   ;; We found a non-nil element! Now check the next
		   (loop for n2 from (- n 1) downto 0
		      do
			(when (not (eq (aref *game-array* m n2) nil))
			  ;; We found another non-nil element to its left!
			  (when (= (aref *game-array* m n)
				   (aref *game-array* m n2))
			    ;; Add points
			    (incf *game-score* (* 10 (aref *game-array* m n)))
			    ;; Delete the current element; duplicate the one
			    ;; to the right
			    (setf (aref *game-array* m n2) nil)
			    (setf (aref *game-array* m n)
				  (* (aref *game-array* m n) 2)))
			  ;; Regardless of result move iterator to our current
			  ;; position and force break
			  ;;(setf n n2)
			  (setf n2 0)
			  ;;(return)
			  ))))
	   ;; Regardless, move everyone to the right using a bubble
	   (loop for n from 3 downto 0
	      do (when (eq (aref *game-array* m n) nil)
		   (loop for n2 from (- n 1) downto 0
		      do (when (not (eq (aref *game-array* m n2) nil))
			   (format t "Swapping ~ax~a with ~ax~a~%"
				   m n
				   m n2)
			   (setf (aref *game-array* m n)
				 (aref *game-array* m n2))
			   (setf (aref *game-array* m n2) nil)
			   (setf n2 0)))))))
       ;; Spawn a new block if necessary
       (when spawn-flag
	 (spawn-new-block))
       ))))


(defun draw-board ()
  (let ((init-pos (make-vec2 :x 10.0 :y 30.0))
	(hs (/ *game-square-bg-sz* 2.0)))
    (dotimes (m 4)
      (dotimes (n 4)
	;; Draw background
	;;(gl:color 0.988 0.537 0.674)
	(gl:color 0.6 0.6 0.6)
	(draw-square (make-vec2
		      :x (+ (vec2-x init-pos) hs (* m 120.0))
		      :y (+ (vec2-y init-pos) hs (* n 120.0))))
	;; Draw outline
	(gl:polygon-mode :front-and-back :line)
	(gl:line-width 3.0)
	;;(gl:color 0.88 0.4 0.34)
	(gl:color 0.8 0.8 0.8)
	(draw-square (make-vec2
		      :x (+ (vec2-x init-pos) hs (* m 120.0))
		      :y (+ (vec2-y init-pos) hs (* n 120.0))))
	(gl:polygon-mode :front-and-back :fill)
	(gl:line-width 1.0)))
    ;; Draw actual squares
    (when (not (eq *game-array* nil))
      (dotimes (n 4)
	(dotimes (m 4)
	  (when (not (eq (aref *game-array* m n) nil))
	    (set-square-color (aref *game-array* m n))
	    (draw-square (make-vec2
			  :x (+ (vec2-x init-pos) hs (* n 120.0))
			  :y (+ (vec2-y init-pos) hs (* m 120.0))))
	    (draw-string *game-font*
			 (write-to-string (aref *game-array* m n))
			 (make-vec2
			  :x (+ (vec2-x init-pos) hs (* n 120.0))
			  :y (+ (vec2-y init-pos) hs (* m 120.0)))
			 '(1.0 1.0 1.0)
			 4.0)
	    ))))))

  








(defun setup-view ()
  ;; Disable VSync
  ;;(sdl2:gl-set-swap-interval 0)

  ;; Setup viewport and ortographic view
  (gl:viewport 0 0 *window-width* *window-height*)
  (gl:matrix-mode :projection)
  (gl:ortho 0 *window-width* *window-height* 0 0 1)
  (gl:matrix-mode :modelview)
  (gl:load-identity)
  (gl:clear-color 1.0 1.0 1.0 1.0))


(defun init ()
  (setf *frame-count* 0)
  (setf *last-fps-message-time* (sdl2:get-ticks))

  ;; Init game controllers
  (loop for i from 0 upto (- (sdl2:joystick-count) 1)
     do (when (sdl2:game-controller-p i)
	  (let ((controller (sdl2:game-controller-open i)))
	    (setf *game-controllers* (acons i controller *game-controllers*)))))
  (gl:enable :blend)
  (gl:blend-func :src-alpha :one-minus-src-alpha)

  (init-array))

(defun load-content ()
  (setf (texture *game-font*) (load-texture "./font.png")))

(defun unload-content ()
  (gl:delete-textures (list (texture-name (texture *game-font*)))))

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
  (declare (ignore delta-t))
  (update-fps)
  (update-board)
  (update-input))


(defun draw ()
  (gl:clear :color-buffer)
  (draw-board)
  ;; Draw score
  (draw-string *game-font* (format nil "~8,'0d" *game-score*)
	       (make-vec2 :x (/ *window-width* 2.0) :y 14.0)
	       '(0.0 0.0 0.0)
	       2.0)
  (sdl2:gl-swap-window *window*))


(defun main-loop ()
  (sdl2:with-init (:video :joystick)
    (setf *window* (sdl2:create-window
		    :title "2048 (SDL2/OpenGL/CommonLisp)"
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

      (init)
      (load-content)
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
		    (setf (input-values-back *input-state*) nil)))
	
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
		    (setf (input-values-back *input-state*) t)))
	
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

      ;; Delete game controllers
      (loop for (i . controller) in *game-controllers*
	 do (sdl2:game-controller-close controller))

      (unload-content)
      
      ;; Delete context and window
      (sdl2:gl-delete-context *gl-context*)
      (sdl2:destroy-window *window*))))

(main-loop)
