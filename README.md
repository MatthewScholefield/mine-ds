# Mine-DS
A 2D survival NDS game inspired by Minecraft. The current version incorperates almost all survival and creative elements.

![Screenshot][SS]

### Features
- Inventory
- Furnaces
- Water
- Food
- Crafting
- Zombies
- Smooth, responsive UI
- Grass + Tree growth
- Stereo sounds based on location
- Particles (Furnace Fire + **Water splashes**)
- Oceans/Plains/Jungles/Deserts/Tundra/Mushroom Biomes
- Various options (Disable regen, audio, graphics)

# How to get involved
- Look at the issues page to see some items that need to get added
- Setup the build environment
- Look at some of the code related to that aspect
- Message with any questions
- Submit a pull request

# How to Compile

To build Mine DS you need to download a group of tools build by [devkitPro](https://devkitpro.org/wiki/Getting_Started).

#### Windows
- Download the latest exe devkitpro [installer from here](https://devkitpro.org/wiki/Getting_Started#Windows)
- Run the downloaded installer and check "devkitARM" in the custom install
- Download the project to a directory and in a command prompt type "make"

#### Linux
- Follow [these instructions](https://devkitpro.org/wiki/Getting_Started#Unix-like_platforms) (under "Unix-like Platforms")
- Run `sudo dkp-pacman -S nds-dev` (or `pacman` if running arch)
- Download the Mine DS source to a directory and in a command prompt type "make"

# Contact
Contact me with any questions at [matthew331199@gmail.com][2]

[1]:http://sourceforge.net/projects/devkitpro/files/Automated%20Installer/
[2]:mailto://matthew331199@gmail.com
[SS]:http://i.imgbox.com/ntnI7Sek.png "Screenshot"
