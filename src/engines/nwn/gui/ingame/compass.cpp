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

/** @file engines/nwn/gui/ingame/compass.cpp
 *  The NWN ingame compass.
 */

#include "common/util.h"

#include "graphics/graphics.h"
#include "graphics/camera.h"

#include "graphics/aurora/modelnode.h"
#include "graphics/aurora/model.h"

#include "engines/nwn/gui/widgets/panel.h"

#include "engines/nwn/gui/ingame/compass.h"

namespace Engines {

namespace NWN {

// TODO: The compass "needle" is hidden by directions disk.
//       This seems to be a Z-order problem, should be fixed
//       alongside the transparency problem once I've revamped models...
CompassWidget::CompassWidget(::Engines::GUI &gui, const Common::UString &tag) :
	ModelWidget(gui, tag, "ctl_compass") {
}

CompassWidget::~CompassWidget() {
}

// TODO: The disk rotation should feel more "natural", i.e. it should
//       be more sluggish.
void CompassWidget::setRotation(const glm::vec3 &rotation) {
	_model->setRotation(glm::vec3(-rotation.x, 0.0, 0.0));
	Graphics::Aurora::ModelNode *pointer = _model->getNode("cmp_pointer");
	if (pointer)
		pointer->setRotation(glm::vec3(0.0, 0.0, rotation.y));
}


Compass::Compass(float position) {
	// Panel

	WidgetPanel *panel = new WidgetPanel(*this, "CompassPanel", "pnl_compass");

	const glm::vec2 panelSize = panel->getSize();

	panel->setPosition(glm::vec3(- panelSize.x, position, 0.0));

	addWidget(panel);


	// Compass

	_compass = new CompassWidget(*this, "Compass");

	_compass->setPosition(glm::vec3(- (panelSize.x / 2.0), position + (panelSize.y / 2.0), -100.0));

	addWidget(_compass);

	notifyResized(glm::ivec2(0, 0), GfxMan.getScreenSize());
}

Compass::~Compass() {
}

void Compass::callbackActive(Widget &widget) {
}

void Compass::notifyResized(const glm::ivec2 &oldSize, const glm::ivec2 &newSize) {
	setPosition(glm::vec3(newSize.x / 2.0, -(newSize.y / 2.0), -10.0));
}

void Compass::notifyCameraMoved() {
	const glm::vec3 orientation = CameraMan.getOrientation();

	_compass->setRotation(orientation + glm::vec3(90.0, 0.0, 0.0));
}

} // End of namespace NWN

} // End of namespace Engines
