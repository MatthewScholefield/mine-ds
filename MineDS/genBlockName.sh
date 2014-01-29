cd source
uniq -f 2 blockID.h>blockName.cpp
perl -p -i -e 's/#define (.)([^_ ]*)(?(?=_)(_.)([^_ ]*)(?(?=_)(_.)([^ ]*)))/case \1\2\3\4\5\6: name = "\1\L\2\E\3\L\4\E\5\L\6\E"; break;/' blockName.cpp
perl -p -i -e 's/case ([^"]*)"([^_"]*)(?(?=_)_([^_"]*)(?(?=_)_([^"]*)))/\tcase \1"\2 \3 \4/' blockName.cpp
perl -p -i -e 's/([a-zA-Z]) (?(?= ) (?(?= ) ))"/\1"/' blockName.cpp
perl -p -i -e 's/([^:]*: name = [^;]*; break;) [0-9]*/\1/' blockName.cpp
perl -p -i -e '$_="" unless /case AIR: name = "Air"; break;/ .. 1' blockName.cpp
sed -i '1s/^/#include "blockID.h"\n\nconst char *getName(int blockID)\n{\n\tconst char *name;\n\tswitch (blockID)\n\t{\n/' blockName.cpp
sed -i '$s/$/\n\t}\n\treturn name;\n}/' blockName.cpp