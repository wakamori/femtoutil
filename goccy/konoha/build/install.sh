#!/bin/sh

ktarget=konoha1
kpath=/usr/local
build=build
libext=so
uname=`uname`

if test $uname = "Darwin" ; then
	libext="dylib"
fi

if [ ! -z $1 ] ; then
	ktarget=$1
fi

if [ ! -z $2 ] ; then
	kpath=$2
fi

echo "install ${ktarget} to ${kpath}"
echo "copying.."

install -d ${kpath}/bin
install -d ${kpath}/lib
install -d ${kpath}/include
install -m 755 ${build}/${ktarget} ${kpath}/bin/${ktarget}
install -m 755 ${build}/${ktarget} ${kpath}/bin/konoha
cp -f ${build}/lib${ktarget}.${libext} ${kpath}/lib/
cp -f ${build}/lib${ktarget}.a ${kpath}/lib/
cp -f include/${ktarget}.h ${kpath}/include/
install -d ${kpath}/include/${ktarget}

cp include/${ktarget}/*.h ${kpath}/include/${ktarget}


