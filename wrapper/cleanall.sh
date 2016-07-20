#! /bin/bash

#cd /home/cylee/alps/wrapper/SecureApp/wrapStub/
cd ./SecureApp/wrapStub/
make clean
#cd /home/cylee/alps/wrapper/
cd ../../

echo "\nSecure : Stub was cleaned\n"

#cd /home/cylee/alps/wrapper/NormalApp/wrapLib/
cd ./NormalApp/wrapLib/
make clean
#cd /home/cylee/alps/wrapper/
cd ../../

echo "\nNormal : Lib was cleaned\n"
