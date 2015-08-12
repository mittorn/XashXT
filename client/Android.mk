#XashXT client port for android
#Copyright (c) nicknekit

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := client
#ifeq ($(XASH_SDL),1)
#APP_PLATFORM := android-12
#LOCAL_SHARED_LIBRARIES += SDL2 
#LOCAL_CFLAGS += -DXASH_SDL
#else
APP_PLATFORM := android-8
#endif
LOCAL_CONLYFLAGS += -std=c99

LOCAL_CFLAGS += $(CFLAGS_OPT)
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a-hard)
LOCAL_CFLAGS += $(CFLAGS_OPT_ARM) -mfloat-abi=hard -mhard-float
LOCAL_MODULE_FILENAME = libclient_hardfp
endif
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_CFLAGS += $(CFLAGS_OPT_ARM) -mfloat-abi=softfp
endif
ifeq ($(TARGET_ARCH_ABI),armeabi)
LOCAL_CFLAGS += $(CFLAGS_OPT_ARMv5)
endif
ifeq ($(TARGET_ARCH_ABI),x86)
LOCAL_CFLAGS += $(CFLAGS_OPT_X86)
endif

LOCAL_CFLAGS += -fsigned-char -DCLIENT_DLL=1

LOCAL_C_INCLUDES := $(SDL_PATH)/include \
		 $(LOCAL_PATH)/. \
		 $(LOCAL_PATH)/./render \
		 $(LOCAL_PATH)/../common \
		 $(LOCAL_PATH)/../engine \
		 $(LOCAL_PATH)/../phys_shared \
		 $(LOCAL_PATH)/../game_shared

LOCAL_SRC_FILES := ammo.cpp \
           ammo_secondary.cpp \
           ammohistory.cpp \
           battery.cpp \
           cdll_int.cpp \
           ../game_shared/common.cpp \
           death.cpp \
           flashlight.cpp \
           geiger.cpp \
           health.cpp \
           hud.cpp \
           hud_msg.cpp \
           hud_redraw.cpp \
           hud_update.cpp \
           input.cpp \
           inputw32.cpp \
           input_touch.cpp \
           ../game_shared/mathlib.cpp \
           ../game_shared/matrix.cpp \
           menu.cpp \
           message.cpp \
           motd.cpp \
           parsemsg.cpp \
           ../game_shared/parser.cpp \
           ../game_shared/playermove.cpp \
           render/r_main.cpp \
           render/r_misc.cpp \
           render/r_studio.cpp \
           render/r_tempents.cpp \
           render/r_view.cpp \
           saytext.cpp \
           scoreboard.cpp \
           status_icons.cpp \
           statusbar.cpp \
           ../game_shared/stringlib.cpp \
           text_message.cpp \
           train.cpp \
	   weapons/w_events.cpp \
	   weapons/w_common.cpp \
           utils.cpp

#LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
