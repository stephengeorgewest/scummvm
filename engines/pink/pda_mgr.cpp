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

#include "pink/pda_mgr.h"
#include "pink/pink.h"
#include "pink/objects/actors/pda_button_actor.h"
#include "pink/objects/actors/lead_actor.h"
#include "pink/objects/pages/pda_page.h"

namespace Pink {

PDAMgr::PDAMgr(Pink::PinkEngine *game)
	: _game(game), _page(nullptr), _cursorMgr(game, nullptr) {}

PDAMgr::~PDAMgr() {
	for (uint i = 0; i < _globalActors.size(); ++i) {
		delete _globalActors[i];
	}
	delete _page;
}

void PDAMgr::saveState(Archive &archive) {
	if (_page)
		archive.writeString(_page->getName());
	else
		archive.writeString("");
}

void PDAMgr::execute(const Command &command) {
	switch (command.type) {
	case Command::kGoToPage:
		goToPage(command.arg);
		break;
	case Command::kClose:
		close();
		break;
	default:
		break;
	}
}

void PDAMgr::goToPage(const Common::String &pageName) {
	if (_page && _page->getName() == pageName)
		return;

	loadGlobal();

	PDAPage *newPage = new PDAPage(PDAPage::create(pageName, *this));
	delete _page;
	_page = newPage;

	_page->init();

	for (uint i = 0; i < _globalActors.size(); ++i) {
		_globalActors[i]->setPage(_page);
	}

	_cursorMgr.setPage(_page);
}

void PDAMgr::onLeftButtonClick(Common::Point point) {
	Actor *actor = _game->getDirector()->getActorByPoint(point);
	if (actor)
		actor->onLeftClickMessage();
}

void PDAMgr::onMouseMove(Common::Point point) {
	Actor *actor = _game->getDirector()->getActorByPoint(point);
	if (actor && dynamic_cast<PDAButtonActor *>(actor))
		actor->onMouseOver(point, &_cursorMgr);
	else
		_cursorMgr.setCursor(kPDADefaultCursor, point, Common::String());
}

void PDAMgr::close() {
	for (uint i = 0; i < _globalActors.size(); ++i) {
		delete _globalActors[i];
	}
	_globalActors.clear();

	delete _page;
	_page = nullptr;

	_lead->onPDAClose();
}

void PDAMgr::loadGlobal() {
	if (!_globalActors.empty())
		return;

	PDAPage globalPage = PDAPage::create("GLOBAL", *this);
	_globalActors = globalPage.takeActors();
	for (uint i = 0; i < _globalActors.size(); ++i) {
		_globalActors[i]->init(0);
	}
}

} // End of namespace Pink
