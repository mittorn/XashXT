######################################################################
# nicknekit: xashxt client port
######################################################################

TEMPLATE = lib
TARGET = client
DEPENDPATH += . hl
INCLUDEPATH += . ./render ../common ../engine ../game_shared
LIBS += -lSDL2 -lGLEW -lGL -lGLU
QMAKE_CXXFLAGS = -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-parameter -fpermissive -DCLIENT_DLL=1

# Input
HEADERS += ammo.h \
           ammohistory.h \
           ../game_shared\cdll_dll.h \
           hud.h \
           hud_events.h \
           hud_health.h \
           ../game_shared/mathlib.h \
           parsemsg.h \
           ../game_shared/plane.h \
           ../game_shared/pm_defs.h \
           ../game_shared/pm_info.h \
           ../game_shared/pm_materials.h \
           ../game_shared/pm_movevars.h \
           ../game_shared/pm_shared.h \
           render/r_grass.h \
           render/r_opengl.h \
           render/r_particle.h \
           render/r_programs.h \
           render/r_studio.h \
           render/r_view.h \
           render/r_weather.h \
           utils.h \
           ../game_shared/vector.h

SOURCES += ammo.cpp \
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
