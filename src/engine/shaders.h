#ifndef HZSHAD_HEADER_FILE
#define HZSHAD_HEADER_FILE
#include "../shared/hzshared.h"

/* DO NOT FORGET TO SPELL ENGLISH TRADITIONALLY! YOU WILL BREAK EVERYTHING IF YOU SPEAK SIMPLIFIED ENGLISH! */
enum hz_vertex_formats {      /* F = R32 */
	HZ_NO_FORMAT,         /* DO NOT USE "NO FORMAT" - INTERNAL USE ONLY */
	HZ_VERTEX_ONLY,       /* XF YF ZF */
	HZ_COLOUR_VERTEX,     /* XF YF ZF RF GF BF */
	HZ_COLOUR_TEX_VERTEX, /* XF YF ZF RF GF BF SF TF */
	HZ_TEX_VERTEX,        /* XF YF ZF SF TF */
	HZ_TEX_LMAP_VERTEX    /* XF YZ ZF SF TF SlF TlF */
};

X0 init_shaders();

INAT get_uniform_location(const CHR *program, const CHR *uniform);
X0 btset_vertex_format(U8 vtype);
X0 use_shader_program(const CHR *name, U1 set_format);

struct hz_bglobj {
	U64 namehash;
	union {
		UNAT u;
		INAT i;
	} glid;
	U8 vtype;
	U1 failed;
};

/* The shaders shouldn't be public, should they? Just the programs?
UNAT get_shader_id(const CHR *name);
extern struct hz_bglobj *glshaders;
extern UNAT glshader_count;
*/

extern struct hz_bglobj *glprograms;
extern UNAT glprogram_count;

extern struct hz_bglobj *gluniforms;
extern UNAT gluniform_count;
#endif