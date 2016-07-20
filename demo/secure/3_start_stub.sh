#! /bin/bash

cd glStub

if [ ! -c /dev/mydev ]
then
	mknod /dev/mydev c 240 32
fi

echo "Start OpenGL ES Stub\n"

./secure_glStub &

cd ../
