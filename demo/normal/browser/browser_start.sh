#! /bin/bash

mknod /dev/mydev c 240 32
./fancybrowser -platform eglfs file:///root/browser/index.html
