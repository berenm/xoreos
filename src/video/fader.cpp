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

/** @file video/fader.cpp
 *  Testing implementation for the VideoDecoder interface.
 */

#include "common/util.h"

#include "graphics/images/surface.h"

#include "video/fader.h"

#include "events/events.h"

namespace Video {

Fader::Fader(const glm::uvec2 &size, int n) : _c(0), _n(n), _lastUpdate(0) {
	initVideo(size);
}

Fader::~Fader() {
}

bool Fader::hasTime() const {
	if (!_started)
		return true;

	if ((EventMan.getTimestamp() - _lastUpdate) < 20)
		return true;

	return false;
}

void Fader::processData() {
	uint32 curTime  = EventMan.getTimestamp();
	uint32 diffTime = curTime - _lastUpdate;
	if (_started && (diffTime < 20))
		return;

	if (!_started) {
		diffTime = 0;

		_lastUpdate = curTime;
		_started    = true;

		_c = 0;
	} else
		_c += 2;

	// Fade from black to green
	byte *data = _surface->getData();
	for (uint32 i = 0; i < _size.y; i++) {
		byte *rowData = data;

		for (uint32 j = 0; j < _size.x; j++, rowData += 4) {
			rowData[0] = 0;
			rowData[1] = _c;
			rowData[2] = 0;
			rowData[3] = 255;
		}

		data += _surface->getSize().x * 4;
	}

	// Keep a red square in the middle
	int xPos = (_size.x / 2) - 2;
	int yPos = (_size.y / 2) - 2;
	int dPos = (yPos * _surface->getSize().x + xPos) * 4;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			_surface->getData()[dPos + j * 4 + 0] =   0;
			_surface->getData()[dPos + j * 4 + 1] =   0;
			_surface->getData()[dPos + j * 4 + 2] = 255;
			_surface->getData()[dPos + j * 4 + 3] = 255;
		}
		dPos += _surface->getSize().x * 4;
	}

	_lastUpdate = curTime;

	if (_c == 0)
		if (_n-- <= 0)
			finish();

	_needCopy = true;
}

} // End of namespace Video
