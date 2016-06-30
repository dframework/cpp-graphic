#!/bin/sh

D_PWD=`pwd`
D_WORKSPACE="workspace"
D_DEST_NM="freetype-2.6.2"
D_DEST_FN="${D_DEST_NM}.tar.bz2"
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
D_WITH_PNG="--with-png=$D_PREV/libpng/workspace/build"

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
  echo ""
}

configure(){
  local stoped="_configure.stop"
  if test -f $stoped ; then
      return 0
      #rm -rf $stoped
  fi
  if [ "$D_OSNAME" = "darwin" ]; then
      echo -n " * Configure ... "
      echo "./configure $D_MINGW --prefix=$D_DEST_BD ${D_WITH_PNG}"
      ((./configure $D_MINGW --prefix=$D_DEST_BD ${D_WITH_PNG}   > "_configure.log" 2>&1 \
          && echo "1" > $stoped) || echo "0" > $stoped) & spinner $stoped
  else
      echo "./configure $D_MINGW --prefix=$D_DEST_BD ${D_WITH_PNG}"
      echo -n " * Configure ... "
      ((./configure $D_MINGW --prefix=$D_DEST_BD ${D_WITH_PNG} > "_configure.log" 2>&1 \
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
    tmp=`tar xvf $D_DEST_FN 2>&1`
    dest_exit $? "ERROR: tar xvf $D_DEST_FN at ${D_WORKSPACE}/\n${tmp}"
  fi

  #####################################################
  cd $D_DEST_NM
  dest_exit $? "ERROR: cd $D_DEST_NM at ${D_WORKSPACE}/"
  echo "Entering ${D_WORKSPACE}/${D_DEST_NM} ..."
}


D_SRCS=""
D_HEADERS=""

src_find(){
  local fpath=
  local fdir=
  local fnm=

  mkdir -p src-$D_OSNAME/self-inc
  dest_exit $? "ERROR: mkdir -p src-$D_OSNAME/self-inc"

  mkdir -p src-$D_OSNAME/unix
  dest_exit $? "ERROR: mkdir -p src-$D_OSNAME/unix"

  cp -R ${D_WORKSPACE}/${D_DEST_NM}/include ./src-$D_OSNAME/
  cp -R ${D_WORKSPACE}/${D_DEST_NM}/builds/unix/ftsystem.c ./src-$D_OSNAME/
  cp -R ${D_WORKSPACE}/${D_DEST_NM}/builds/unix/*.h ./src-$D_OSNAME/unix/
  cp -R ${D_WORKSPACE}/${D_DEST_NM}/objs/ftmodule.h ./src-$D_OSNAME/include/

  while read line
  do
      fpath=`expr "$line" : "^[[:print:]]\{1,\}[[:blank:]]\{1,\}\([-_./a-zA-Z0-9]\{1,\}\.c\)[[:blank:]]\{1,\}-fPIC[[:blank:]]\{1,\}[[:print:]]*"`
      if [ "$fpath" = "" ]; then
          continue
      fi


      fdir=`expr "$fpath" : "\(^[[:print:]]\{1,\}\)\/[-_.a-zA-Z0-9]\{1,\}\.c$"`
      fnm=`expr "$fpath" : "^[[:print:]]\{1,\}\/\([-_.a-zA-Z0-9]\{1,\}\.c\)$"`

      echo "${fdir} => ${fnm}"

      cp $fpath ./src-$D_OSNAME/
      cp $fdir/*.h ./src-$D_OSNAME/self-inc/

      D_SRCS="${D_SRCS} ${fnm}"
  done < "${D_WORKSPACE}/${D_DEST_NM}/${D_MAKELOG}"
}


make_mk(){

  local MK_S=""
  MK_S="include \$(CLEAR_VARS)"
  MK_S="${MK_S}\n\nLOCAL_PATH := \$(call my-dir)"
  MK_S="${MK_S}\n\nLOCAL_SRC_FILES := ${D_SRCS}"
  MK_S="${MK_S}\n\nLOCAL_INCLUDES := \${LOCAL_PATH}/include \${LOCAL_PATH}/self-inc"
  MK_S="${MK_S}\n\nLOCAL_INCLUDES += \${LOCAL_PATH}/unix"
  MK_S="${MK_S}\n\nLOCAL_CFLAGS := -fPIC -DPIC -pedantic -ansi -DFT_CONFIG_OPTION_SYSTEM_ZLIB -DFT_CONFIG_CONFIG_H=<ftconfig.h> -DFT2_BUILD_LIBRARY -DFT_CONFIG_MODULES_H=<ftmodule.h> "
  MK_S="${MK_S}\n\nLOCAL_MODULE := libfreetype"
  MK_S="${MK_S}\n\ninclude \$(BUILD_STATIC_LIBRARY)"

  `echo -e "${MK_S}" > ${D_PWD}/src-$D_OSNAME/Dframework.mk`
  dest_exit $? "ERROR: Not make mk file. at ${D_PWD}/src-$D_OSNAME/Dframework.mk"

}

init(){
  start
  configure
  making

  make install
  dest_exit $? "ERROR: make install at ${D_WORKSPACE}/${D_DEST_NM}"

  cd $D_PWD
  dest_exit $? "ERROR: cd $D_PWD"
}

init

#src_find
#make_mk

#exit 0

mkdir -p $D_OSNAME
dest_exit $? "ERROR: mkdir $D_OSNAME"

rm -rf $D_OSNAME/*

cp -R $D_WORKSPACE/build/include ./$D_OSNAME/
dest_exit $? "ERROR: cp -R $D_WORKSPACE/build/include ./$D_OSNAME/"

cp -R $D_WORKSPACE/build/lib ./$D_OSNAME/
dest_exit $? "ERROR: cp -R $D_WORKSPACE/build/lib ./$D_OSNAME/"

echo ""
echo "Success for LINUX."
echo ""


