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

/** @file graphics/aurora/abcfont.h
 *  An ABC/SBM font, as used by Jade Empire.
 */

#ifndef GRAPHICS_AURORA_ABCFONT_H
#define GRAPHICS_AURORA_ABCFONT_H

#include "common/types.h"

#include "graphics/font.h"

#include "graphics/aurora/textureman.h"

namespace Common {
	class UString;
	class SeekableReadStream;
}

namespace Graphics {

namespace Aurora {

/** An ABC/SBM font, as used by Jade Empire. */
class ABCFont : public Graphics::Font {
public:
	ABCFont(const Common::UString &name);
	~ABCFont();

	float getWidth (uint32 c) const;
	float getHeight()         const;

	void draw(uint32 c) const;

private:
	/** A font character. */
	struct Char {
		glm::uvec2 position;
		uint8      width;
		int8       spaceL;
		int8       spaceR;

		glm::vec2 tpos[4];
		glm::vec2 vpos[4];
	};

	TextureHandle _texture;

	uint8 _base;

	Char _invalid;
	Char _ascii[128];
	std::map<uint32, Char> _extended;

	void load(const Common::UString &name);

	void readCharDesc(Char &c, Common::SeekableReadStream &abc);
	void calcCharVertices(Char &c);

	const Char &findChar(uint32 c) const;
};

} // End of namespace Aurora

} // End of namespace Graphics

#endif // GRAPHICS_AURORA_ABCFONT_H
