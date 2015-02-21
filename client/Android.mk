#nickenkit: xashxt client port for android

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := client

SDL_PATH := /home/nicknekit/SDL2

APP_PLATFORM := android-12

LOCAL_CFLAGS += -std=c99
LOCAL_CPPFLAGS += -fpermissive -DCLIENT_DLL=1


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
           ../game_shared/mathlib.cpp \
           ../game_shared/matrix.cpp \
           menu.cpp \
           message.cpp \
           motd.cpp \
           parsemsg.cpp \
           ../game_shared/parser.cpp \
           ../game_shared/playermove.cpp \
           render/r_backend.cpp \
           render/r_beams.cpp \
           render/r_bloom.cpp \
           render/r_cull.cpp \
           render/r_debug.cpp \
           render/r_grass.cpp \
           render/r_light.cpp \
           render/r_main.cpp \
           render/r_mirror.cpp \
           render/r_misc.cpp \
           render/r_monitor.cpp \
           render/r_movie.cpp \
           render/r_opengl.cpp \
           render/r_particle.cpp \
           render/r_portal.cpp \
           render/r_shadows.cpp \
           render/r_sprite.cpp \
           render/r_studio.cpp \
           render/r_surf.cpp \
           render/r_tempents.cpp \
           render/r_view.cpp \
           render/r_warp.cpp \
           render/r_weather.cpp \
           saytext.cpp \
           scoreboard.cpp \
           status_icons.cpp \
           statusbar.cpp \
           ../game_shared/stringlib.cpp \
           text_message.cpp \
           train.cpp \
           utils.cpp


LOCAL_SHARED_LIBRARIES += SDL2 

LOCAL_LDLIBS := -llog


include $(BUILD_SHARED_LIBRARY)
