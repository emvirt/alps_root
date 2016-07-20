#/bin/bash

if [ ! -c /dev/mydev ]
then
       mknod /dev/mydev c 240 32
fi

./reset_tz_log_counter
