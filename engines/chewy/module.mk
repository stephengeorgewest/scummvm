MODULE := engines/chewy

MODULE_OBJS = \
	ailclass.o \
	atds.o \
	chewy.o \
	cursor.o \
	datei.o \
	debugger.o \
	detail.o \
	effect.o \
	events.o \
	events_base.o \
	fehler.o \
	file.o \
	flic.o \
	gedclass.o \
	global.o \
	inits.o \
	io_game.o \
	m_event.o \
	main.o \
	main_menu.o \
	mouse.o \
	mcga.o \
	mcga_grafik.o \
	memory.o \
	menus.o \
	metaengine.o \
	movclass.o \
	ngstypes.o \
	objekt.o \
	objekte.o \
	r_event.o \
	room.o \
	resource.o \
	sound.o \
	sprite.o \
	t_event.o \
	temp_file.o \
	text.o \
	timer.o \
	types.o \
	video/cfo_decoder.o \
	rooms/room0.o \
	rooms/room1.o \
	rooms/room2.o \
	rooms/room3.o \
	rooms/room4.o \
	rooms/room5.o \
	rooms/room6.o \
	rooms/room7.o \
	rooms/room8.o \
	rooms/room9.o \
	rooms/room10.o \
	rooms/room11.o \
	rooms/room12.o \
	rooms/room13.o \
	rooms/room14.o \
	rooms/room16.o \
	rooms/room17.o \
	rooms/room18.o \
	rooms/room19.o \
	rooms/room21.o \
	rooms/room22.o \
	rooms/room23.o \
	rooms/room24.o \
	rooms/room25.o \
	rooms/room26.o \
	rooms/room27.o \
	rooms/room28.o \
	rooms/room29.o \
	rooms/room31.o \
	rooms/room32.o \
	rooms/room33.o \
	rooms/room34.o \
	rooms/room35.o \
	rooms/room37.o \
	rooms/room39.o \
	rooms/room40.o \
	rooms/room41.o \
	rooms/room42.o \
	rooms/room43.o \
	rooms/room44.o \
	rooms/room45.o \
	rooms/room46.o \
	rooms/room47.o \
	rooms/room48.o \
	rooms/room49.o \
	rooms/room50.o \
	rooms/room51.o \
	rooms/room52.o \
	rooms/room53.o \
	rooms/room54.o \
	rooms/room55.o \
	rooms/room56.o \
	rooms/room57.o \
	rooms/room58.o \
	rooms/room59.o \
	rooms/room62.o \
	rooms/room63.o \
	rooms/room64.o \
	rooms/room65.o \
	rooms/room66.o \
	rooms/room67.o \
	rooms/room68.o \
	rooms/room69.o \
	rooms/room70.o \
	rooms/room71.o \
	rooms/room72.o \
	rooms/room73.o \
	rooms/room74.o \
	rooms/room75.o \
	rooms/room76.o \
	rooms/room77.o \
	rooms/room78.o \
	rooms/room79.o \
	rooms/room80.o \
	rooms/room81.o \
	rooms/room82.o \
	rooms/room83.o \
	rooms/room84.o \
	rooms/room85.o \
	rooms/room86.o \
	rooms/room87.o \
	rooms/room88.o \
	rooms/room89.o \
	rooms/room90.o \
	rooms/room91.o \
	rooms/room92.o \
	rooms/room93.o \
	rooms/room94.o \
	rooms/room95.o \
	rooms/room96.o \
	rooms/room97.o

# This module can be built as a plugin
ifeq ($(ENABLE_CHEWY), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
