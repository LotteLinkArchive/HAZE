#ifndef HZENGIN_HEADER_FILE
#define HZENGIN_HEADER_FILE
#include "sdlimp.h"
#include "../shared/hzshared.h"
/* Display a fatal error via SDL */
void errwindow(CHR *s);

struct hzwinprop {
	SDL_Window *window;
	SDL_GLContext glcontext;
	U32 winflags;
	INAT width;
	INAT height;
	U1 quit;
	U1 fullscreen;
};

extern struct hzwinprop primarywin;
#endif
