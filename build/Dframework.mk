LOCAL_PATH := $(call my-dir)

@. ${LOCAL_PATH}/../Application.sh

freetype_PATH:=${DFRAMEWORK_CPP_GRAPHIC}/packages/libfreetype/${DDK_ENV_TARGET_OS}

include $(CLEAR_VARS)


@cp ${freetype_PATH}/lib/libfreetype.* ${DDK_ENV_TARGET_BUILD}/
@if [ $? -ne 0 ]; then
@    echo "ERROR: cp ${freetype_PATH}/lib/libfreetype.* ${DDK_ENV_TARGET_BUILD}/"
@fi

LOCAL_STATIC_LIBRARIES :=       \
  libdframework-graphic-gd      \
  libdframework-graphic-graph   \
  libdframework-graphic-monitor \
  libjpeg                       \
  libpng                        \
  libgd

#  libfreetype                   \

ifneq (${DDK_ENV_TARGET_OS}, "windows")
  LOCAL_LDFLAGS += -lz
endif

LOCAL_VERSION := 0.0.8
LOCAL_MODULE := libdframework-graphic

include $(BUILD_STATIC_LIBRARY)

ifeq (${DDK_ENV_TARGET_OS}, "linux")
  include $(BUILD_SHARED_LIBRARY)
elifeq (${DDK_ENV_TARGET_OS}, "darwin")
  include $(BUILD_SHARED_LIBRARY)
endif

pkg_path:=/usr/local
pkg_subname:=graphic

package:

ifeq (${DDK_ENV_TARGET_OS}, "windows")
  dest_NM := ${LOCAL_MODULE}
  dest_FNM := ${dest_NM}-${LOCAL_VERSION}.zip
  org_PATH := ${DDK_ENV_TARGET_PKG}/${dest_NM}
  dest_PATH := ${org_PATH}/${dest_NM}
  @if test -d ${org_PATH} ; then
  @  rm -rf ${org_PATH}
  @fi

  @mkdir -p ${dest_PATH}
  @cp ${DDK_ENV_TARGET_BUILD}/libdframework-${pkg_subname}.a ${dest_PATH}/libdframework-${pkg_subname}.lib
  @cp -R ../include ${dest_PATH}/
  @d_pwd=`pwd`
  @cd ${org_PATH}
  @zip -r libdframework-${pkg_subname}-${LOCAL_VERSION}.zip ./libdframework-${pkg_subname}/* 
  @cd ${d_pwd}
  echo ""
  echo "  Complete packaging for windows (${org_PATH})"
  echo ""
elifeq (${DDK_ENV_TARGET_OS}, "ios")
   @echo "Not supported ios"
   @exit 1
elifeq (${DDK_ENV_TARGET_OS}, "android")
   @echo "Not supported android"
   @exit 1
else
   $(package-start sis,lib,${LOCAL_MODULE},${LOCAL_VERSION})

   ifeq (${DDK_ENV_TARGET_OS}, "darwin")
       $(package ${DDK_ENV_TARGET_PATH}/build/${LOCAL_MODULE}.${LOCAL_VERSION}.${DDC_SHARED_LIB_EXT} ${pkg_path}/lib/)
   else
       $(package ${DDK_ENV_TARGET_PATH}/build/${LOCAL_MODULE}.${DDC_SHARED_LIB_EXT}.${LOCAL_VERSION} ${pkg_path}/lib/)
   endif

   $(package ${DDK_ENV_TARGET_PATH}/build/${LOCAL_MODULE}.a ${pkg_path}/lib/)
   $(package ${LOCAL_PATH}/../include ${pkg_path}/)
   $(package-end)
endif


package-install:

ifeq (${DDK_ENV_TARGET_OS}, "windows")
   @echo "Not supported windows"
eleq (${DDK_ENV_TARGET_OS}, "ios")
   @echo "Not supported ios"
eleq (${DDK_ENV_TARGET_OS}, "android")
   @echo "Not supported android"
else
   $(package-install sis,lib,${LOCAL_MODULE},${LOCAL_VERSION})
endif


package-all:

ifeq (${DDK_ENV_TARGET_OS}, "windows")
   @echo "Not supported windows"
eleq (${DDK_ENV_TARGET_OS}, "ios")
   @echo "Not supported ios"
eleq (${DDK_ENV_TARGET_OS}, "android")
   @echo "Not supported android"
else
    $(target package)
    $(target package-install)
endif


dist:

ifeq (${DDK_ENV_TARGET_OS}, "windows")
   dist_pass=`sudo cat /root/sis-pass`
   dist_host=`sudo cat /root/sis-dist`
   org_PATH := ${DDK_ENV_TARGET_PKG}/${LOCAL_MODULE}
   org_NM := ${org_PATH}/libdframework-${pkg_subname}-${LOCAL_VERSION}.zip
   sshpass -p${dist_pass} scp ${org_NM} ${dist_host}:/data/www-real/zonedrm/www/static/download/
   if [ $? -ne 0 ]; then
       echo "    - cp ${org_NM} ... FAIL"
   else
       echo "    - cp ${org_NM} ... OK"
   fi
eleq (${DDK_ENV_TARGET_OS}, "ios")
   @echo "Not supported ios"
eleq (${DDK_ENV_TARGET_OS}, "android")
   @echo "Not supported android"
else
   dist_pass=`sudo cat /root/sis-pass`
   dist_host=`sudo cat /root/sis-dist`
   #dist_nm=$(call_package_get_pkgname "${LOCAL_MODULE}" "${LOCAL_VERSION}" "lib")
   dist_nm=$(call_package_get_pkgname ${LOCAL_MODULE} ${LOCAL_VERSION} lib)
   dist_prefix="${DDK_ENV_TARGET_PKG}/${LOCAL_MODULE}/sis/${dist_nm}"
   sshpass -p${dist_pass} scp ${dist_prefix}.sh ${dist_host}:/data/www-real/zonedrm/www/static/download/
   if [ $? -ne 0 ]; then
       echo "    - cp ${dist_nm}.sh ... FAIL"
   else
       echo "    - cp ${dist_nm}.sh ... OK"
   fi

endif

