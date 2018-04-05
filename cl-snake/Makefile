SBCL=sbcl
QUICKLISP=~/quicklisp/setup.lisp
all: quicklisp-manifest.txt
	buildapp --manifest-file quicklisp-manifest.txt \
			--load snake.lisp \
			--entry main-entry --output cl-snake
	rm quicklisp-manifest.txt

quicklisp-manifest.txt:
	$(SBCL) --no-userinit --no-sysinit --non-interactive \
				--load $(QUICKLISP) \
				--eval '(ql:quickload "sdl2")' \
				--eval '(ql:quickload "cl-opengl")' \
				--eval '(ql:quickload "bordeaux-threads")' \
				--eval '(ql:write-asdf-manifest-file "quicklisp-manifest.txt")'

# Well this one ain't really working :/
win32:
	sbcl-win32 --no-userinit --no-sysinit --non-interactive \
				--load 'C:/users/alchemist/.sbclrc' \
				--load snake.lisp \
				--eval '(ql:quickload \"sdl2\")' \
				--eval '(ql:quickload \"cl-opengl\")' \
				--eval '(ql:quickload \"bordeaux-threads\")' \
				--eval '(sb-ext:save-lisp-and-die \"cl-snake.exe\" :toplevel #\'main-loop :executable t)'
