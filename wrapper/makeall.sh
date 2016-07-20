#! /bin/bash

#cd /home/cylee/alps/wrapper/SecureApp/wrapStub/
cd ./SecureApp/wrapStub/
make
#cd /home/cylee/alps/wrapper/
cd ../../

echo "\nSecure : Stub was compiled\n"

#cd /home/cylee/alps/wrapper/NormalApp/wrapLib/
cd ./NormalApp/wrapLib/
make
#cd /home/cylee/alps/wrapper/
cd ../../

echo "\nNormal : Lib was compiled\n"
