#!/bin/sh


D_PWD=`pwd`
D_WORKSPACE="workspace"
D_DEST_NM="libjpeg-9b"
D_DEST_FN="${D_DEST_NM}.tar.gz"
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

tar xvfz $D_DEST_FN
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


D_SRCS="jaricom.c jcapimin.c jcapistd.c jcarith.c jccoefct.c jccolor.c jcdctmgr.c jchuff.c jcinit.c jcmainct.c jcmarker.c jcmaster.c jcomapi.c jcparam.c jcprepct.c jcsample.c jctrans.c jdapimin.c jdapistd.c jdarith.c jdatadst.c jdatasrc.c jdcoefct.c jdcolor.c jddctmgr.c jdhuff.c jdinput.c jdmainct.c jdmarker.c jdmaster.c jdmerge.c jdpostct.c jdsample.c jdtrans.c jerror.c jfdctflt.c jfdctfst.c jfdctint.c jidctflt.c jidctfst.c jidctint.c jquant1.c jquant2.c jutils.c jmemmgr.c jmemnobs.c"
D_HEADERS="cderror.h cdjpeg.h jconfig.h jdct.h jerror.h jinclude.h jmemsys.h jmorecfg.h jpegint.h jpeglib.h jversion.h transupp.h"

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
  MK_S="${MK_S}\n\nLOCAL_MODULE := libjpeg"
  MK_S="${MK_S}\n\ninclude \$(BUILD_STATIC_LIBRARY)"

  `echo -e "${MK_S}" > ${D_PWD}/src-$D_OSNAME/Dframework.mk`
  dest_exit $? "ERROR: Not make mk file. at ${D_PWD}/src-$D_OSNAME/Dframework.mk"

}

make_mk

mkdir -p $D_OSNAME
dest_exit $? "ERROR: mkdir $D_OSNAME"

mv $D_WORKSPACE/build/include ./$D_OSNAME/
dest_exit $? "ERROR: mv $D_WORKSPACE/build/include ./$D_OSNAME/"

mv $D_WORKSPACE/build/lib ./$D_OSNAME/lib
dest_exit $? "ERROR: mv $D_WORKSPACE/build/lib ./$D_OSNAME/lib"




