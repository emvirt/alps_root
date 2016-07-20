#! /bin/bash

cd ./secure/linux-3.0.35/
make clean
cd ../../
echo "\nSecure linux was cleaned\n"

cd ./normal/linux-3.0.35/
make clean
cd ../../
echo "\nNormal linux was cleaned\n"
