#include <SDL2/SDL_error.h>
#include <stdlib.h>
#define SDL_MAIN_HANDLED
#include "sdlimp.h"
#include <stdio.h>
#include <string.h>
#include "../shared/hzshared.h"
#include "engine.h"

#define DEFWIDTH 800
#define DEFHEIGHT 600

__extension__ struct hzwinprop primarywin = {};

INAT main(INAT argc, CHR *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		errwindow("Unable to initialize video!\n SDL Error: %s", SDL_GetError());
	}

	primarywin.winflags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN;
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	if (!(primarywin.window = SDL_CreateWindow(
		"HAZE",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		DEFWIDTH, DEFHEIGHT,
		primarywin.winflags))) {
		errwindow("Unable to create the primary window!\n SDL Error: %s", SDL_GetError());
	}

	if (!(primarywin.glcontext = SDL_GL_CreateContext(primarywin.window))) {
		errwindow("Unable to create GL context! Does your device support OpenGL?\n"
			"Are you sure you're using the very latest versions of your graphics drivers?\n"
			"You might be able to resolve this by using Mesa software rendering.\n\n"
			"SDL Error: %s", SDL_GetError());
	}

	while (!primarywin.quit) {
		SDL_Event Event;
		while (SDL_PollEvent(&Event)) {
			switch (Event.type) {
			case SDL_QUIT:
				primarywin.quit = true;
				break;
			default:
				break;
			}
		}

		SDL_GetWindowSize(primarywin.window, &primarywin.width, &primarywin.height);

		glClearColor(1.f, 0.f, 1.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(primarywin.window);
	}

	SDL_DestroyWindow(primarywin.window);
	SDL_Quit();
	return EXIT_SUCCESS;

}
