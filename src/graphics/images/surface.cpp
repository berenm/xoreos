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

/** @file graphics/images/surface.cpp
 *  An image surface, in BGRA format.
 */

#include <cassert>

#include "graphics/images/surface.h"

namespace Graphics {

Surface::Surface(int width, int height) {
	assert((width > 0) && (height > 0));

	_compressed = false;
	_hasAlpha   = true;
	_format     = kPixelFormatBGRA;
	_formatRaw  = kPixelFormatRGBA8;
	_dataType   = kPixelDataType8;

	_mipMaps.push_back(new MipMap);

	_mipMaps[0]->width  = width;
	_mipMaps[0]->height = height;
	_mipMaps[0]->data.resize(_mipMaps[0]->width * _mipMaps[0]->height * 4);
}

Surface::~Surface() {
}

int Surface::getWidth() const {
	return _mipMaps[0]->width;
}

int Surface::getHeight() const {
	return _mipMaps[0]->height;
}

byte *Surface::getData() {
	return &_mipMaps[0]->data[0];
}

const byte *Surface::getData() const {
	return &_mipMaps[0]->data[0];
}

void Surface::fill(byte r, byte g, byte b, byte a) {
	if ((r == g) && (r == b) && (r == a)) {
		memset(&_mipMaps[0]->data[0], r, _mipMaps[0]->data.size());
		return;
	}

	byte  *data = &_mipMaps[0]->data[0];
	uint32 size = _mipMaps[0]->data.size() / 4;
	while (size-- > 0) {
		*data++ = b;
		*data++ = g;
		*data++ = r;
		*data++ = a;
	}
}

} // End of namespace Graphics
