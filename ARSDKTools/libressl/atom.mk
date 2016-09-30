ifdef ARSDK_BUILD_FOR_APP

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libressl
LOCAL_DESCRIPTION := LibreSSL
LOCAL_CATEGORY_PATH := libs

LOCAL_AUTOTOOLS_VERSION := 2.2.1
LOCAL_AUTOTOOLS_ARCHIVE := libressl-$(LOCAL_AUTOTOOLS_VERSION).tar.gz
LOCAL_AUTOTOOLS_SUBDIR := libressl-$(LOCAL_AUTOTOOLS_VERSION)

LOCAL_AUTOTOOLS_PATCHES := \
	0001-libressl-2.2.1-maintainer-mode.patch \
	0002-libressl-2.2.1-android-compilation.patch \
	0003-libressl-2.2.1-ios-compilation.patch \
	0004-libressl-2.2.1-armv5.patch

LOCAL_AUTOTOOLS_CONFIGURE_ARGS := \
	--enable-static \
	--disable-shared

# Don't redefine alloc functions
LOCAL_AUTOTOOLS_CONFIGURE_ENV := \
	ac_cv_func_malloc_0_nonnull=yes \
	ac_cv_func_realloc_0_nonnull=yes \

include $(BUILD_AUTOTOOLS)

endif
