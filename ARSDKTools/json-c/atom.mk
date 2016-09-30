LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := json
LOCAL_DESCRIPTION := JSON-C
LOCAL_CATEGORY_PATH := libs

LOCAL_AUTOTOOLS_VERSION := 0.9
LOCAL_AUTOTOOLS_ARCHIVE := json-c-$(LOCAL_AUTOTOOLS_VERSION).tar.gz
LOCAL_AUTOTOOLS_SUBDIR := json-c-$(LOCAL_AUTOTOOLS_VERSION)

LOCAL_EXPORT_LDLIBS := -ljson

ifdef ARSDK_BUILD_FOR_APP

LOCAL_AUTOTOOLS_PATCHES := \
	0001-json-c-0.9-maintainer-mode.patch \
	0002-json-c-0.9-avoid-version.patch \
	0003-json-c-0.9-android-build.patch

ifeq ("$(TARGET_OS_FLAVOUR)","android")
LOCAL_AUTOTOOLS_CONFIGURE_ARGS += \
	--disable-so-version
endif

endif

# Don't redefine alloc functions
LOCAL_AUTOTOOLS_CONFIGURE_ENV := \
	ac_cv_func_malloc_0_nonnull=yes \
	ac_cv_func_realloc_0_nonnull=yes \

include $(BUILD_AUTOTOOLS)
