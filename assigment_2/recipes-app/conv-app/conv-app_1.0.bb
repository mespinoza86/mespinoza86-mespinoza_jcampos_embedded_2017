#
# This file was derived from the 'Hello World!' example recipe in the
# Yocto Project Development Manual.
#

DESCRIPTION = "Convolution application"
SECTION = "tec"
DEPENDS = ""
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

# This tells bitbake where to find the files we're providing on the local filesystem
FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

# Use local tarball
SRC_URI = "file://conv.tar.gz \
	   file://gen_conv.sh \
           file://lena512.bmp \
          "

# Make sure our source directory (for the build) matches the directory structure in the tarball
S = "${WORKDIR}/conv"
FILES_${PN} += " /usr/gen_conv.sh "
FILES_${PN} += " /usr/lena512.bmp "


inherit autotools

# The autotools configuration I am basing this on seems to have a problem with a race condition when parallel make is enabled
PARALLEL_MAKE = ""

do_install_append(){
	echo "jecampos  ${WORKDIR} ... ${D}"
	install -d {$D}/home/
	echo "jecampos algo1"
#	install -d {$D}/home/root/
	install -m 0777 ${WORKDIR}/gen_conv.sh ${D}/usr/
	install -m 0755 ${WORKDIR}/lena512.bmp ${D}/usr/

}
