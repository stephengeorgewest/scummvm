MODULE := engines/mm

MODULE_OBJS := \
	metaengine.o \
	utils/engine_data.o \
	mm1/events.o \
	mm1/mm1.o \
	mm1/gfx/gfx.o \
	mm1/gfx/screen_decoder.o \
	mm1/views/title_view.o \
	mm1/views/text_view.o \
	xeen/worldofxeen/clouds_cutscenes.o \
	xeen/worldofxeen/darkside_cutscenes.o \
	xeen/worldofxeen/worldofxeen_cutscenes.o \
	xeen/worldofxeen/worldofxeen_menu.o \
	xeen/worldofxeen/worldofxeen.o \
	xeen/swordsofxeen/swordsofxeen.o \
	xeen/swordsofxeen/swordsofxeen_menu.o \
	xeen/dialogs/credits_screen.o \
	xeen/dialogs/dialogs.o \
	xeen/dialogs/dialogs_awards.o \
	xeen/dialogs/dialogs_char_info.o \
	xeen/dialogs/dialogs_control_panel.o \
	xeen/dialogs/dialogs_copy_protection.o \
	xeen/dialogs/dialogs_create_char.o \
	xeen/dialogs/dialogs_difficulty.o \
	xeen/dialogs/dialogs_dismiss.o \
	xeen/dialogs/dialogs_exchange.o \
	xeen/dialogs/dialogs_info.o \
	xeen/dialogs/dialogs_input.o \
	xeen/dialogs/dialogs_items.o \
	xeen/dialogs/dialogs_map.o \
	xeen/dialogs/dialogs_message.o \
	xeen/dialogs/dialogs_party.o \
	xeen/dialogs/dialogs_query.o \
	xeen/dialogs/dialogs_quests.o \
	xeen/dialogs/dialogs_quick_fight.o \
	xeen/dialogs/dialogs_quick_ref.o \
	xeen/dialogs/dialogs_spells.o \
	xeen/dialogs/dialogs_whowill.o \
	xeen/dialogs/please_wait.o \
	xeen/character.o \
	xeen/combat.o \
	xeen/cutscenes.o \
	xeen/debugger.o \
	xeen/events.o \
	xeen/files.o \
	xeen/font.o \
	xeen/interface.o \
	xeen/interface_minimap.o \
	xeen/interface_scene.o \
	xeen/item.o \
	xeen/locations.o \
	xeen/map.o \
	xeen/party.o \
	xeen/patcher.o \
	xeen/resources.o \
	xeen/saves.o \
	xeen/screen.o \
	xeen/scripts.o \
	xeen/sound.o \
	xeen/sound_driver.o \
	xeen/sound_driver_adlib.o \
	xeen/spells.o \
	xeen/sprites.o \
	xeen/subtitles.o \
	xeen/window.o \
	xeen/xeen.o \
	xeen/xsurface.o

# This module can be built as a plugin
ifeq ($(ENABLE_MM), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
