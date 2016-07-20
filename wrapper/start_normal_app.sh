#! /bin/bash

if [ $# == 1 ]
then {
        mknod /dev/mydev c 240 32
        ./$1
}
else {
        echo "usage : "
        echo "  put a application binary file"
        echo "  ./start_normal_app.sh {application}"
}
fi
