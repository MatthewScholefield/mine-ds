project(
	'mine-ds',
	'cpp',
	default_options : ['cpp_std=c++11']
)

devkitpro = '/opt/devkitpro'
devkitarm = devkitpro + '/devkitARM'

add_global_arguments(
    '-g', '-Wall', '-Werror', '-Wfatal-errors',
    '-O3', '-DARM9', '-DNITRO_AND_FAT', language : 'cpp'
)
add_global_link_arguments(
    '-Lgenerated', '-specs=ds_arm9.specs', '-g', '-mthumb',
    '-mthumb-interwork', '-march=armv5te', '-mtune=arm946e-s',
    language : 'cpp'
)

compiler = meson.get_compiler('cpp')

deps = []
foreach libname : ['dswifi9', 'mm9', 'filesystem', 'fat', 'nds9']
    deps += [compiler.find_library(libname, dirs : [devkitpro + '/libnds/lib'])]
endforeach

inc = [
    include_directories('source'),
    include_directories(devkitpro + '/libnds/include')
]

subdir('gfx')
subdir('audio')
subdir('source')
