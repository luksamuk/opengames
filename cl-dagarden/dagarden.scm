;;;; Sonic CD's D.A. Garden Clone
;;;; by luksamuk

;;; Eggs you should install:
;;; - sdl2
;;; - opengl
;;; - loops
;;; - defstruct

(use (prefix sdl2 sdl2:)
     (prefix sdl2-image img:)
     (only lolevel
	   object-evict object-release)) ; SDL2
(require-extension gl)        ; OpenGL
(require-extension loops)     ; Loops such as do-while
(require-extension defstruct) ; Structs
(use srfi-4)                  ; required for loading imgs
(use sdl-mixer)

;;; Just because.
;;(repl-prompt (lambda () "* > "))


;;; Global variables
(define window #f)
(define gl-context #f)
(define running #t)
(define viewport-width 800)
(define viewport-height 600)


;; FPS-related
(define last-time 0)
(define fps-last-prompt-time 0)
(define fps-prompt-delay-interval 2000) ; 2 secs


;;; Game Types
(defstruct input-values up down left right)
(defstruct vec2 x y)
(defstruct Texture handle w h)


;; Input variables
(define p1-input-state
  (make-input-values
   up:    #f
   down:  #f
   left:  #f
   right: #f))


;; Game variables
(define sq-position
  (make-vec2
   x: (/ viewport-width 2.0)
   y: (/ viewport-height 2.0)))
(define rotation 0.0)
(define sq-size 200.0)
(define h-sq-size (/ sq-size 2.0))
(define t-littleplanet #f)
(define m-dagarden #f)


(define (init-gl-attrs)
  ;; OpenGL attributes
  (set! (sdl2:gl-attribute 'context-major-version) 3)
  (set! (sdl2:gl-attribute 'context-minor-version) 1)
  (set! (sdl2:gl-attribute 'red-size) 8)
  (set! (sdl2:gl-attribute 'green-size) 8)
  (set! (sdl2:gl-attribute 'blue-size) 8)
  (set! (sdl2:gl-attribute 'alpha-size) 8)
  (set! (sdl2:gl-attribute 'buffer-size) 32)
  (set! (sdl2:gl-attribute 'doublebuffer) 1)
  (set! (sdl2:gl-attribute 'depth-size) 24)
  (set! (sdl2:gl-attribute 'share-with-current-context) 1))

;;; Initialization func
(define (init)
  ;; Disables VSync
  (set! (sdl2:gl-swap-interval) 0)
  ;; Sets clear color
  (gl:ClearColor 0.0 0.0 0.0 1.0)

  ;; Alpha blending
  (gl:Enable gl:BLEND)
  (gl:BlendFunc gl:SRC_ALPHA gl:ONE_MINUS_SRC_ALPHA)
  
  ;; Define our orthographic view
  (gl:MatrixMode gl:PROJECTION)
  (gl:LoadIdentity)
  (gl:Ortho 0 viewport-width viewport-height 0 -1 1)
  (gl:MatrixMode gl:MODELVIEW)
  (open-audio))



(define (load-texture texture-path)
  (let ((tex-surface (img:load* texture-path))
	(pixelmode 0)
	(glTexture (make-u32vector 1)))
    (case (sdl2:pixel-format-bytes-per-pixel (sdl2:surface-format tex-surface))
      ((4) (set! pixelmode gl:RGBA8))
      ((3) (set! pixelmode gl:RGB8)))

    (sdl2:gl-make-current! window gl-context)
    
    (gl:Enable gl:TEXTURE_2D)
    (gl:GenTextures 1 glTexture)
    (gl:BindTexture gl:TEXTURE_2D  (u32vector-ref glTexture 0))
    (gl:TexParameteri gl:TEXTURE_2D gl:TEXTURE_WRAP_S gl:CLAMP_TO_EDGE)
    (gl:TexParameteri gl:TEXTURE_2D gl:TEXTURE_WRAP_T gl:CLAMP_TO_EDGE)
    (gl:TexParameteri gl:TEXTURE_2D gl:TEXTURE_MIN_FILTER gl:NEAREST)
    (gl:TexParameteri gl:TEXTURE_2D gl:TEXTURE_MAG_FILTER gl:NEAREST)

    (gl:TexImage2D gl:TEXTURE_2D
		   0
		   pixelmode
		   (sdl2:surface-w tex-surface)
		   (sdl2:surface-h tex-surface)
		   0
		   pixelmode
		   gl:UNSIGNED_BYTE
		   (sdl2:surface-pixels-raw tex-surface))
    ;;(gl:Finish)
    (gl:Disable gl:TEXTURE_2D)
    (let ((myTexture (make-Texture
		      ref: #f
		      w: 0
		      h: 0)))
      (Texture-handle-set! myTexture glTexture)
      (Texture-w-set! myTexture (sdl2:surface-w tex-surface))
      (Texture-h-set! myTexture (sdl2:surface-h tex-surface))
      (sdl2:free-surface! tex-surface)
      (format #t "Loaded texture ~a\n." texture-path)
      (print (Texture-handle myTexture))
      ;; Return my texture
      myTexture)))



(define (load-content)
  ;;(set! m-dagarden (load-music "./music/DAGardenBeta.mp3"))
  ;;(play-music m-dagarden)
  (set! t-littleplanet (load-texture "./sprites/littleplanet.png")))


(define (unload-content)
  (close-audio)
  (let ((tex-handle (Texture-handle t-littleplanet)))
    (gl:DeleteTextures 1 tex-handle)))

;;; Update func
(define (update delta-t)
  ;; Square rotation
  (set! rotation (+ rotation (* 30.0 delta-t)))
  (if (>= rotation 360.0)
      (set! rotation (- rotation 360.0)))

  ;; Square movement
  ;; X
  (cond ((input-values-right p1-input-state)
	 (vec2-x-set! sq-position (+ (vec2-x sq-position)
				     (* 300.0 delta-t))))
	((input-values-left p1-input-state)
	 (vec2-x-set! sq-position (- (vec2-x sq-position)
				     (* 300.0 delta-t)))))
  ;; Y
  (cond ((input-values-down p1-input-state)
	 (vec2-y-set! sq-position (+ (vec2-y sq-position)
				     (* 300.0 delta-t))))
	((input-values-up p1-input-state)
	 (vec2-y-set! sq-position (- (vec2-y sq-position)
				     (* 300.0 delta-t)))))
  
  
  ;; FPS display
  (if (>= (- (sdl2:get-ticks) fps-last-prompt-time) fps-prompt-delay-interval)
      (begin
	(set! fps-last-prompt-time (sdl2:get-ticks))
	(format #t "FPS: ~a, deltaT: ~as\n"
		(if (zero? delta-t) "inf" (/ 1.0 delta-t))
		delta-t))))

;;; Draw func
(define (draw)
  (gl:Clear gl:COLOR_BUFFER_BIT)

  (gl:Enable gl:TEXTURE_2D)
  (gl:BindTexture gl:TEXTURE_2D (u32vector-ref (Texture-handle t-littleplanet) 0))
  (gl:TexEnvf gl:TEXTURE_ENV gl:TEXTURE_ENV_MODE gl:MODULATE)

  (gl:TexParameteri gl:TEXTURE_2D gl:TEXTURE_MIN_FILTER gl:NEAREST)
  (gl:TexParameteri gl:TEXTURE_2D gl:TEXTURE_MAG_FILTER gl:NEAREST)
  
  (gl:PushMatrix)
  (gl:Translatef (vec2-x sq-position)
		 (vec2-y sq-position)
		 0.0)
  (gl:Rotatef rotation 0.0 0.0 1.0)
  (gl:Color3f 1.0 1.0 1.0)
  ;; Draw quad
  (gl:Begin gl:QUADS)
    (gl:TexCoord2f 0.0 0.0)
    (gl:Vertex2f (* -1 h-sq-size) (* -1 h-sq-size))
  
    (gl:TexCoord2f 1.0 0.0)
    (gl:Vertex2f h-sq-size (* -1 h-sq-size))

    (gl:TexCoord2f 1.0 1.0)
    (gl:Vertex2f h-sq-size h-sq-size)
    
    (gl:TexCoord2f 0.0 1.0)
    (gl:Vertex2f (* -1 h-sq-size) h-sq-size)
  (gl:End)
  (gl:PopMatrix)
  (gl:Disable gl:TEXTURE_2D)
  
  (sdl2:gl-swap-window! window))


(define main-loop
  (lambda ()
    ;; Initialize SDL2
    (sdl2:set-main-ready!)
    (sdl2:init! '(video joystick))
    (init-gl-attrs)
    ;; Create SDL2 window
    (set! window (sdl2:create-window! "SDL2 Scheme"
                                      'centered 'centered
                                      viewport-width viewport-height
                                      '(shown opengl)))
    ;; Create OpenGL context
    (set! gl-context (sdl2:gl-create-context! window))
    (sdl2:gl-make-current! window gl-context)
    
    ;; Initialization
    (init)
    (load-content)

    ;; Actual game loop
    (do-while running
	      ;; Handle SDL2 events
	      (let ((event (sdl2:make-event)))
		(do-while (sdl2:poll-event! event)
			  (case (sdl2:event-type event)
			    
			    ('quit (set! running #f))
			    
			    ('key-down
			     (case (sdl2:keyboard-event-scancode event)
			       ('escape (set! running #f))

			       ('up (input-values-up-set! p1-input-state #t))
			       ('down (input-values-down-set! p1-input-state #t))
			       ('left (input-values-left-set! p1-input-state #t))
			       ('right (input-values-right-set! p1-input-state #t))))
			     
			    ('key-up
			     (case (sdl2:keyboard-event-scancode event)
			       ('up (input-values-up-set! p1-input-state #f))
			       ('down (input-values-down-set! p1-input-state #f))
			       ('left (input-values-left-set! p1-input-state #f))
			       ('right (input-values-right-set! p1-input-state #f))))
					
			    )))
	      ;; Update funcall.
	      ;; Notice DeltaTime calculation
	      (let ((current-time (sdl2:get-ticks)))
		(update (/ (- current-time last-time) 1000.0))
		(set! last-time current-time))
	      ;; Draw funcall
	      (draw))
    ;; Closure
    (unload-content)
    (sdl2:gl-delete-context! gl-context)
    (sdl2:destroy-window! window)
    (sdl2:quit!)))

;; Call main loop func
(main-loop)
