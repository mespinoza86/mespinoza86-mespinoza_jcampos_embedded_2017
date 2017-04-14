SUMMARY = "Example of how to build an external Linux kernel module"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=12f884d2ae1ff87c09e5b7ccc2c4ca7e"

inherit module

SRC_URI = "file://Makefile \
           file://conv.c \
           file://chardev.h \
           file://COPYING \
          "

S = "${WORKDIR}"
FILES_${PN} += "${libdir}/*"

# The inherit of module.bbclass will automatically name module packages with
# "kernel-module-" prefix as required by the oe-core build environment.


######## #yocto recipe to build a kernel module out of the kernel tree 
######## 
######## DESCRIPTION = "Chardriver kernel module out of the kernel tree" 
######## LICENSE = "CLOSED" 
######## 
######## PROVIDES = "virtual/kernel-module-conv-mod" 
######## RPROVIDES_${PN} = "virtual/kernel-module-driver" 
######## RPROVIDES_${PN}-dev = "virtual/kernel-module-driver" 
######## 
######## #PR = "r0" 
######## 
######## inherit module 
######## 
######## SRC_URI = "file://Makefile \
########            file://conv.c \
########            file://COPYING \
######## 	   file://chardev.h \
######## 	   file://gen_conv.sh \
########           "
######## 
######## S = "${WORKDIR}" 
######## # The inherit of module.bbclass will automatically name module packages with
######## # "kernel-module-" prefix as required by the oe-core build environment.
