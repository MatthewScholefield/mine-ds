@echo off
setlocal EnableDelayedExpansion
ls -d */ | tr '/' '\n' | sed -e "/^$/d" > folder.txt
nl folder.txt
set /p folderID=Enter the number of the directory you would like to convert: 
head -%folderID% folder.txt | tail -1 > setFolder.txt
set /p folder=<setFolder.txt

mkdir tmp
mv %folder% tmp/pack
cp default.txt tmp/default.txt
cd tmp

:: Retreives blockID.h and mining.h without downloading everything
git clone --no-checkout --depth 1 https://github.com/Wolfgange3311999/Mine-DS.git
cd Mine-DS
git show HEAD:source/blockID.h > blockID.h
git show HEAD:source/mining.h > mining.h
cd ..
mv Mine-DS/blockID.h blockID.h
mv Mine-DS/mining.h mining.h
cp blockID.h tmp.txt

:: Removes the line with "AIR 0" and before
sed -i '0,/^AIR 0$/d' tmp.txt

:: Removes duplicates
uniq -f 2 tmp.txt>names.txt

:: Corrects line endings
dos2unix names.txt

:: Extract name
sed -i -E "s/#define ([^ ]*) [0-9]*/\1/" names.txt

:: Extract first word
sed -E "s/^([^_]*).*$/\1/" names.txt > par1.txt

:: Remove single worded names
sed -E "s/^[^_]*$//" names.txt > par2.txt

:: Remove first and last words
sed -i -E "s/^[^_]*_([^_]*)_?[^_]*$/\1/" par2.txt

:: Remove first and second words
sed -E "s/^[^_]*_?[^_]*_?([^_]*$)/\1/" names.txt > par3.txt

:: Create pink (Transparent) background base image
convert -size 256x256 canvas:rgb(256,0,256) final.png

:: Extract max blocks from mining.h
sed -E "s/#define NUM_BLOCKS ([0-9]*)/``REMOVE``\n\1/" mining.h | sed -e '0,/^``REMOVE``$/d' | head -1 > max.txt

:: Assign to m
set /P m=<max.txt
set /A m=m+1

set useDefault=0

if exist default.txt (
set "useDefault="
set /P useDefault=Default Values found. Use them ^(Y/n^)? 
)
if not defined useDefault set useDefault=1
if /I "%useDefault%" EQU "Y" set useDefault=1
if "%useDefault%"=="1" (
 cls
 echo Converting texture pack ...
 goto after
)
set useDefault=0
echo.>default.txt
set loop=1
<nul set /p ="Creating default text file "
:generate
echo.>>default.txt
if "%loop%"=="%m%" goto after
set /A loop=loop+1
<nul set /p ="."
goto generate

:after
if "%useDefault%"=="1" goto doNotAsk
set "c="
cls
set /P c=Ask for each file (y/N)? 
if not defined c set useDefault=2
if /I "%c%" EQU "Y" set useDefault=0
if /I "%c%" EQU "N" set useDefault=2
:doNotAsk

set n=1
:loop
set l=0
set custom=0
set /A mod=(n) %% 16
set /A x=(mod)*16
set /A mult=(n)/16
set /A y=mult*16

set "first="
head -%n% par1.txt | tail -1 > first.txt
set /p first=<first.txt

if "%useDefault%"=="0" goto guess
if "%useDefault%"=="2" goto guess
head -%n% default.txt | tail -1 > line.txt
set /P line=<line.txt
:: SNOW_TOP
if "%n%"=="18" (
 set /A y=^(15+y^)
 convert -crop 16x1+0+15 pack/assets/minecraft/textures/blocks/snow.png miff:- | composite -geometry +!x!+!y! - final.png final.png
) else if "%n%"=="75" (
 call:drawChest
) else if "%n%"=="11" (
 set input=%line%
 call:colorize 205 255 180
) else if "%n%"=="12" (
 set input=%line%
 call:colorize 190 255 190 85
) else if "%n%"=="13" (
 set input=%line%
 call:colorize 150 255 130 80
) else if "%n%"=="51" (
 set input=%line%
 call:colorize 180 255 180
) else (
 convert -crop 16x16+0+0 %line% gif:- | composite -geometry +%x%+%y% - final.png final.png
)
<nul set /p ="."

set /A n=n+1
if "%n%"=="%m%" goto end
goto loop

:guess
head -%n% par1.txt | tail -1 | xargs -I ` find . -regextype sed -iregex ".*[^a-z]`[^a-z].*png$" > searchResults.txt
head -%n% par2.txt | tail -1 > wrd2.txt
set "wrd="
set /p wrd=<wrd2.txt
if not defined wrd goto complete
if "%wrd%"=="" goto complete
grep -i -E ".*[^a-z]%wrd%[^a-z].*" searchResults.txt | head -1 > refined.txt
set "ref="
set /p ref=<refined.txt
if not defined ref goto complete
if "%ref%"=="" goto complete
mv searchResults.txt new.txt
grep -i -E ".*[^a-z]%wrd%[^a-z].*" new.txt > searchResults.txt

:complete
set /A percent=(n*100)/m
set "nullTest="
set /p nullTest=<searchResults.txt

if "%useDefault%"=="2" (
 if defined nullTest (
  echo %percent%%%
  goto continue
 ) else (
  goto searchTerm
 )
)
cls
if not "%l%"=="0" (
 head -%l% searchResults.txt | tail -1
) else (
 awk '{ print length, $0 }' searchResults.txt | sort -n | cut -d" " -f2- | head -1
)
echo "for the BLOCK:"
head -%n% names.txt | tail -1

set "c="
set /P "c=Is this correct (Y/n)? "
if not defined c goto continue
if /I "%c%" EQU "Y" goto continue
if /I "%c%" EQU "N" goto customize
echo Enter either "Y" or "N"
pause
goto complete

:customize
set custom=1
cls
echo ==Search Results==
cat searchResults.txt
echo.
echo (Starting at 1. Enter 0 to customize search term)
set /P setLine=Which search result is correct? 
if NOT "%setLine%"=="0" (
 set l=%setLine%
 goto complete
)
:searchTerm
cls
head -%n% names.txt | tail -1
set /P search=Enter a search term for the block above:
find . -regextype sed -iregex ".*[^a-z]%search%[^a-z].*png$" > searchResults.txt
goto customize

:continue
:: SNOW_TOP
if "%n%"=="15" (
 convert -crop 16x1+0+15 pack/assets/minecraft/textures/blocks/snow.png tmp.png
 set /A y=y+15
 composite -geometry +%x%+%y% tmp.png final.png final.png
 echo pack/assets/minecraft/textures/blocks/snow.png> fin.txt
) else if "%n%"=="75"(
 call:drawChest
 echo %chest%>fin.txt
) else if "%n%"=="11" (
 awk '{ print length, $0 }' searchResults.txt | sort -n | cut -d" " -f2- | head -1 | sed -E -e "s/\.\\/(.*)/\1/" > fin.txt
 set /p input=<fin.txt
 call:colorize 205 255 180
) else if "%n%"=="12" (
 awk '{ print length, $0 }' searchResults.txt | sort -n | cut -d" " -f2- | head -1 | sed -E -e "s/\.\\/(.*)/\1/" > fin.txt
 set /p input=<fin.txt
 call:colorize 110 255 110 50
) else if "%n%"=="13" (
 awk '{ print length, $0 }' searchResults.txt | sort -n | cut -d" " -f2- | head -1 | sed -E -e "s/\.\\/(.*)/\1/" > fin.txt
 set /p input=<fin.txt
 call:colorize 150 255 130 80
) else if "%n%"=="51" (
 awk '{ print length, $0 }' searchResults.txt | sort -n | cut -d" " -f2- | head -1 | sed -E -e "s/\.\\/(.*)/\1/" > fin.txt
 set /p input=<fin.txt
 call:colorize 180 255 180
) else (
 if "%l%"=="0" (
  awk '{ print length, $0 }' searchResults.txt | sort -n | cut -d" " -f2- | head -1 | sed -E -e "s/\.\\/(.*)/\1/" | tee fin.txt | xargs -I ` convert -crop 16x16+0+0 ` gif:- | composite -geometry +%x%+%y% - final.png final.png
 ) else (
  cat searchResults.txt | head -%l% | tail -1 | sed -E -e "s/\.\\/(.*)/\1/" | tee fin.txt | xargs -I ` convert -crop 16x16+0+0 ` gif:- | composite -geometry +%x%+%y% - final.png final.png
 )
)
cat fin.txt | sed -E -e "s/\\//\\\\\//g" > output.txt
set /p output=<output.txt
sed -i "%n%s/.*/%output%/" default.txt
set /A n=n+1
if "%n%"=="%m%" goto end
goto loop
:end
cd ..
mv tmp/pack %folder%
mv tmp/final.png %folder%.png
cp tmp/default.txt default.txt
rm -rf tmp
rm -f setFolder.txt
rm -f folder.txt
goto:eof

::-----------------------------
::functions here
::-----------------------------
:drawChest
set chest=pack/assets/minecraft/textures/entity/chest/normal.png
set /A x=x+1
set /A y=y+1
convert -crop 14x5+14+14 %chest% miff:- | composite -geometry +%x%+%y% - final.png final.png
set /A y=y+5
convert -crop 14x10+14+33 %chest% miff:- | composite -geometry +%x%+%y% - final.png final.png
set /A x=x+6
set /A y=y-2
convert -crop 2x4+2+1 %chest% miff:- | composite -geometry +%x%+%y% - final.png final.png
goto:eof

:colorize
set "amount=%~4"
if not defined amount set amount=90
convert %input% -fill rgb(%~1,%~2,%~3) -tint %amount% gif:- | composite -geometry +%x%+%y% - final.png final.png
goto:eof