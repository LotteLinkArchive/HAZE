#include "shaders.h"
#include "engine.h"
#include <cwalk.h>
#include <hztoml/toml.h>
#include <stdio.h>
#include "config.h"
#include "enumstr.h"
#include "pathy.h"

struct hz_bglobj *glshaders = NULL;
UNAT glshader_count = 0;

struct hz_bglobj *glprograms = NULL;
UNAT glprogram_count = 0;

struct hz_bglobj *gluniforms = NULL;
UNAT gluniform_count = 0;

U8 current_vertex_format = HZ_NO_FORMAT;

UNAT get_shader_id(const CHR *name)
{
	struct hz_bglobj obj = get_bglobj(glshaders, glshader_count, STRHASH(name));
	if (!obj.failed) return obj.glid.u;

	return 0;
}

X0 init_shaders()
{
	CHR pbuf[MAX_PATH_LENGTH + 1], sbuf[MAX_PATH_LENGTH + 1];
	CHR *depot = get_conf_prop_err("glsl.depot", gameconf, TOML_STRING).u.d.u.s;
	cwk_fspath_join(gameconf_dir, depot, (CHR *)pbuf, MAX_PATH_LENGTH);
	free(depot);

	CHR infolog[SHADER_LOG_SIZE + 1];

	toml_table_t *shaders = get_conf_prop_err("glsl.shaders", gameconf, TOML_TABLE).u.t;
	for (UNAT i = 0; ; i++) {
		const CHR* key = toml_key_in(shaders, i);
		if (!key) break;
		toml_table_t *shader = toml_table_in(shaders, key);
		CHR *sdat = get_conf_prop_err("type", shader, TOML_STRING).u.d.u.s;
		GLenum type = hz_enumstr_db(sdat);
		free(sdat);
		sdat = get_conf_prop_err("file", shader, TOML_STRING).u.d.u.s;
		cwk_fspath_join(pbuf, sdat, sbuf, MAX_PATH_LENGTH);
		free(sdat);
		
		/* We now have the name of the shader, the type, and the path */
		GLuint glshader = glCreateShader(type);
		if (!glshader) errwindow("Unable to create shader %s, is the type valid?", key);

		FILE *fp;
		if (!(fp = fopen(sbuf, "r")))
			errwindow("Unable to read shader source file (%s) for shader %s!", sbuf, key);
		/* Does this fseek even need error handling? It should be impossible for this to fail, seeking to the
		 * end of the file will never be an invalid position.
		 */
		fseek(fp, 0, SEEK_END);
		UNAT srclength = ftell(fp), srcbytes = (srclength + 1) * sizeof(CHR);
		rewind(fp);
		CHR *glslsrc = malloc(srcbytes);
		memset(glslsrc, 0, srcbytes);
		fread(glslsrc, srclength, 1, fp);
		fclose(fp);

		glShaderSource(glshader, 1, (const GLchar *const *)&glslsrc, NULL);
		free(glslsrc);
		glCompileShader(glshader);

		GLint vs_success;
		glGetShaderiv(glshader, GL_COMPILE_STATUS, &vs_success);
		if (!vs_success) {
			glGetShaderInfoLog(glshader, SHADER_LOG_SIZE, NULL, infolog);
			errwindow("!!!GLSL Shader compile failed!!! (%s)\n\n%s", key, infolog);
		}

		glshaders = oomrealloc(glshaders, (glshader_count + 1) * sizeof(struct hz_bglobj));
		struct hz_bglobj packaged = {
			.namehash = STRHASH(key),
			.glid.u = glshader,
			.failed = false	
		};
		glshaders[glshader_count] = packaged;
		glshader_count++;
	}

	sort_bglobj_list(glshaders, glshader_count);

	UNAT si;
	shaders = get_conf_prop_err("glsl.programs", gameconf, TOML_TABLE).u.t;
	for (UNAT i = 0; ; i++) {
		const CHR* key = toml_key_in(shaders, i);
		if (!key) break;
		toml_table_t *program = toml_table_in(shaders, key);

		CHR *stype = get_conf_prop_err("vformat", program, TOML_STRING).u.d.u.s;
		struct hz_bglobj glprogram = {
			.namehash = STRHASH(key),
			.glid.u = glCreateProgram(),
			.vtype = hz_enumstr_db(stype),
			.failed = false
		};
		free(stype);

		toml_array_t *shaders  = get_conf_prop_err("shaders",  program, TOML_ARRAY).u.a;
		toml_array_t *uniforms = get_conf_prop_err("uniforms", program, TOML_ARRAY).u.a;

		for (si = 0; si < toml_array_nelem(shaders); si++) {
			toml_datum_t name = toml_string_at(shaders, si);
			if (!name.ok) errwindow("Program %s contains shader names that aren't strings!", key);
			UNAT shid = get_shader_id(name.u.s);
			if (!shid) errwindow("Program %s references invalid shader %s", key, name.u.s);
			glAttachShader(glprogram.glid.u, shid);
			free(name.u.s);
		}

		glLinkProgram(glprogram.glid.u);
		GLint vs_success;
		glGetShaderiv(glprogram.glid.u, GL_COMPILE_STATUS, &vs_success);
		if (!vs_success) {
			glGetProgramInfoLog(glprogram.glid.u, SHADER_LOG_SIZE, NULL, infolog);
			errwindow("!!!GLSL Program link failed!!! (%s)\n\n%s", key, infolog);
		}

		glprograms = oomrealloc(glprograms, (glprogram_count + 1) * sizeof(struct hz_bglobj));
		glprograms[glprogram_count] = glprogram;
		glprogram_count++;

		for (si = 0; si < toml_array_nelem(uniforms); si++) {
			toml_datum_t name = toml_string_at(uniforms, si);
			if (!name.ok) errwindow("Program %s contains uniform names that aren't strings!", key);
			struct hz_bglobj gluniform = {
				.glid.i = glGetUniformLocation(glprogram.glid.u, name.u.s),
				.namehash = STRHASH(key) ^ STRHASH(name.u.s),
				.failed = false
			};
			if (gluniform.glid.i == -1)
				errwindow(
					"Error loading uniform for program %s. %s does not correspond to an active "
					"uniform variable in the program, name starts with a reserved prefix or the "
					"provided name is associated with an atomic counter or named uniform block.",
					key, name.u.s);
			free(name.u.s);

			gluniforms = oomrealloc(gluniforms, (gluniform_count + 1) * sizeof(struct hz_bglobj));
			gluniforms[gluniform_count] = gluniform;
			gluniform_count++;
		}
	}

	for (si = 0; si < glshader_count; si++) glDeleteShader(glshaders[si].glid.u);
	glshader_count = 0;
	free(glshaders);
	glshaders = NULL;

	sort_bglobj_list(gluniforms, gluniform_count);
	sort_bglobj_list(glprograms, glprogram_count);
}

INAT get_uniform_location(const CHR *program, const CHR *uniform)
{
	struct hz_bglobj obj = get_bglobj(gluniforms, gluniform_count, STRHASH(program) ^ STRHASH(uniform));
	if (!obj.failed) return obj.glid.i;

	errwindow("Attempted to retrieve unknown uniform %s for GLSL shader program %s!", uniform, program);
	return -1;
}

X0 btset_vertex_format(U8 vtype)
{
	if (current_vertex_format == vtype) goto vfsignored;
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	switch (vtype) {
	case HZ_VERTEX_ONLY:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(R32), (void*)0);
		glEnableVertexAttribArray(0);
		break;
	case HZ_COLOUR_VERTEX:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(R32), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(R32), (void*)(3 * sizeof(R32)));
		glEnableVertexAttribArray(1);
		break;
	case HZ_COLOUR_TEX_VERTEX:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(R32), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(R32), (void*)(3 * sizeof(R32)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(R32), (void*)(6 * sizeof(R32)));
		glEnableVertexAttribArray(2);
		break;
	case HZ_TEX_VERTEX:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(R32), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(R32), (void*)(3 * sizeof(R32)));
		glEnableVertexAttribArray(1);
		break;
	case HZ_TEX_LMAP_VERTEX:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(R32), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(R32), (void*)(3 * sizeof(R32)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(R32), (void*)(5 * sizeof(R32)));
		glEnableVertexAttribArray(2);
		break;
	default:
		errwindow("HZOpenGL Error: Tried to use an invalid vertex format? (%u)", vtype);
	}
	current_vertex_format = vtype;
vfsignored:
	return;
}

X0 use_shader_program(const CHR *name, U1 set_format)
{
	struct hz_bglobj program = get_bglobj(glprograms, glprogram_count, STRHASH(name));
	if (program.failed) errwindow("Failed to activate GLSL Shader Program %s", name);

	if (set_format) btset_vertex_format(program.vtype);
	glUseProgram(program.glid.u);
}
