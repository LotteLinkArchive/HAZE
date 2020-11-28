hazemainsrc = [
	'main.c',
	'engine.c'
]
executable('haze', hazemainsrc, link_with : hzshared,
	dependencies : [sdl2_dep, thread_dep, gl_dep, hztoml_dep, glew_dep], install : true)