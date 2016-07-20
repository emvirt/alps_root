#! /bin/bash

echo "mount directory = "$1

if [ $1 ]
then {
	# copyt binaries to the sd card for the secure world
#	cp ./hook_secure_display/*.h $1/usr/include/EGL/
#	cp ./hook_secure_display/libhalfdpy.so $1/usr/lib/

	cp ./wrapper/SecureApp/wrapStub/*.h $1/usr/include/wrapStub/
	cp ./wrapper/SecureApp/wrapStub/secure_glStub $1/root/glStub
#	cp ./wrapper/start_secure_stub.sh $1/root/

	# copy binaries to my root filesystem for the secure world
#	cp ./hook_secure_display/*.h /home/cylee/rootfs_secure/usr/include/EGL/
#	cp ./hook_secure_display/libhalfdpy.so /home/cylee/rootfs_secure/usr/lib/

	cp ./wrapper/SecureApp/wrapStub/*.h /home/cylee/rootfs_secure/usr/include/wrapStub/
	cp ./wrapper/SecureApp/wrapStub/secure_glStub /home/cylee/rootfs_secure/root/
}
else {
	echo "put a mount directory"
}
fi

