#! /bin/bash

cd ./secure/linux-3.0.35/
make -j24 uImage
cd ../../
echo "\nSecure linux was compiled\n"

cd ./normal/linux-3.0.35/
make -j24 uImage
cd ../../
echo "\nNormal linux was compiled\n"
