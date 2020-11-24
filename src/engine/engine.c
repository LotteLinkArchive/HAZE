#include "sdlimp.h"
#include "engine.h"

void errwindow(CHR *s)
{
	printf("ERROR: %s\n", s);

	SDL_Quit();
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "HAZE Fatal Exception", s, NULL);

	exit(EXIT_FAILURE);
}
