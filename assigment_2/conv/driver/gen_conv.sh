#!/bin/bash
sudo rm /dev/conv
sudo rmmod conv
sudo insmod ./conv.ko
sudo mknod -m 777 /dev/conv c 200 0
