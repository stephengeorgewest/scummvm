/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "mtropolis/debug.h"

#include "gui/dialog.h"

namespace MTropolis {

DebugInspector::DebugInspector(IDebuggable *debuggable) {
}

DebugInspector::~DebugInspector() {
}

void DebugInspector::onDestroyed() {
	_debuggable = nullptr;
}

Debugger::Debugger(Runtime *runtime) : _paused(false), _runtime(runtime) {
}

Debugger::~Debugger() {
}

void Debugger::setPaused(bool paused) {
	_paused = paused;
}

bool Debugger::isPaused() const {
	return _paused;
}

void Debugger::notify(DebugSeverity severity, const Common::String& str) {
	// TODO
}

void Debugger::notifyFmt(DebugSeverity severity, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	this->vnotifyFmt(severity, fmt, args);
	va_end(args);
}

void Debugger::vnotifyFmt(DebugSeverity severity, const char* fmt, va_list args) {
	Common::String str(Common::String::vformat(fmt, args));
	this->notify(severity, fmt);
}

} // End of namespace MTropolis
