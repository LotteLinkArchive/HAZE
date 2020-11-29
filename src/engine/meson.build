hazemainsrc = [
	'main.c',
	'engine.c',
	'enumstr.c',
	'shaders.c',
	'pathy.c',
	'textures.c'
]
executable('haze', hazemainsrc, link_with : hzshared,
	dependencies : [sdl2_dep, thread_dep, gl_dep, hztoml_dep, glew_dep, cwalk_dep], install : true)