#!/bin/sh

ktarget=$1    # math
kpath=$2      # /usr/local/konoha/package/1.0
build=build
libext=so
uname=`uname`
pkgname=$3    # konoha.math

if test $uname = "Darwin" ; then
	libext="dylib"
fi

echo "pkginstall ${ktarget} to ${kpath}"


if [ -f ${build}/${ktarget}.${libext} ] ; then
	echo "copying ${pkgname} .."
	install -d ${kpath}/${pkgname}
	cp ${build}/${ktarget}.${libext} ${kpath}/${pkgname}
	cp package/${pkgname}/${ktarget}.k ${kpath}/${pkgname}
else
	echo "package libfile has not found : ${build}/${ktarget}.${libext}"
fi
