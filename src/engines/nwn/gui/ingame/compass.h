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

/** @file engines/nwn/gui/ingame/compass.h
 *  The NWN ingame compass.
 */

#ifndef ENGINES_NWN_GUI_INGAME_COMPASS_H
#define ENGINES_NWN_GUI_INGAME_COMPASS_H

#include "events/notifyable.h"

#include "engines/nwn/gui/widgets/modelwidget.h"

#include "engines/nwn/gui/gui.h"

namespace Engines {

namespace NWN {

/** The NWN compass widget. */
class CompassWidget : public ModelWidget {
public:
	CompassWidget(::Engines::GUI &gui, const Common::UString &tag);
	~CompassWidget();

	void setRotation(const glm::vec3 &rotation);
};

/** The NWN ingame compass. */
class Compass : public GUI, public Events::Notifyable {
public:
	Compass(float position);
	~Compass();

protected:
	void callbackActive(Widget &widget);

private:
	CompassWidget *_compass;

	void notifyResized(const glm::ivec2 &oldSize, const glm::ivec2 &newSize);
	void notifyCameraMoved();
};

} // End of namespace NWN

} // End of namespace Engines

#endif // ENGINES_NWN_GUI_INGAME_COMPASS_H
