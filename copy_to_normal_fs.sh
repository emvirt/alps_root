#! /bin/bash

echo "mount directory = "$1

if [ $1 ]
then {
	# copy binaries to the sd card for the normal world
	cp ./wrapper/NormalApp/wrapLib/*.h $1/usr/include/wrapLib/
	cp ./wrapper/NormalApp/wrapLib/libwrapglLib.so $1/usr/lib/

#	cp ./wrapper/NormalApp/SimpleTriangle/normal_triapp $1/root/
#	cp ./wrapper/start_normal_app.sh $1/root/

	# copy binaries to my root filesystem for the normal world
	cp ./wrapper/NormalApp/wrapLib/*.h /home/cylee/rootfs_normal/usr/include/wrapLib/
	cp ./wrapper/NormalApp/wrapLib/libwrapglLib.so /home/cylee/rootfs_normal/usr/lib/

#	cp ./wrapper/NormalApp/SimpleTriangle/normal_triapp /home/cylee/rootfs_normal/root/
}
else {
	echo "put a mount directory"
}
fi

