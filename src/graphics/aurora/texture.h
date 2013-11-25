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

/** @file graphics/aurora/texture.h
 *  A texture as used in the Aurora engines.
 */

#ifndef GRAPHICS_AURORA_TEXTURE_H
#define GRAPHICS_AURORA_TEXTURE_H

#include "common/ustring.h"

#include "graphics/types.h"
#include "graphics/texture.h"

#include "aurora/types.h"

namespace Common {
	class SeekableReadStream;
}

namespace Graphics {

class ImageDecoder;
class TXI;

namespace Aurora {

/** A texture. */
class Texture : public Graphics::Texture {
public:
	/** Create a texture from this image resource. */
	Texture(const Common::UString &name);
	/** Take over the image and create a texture from it. */
	Texture(ImageDecoder *image, const TXI *txi = 0);
	~Texture();

	glm::uvec2 getSize() const;

	bool hasAlpha() const;

	/** Return the TXI. */
	const TXI &getTXI() const;

	/** Reload the texture from this image resource. */
	bool reload(const Common::UString &name = "");
	/** Reload the texture from this image. */
	bool reload(ImageDecoder *image, const TXI *txi = 0);

	/** Dump the texture into a TGA. */
	bool dumpTGA(const Common::UString &fileName) const;

protected:
	// GLContainer
	void doRebuild();
	void doDestroy();

private:
	Common::UString _name;

	TextureID _textureID; ///< OpenGL texture ID.

	::Aurora::FileType _type; ///< The texture's image's file type.

	ImageDecoder *_image; ///< The actual image.
	TXI *_txi;            ///< The TXI.

	glm::uvec2 _size;

	void load(const Common::UString &name);
	void load(ImageDecoder *image);

	void loadTXI(Common::SeekableReadStream *stream);
	void loadImage();

	TextureID getID() const;

	friend class TextureManager;
};

} // End of namespace Aurora

} // End of namespace Graphics

#endif // GRAPHICS_AURORA_TEXTURE_H
