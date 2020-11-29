#include <stdlib.h>
#define SDL_MAIN_HANDLED
#include "sdlimp.h"
#include <stdio.h>
#include <string.h>
#include "../shared/hzshared.h"
#include "engine.h"
#include <errno.h>
#include <cwalk.h>
#include "config.h"
#include "shaders.h"

#define DEFWIDTH 800
#define DEFHEIGHT 600

/* Declared in engine.h */
__extension__ struct hzwinprop primarywin = {};
toml_table_t *gameconf;
CHR *gameconf_path;
CHR *gameconf_dir;

INAT main(INAT argc, CHR *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		/* This might look stupid, but keep in mind that errwindow() does a printf() as a fallback too */
		errwindow("Unable to initialize video!\n SDL Error: %s", SDL_GetError());
	}

	if (argc < 2)
		errwindow("No game specified!\n\n"
			"The very first argument must be the relative or absolute path to a TOML game config file."); 

	/* Open the config file and parse content */
	gameconf_path = argv[1];
	SX tpl = strlen(gameconf_path);
	cwk_path_get_dirname(gameconf_path, &tpl);
	static CHR gcd[MAX_PATH_LENGTH];
	gameconf_dir = (CHR *)gcd;
	strncpy(gameconf_dir, gameconf_path, tpl);
	FILE* fp;
	CHR toml_errbuf[255];
	if (0 == (fp = fopen(gameconf_path, "r")))
		errwindow("Unable to open game config file!\n(Path: %s)\n\nValue of ERRNO: %u\n"
			"ERRNO Translation: %s", gameconf_path, errno, strerror(errno));
	gameconf = toml_parse_file(fp, toml_errbuf, sizeof(toml_errbuf));
	fclose(fp);      
	if (0 == gameconf)
		errwindow("Critical issue with game config file parsing stage.\n\nTOML Error: %s", toml_errbuf);

	/* Set the window flags and OpenGL version */
	primarywin.winflags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN;
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	CHR *title = get_conf_prop_err("window.title", gameconf, TOML_STRING).u.d.u.s;
	if (!(primarywin.window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		get_conf_prop_err("window.width", gameconf, TOML_INT).u.d.u.i,
		get_conf_prop_err("window.height", gameconf, TOML_INT).u.d.u.i,
		primarywin.winflags)))
		errwindow("Unable to create the primary window!\n SDL Error: %s", SDL_GetError());
	free(title);

	if (!(primarywin.glcontext = SDL_GL_CreateContext(primarywin.window)))
		errwindow("Unable to create GL context! Does your device support OpenGL?\n"
			"Are you sure you're using the very latest versions of your graphics drivers?\n"
			"You might be able to resolve this by using Mesa software rendering.\n\n"
			"SDL Error: %s", SDL_GetError());

	/* Initialize GLEW */
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if(glewError != GLEW_OK) errwindow("Error initializing GLEW! %s\n", glewGetErrorString(glewError));

	/* Initialize shaders - compiling, linking to programs, reading configs, etc */
	init_shaders();

	/* This makes our buffer swap syncronized with the monitor's vertical refresh */
	SDL_GL_SetSwapInterval(1);

	if (get_conf_prop_err("window.start_fullscreen", gameconf, TOML_BOOL).u.d.u.b) togglefullscreen();

	glClearColor(1.f, 0.f, 1.f, 0.f);

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

		glClear(GL_COLOR_BUFFER_BIT);
		use_shader_program("puckprog", true);

		SDL_GL_SwapWindow(primarywin.window);
	}

	cleanup();
	return EXIT_SUCCESS;
}
