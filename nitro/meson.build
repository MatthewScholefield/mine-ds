
nitro_files = [
    soundbank_bin,
    'soundtrack.wav'
]

nitro_folder = custom_target(
    'nitro-data',
    input : [nitro_files],
    output : '.',
    command : [find_program('cp'), '-t', '@OUTPUT@', '@INPUT@']
)
