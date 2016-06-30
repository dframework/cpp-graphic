#!/bin/sh

D_PWD=`pwd`
D_WORKSPACE="workspace"
D_DEST_NM="libgd-2.2.1"
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

################################################################
cd ../
D_PREV=`pwd`
D_WITH_JPEG="--with-jpeg=$D_PREV/libjpeg/$D_OSNAME"
D_WITH_PNG="--with-png=$D_PREV/libpng/workspace/build"
D_WITH_FT="--with-freetype=$D_PREV/libfreetype/workspace/build"


cd $D_PWD

dest_exit(){
    if [ $1 -ne 0 ]; then
        echo $2
        exit $1
    fi
}

spinner(){
  local stoped=$1
  local delay=0.75
  local spinstr='|/-\'
  local val=""
  while [ "$stoped" ]; do
    if test -f $stoped ; then
      val=`cat $stoped`
      #rm -rf $stoped
      stoped=""
      continue
    fi

    local temp=${spinstr#?}
    printf "[%c] " "$spinstr"
    local spinstr=$temp${spinstr%"$temp"}
    sleep $delay
    printf "\b\b\b\b"
  done

  if [ "$val" = "0" ]; then
    printf " [ FAILED ]    "
    echo ""
    exit 1
  else
    printf " [ OK ]   "
  fi
    printf "\b\b\b\b"
    echo ""
}

downloading(){
  cp ../$D_DEST_FN ./
}

downloading2(){
  #https://github.com/libgd/libgd/releases/download/gd-2.2.1/libgd-2.2.1.tar.gz
  local stoped="_download.stop"
  if test -f $stoped ; then
      rm -rf $stoped
  fi
  if [ "$D_OSNAME" = "darwin" ]; then
      echo -n " * curl https://github.com/libgd/libgd/releases/download/gd-2.2.1/$D_DEST_FN -o $D_DEST_FN ... "
      ((curl https://github.com/libgd/libgd/releases/download/gd-2.2.1/$D_DEST_FN -o $D_DEST_FN > /dev/null 2>&1 \
          && echo "1" > $stoped) || echo "0" > $stoped) & spinner $stoped
  else
      echo -n " * wget https://github.com/libgd/libgd/releases/download/gd-2.2.1/$D_DEST_FN ... "
      ((wget https://github.com/libgd/libgd/releases/download/gd-2.2.1/$D_DEST_FN > /dev/null 2>&1 \
          && echo "1" > $stoped) || echo "0" > $stoped) & spinner $stoped
  fi
}

configure(){
  local stoped="_configure.stop"
  if test -f $stoped ; then
      return 0
      #rm -rf $stoped
  fi
  if [ "$D_OSNAME" = "darwin" ]; then
      echo -n " * Configure ... "
      echo "./configure $D_MINGW --prefix=$D_DEST_BD ${D_LIBSSL_PREFIX} ${D_WITH_JPEG} ${D_WITH_PNG} ${D_WITH_FT}"
      ((./configure $D_MINGW --prefix=$D_DEST_BD ${D_WITH_JPEG} ${D_WITH_PNG} ${D_WITH_FT}   > "_configure.log" 2>&1 \
          && echo "1" > $stoped) || echo "0" > $stoped) & spinner $stoped
  else
      echo "./configure $D_MINGW --prefix=$D_DEST_BD ${D_WITH_JPEG} ${D_WITH_PNG} ${D_WITH_FT}"
      echo -n " * Configure ... "
      ((./configure $D_MINGW --prefix=$D_DEST_BD ${D_WITH_JPEG} ${D_WITH_PNG} ${D_WITH_FT} > "_configure.log" 2>&1 \
          && echo "1" > $stoped) || echo "0" > $stoped) & spinner $stoped
  fi
}

making(){
  local stoped="_make.stop"
  if test -f $stoped ; then
      return 0
      #rm -rf $stoped
  fi
  echo -n " * Make ... "
  ((make > $D_MAKELOG 2>&1 \
    && echo "1" > $stoped) || echo "0" > $stoped) & spinner $stoped
}

start(){
  if test ! -d $D_DEST_SRC ; then
    mkdir -p $D_DEST_SRC
    dest_exit $? "ERROR: mkdir -p $D_DEST_SRC at ${D_PWD}"
    mkdir -p $D_DEST_SRC/include
    dest_exit $? "ERROR: mkdir -p $D_DEST_SRC/include at ${D_PWD}"
  fi

  if test ! -d $D_WORKSPACE ; then
    mkdir -p $D_WORKSPACE
    dest_exit $? "ERROR: mkdir -p ${D_WORKSPACE} at ${D_PWD}"
  fi

  cd $D_WORKSPACE
  dest_exit $? "ERROR: cd ${D_WORKSPACE} at ${D_PWD}"
  echo "Entering ${D_WORKSPACE} ..."

  #####################################################
  if test ! -f $D_DEST_FN ; then
    downloading
  fi

  #####################################################
  if test -d $D_DEST_NM ; then
    rm -rf $D_DEST_NM
    dest_exit $? "ERROR: rm -rf $D_DEST_NM"
  fi

  if test ! -d $D_DEST_NM ; then
    tmp=`tar xvfz $D_DEST_FN 2>&1`
    dest_exit $? "ERROR: tar xvfz $D_DEST_FN at ${D_WORKSPACE}/\n${tmp}"
  fi

  #####################################################
  cd $D_DEST_NM
  dest_exit $? "ERROR: cd $D_DEST_NM at ${D_WORKSPACE}/"
  echo "Entering ${D_WORKSPACE}/${D_DEST_NM} ..."
}



start

configure

making

make install
dest_exit $? "ERROR: make install at ${D_WORKSPACE}/${D_DEST_NM}"

cd $D_PWD
dest_exit $? "ERROR: cd $D_PWD"

mkdir -p src-$D_OSNAME/
dest_exit $? "ERROR: mkdir -p src-$D_OSNAME/"

D_SRCS="gd.c gd_bmp.c gd_color.c gd_color_map.c gd_color_match.c gd_crop.c gd_filename.c gd_filter.c gd_gd.c gd_gd2.c gd_gif_in.c gd_gif_out.c gd_interpolation.c gd_io.c gd_io_dp.c gd_io_file.c gd_io_ss.c gd_jpeg.c gd_matrix.c gd_nnquant.c gd_png.c gd_rotate.c gd_security.c gd_ss.c gd_tga.c gd_tiff.c gd_topal.c gd_transform.c gd_version.c gd_wbmp.c gd_webp.c gd_xbm.c gdcache.c gdfontg.c gdfontl.c gdfontmb.c gdfonts.c gdfontt.c gdft.c gdfx.c gdhelpers.c gdkanji.c gdtables.c gdxpm.c wbmp.c"
D_HEADERS="bmp.h config.h entities.h gd.h gd_color.h gd_color_map.h gd_errors.h gd_intern.h gd_io.h gd_io_stream.h gd_nnquant.h gd_tga.h gdcache.h gdfontg.h gdfontl.h gdfontmb.h gdfonts.h gdfontt.h gdfx.h gdhelpers.h gdpp.h jisx0208.h wbmp.h"

for x in $D_SRCS
do
  src_x=$D_WORKSPACE/$D_DEST_NM/src/$x
  if test ! -f $src_x ; then
      dest_exit 1 "ERROR: Not found source file ($src_x)"
  fi
  cp $src_x ./src-$D_OSNAME/
done

for x in $D_HEADERS
do
  src_x=$D_WORKSPACE/$D_DEST_NM/src/$x
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
  MK_S="${MK_S}\n\nLOCAL_INCLUDES := \${LOCAL_PATH}/../../libpng/linux/include \${LOCAL_PATH}/../../libjpeg/linux/include \${LOCAL_PATH}/../../libfreetype/linux/include/freetype2"
  MK_S="${MK_S}\n\nLOCAL_CFLAGS := -fPIC -DHAVE_CONFIG_H=1"
  MK_S="${MK_S}\n\nLOCAL_MODULE := libgd"
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

echo ""
echo "Success for LINUX."
echo ""


