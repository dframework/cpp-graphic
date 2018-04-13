include $(CLEAR_VARS)

LOCAL_PATH := $(call my-dir)

LOCAL_SRC_FILES := gd.c gd_bmp.c gd_color.c gd_color_map.c gd_color_match.c gd_crop.c gd_filename.c gd_filter.c gd_gd.c gd_gd2.c gd_gif_in.c gd_gif_out.c gd_interpolation.c gd_io.c gd_io_dp.c gd_io_file.c gd_io_ss.c gd_jpeg.c gd_matrix.c gd_nnquant.c gd_png.c gd_rotate.c gd_security.c gd_ss.c gd_tga.c gd_tiff.c gd_topal.c gd_transform.c gd_version.c gd_wbmp.c gd_webp.c gd_xbm.c gdcache.c gdfontg.c gdfontl.c gdfontmb.c gdfonts.c gdfontt.c gdft.c gdfx.c gdhelpers.c gdkanji.c gdtables.c gdxpm.c wbmp.c

LOCAL_INCLUDES := ${LOCAL_PATH}/../../libpng/linux/include ${LOCAL_PATH}/../../libjpeg/linux/include ${LOCAL_PATH}/../../libfreetype/linux/include/freetype2

LOCAL_CFLAGS := -fPIC -DHAVE_CONFIG_H=1

LOCAL_MODULE := libgd

include $(BUILD_STATIC_LIBRARY)
