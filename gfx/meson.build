images = [
    'block_small',
    'font',
    'mobs',
    'particles',
    'sub',
    'sub_bg'
]

grit = find_program('grit', '/opt/devkitpro/tools/bin/grit')
grit_gen = generator(
    grit,
    output  : ['@BASENAME@.s'],
    arguments : ['@INPUT@', '-fts', '-o@BUILD_DIR@/@BASENAME@']
)
images_with_ext = []
foreach image : images
    images_with_ext += [image + '.png']
endforeach
image_sources = grit_gen.process(images_with_ext)
