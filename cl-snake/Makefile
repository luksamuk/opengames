SBCL      = sbcl
QUICKLISP = ~/quicklisp/setup.lisp

all:
	$(SBCL) --no-userinit --no-sysinit --non-interactive  \
			--load $(QUICKLISP) \
			--load build.lisp

# Compiling for Windows (32-bit)
win32: SBCL      := sbcl-win32
win32: QUICKLISP := C:/users/alchemist/quicklisp/setup.lisp
win32: all
