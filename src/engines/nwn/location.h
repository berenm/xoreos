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

/** @file engines/nwn/location.h
 *  A location within a NWN area.
 */

#ifndef ENGINES_NWN_LOCATION_H
#define ENGINES_NWN_LOCATION_H

#include "aurora/nwscript/enginetype.h"
#include "common/types.h"

namespace Engines {

namespace NWN {

class Area;

class Location : public Aurora::NWScript::EngineType {
public:
	Location();
	~Location();

	/** Clone factory method. */
	Location *clone() const;

	/** Return the location's area. */
	Area *getArea() const;
	/** Set the location's area. */
	void setArea(Area *area);

	/** Return the location's position. */
	glm::vec3 getPosition() const;
	/** Set the location's position. */
	void setPosition(const glm::vec3 &position);

	/** Return the location's orientation. */
	float getFacing() const;
	/** Set the location's orientation. */
	void setFacing(float facing);

private:
	Area *_area; ///< The location's area.

	glm::vec3 _position; ///< The location's position.
	float _facing;       ///< The location's orientation.
};

} // End of namespace NWN

} // End of namespace Engines

#endif // ENGINES_NWN_LOCATION_H
