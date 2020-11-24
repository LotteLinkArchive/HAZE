#include "sdlimp.h"
#include "engine.h"

void fataltrigger(CHR *s)
{
	printf("ERROR: %s\n", s);

	SDL_Quit();
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "HAZE Fatal Exception", s, NULL);

	exit(EXIT_FAILURE);
}
