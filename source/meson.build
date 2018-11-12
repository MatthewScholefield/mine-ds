sources = files(
    'FileSystem.cpp',
    'utils.cpp',
    'adpcmparse.cpp',
    'shittywavparse.cpp',
    'SoundSystem.cpp',
    'SmoothCoord.cpp',
    'MessageSystem.cpp',
    'main.cpp',
    'Player.cpp',
    'Block.cpp'
)

subdir('graphics')
subdir('world')

elf = executable(
    'mine-ds.elf', [sources, image_sources, soundbank_h],
    include_directories : inc,
    dependencies : deps
)

ndstool = find_program('ndstool', devkitpro + '/tools/bin/ndstool')

nds = custom_target(
    'mine-ds.nds',
    build_by_default : true,
    output : 'mine-ds.nds',
    input : [elf, '../icon.bmp', nitro_folder],
    command : [
        ndstool, '-c', '@OUTPUT@', '-9', '@INPUT0@', '-b', '@INPUT1@',
        'Mine DS v0.4.1;A 2D Minecraft;inspired homebrew',
        '-d', '@INPUT2@'
    ]
)
