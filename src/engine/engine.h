#ifndef HZENGIN_HEADER_FILE
#define HZENGIN_HEADER_FILE
#include "sdlimp.h"
#include <stdarg.h>
#include "../shared/hzshared.h"
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

/* Display a fatal error via SDL */
H_EXTERN X0 errwindow(const CHR *s, ...);
/* Toggle primary window fullscreen state */
H_EXTERN X0 togglefullscreen();
/* Malloc/realloc which reports memory errors and exits. */
H_EXTERN X0 *oomrealloc(X0 *ptr, SX size);
#endif
