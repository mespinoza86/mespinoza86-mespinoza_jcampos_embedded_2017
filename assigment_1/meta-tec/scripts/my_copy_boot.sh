#!/bin/bash

echo "Copying dtbs"
for f in ${DTBS}; do
	sudo cp ${SRCDIR}/Image-${f} /media/card/${f}

	if [ $? -ne 0 ]; then
		echo "Error copying dtb: $f"
		sudo umount ${DEV}
		exit 1
	fi
done

echo "Copying kernel"
if [ "${MACHINE}" = "raspberrypi2" ]; then 
	sudo cp ${SRCDIR}/Image /media/card/kernel7.img
else
	sudo cp ${SRCDIR}/Image /media/card/kernel.img
fi

if [ $? -ne 0 ]; then
	echo "Error copying kernel"
	sudo umount ${DEV}
	exit 1
fi

if [ -f ./config.txt ]; then
	echo "Copying local config.txt to card"
	sudo cp ./config.txt /media/card

	if [ $? -ne 0 ]; then
		echo "Error copying local config.txt to card"
		sudo umount ${DEV}
		exit 1
	fi
fi
  
if [ -f ./cmdline.txt ]; then
	echo "Copying local cmdline.txt to card"
	sudo cp ./cmdline.txt /media/card

	if [ $? -ne 0 ]; then
		echo "Error copying local cmdline.txt to card"
		sudo umount ${DEV}
		exit 1
	fi
fi

echo "Unmounting ${DEV}"
sudo umount ${DEV}

echo "Done"

