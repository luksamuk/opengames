all: quicklisp-manifest.txt
	buildapp --manifest-file quicklisp-manifest.txt \
			--load snake.lisp \
			--entry main-entry --output cl-snake
	rm quicklisp-manifest.txt

quicklisp-manifest.txt:
	sbcl --no-userinit --no-sysinit --non-interactive \
				--load ~/quicklisp/setup.lisp \
				--eval '(ql:quickload "sdl2")' \
				--eval '(ql:quickload "cl-opengl")' \
				--eval '(ql:quickload "bordeaux-threads")' \
				--eval '(ql:write-asdf-manifest-file "quicklisp-manifest.txt")'
