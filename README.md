# Tools
### Texture Compiler
Used to combine 3 png images into one Mine DS texture file using grit. It requires grit in the system path (Or the same folder).

**Usage:** `texCompiler [FOLDER]`

The folder should contain the following:
- blocks.png
- mobs.png
- UI.png

#### Detailed instructions (Windows)

 - Download this repository as a zip file with the button on this page
 - Extract the zip file to any location
 - Inside the `Texture Compiler/Binaries/Windows/` folder create a new folder with the name of your texture pack
 - Place the three files called `blocks.png`, `mobs.png`, and `UI.png` into this new folder
 - Lastly, open up a command prompt and navigate to the the place where you extracted the files. Once you are in the `Texture Compiler/Binaries/Windows/` folder run `texConvert.exe MyTexture` where `MyTexture` is the name of the folder with the 3 files in it
