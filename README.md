# DroidBlaster
Android application with native code only (Android NDK) without Java

# Dependencies
-----
## libpng
### Setup
1. Download libpng with sources from [http://www.libpng.org/pub/png/libpng.html](http://www.libpng.org/pub/png/libpng.html) (now use 1.6.32)
2. Create dirrectory `libpng ` in `$ANDROID_NDK/sources/`. Move all files from downloaded libpng to `$ANDROID_NDK/sources/libpng/`.
3. Copy file `libpng/scripts/pnglibconf.h.prebuilt` to `$ANDROID_NDK/sources/libpng/` and rename `pnglibconf.h`.
4. Create file `Android.mk` in `$ANDROID_NDK/sources/libpng/`.

```
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LS_C=$(subst $(1)/,,$(wildcard $(1)/*.c))

LOCAL_MODULE := png
LOCAL_SRC_FILES := \ 
    $(filter-out example.c pngtest.c,$(call LS_C,$(LOCAL_PATH)))
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_LDLIBS := -lz

include $(BUILD_STATIC_LIBRARY)
```
#### Quick fix in libpng 1.6.32 for arm64-v8a 
arm64-v8a build fail with missing neon intrinsics.
Changing the line number 132 in `pngpriv.h` form

```
define PNG_ARM_NEON_OPT 2
```
to

```
define PNG_ARM_NEON_OPT 0
```
will avoid neon optimization in arm and will fix the bug