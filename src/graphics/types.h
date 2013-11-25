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

/** @file graphics/types.h
 *  Basic graphics types.
 */

#ifndef GRAPHICS_TYPES_H
#define GRAPHICS_TYPES_H

#include <vector>

#include <SDL.h>

#include "glew/glew.h"

#include "common/types.h"

namespace Graphics {

typedef GLuint TextureID;
typedef GLuint ListID;

enum PixelFormat {
	kPixelFormatRGB  = GL_RGB ,
	kPixelFormatRGBA = GL_RGBA,
	kPixelFormatBGR  = GL_BGR ,
	kPixelFormatBGRA = GL_BGRA,
};

enum PixelFormatRaw {
	kPixelFormatRGBA8  = GL_RGBA8,
	kPixelFormatRGB8   = GL_RGB8,
	kPixelFormatRGB5A1 = GL_RGB5_A1,
	kPixelFormatRGB5   = GL_RGB5,
	kPixelFormatDXT1   = GL_COMPRESSED_RGB_S3TC_DXT1_EXT,
	kPixelFormatDXT3   = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
	kPixelFormatDXT5   = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
};

enum PixelDataType {
	kPixelDataType8    = GL_UNSIGNED_BYTE,
	kPixelDataType1555 = GL_UNSIGNED_SHORT_1_5_5_5_REV,
	kPixelDataType565  = GL_UNSIGNED_SHORT_5_6_5
};

enum QueueType {
	kQueueTexture               = 0, ///< A texture.
	kQueueNewTexture               , ///< A newly created texture.
	kQueueWorldObject              , ///< An object in 3D space.
	kQueueVisibleWorldObject       , ///< A visible object in 3D space.
	kQueueGUIFrontObject           , ///< A GUI object.
	kQueueVisibleGUIFrontObject    , ///< A visible GUI object.
	kQueueVideo                    , ///< A video.
	kQueueVisibleVideo             , ///< A currently playing video.
	kQueueGLContainer              , ///< An object containing OpenGL structures.
	kQueueMAX                        ///< For range checks.
};

enum RenderableType {
	kRenderableTypeVideo    = 0,
	kRenderableTypeObject      ,
	kRenderableTypeGUIFront
};

enum RenderPass {
	kRenderPassOpaque      = 0, ///< Only render opaque parts.
	kRenderPassTransparent = 1, ///< Only render transparent parts.
	kRenderPassAll         = 2, ///< Render all parts.
};

struct ColorPosition {
	uint32 position;

	bool defaultColor;

	glm::vec4 color;
};

typedef std::vector<ColorPosition> ColorPositions;

#undef glCompressedTexImage2D
// Aliased to either glCompressedTexImage2D or glCompressedTexImage2DARB, whichever is available
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;

} // End of namespace Graphics

#endif // GRAPHICS_TYPES_H
