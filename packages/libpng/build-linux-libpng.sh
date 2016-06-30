#!/bin/sh


D_PWD=`pwd`
D_WORKSPACE="workspace"
D_DEST_NM="libpng-1.6.23"
D_DEST_FN="${D_DEST_NM}.tar.xz"
D_DEST_BD="${D_PWD}/${D_WORKSPACE}/build"

D_SRCPATH=""
D_SRCS=""
D_MAKELOG="_make.log"

D_OSNAME=$1

if [ "$D_OSNAME" = "" ]; then
  tmp_unamea=`uname -a`
  tmp_unameb=`expr "${tmp_unamea}" : '\(^[A-Za-z0-9]\{1,\}\)[[:blank:]]'`
  D_OSNAME=`echo $tmp_unameb | tr "[A-Z]" "[a-z]"`
fi

dest_exit(){
    if [ $1 -ne 0 ]; then
        echo $2
        exit $1
    fi
}


mkdir -p $D_WORKSPACE
dest_exit $? "ERROR: mkdir -p $D_WORKSPACE"


cp $D_DEST_FN $D_WORKSPACE/
dest_exit $? "ERROR: cp $D_DEST_FN $D_WORKSPACE/"

cd $D_WORKSPACE
dest_exit $? "ERROR: cd $D_WORKSPACE"

tar xvf $D_DEST_FN
dest_exit $? "ERROR: tar xvfz $D_DEST_FN"

cd $D_DEST_NM
dest_exit $? "ERROR: cd $D_DEST_NM"

./configure --prefix=$D_DEST_BD
dest_exit $? "ERROR: ./configure --prefix=$D_DEST_BD"

make
dest_exit $? "ERROR: make"

make install
dest_exit $? "ERROR: make install"

cd $D_PWD
dest_exit $? "ERROR: cd $D_PWD"

mkdir -p src-$D_OSNAME/
dest_exit $? "ERROR: mkdir -p src-$D_OSNAME/"

D_SRCS="png.c pngerror.c pngget.c pngmem.c pngpread.c pngread.c pngrio.c pngrtran.c pngrutil.c pngset.c pngtrans.c pngwio.c pngwrite.c pngwtran.c pngwutil.c"
D_HEADERS="config.h png.h pngconf.h pngdebug.h pnginfo.h pnglibconf.h pngprefix.h pngpriv.h pngstruct.h"

for x in $D_SRCS
do
  src_x=$D_WORKSPACE/$D_DEST_NM/$x
  if test ! -f $src_x ; then
      dest_exit 1 "ERROR: Not found source file ($src_x)"
  fi
  cp $src_x ./src-$D_OSNAME/
done

for x in $D_HEADERS
do
  src_x=$D_WORKSPACE/$D_DEST_NM/$x
  if test ! -f $src_x ; then
      dest_exit 1 "ERROR: Not found source file ($src_x)"
  fi
  cp $src_x ./src-$D_OSNAME/
done

make_mk(){
  local MK_S=""
  MK_S="include \$(CLEAR_VARS)"
  MK_S="${MK_S}\n\nLOCAL_PATH := \$(call my-dir)"
  MK_S="${MK_S}\n\nLOCAL_SRC_FILES := ${D_SRCS}"
  MK_S="${MK_S}\n\nLOCAL_INCLUDES := "
  MK_S="${MK_S}\n\nLOCAL_CFLAGS := -fPIC -DHAVE_CONFIG_H=1"
  MK_S="${MK_S}\n\nLOCAL_MODULE := libpng"
  MK_S="${MK_S}\n\ninclude \$(BUILD_STATIC_LIBRARY)"

  `echo -e "${MK_S}" > ${D_PWD}/src-$D_OSNAME/Dframework.mk`
  dest_exit $? "ERROR: Not make mk file. at ${D_PWD}/src-$D_OSNAME/Dframework.mk"
}

make_mk

exit 0

mkdir -p $D_OSNAME
dest_exit $? "ERROR: mkdir $D_OSNAME"

mv $D_WORKSPACE/build/include ./$D_OSNAME/
dest_exit $? "ERROR: mv $D_WORKSPACE/build/include ./$D_OSNAME/"

mv $D_WORKSPACE/build/lib ./$D_OSNAME/lib
dest_exit $? "ERROR: mv $D_WORKSPACE/build/lib ./$D_OSNAME/lib"


