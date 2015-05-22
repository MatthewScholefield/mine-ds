del c:\fcsrimage\Mine-DS.nds
cp Mine-DS.nds c:\fcsrimage\Mine-DS.nds
cd c:\fcsrimage\
padbin.exe 512 Mine-DS.nds
call build.bat image.img fcsr
del final.nds
copy /b Mine-DS.nds + image.img final.nds
dlditool.exe fcsr.dldi final.nds
del c:\Users\Matthew\Documents\GitHub\Mine-DS\EmulatorVersion.nds
cp final.nds c:\Users\Matthew\Documents\GitHub\Mine-DS\EmulatorVersion.nds