/* xoreos - A reimplementation of BioWare's Aurora engine
 *
 * xoreos is the legal property of its developers, whose names can be
 * found in the AUTHORS file distributed with this source
 * distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
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
 *
 * The Infinity, Aurora, Odyssey, Eclipse and Lycium engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 */

/** @file engines/nwn/gui/options/gorepass.cpp
 *  The NWN violence/gore password options menu.
 */

#include "engines/nwn/gui/widgets/panel.h"

#include "engines/nwn/gui/options/gorepass.h"

namespace Engines {

namespace NWN {

OptionsGorePasswordMenu::OptionsGorePasswordMenu(bool isMain) {
	load("options_gorepass");

	if (isMain) {
		WidgetPanel *backdrop = new WidgetPanel(*this, "PNL_MAINMENU", "pnl_mainmenu");
		backdrop->setPosition(glm::vec3(0.0, 0.0, 100.0));
		addWidget(backdrop);
	}

	// TODO: Password
	getWidget("OldPassword"    , true)->setDisabled(true);
	getWidget("NewPassword"    , true)->setDisabled(true);
	getWidget("ConfirmPassword", true)->setDisabled(true);
}

OptionsGorePasswordMenu::~OptionsGorePasswordMenu() {
}

void OptionsGorePasswordMenu::callbackActive(Widget &widget) {
	if ((widget.getTag() == "CancelButton") ||
	    (widget.getTag() == "CTL_BTN_X")) {
		_returnCode = 1;
		return;
	}

	if (widget.getTag() == "OkButton") {
		_returnCode = 2;
		return;
	}
}

} // End of namespace NWN

} // End of namespace Engines
