#!/bin/sh
rm /dev/conv
rmmod conv
insmod /lib/modules/4.4.45/extra/conv.ko
mknod -m 777 /dev/conv c 200 0
