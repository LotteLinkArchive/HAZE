project('HAZE', 'c')

cc = meson.get_compiler('c')

m_dep = cc.find_library('m', required : false)
thread_dep = dependency('threads')
sdl2_dep = dependency('sdl2')
gl_dep = dependency('GL')
glew_dep = dependency('glew')
hztoml_dep = cc.find_library('hztoml', required : true)
cwalk_dep = cc.find_library('cwalk', required : true)
cglm_dep = dependency('cglm')
xxhash_dep = cc.find_library('xxhash', required : true)

#hztoml_proj = subproject('libhztoml')
#hztoml_dep = hztoml_proj.get_variable('hztoml_dep')

subdir('src')
