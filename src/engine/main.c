#include <stdlib.h>
#define SDL_MAIN_HANDLED
#include "sdlimp.h"
#include <stdio.h>
#include <string.h>
#include "../shared/global.h"
#include "engine.h"

#define DEFWIDTH 800
#define DEFHEIGHT 600

__extension__ struct hzwinprop primarywin = {};

INAT main(INAT argc, CHR *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		errwindow("Unable to initialize video!");
	}

	primarywin.winflags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	if (!(primarywin.window = SDL_CreateWindow(
		"HAZE",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		DEFWIDTH, DEFHEIGHT,
		primarywin.winflags))) {
		errwindow("Unable to create primary window!");
	}

	if (!(primarywin.glcontext = SDL_GL_CreateContext(primarywin.window))) {
		errwindow("Unable to create GL context!");
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

		glViewport(0, 0, primarywin.width, primarywin.height);
		glClearColor(1.f, 0.f, 1.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(primarywin.window);
	}

	SDL_DestroyWindow(primarywin.window);
	SDL_Quit();
	return EXIT_SUCCESS;

}
