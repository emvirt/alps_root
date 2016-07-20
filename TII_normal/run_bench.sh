#/bin/bash

if [ ! -c /dev/mydev ]
then
       mknod /dev/mydev c 240 32
fi

benchmark_list="R W"
#file_size_list="1 2 4 6 8 10 20 40 60 80 100 200 400 600 800 1000"
#Unit is Byte!!
file_size_list="100 200 300 400"
input_file="/root/input_file"
benchapp="./TZSDCard_bench"

./reset_tz_log_counter

for i in $(seq 1 100)
do
        echo "$i times..."
        for benchmark in ${benchmark_list}
        do
                if [ "R" == ${benchmark} ]
                then
                        echo Read Performance Experiment
                fi
                if [ "W" == ${benchmark} ]
                then
                        echo Write Performance Experiment
                fi
                if [ "B" == ${benchmark} ]
                then
                        echo RW Performance Experiment
                fi
                for file_size in ${file_size_list}
                do
                        ${benchapp} ${input_file} ${benchmark} ${file_size}
                done
        done
        sleep 1
done