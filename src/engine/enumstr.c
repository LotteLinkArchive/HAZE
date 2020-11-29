#include "enumstr.h"
#include "shaders.h"
#include "engine.h"

#define ENUM_TO_VAL(var, str, enum)\
	if (strcmp(str, #enum) == 0) {\
		var = enum;\
	}

I32 hz_enumstr_db(const CHR *val)
{
	I32 var;
	#define ETV_VV(e) ENUM_TO_VAL(var, val, e) 
	ETV_VV(GL_VERTEX_SHADER)
	ETV_VV(GL_FRAGMENT_SHADER)
	ETV_VV(HZ_COLOUR_VERTEX)
	ETV_VV(HZ_COLOUR_TEX_VERTEX)
	ETV_VV(HZ_TEX_VERTEX)
	ETV_VV(HZ_TEX_LMAP_VERTEX)
	ETV_VV(HZ_VERTEX_ONLY)
	#undef ETV_VV
	return var;
} 