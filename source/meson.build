sources = [
    'mainGame.cpp',
    'general.cpp',
    'fontHandler.cpp',
    'communications.cpp',
    'blocks.cpp',
    'world/worldGen.cpp',
    'world/World.cpp',
    'world/worldRender.cpp',
    'audio/adpcmparse.cpp',
    'audio/shittywavparse.cpp',
    'Config.cpp',
    'mining.cpp',
    'Furnace.cpp',
    'InvBlock.cpp',
    'blockName.cpp',
    'graphics/handlers/InvGfxHandler.cpp',
    'graphics/subBgHandler.cpp',
    'graphics/UIElement.cpp',
    'graphics/graphics.cpp',
    'graphics/Button.cpp',
    'graphics/Menu.cpp',
    'graphics/UI.cpp',
    'graphics/Slider.cpp',
    'graphics/CheckButton.cpp',
    'graphics/3DHandler.cpp',
    'graphics/particleHandler.cpp',
    'graphics/interfaces/InventoryInterface.cpp',
    'graphics/interfaces/DeathScreenInterface.cpp',
    'graphics/interfaces/CraftingInterface.cpp',
    'graphics/interfaces/ChestInterface.cpp',
    'graphics/interfaces/Interface.cpp',
    'graphics/interfaces/FurnaceInterface.cpp',
    'graphics/interfaces/interfaceHandler.cpp',
    'graphics/interfaces/PageInterface.cpp',
    'blockPages.cpp',
    'biomes.cpp',
    'titleScreen.cpp',
    'chests.cpp',
    'trees.cpp',
    'inventory.cpp',
    'collision.cpp',
    'blockUpdaters/JungleLeafUpdater.cpp',
    'blockUpdaters/MyceliumUpdater.cpp',
    'blockUpdaters/OakSaplingUpdater.cpp',
    'blockUpdaters/OakLeafUpdater.cpp',
    'blockUpdaters/LadderUpdater.cpp',
    'blockUpdaters/blockUpdater.cpp',
    'blockUpdaters/JungleGrassUpdater.cpp',
    'blockUpdaters/SpruceSaplingUpdater.cpp',
    'blockUpdaters/AirUpdater.cpp',
    'blockUpdaters/CactusUpdater.cpp',
    'blockUpdaters/WaterUpdater.cpp',
    'blockUpdaters/plantUpdaters.cpp',
    'blockUpdaters/SnowGrassUpdater.cpp',
    'blockUpdaters/DoorUpdater.cpp',
    'blockUpdaters/SnowTopUpdater.cpp',
    'blockUpdaters/LitFurnaceUpdater.cpp',
    'blockUpdaters/RedwoodLeafUpdater.cpp',
    'blockUpdaters/GrassUpdater.cpp',
    'blockUpdaters/FurnaceUpdater.cpp',
    'blockUpdaters/JungleSaplingUpdater.cpp',
    'blockUpdaters/DirtUpdater.cpp',
    'sounds.cpp',
    'main.cpp',
    'dayNight.cpp',
    'FixedPoint.cpp',
    'nifi.cpp',
    'mobs/mobHandler.cpp',
    'mobs/ZombieMob.cpp',
    'mobs/AnimalMob.cpp',
    'mobs/mobFunctions.cpp',
    'mobs/PlayerMob.cpp',
    'mobs/MultiplayerMob.cpp',
    'mobs/BaseMob.cpp',
    'mobs/ItemMob.cpp',
    'mobs/mobCollisions.cpp',
    'mobs/WaterMob.cpp',
    'mobs/HerobrineMob.cpp',
    'deathScreen.cpp',
    'files.cpp',
    'localizations/locale.cpp',
    'furnaceHandler.cpp'
]

lang_files = [
    'localizations/DE.lang',
    'localizations/EN.lang',
    'localizations/EO.lang'
]

lang_outputs = [
    'DE.h',
    'EN.h',
    'EO.h'
]

localize_prog = find_program(meson.source_root() + '/localize.py')
localize_sources = custom_target(
    'localizations',
    input : lang_files,
    output : lang_outputs,
    command : [localize_prog, '@INPUT@', '-o', '@OUTDIR@']
)

elf = executable(
    'mine-ds.elf', [sources, image_sources, soundbank_h, localize_sources],
    include_directories : inc,
    dependencies : deps
)

ndstool = find_program('ndstool', devkitpro + '/tools/bin/ndstool')

nds = custom_target(
    'mine-ds.nds',
    output : 'mine-ds.nds',
    input : elf,
    command : [
        ndstool, '-c', '@OUTPUT@', '-9', '@INPUT@', '-b', '../icon.bmp',
        'Mine DS v0.4.1;A 2D Minecraft;inspired homebrew',
        '-d', '../nitro'
    ],
    install : true,
    install_dir : 'subdir'
)
