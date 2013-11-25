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

/** @file engines/nwn/location.cpp
 *  A location within a NWN area.
 */

#include "engines/nwn/location.h"

namespace Engines {

namespace NWN {

Location::Location() : _area(0), _facing(0.0f) {
	_position[0] = 0.0f;
	_position[1] = 0.0f;
	_position[2] = 0.0f;
}

Location::~Location() {
}

Location *Location::clone() const {
	return new Location(*this);
}

Area *Location::getArea() const {
	return _area;
}

void Location::setArea(Area *area) {
	_area = area;
}

glm::vec3 Location::getPosition() const {
	return _position;
}

void Location::setPosition(const glm::vec3 &position) {
	_position = position;
}

float Location::getFacing() const {
	return _facing;
}

void Location::setFacing(float facing) {
	_facing = facing;
}

} // End of namespace NWN

} // End of namespace Engines
