/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "ags/shared/core/platform.h"
#include "ags/shared/ac/common.h"
#include "ags/engine/ac/display.h"
#include "ags/engine/ac/draw.h"
#include "ags/shared/ac/game_version.h"
#include "ags/engine/ac/gamesetup.h"
#include "ags/shared/ac/gamesetupstruct.h"
#include "ags/engine/ac/gamestate.h"
#include "ags/engine/ac/mouse.h"
#include "ags/engine/ac/runtime_defines.h"
#include "ags/engine/ac/walkbehind.h"
#include "ags/engine/ac/dynobj/scriptsystem.h"
#include "ags/shared/debugging/out.h"
#include "ags/engine/device/mousew32.h"
#include "ags/shared/font/fonts.h"
#include "ags/engine/gfx/ali3dexception.h"
#include "ags/engine/gfx/graphicsdriver.h"
#include "ags/shared/gui/guimain.h"
#include "ags/shared/gui/guiinv.h"
#include "ags/engine/main/graphics_mode.h"
#include "ags/engine/main/engine_setup.h"
#include "ags/engine/media/video/video.h"
#include "ags/engine/platform/base/agsplatformdriver.h"
#include "ags/globals.h"

namespace AGS3 {

using namespace AGS::Shared;
using namespace AGS::Engine;

// Convert guis position and size to proper game resolution.
// Necessary for pre 3.1.0 games only to sync with modern engine.
void convert_gui_to_game_resolution(GameDataVersion filever) {
	if (filever >= kGameVersion_310)
		return;

	const int mul = _GP(game).GetDataUpscaleMult();
	for (int i = 0; i < _GP(game).numcursors; ++i) {
		_GP(game).mcurs[i].hotx *= mul;
		_GP(game).mcurs[i].hoty *= mul;
	}

	for (int i = 0; i < _GP(game).numinvitems; ++i) {
		_GP(game).invinfo[i].hotx *= mul;
		_GP(game).invinfo[i].hoty *= mul;
	}

	for (int i = 0; i < _GP(game).numgui; ++i) {
		GUIMain *cgp = &_GP(guis)[i];
		cgp->X *= mul;
		cgp->Y *= mul;
		if (cgp->Width < 1)
			cgp->Width = 1;
		if (cgp->Height < 1)
			cgp->Height = 1;
		// This is probably a way to fix GUIs meant to be covering whole screen
		if (cgp->Width == _GP(game).GetDataRes().Width - 1)
			cgp->Width = _GP(game).GetDataRes().Width;

		cgp->Width *= mul;
		cgp->Height *= mul;

		cgp->PopupAtMouseY *= mul;

		for (int j = 0; j < cgp->GetControlCount(); ++j) {
			GUIObject *guio = cgp->GetControl(j);
			guio->X *= mul;
			guio->Y *= mul;
			guio->Width *= mul;
			guio->Height *= mul;
			guio->IsActivated = false;
			guio->OnResized();
		}
	}
}

// Convert certain coordinates to data resolution (only if it's different from game resolution).
// Necessary for 3.1.0 and above games with legacy "low-res coordinates" setting.
void convert_objects_to_data_resolution(GameDataVersion filever) {
	if (filever < kGameVersion_310 || _GP(game).GetDataUpscaleMult() == 1)
		return;

	const int mul = _GP(game).GetDataUpscaleMult();
	for (int i = 0; i < _GP(game).numcharacters; ++i) {
		_GP(game).chars[i].x /= mul;
		_GP(game).chars[i].y /= mul;
	}

	for (int i = 0; i < _G(numguiinv); ++i) {
		_GP(guiinv)[i].ItemWidth /= mul;
		_GP(guiinv)[i].ItemHeight /= mul;
		_GP(guiinv)[i].OnResized();
	}
}

void engine_setup_system_gamesize() {
	_GP(scsystem).width = _GP(game).GetGameRes().Width;
	_GP(scsystem).height = _GP(game).GetGameRes().Height;
	_GP(scsystem).viewport_width = game_to_data_coord(_GP(play).GetMainViewport().GetWidth());
	_GP(scsystem).viewport_height = game_to_data_coord(_GP(play).GetMainViewport().GetHeight());
}

void engine_init_resolution_settings(const Size game_size) {
	Debug::Printf("Initializing resolution settings");
	_GP(usetup).textheight = getfontheight_outlined(0) + 1;

	Debug::Printf(kDbgMsg_Info, "Game native resolution: %d x %d (%d bit)%s", game_size.Width, game_size.Height, _GP(game).color_depth * 8,
		_GP(game).IsLegacyLetterbox() ? " letterbox-by-design" : "");

	convert_gui_to_game_resolution(_G(loaded_game_file_version));
	convert_objects_to_data_resolution(_G(loaded_game_file_version));

	Rect viewport = RectWH(game_size);
	_GP(play).SetMainViewport(viewport);
	_GP(play).SetUIViewport(viewport);
	engine_setup_system_gamesize();
}

// Setup gfx driver callbacks and options
void engine_post_gfxmode_driver_setup() {
	_G(gfxDriver)->SetCallbackForPolling(update_polled_stuff_if_runtime);
	_G(gfxDriver)->SetCallbackToDrawScreen(draw_game_screen_callback, construct_engine_overlay);
	_G(gfxDriver)->SetCallbackForNullSprite(GfxDriverNullSpriteCallback);
}

// Reset gfx driver callbacks
void engine_pre_gfxmode_driver_cleanup() {
	_G(gfxDriver)->SetCallbackForPolling(nullptr);
	_G(gfxDriver)->SetCallbackToDrawScreen(nullptr, nullptr);
	_G(gfxDriver)->SetCallbackForNullSprite(nullptr);
	_G(gfxDriver)->SetMemoryBackBuffer(nullptr);
}

// Setup virtual screen
void engine_post_gfxmode_screen_setup(const DisplayMode &dm, bool recreate_bitmaps) {
	if (recreate_bitmaps) {
		// TODO: find out if
		// - we need to support this case at all;
		// - if yes then which bitmaps need to be recreated (probably only video bitmaps and textures?)
	}
}

void engine_pre_gfxmode_screen_cleanup() {
}

// Release virtual screen
void engine_pre_gfxsystem_screen_destroy() {
}

// Setup color conversion parameters
void engine_setup_color_conversions(int coldepth) {
	// default shifts for how we store the sprite data1
	_G(_rgb_r_shift_32) = 16;
	_G(_rgb_g_shift_32) = 8;
	_G(_rgb_b_shift_32) = 0;
	_G(_rgb_r_shift_16) = 11;
	_G(_rgb_g_shift_16) = 5;
	_G(_rgb_b_shift_16) = 0;
	_G(_rgb_r_shift_15) = 10;
	_G(_rgb_g_shift_15) = 5;
	_G(_rgb_b_shift_15) = 0;

	// Most cards do 5-6-5 RGB, which is the format the files are saved in
	// Some do 5-6-5 BGR, or  6-5-5 RGB, in which case convert the gfx
	if ((coldepth == 16) && ((_G(_rgb_b_shift_16) != 0) || (_G(_rgb_r_shift_16) != 11))) {
		_G(convert_16bit_bgr) = 1;
		if (_G(_rgb_r_shift_16) == 10) {
			// some very old graphics cards lie about being 16-bit when they
			// are in fact 15-bit ... get around this
			_G(places_r) = 3;
			_G(places_g) = 3;
		}
	}
	if (coldepth > 16) {
		// when we're using 32-bit colour, it converts hi-color images
		// the wrong way round - so fix that

#if AGS_PLATFORM_OS_IOS || AGS_PLATFORM_OS_ANDROID
		_G(_rgb_b_shift_16) = 0;
		_G(_rgb_g_shift_16) = 5;
		_G(_rgb_r_shift_16) = 11;

		_G(_rgb_b_shift_15) = 0;
		_G(_rgb_g_shift_15) = 5;
		_G(_rgb_r_shift_15) = 10;

		_G(_rgb_r_shift_32) = 0;
		_G(_rgb_g_shift_32) = 8;
		_G(_rgb_b_shift_32) = 16;
#else
		_G(_rgb_r_shift_16) = 11;
		_G(_rgb_g_shift_16) = 5;
		_G(_rgb_b_shift_16) = 0;
#endif
	} else if (coldepth == 16) {
		// ensure that any 32-bit graphics displayed are converted
		// properly to the current depth
		_G(_rgb_r_shift_32) = 16;
		_G(_rgb_g_shift_32) = 8;
		_G(_rgb_b_shift_32) = 0;
	} else if (coldepth < 16) {
		// ensure that any 32-bit graphics displayed are converted
		// properly to the current depth
#if AGS_PLATFORM_OS_WINDOWS
		_G(_rgb_r_shift_32) = 16;
		_G(_rgb_g_shift_32) = 8;
		_G(_rgb_b_shift_32) = 0;
#else
		_G(_rgb_r_shift_32) = 0;
		_G(_rgb_g_shift_32) = 8;
		_G(_rgb_b_shift_32) = 16;

		_G(_rgb_b_shift_15) = 0;
		_G(_rgb_g_shift_15) = 5;
		_G(_rgb_r_shift_15) = 10;
#endif
	}

	set_color_conversion(COLORCONV_MOST | COLORCONV_EXPAND_256);
}

// Setup drawing modes and color conversions;
// they depend primarily on gfx driver capabilities and new color depth
void engine_post_gfxmode_draw_setup(const DisplayMode &dm) {
	engine_setup_color_conversions(dm.ColorDepth);
	init_draw_method();
}

// Cleanup auxiliary drawing objects
void engine_pre_gfxmode_draw_cleanup() {
	dispose_draw_method();
}

// Setup mouse control mode and graphic area
void engine_post_gfxmode_mouse_setup(const DisplayMode &dm, const Size &init_desktop) {
	// Assign mouse control parameters.
	//
	// NOTE that we setup speed and other related properties regardless of
	// whether mouse control was requested because it may be enabled later.
	_GP(mouse).SetSpeedUnit(1.f);
	if (_GP(usetup).mouse_speed_def == kMouseSpeed_CurrentDisplay) {
		Size cur_desktop;
		if (get_desktop_resolution(&cur_desktop.Width, &cur_desktop.Height) == 0)
			_GP(mouse).SetSpeedUnit(Math::Max((float)cur_desktop.Width / (float)init_desktop.Width,
			(float)cur_desktop.Height / (float)init_desktop.Height));
	}

	Mouse_EnableControl(_GP(usetup).mouse_ctrl_enabled);
	Debug::Printf(kDbgMsg_Info, "Mouse control: %s, base: %f, speed: %f", _GP(mouse).IsControlEnabled() ? "on" : "off",
		_GP(mouse).GetSpeedUnit(), _GP(mouse).GetSpeed());

	on_coordinates_scaling_changed();

	// If auto lock option is set, lock mouse to the game window
	if (_GP(usetup).mouse_auto_lock && _GP(scsystem).windowed != 0)
		_GP(mouse).TryLockToWindow();
}

// Reset mouse controls before changing gfx mode
void engine_pre_gfxmode_mouse_cleanup() {
	// Always disable mouse control and unlock mouse when releasing down gfx mode
	_GP(mouse).DisableControl();
	_GP(mouse).UnlockFromWindow();
}

// Fill in scsystem struct with display mode parameters
void engine_setup_scsystem_screen(const DisplayMode &dm) {
	_GP(scsystem).coldepth = dm.ColorDepth;
	_GP(scsystem).windowed = dm.Windowed;
	_GP(scsystem).vsync = dm.Vsync;
}

void engine_post_gfxmode_setup(const Size &init_desktop) {
	DisplayMode dm = _G(gfxDriver)->GetDisplayMode();
	// If color depth has changed (or graphics mode was inited for the
	// very first time), we also need to recreate bitmaps
	bool has_driver_changed = _GP(scsystem).coldepth != dm.ColorDepth;

	engine_setup_scsystem_screen(dm);
	engine_post_gfxmode_driver_setup();
	engine_post_gfxmode_screen_setup(dm, has_driver_changed);
	if (has_driver_changed)
		engine_post_gfxmode_draw_setup(dm);
	engine_post_gfxmode_mouse_setup(dm, init_desktop);

	// TODO: the only reason this call was put here is that it requires
	// "windowed" flag to be specified. Find out whether this function
	// has anything to do with graphics mode at all. It is quite possible
	// that we may split it into two functions, or remove parameter.
	_G(platform)->PostAllegroInit(_GP(scsystem).windowed != 0);

	video_on_gfxmode_changed();
	invalidate_screen();
}

void engine_pre_gfxmode_release() {
	engine_pre_gfxmode_mouse_cleanup();
	engine_pre_gfxmode_driver_cleanup();
	engine_pre_gfxmode_screen_cleanup();
}

void engine_pre_gfxsystem_shutdown() {
	engine_pre_gfxmode_release();
	engine_pre_gfxmode_draw_cleanup();
	engine_pre_gfxsystem_screen_destroy();
}

void on_coordinates_scaling_changed() {
	// Reset mouse graphic area and bounds
	_GP(mouse).SetGraphicArea();
	// If mouse bounds do not have valid values yet, then limit cursor to viewport
	if (_GP(play).mboundx1 == 0 && _GP(play).mboundy1 == 0 && _GP(play).mboundx2 == 0 && _GP(play).mboundy2 == 0)
		_GP(mouse).SetMoveLimit(_GP(play).GetMainViewport());
	else
		_GP(mouse).SetMoveLimit(Rect(_GP(play).mboundx1, _GP(play).mboundy1, _GP(play).mboundx2, _GP(play).mboundy2));
}

} // namespace AGS3
