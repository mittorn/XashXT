######################################################################
# nicknekit: xashxt client port
######################################################################

QT -= core widgets

TEMPLATE = lib
TARGET = client
DEPENDPATH += . hl
INCLUDEPATH += . ./render ../common ../engine ../game_shared
LIBS += -lSDL2
QMAKE_CXXFLAGS = -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-parameter -DCLIENT_DLL=1

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
           render/r_studio.h \
           render/r_view.h \
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
           utils.cpp \
           weapons/w_events.cpp \
           weapons/w_common.cpp
