mkdir tmp
mv pack tmp/pack
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
sed -E "s/#define NUM_BLOCKS ([0-9]*)/``REMOVE``\n\1/" mining.h | sed -e '0,/^``REMOVE``$/d' | sed -n -e "1p" > max.txt

:: Assign to m
set /p m=<max.txt

set n=1
:loop
set /A mod=(n) %% 16
set /A x=(mod)*16
set /A mult=(n)/16
set /A y=mult*16

sed -n "%n%p" par1.txt | xargs -I ` find . -regextype sed -iregex ".*[^a-z]`[^a-z].*" > searchResults.txt
cat searchResults.txt
sed -n "2p" searchResults.txt > second.txt
set /p result=<second.txt
if "%result%"=="" goto skip
sed '%n%!d' par2.txt > wrd2.txt
set /p wrd=<wrd2.txt
if "%wrd%"=="" goto skip
grep -i -E ".*[^a-z]%wrd%[^a-z].*" searchResults.txt > refined.txt
sed -i '1!d' refined.txt
set /p ref=<refined.txt
if "%ref%"=="" goto skip
cat refined.txt > searchResults.txt
:skip
cat searchResults.txt | sed -n -e "1p" | sed -E -e "s/.\/(.*)/\1/" | xargs -I ` composite -geometry +%x%+%y% ` final.png final.png

set /A n=n+1
if "%n%"=="%m%" goto end
goto loop
:end
cd ..
mv tmp/pack pack
mv tmp/final.png final.png
del /F /Q tmp