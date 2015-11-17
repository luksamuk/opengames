import std.stdio;
import core.thread;
import derelict.sdl2.sdl;
import derelict.sdl2.mixer;
import derelict.opengl3.gl;

SDL_Window* window;
SDL_GLContext glContext;

bool isgamerunning = true;

// There's no difference in immutable and
// const for value types, but I wanted to
// be different. Please do not call me a
// hipster
immutable int W_WIDTH  = 1280;
immutable int W_HEIGHT = 720;

void main()
{
	DerelictSDL2.load();
	DerelictSDL2Mixer.load();
	DerelictGL.load();

	window = SDL_CreateWindow("OpenGL",
			 SDL_WINDOWPOS_CENTERED,
			 SDL_WINDOWPOS_CENTERED,
			 W_WIDTH, W_HEIGHT,
			 SDL_WINDOW_OPENGL);
	
	if(!window) {
		writefln("Error: %d", SDL_GetError());
		return;
	}

	// GL Init
	glContext = SDL_GL_CreateContext(window);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
		glOrtho(0, W_WIDTH, W_HEIGHT, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	while(isgamerunning)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					isgamerunning = false;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							isgamerunning = false;
							break;
						default: break;
					}
				default: break;
			}
		}

		// Update


		glClear(GL_COLOR_BUFFER_BIT);

		// Draw

		Thread.sleep(dur!("msecs")(50));
		SDL_GL_SwapWindow(window);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
}
