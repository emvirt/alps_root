#! /bin/bash

cd browser

if [ ! -c /dev/mydev ]
then
	mknod /dev/mydev c 240 32
fi

echo "Start Browser\n"

./fancybrowser -platform eglfs file:///root/browser/cnn/index.html &

cd ../
