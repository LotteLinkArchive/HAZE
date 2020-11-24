#define SDL_MAIN_HANDLED
#include "sdlimp.h"
#include <stdio.h>
#include <string.h>
#include "../shared/global.h"
#include "engine.h"

INAT main(INAT argc, CHR *argv[])
{
	printf("-- it works --\n");
	fataltrigger("test");
}
