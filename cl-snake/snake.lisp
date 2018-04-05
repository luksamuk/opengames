;;;; cl-snake
;;;; A snake clone.
;;;; Copyright © 2013-2018 Lucas Vieira
;;;;
;;;; Licensed under the MIT License.
;;;; Permission is hereby granted, free of charge, to any person obtaining a copy of
;;;; this software and associated documentation files (the "Software"), to deal in
;;;; the Software without restriction, including without limitation the rights to
;;;; use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
;;;; the Software, and to permit persons to whom the Software is furnished to do so,
;;;; subject to the following conditions:
;;;; 
;;;; The above copyright notice and this permission notice shall be included in all
;;;; copies or substantial portions of the Software.
;;;; 
;;;; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
;;;; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
;;;; FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
;;;; COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
;;;; IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
;;;; CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.    



;;; Uncomment the following and eval these lines to run it from
;;; SBCL with Quicklisp. They remain commented for compilation
;;; only
;;(ql:quickload "sdl2")
;;(ql:quickload "cl-opengl")
;;(ql:quickload "bordeaux-threads")

(require 'sdl2)
(require 'cl-opengl)
(require 'bordeaux-threads)

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
(defparameter *snake-init-num-pieces* 2)
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

;; Player stuff
(defparameter *player-score* 0)

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
  (:documentation "Initializes or redefines snake and game values."))

(defgeneric update-snake (snake delta-t)
  (:documentation "Updates the snake."))

(defgeneric draw-snake (snake)
  (:documentation "Draws the snake."))


(defmethod init-snake ((thesnake snake))
  (setf (pos thesnake) (make-vec2
		   :x (/ *window-width* 2.0)
		   :y (/ *window-height* 2.0)))
  (setf (piece-list thesnake) nil)
  (setf (fruit-list thesnake) nil)
  (setf (dir thesnake) (make-vec2 :x 1.0))
  (setf *player-score* 0)
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

(defun is-overlapping-snake (fst thesnake)
  (loop for x in (piece-list thesnake)
       do (if (is-overlapping-piece fst x)
	      (return t)))
  nil)

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
	       ;;(format t "Snake collided with tail @ piece #~a~%" (- max-piece i))
	       ;;(format t "Final score: ~apts.~%" *player-score*)
	       ;;(setf *running* nil)
	       (init-snake thesnake)
	       ))
	 (incf i)))

  ;; With fruit
  (when (is-overlapping-piece (pos thesnake) *fruit-position*)
    ;; Add fruit position to end of growth list
    (set-append (make-vec2 :x (vec2-x *fruit-position*)
			   :y (vec2-y *fruit-position*))
		(fruit-list thesnake))
    ;; Add to score
    (incf *player-score*)
    ;; Reset fruit position
    (loop do
	 (setf *fruit-position*
	       (make-vec2 :x (float (* (+ (random (- (/ *window-width* *snake-piece-size*) 1)) 1)
				*snake-piece-size*))
		          :y (float (* (+ (random (- (/ *window-height* *snake-piece-size*) 1)) 1)
				*snake-piece-size*))))
       while (is-overlapping-snake *fruit-position* thesnake)))
  
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
    (gl:color 0.3 0.2 0.5)
    (gl:with-primitive :quads
      (gl:vertex (- *snake-piece-half-size*) (- *snake-piece-half-size*))
      (gl:vertex *snake-piece-half-size* (- *snake-piece-half-size*))
      (gl:vertex *snake-piece-half-size* *snake-piece-half-size*)
      (gl:vertex (- *snake-piece-half-size*) *snake-piece-half-size*))
    ;; Small point in the center of the piece, for style
    (gl:color 0.5 0.3 0.6)
    (gl:with-primitive :polygon
	(loop for x from 0 to 360 by 5
	   do (gl:vertex (* 2 (cos x))
			 (* 2 (sin x)))))
    (gl:flush)))

(defun draw-snake-head (pos angle)
  (gl:with-pushed-matrix
    (gl:translate (vec2-x pos) (vec2-y pos) 0.0)
    (gl:rotate angle 0.0 0.0 1.0)
    (gl:color 0.3 0.2 0.5)
    (gl:with-primitive :polygon
      ;; Left vertices
      (gl:vertex 0.0 *snake-piece-half-size*)
      (gl:vertex (- *snake-piece-half-size*) *snake-piece-half-size*)
      (gl:vertex (- *snake-piece-half-size*)  (- *snake-piece-half-size*))
      (gl:vertex 0.0 (- *snake-piece-half-size*))
      ;; Right arc
      (loop for x from -90 upto 90 by 5
	 do (gl:vertex (* *snake-piece-half-size* (cos x))
		       (* *snake-piece-half-size* (sin x)))))
    ;; Eyes
    (gl:color 0.6 0.6 0.6)
    (gl:with-pushed-matrix
      (gl:translate (/ *snake-piece-half-size* 2.0)
		    (- (/ *snake-piece-half-size* 2.0))
		    0.0)
      (gl:with-primitive :polygon
	(loop for x from 0 to 360 by 5
	   do (gl:vertex (* 2 (cos x))
			 (* 2 (sin x))))))
    (gl:with-pushed-matrix
      (gl:translate (/ *snake-piece-half-size* 2.0)
		    (/ *snake-piece-half-size* 2.0)
		    0.0)
      (gl:with-primitive :polygon
	(loop for x from 0 to 360 by 5
	   do (gl:vertex (* 2 (cos x))
			 (* 2 (sin x))))))
    (gl:flush)))


(defun draw-fruit-leaf ()
  (let ((snake-piece-quarter-size
	 (/ *snake-piece-half-size* 2.0)))
  (gl:with-pushed-matrix
    (gl:translate 0.0
		  (- snake-piece-quarter-size)
		  0.0)
    (gl:with-primitive :polygon
      (loop for x from 0 upto 360 by 5
	 do (gl:vertex (* 2 (cos x))
		       (* snake-piece-quarter-size
			  (sin x))))))))

(defun draw-fruit (pos)
  (gl:with-pushed-matrix
    (gl:translate (vec2-x pos) (vec2-y pos) 0.0)
    (gl:color 0.5 0.2 0.3)
    (gl:with-primitive :polygon
      (loop for x from 0 upto 360 by 5
	 do (gl:vertex (* *snake-piece-half-size* (cos x))
		       (* *snake-piece-half-size* (sin x)))))
    ;; Leaves
    (gl:translate 0.0 (- *snake-piece-half-size*) 0.0)
    (gl:color 0.2 0.5 0.2)
    (draw-fruit-leaf)
    (gl:rotate 45.0 0.0 0.0 1.0)
    (draw-fruit-leaf)
  (gl:flush)))

(defun figure-angle-from-direction (dir)
  (if (not (zerop (vec2-x dir)))
      (if (minusp (vec2-x dir)) 180.0 0.0)
      (if (minusp (vec2-y dir)) 270.0 90.0)))

(defmethod draw-snake ((thesnake snake))
  ;; Draw fruit
  (draw-fruit *fruit-position*)
  ;; Draw snake
  (let ((i (- (list-length (piece-list thesnake)) 1)))
    (loop for piece in (piece-list thesnake)
       do (if (zerop i)
	      (draw-snake-head piece
			       (figure-angle-from-direction (dir thesnake)))
	      (draw-snake-piece piece))
	 ;;(format t "Piece #~a : ~s~%" i piece)
	 (decf i))))


;; The actual snake instance!
(defparameter *snake* (make-instance 'snake))


(defun setup-view ()
  ;; Disable VSync
  ;;(sdl2:gl-set-swap-interval 0)

  ;; Setup viewport and ortographic view
  (gl:viewport 0 0 *window-width* *window-height*)
  (gl:matrix-mode :projection)
  (gl:ortho 0 *window-width* *window-height* 0 0 1)
  (gl:matrix-mode :modelview)
  (gl:load-identity)
  (gl:clear-color 0.0 0.0 0.0 1.0))


(defun init ()
  ;;(format t "===SNAKE GAME===~%")
  (setf *frame-count* 0)
  (setf *last-fps-message-time* (sdl2:get-ticks))
  (setf *random-state* (make-random-state t))

  ;; Init game controllers
  (loop for i from 0 upto (- (sdl2:joystick-count) 1)
     do (when (sdl2:game-controller-p i)
	  ;;(format t "Found gamepad: ~a~%" (sdl2:game-controller-name-for-index i))
	  ;; Will only be fetching the controllers.
	  ;; Won't get joysticks because haptic systems
	  ;; are not important right now
	  (let ((controller (sdl2:game-controller-open i)))
	    (setf *game-controllers* (acons i controller *game-controllers*)))))
  
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
      ;;(format t "~s seconds: ~s fps, ~s ms/frame~%"
	  ;;    seconds
	  ;;    *fps*
	  ;;    (if (zerop frames) "<infinite>" (* 1000 (/ seconds frames))))
      )
    (setf *last-fps-message-time* (sdl2:get-ticks))
    (setf *last-fps-message-frame-count* *frame-count*)))


(defun update-input ()
  ;;(format t "Current:~%~a~%Old:~%~a~%~%" *input-state* *old-input-state*)

  ;; Pause game
  (when (has-pressed :btn-start)
    (setf *running* (not *running*))
    ;;(if (not *running*)
	  ;;(format t "Game Paused~%")
      ;;(format t "Continuing game~%"))
    )

  ;; Quit game (Esc or Back)
  (when (has-pressed :btn-back)
    ;; Do not use sdl2:quit if you're
    ;; gonna keep the REPL open, seriously.
    ;; Just use SDL2's events.
    (sdl2:push-quit-event))
  
  ;; Update old state after processing
  (setf *old-input-state* (copy-structure *input-state*)))


(defun update (delta-t)
  (if *running*
      (update-snake *snake* delta-t))
  (update-fps)
  (update-input))


(defun draw()
  (gl:clear :color-buffer)
  (draw-snake *snake*)
  (sdl2:gl-swap-window *window*))


(defun main-loop ()
  (sdl2:with-init (:video :joystick)
    (setf *window* (sdl2:create-window
		    :title "Lisp Snake"
		    ;;:x (- 1820 (round (* *window-width* 1)))
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

	
	;;(:joyhatmotion (:which controller-id :hat hat :value value)
	;;	       (format t
	;;		       "Controller hat: Controller: ~a, Hat: ~a, Value: ~a~%"
	;;		       controller-id hat value))
	
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
      
      ;; Delete context and window
      (sdl2:gl-delete-context *gl-context*)
      (sdl2:destroy-window *window*))))

;;(main-loop)



;; NOTE: This is a function that only serves the purpose of serving as
;; entry-level for compilation tools, such as buildapp, which I used
;; to compile this.
;; For more info, visit https://www.xach.com/lisp/buildapp/
(defun main-entry (argv)
  "Defines a main entry for compiling from buildapp"
  (declare (ignore argv))
  (main-loop))
