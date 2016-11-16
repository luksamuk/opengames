(eval-when (:compile-toplevel :load-toplevel :execute)
  (format t "Loading Quicklisp Libs~%")
  (finish-output)
  (ql:quickload "sdl2")
  (ql:quickload "sdl2-image")
  (ql:quickload "cl-opengl")
  (ql:quickload "bordeaux-threads")
  (format t "Loading symbols~%")
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
(defparameter *angle* 0.0)
(defparameter *running* t)


;; Input-related
(defstruct input-values
  (up nil :type boolean)
  (down nil :type boolean)
  (left nil :type boolean)
  (right nil :type boolean)
  (restart nil :type boolean))

;; Input info
(defparameter *input-state* (make-input-values))


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



;;; Snake stuff

;; Snake piece size
(defvar *snake-piece-size* 20)
(defvar *snake-piece-half-size* (/ *snake-piece-size* 2.0))
;; @ 60hz, the snake should walk:
;; - Easiest Mode: 1 piece per second
;; - Easy Mode: 5 pieces per second
;; - Medium Mode: 15 pieces per second
;; - Hard Mode: 25 pieces per second
;; - Hardest Mode: 50 pieces per second
(defparameter *snake-speed* 15)
;; Starting number of snake pieces
(defparameter *snake-init-num-pieces* 10)
;; Update interval: speed-related.
;; Calculated before each frame.
(defparameter *snake-update-interval* 0)
;; Last movement frame footprint
(defparameter *last-snake-update* 0)
;; Last movement footprint.
;; Prevents movement between movement frames
(defparameter *has-snake-moved* nil)
;; For eating order
(defparameter *has-snake-eaten* nil)

;; Fruit stuff
(defparameter *fruit-position* (make-vec2))

;; Snake class
(defclass snake ()
  ((pos :initform (make-vec2
		   :x (/ *window-width* 2.0)
		   :y (/ *window-height* 2.0))
	:accessor pos)
   (dir :initform (make-vec2 :x 1.0) :accessor dir)
   (spd :initform 5.0 :accessor spd) ;; pixels on current frame
   (piece-list
    :initform nil
    :accessor piece-list)
   (fruit-list
    :initform nil
    :accessor fruit-list)))

(defgeneric init-snake (snake)
  (:documentation "Initializes an object on screen."))

(defgeneric update-snake (snake delta-t)
  (:documentation "Updates an object on screen."))

(defgeneric draw-snake (snake)
  (:documentation "Draws an object on screen."))

(defgeneric add-piece-snake (snake)
  (:documentation "Adds a new piece to the snake."))


(defmethod init-snake ((thesnake snake))
  ;; Spawn fruit
  (setf *fruit-position*
	(make-vec2 :x (* 3 (/ *window-width* 4.0))
		   :y (/ *window-height* 16.0)))
  ;; Spawn first pieces
  (loop for x from 0 to *snake-init-num-pieces*
     do (push (make-vec2 :x (- (/ *window-width* 2.0)
			       (* *snake-piece-size* x))
			 :y (/ *window-height* 2.0))
	      (piece-list thesnake))))



(defun is-overlapping-piece (fst snd)
  (let ((delta-x (- (vec2-x fst) (vec2-x snd)))
	(delta-y (- (vec2-y fst) (vec2-y snd)))
	(sum-of-radius (* *snake-piece-half-size* 2)))
    ;; We consider each snake piece as a circle and
    ;; compare if the circle is overlapping another.
    ;; Since sqrt can be expensive, we just compare
    ;; square values.
    ;; Of course, since the pieces are so tightly
    ;; positioned, weneed to ignore when the distance
    ;; matches the sum of radiuses.
    (< (+ (* delta-x delta-x) (* delta-y delta-y))
	(* sum-of-radius sum-of-radius))))

(defmethod add-piece-snake ((thesnake snake))
  
  )

(defmethod update-snake ((thesnake snake) delta-t)
  ;; Calculate snake walk ratio
  ;; Let the snake be updated every 1/speed seconds.
  (setf *snake-update-interval*
	(/ 1000 *snake-speed*))

  ;; Our snake is actually A QUEUE.
  ;; Just add a new piece, with the new position, related to
  ;; the snake's dir.
  (when (>= (sdl2:get-ticks)
	    (+ *last-snake-update*
	       *snake-update-interval*))
    (progn
      ;;(format t "Updated in ~a ms.~%" *snake-update-interval*)
      ;; Reset last update time
      (setf *last-snake-update* (sdl2:get-ticks))
      ;; Calculate new piece position
      (setf (vec2-x (pos thesnake))
	    (+ (vec2-x (pos thesnake))
	       (* *snake-piece-size*
		  (vec2-x (dir thesnake)))))
      (setf (vec2-y (pos thesnake))
	    (+ (vec2-y (pos thesnake))
	       (* *snake-piece-size*
		  (vec2-y (dir thesnake)))))

      ;; Wrap around play area
      (if (minusp (vec2-x (pos thesnake)))
	  (incf (vec2-x (pos thesnake)) *window-width*))
      (if (> (vec2-x (pos thesnake)) *window-width*)
	  (decf (vec2-x (pos thesnake)) *window-width*))

      (if (minusp (vec2-y (pos thesnake)))
	  (incf (vec2-y (pos thesnake)) *window-height*))
      (if (> (vec2-y (pos thesnake)) *window-height*)
	  (decf (vec2-y (pos thesnake)) *window-height*))
      
      ;; Append new piece to end of list.
      (set-append (make-vec2 :x (vec2-x (pos thesnake))
			     :y (vec2-y (pos thesnake)))
		  (piece-list thesnake))
      ;; Also remove first piece
      ;; But only if the snake is not supposed to
      ;; grow in size
      (if (not *has-snake-eaten*)
	  (pop (piece-list thesnake))
	  ;; Else just reset its eating state
	  (setf *has-snake-eaten* nil))
      ;; Allow snake to move next frame
      (setf *has-snake-moved* nil)))
  
  
  ;; Change movement according to button press
  (if (not *has-snake-moved*)
      (progn
	;; Y axis
	(if (and (input-values-up *input-state*)
		 (not (> (vec2-y (dir thesnake)) 0.0)))
	    (progn
	      (setf (dir thesnake) (make-vec2 :x 0.0 :y -1.0))
	      (setf *has-snake-moved* t)))
	
	(if (and (input-values-down *input-state*)
		 (not (< (vec2-y (dir thesnake)) 0.0)))
	    (progn
	      (setf (dir thesnake) (make-vec2 :x 0.0 :y 1.0))
	      (setf *has-snake-moved* t)))
	;; X axis
	(if (and (input-values-left *input-state*)
		 (not (> (vec2-x (dir thesnake)) 0.0)))
	    (progn
	      (setf (dir thesnake) (make-vec2 :x -1.0 :y 0.0))
	      (setf *has-snake-moved* t)))
	
	(if (and (input-values-right *input-state*)
		 (not (< (vec2-x (dir thesnake)) 0.0)))
	    (progn
	      (setf (dir thesnake) (make-vec2 :x 1.0 :y 0.0))
	      (setf *has-snake-moved* t)))))


  ;; Collision detection
  ;; With own tail
  ;; So basically we'll only have to do that for the head.
  ;; Just iterate and check if overlapping a piece of your own tail.
  (let ((i 0)
	(max-piece (- (list-length (piece-list thesnake))
		      1)))
    (loop for snake-piece in (piece-list thesnake)
       do
	 (if (and (< i max-piece)
		  (is-overlapping-piece (pos thesnake) snake-piece))
	     ;; TO-DO: Game Over
	     (progn
	       (format t "Snake collided with tail @ piece #~a~%" (- max-piece i))
	       (setf *running* nil)))
	 (incf i)))

  ;; With fruit
  (when (is-overlapping-piece (pos thesnake) *fruit-position*)
    ;; Add fruit position to end of growth list
    (set-append (make-vec2 :x (vec2-x *fruit-position*)
			   :y (vec2-y *fruit-position*))
		(fruit-list thesnake))
    ;; Reset fruit position (TO-DO!)
    (setf *fruit-position* (make-vec2 :x -30.0 :y -30.0)) ; out of screen
    )
  
  ;; Grow after eating
  ;; Tail's last piece is always the first on our piece-list
  (when (fruit-list thesnake)
    (if (is-overlapping-piece (car (fruit-list thesnake))
			      (car (piece-list thesnake)))
	;; Just don't remove the last piece when
	;; walking! Voila!
	;; But do remove the fruit from fruit list.
	(progn
	  (pop (fruit-list thesnake))
	  (setf *has-snake-eaten* t)))))
	

  


(defun draw-snake-piece (pos)
  (gl:with-pushed-matrix
    (gl:translate (vec2-x pos) (vec2-y pos) 0.0)
    (gl:with-primitive :quads
      (gl:vertex (- *snake-piece-half-size*) (- *snake-piece-half-size*))
      (gl:vertex *snake-piece-half-size* (- *snake-piece-half-size*))
      (gl:vertex *snake-piece-half-size* *snake-piece-half-size*)
      (gl:vertex (- *snake-piece-half-size*) *snake-piece-half-size*))
    (gl:flush)))


(defmethod draw-snake ((thesnake snake))
  ;; Draw fruit
  (gl:color 0.5 0.2 0.3)
  (draw-snake-piece *fruit-position*)
  ;; Draw snake
  (gl:color 0.3 0.2 0.5)
  (let ((i 0))
    (loop for piece in (piece-list thesnake)
       do (draw-snake-piece piece)
	 ;;(format t "Piece #~a : ~s~%" i piece)
	 (incf i))))



(defparameter *snake* (make-instance 'snake))


(defun setup-view ()
  ;; Disable VSync
  (sdl2:gl-set-swap-interval 0)

  ;; Setup viewport and ortographic view
  (gl:viewport 0 0 *window-width* *window-height*)
  (gl:matrix-mode :projection)
  (gl:ortho 0 *window-width* *window-height* 0 0 1)
  (gl:matrix-mode :modelview)
  (gl:load-identity)
  (gl:clear-color 0.0 0.0 0.0 1.0))


(defun init ()
  (format t "===SNAKE GAME===~%")
  (setf *frame-count* 0)
  (setf *last-fps-message-time* (sdl2:get-ticks))

  ;; Init snake
  (init-snake *snake*))

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

(defun update (delta-t)
  (if *running*
      (update-snake *snake* delta-t))
  (update-fps)
  
  ;; Change angle
  ;;(incf *angle* 0.5)
  ;;(if (> *angle* 360.0)
  ;;    (decf *angle* 360.0))
  )

(defun draw()
  (gl:clear :color-buffer)
  (draw-snake *snake*)
  (sdl2:gl-swap-window *window*))

(defun main-loop ()
  (sdl2:with-init (:video)
    (setf *window* (sdl2:create-window
		    :title "Lisp Snake"
		    :x (- 1820 (round (* *window-width* 1)))
		    :w *window-width* :h *window-height*
		    :flags '(:shown :opengl)))
    (setf *gl-context* (sdl2:gl-create-context *window*))
    (sdl2:gl-make-current *window* *gl-context*)
    (setf cl-opengl-bindings:*gl-get-proc-address*
	  #'sdl2:gl-get-proc-address)
    ;;(setf (sdl2:frame-rate) 60)

    (let ((previous-tick (sdl2:get-ticks)))
      (setup-view)

      ;; Windows fix!
      ;; SBCL holds a "hide" window state on Windows
      ;; so the inferior-lisp doesn't show. So hiding
      ;; and showing SDL2's window will do the trick
      (sdl2:hide-window *window*)
      (sdl2:show-window *window*)

      (init)
      (sdl2:with-event-loop (:method :poll)
	(:quit () t)
	(:keyup (:keysym keysym)
		(when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-up)
		    (setf (input-values-up *input-state*) nil))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-down)
		    (setf (input-values-down *input-state*) nil))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-left)
		    (setf (input-values-left *input-state*) nil))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-right)
		    (setf (input-values-right *input-state*) nil))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-return)
		    (setf (input-values-restart *input-state*) nil)))
	
	(:keydown (:keysym keysym)
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-up)
		    (setf (input-values-up *input-state*) t))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-down)
		    (setf (input-values-down *input-state*) t))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-left)
		    (setf (input-values-left *input-state*) t))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-right)
		    (setf (input-values-right *input-state*) t))
		  (when (sdl2:scancode= (sdl2:scancode-value keysym) :scancode-return)
		    (setf (input-values-restart *input-state*) t)))
	
	(:controlleraxismotion (:which controller-id :axis axis-id :value value)
			       (when (= controller-id 0)
				 (when (= axis-id 0) ;; Horizontal axis
				   (format t "Axis value: ~a~%" value)
				   (setf (input-values-left *input-state*) nil)
				   (setf (input-values-right *input-state*) nil)
				   (cond
				     ((< value 0)
				      (setf (input-values-left *input-state*) t))
				     ((> value 0)
				      (setf (input-values-right *input-state*) t))))
				 ;;to-do
				  ))
	
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
      (sdl2:gl-delete-context *gl-context*)
      (sdl2:destroy-window *window*))))

(main-loop)
