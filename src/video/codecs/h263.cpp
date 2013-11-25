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

/** @file video/codecs/h263.cpp
 *  h.263 video codec.
 */

#include <cstring>

#include <xvid.h>

#include "common/util.h"
#include "common/stream.h"

#include "graphics/yuv_to_rgb.h"

#include "graphics/images/surface.h"

#include "video/codecs/h263.h"

namespace Video {

H263Codec::H263Codec(uint32 width, uint32 height) : _width(width), _height(height) {
	xvid_gbl_init_t xvid_gbl_init;
	memset(&xvid_gbl_init, 0, sizeof(xvid_gbl_init_t));
	xvid_gbl_init.version = XVID_VERSION;
	xvid_gbl_init.debug = 0;//XVID_DEBUG_ERROR | XVID_DEBUG_HEADER | XVID_DEBUG_STARTCODE;
	xvid_global(0, XVID_GBL_INIT, &xvid_gbl_init, 0);

	xvid_dec_create_t xvid_dec_create;
	memset(&xvid_dec_create, 0, sizeof(xvid_dec_create_t));
	xvid_dec_create.version = XVID_VERSION;
	xvid_dec_create.width = width;
	xvid_dec_create.height = height;
	if (xvid_decore(0, XVID_DEC_CREATE, &xvid_dec_create, 0) != 0)
		error("Could not initialize xvid decoder");

	_decHandle = xvid_dec_create.handle;
}

H263Codec::~H263Codec() {
	xvid_decore(_decHandle, XVID_DEC_DESTROY, 0, 0);
}

void H263Codec::decodeFrame(Graphics::Surface &surface, Common::SeekableReadStream &dataStream) {
	// NOTE: When asking libxvidcore to decode the video into BGRA, it fills the alpha
	//       values with 0x00, rendering the output invisible (!).
	//       Since we, surprise, actually want to see the video, we would have to pass
	//       over the whole video data and fix-up the alpha values ourselves. Or
	//       alternatively do the YUV->BGRA conversion ourselves. We chose the latter.

	int dataSize = dataStream.size();
	byte *data = new byte[dataSize];
	dataStream.read(data, dataSize);

	xvid_dec_frame_t xvid_dec_frame;
	memset(&xvid_dec_frame, 0, sizeof(xvid_dec_frame_t));
	xvid_dec_frame.version    = XVID_VERSION;
	xvid_dec_frame.general    = XVID_DEBLOCKY | XVID_DEBLOCKUV | XVID_DERINGY | XVID_DERINGUV;
	xvid_dec_frame.bitstream  = data;
	xvid_dec_frame.length     = dataSize;
	xvid_dec_frame.output.csp = XVID_CSP_INTERNAL;

	xvid_dec_stats_t xvid_dec_stats;
	memset(&xvid_dec_stats, 0, sizeof(xvid_dec_stats_t));
	xvid_dec_stats.version = XVID_VERSION;

	int c = xvid_decore(_decHandle, XVID_DEC_DECODE, &xvid_dec_frame, &xvid_dec_stats);
	if ((dataSize - c) > 1)
		warning("H263Codec::decodeFrame(): %d bytes left in frame", dataSize - c);

	delete[] data;

	if (xvid_dec_frame.output.plane[0] &&
	    xvid_dec_frame.output.plane[1] &&
	    xvid_dec_frame.output.plane[2])
		YUVToRGBMan.convert420(Graphics::YUVToRGBManager::kScaleFull,
				surface.getData(), surface.getSize().x * 4,
				(const byte *) xvid_dec_frame.output.plane[0],
				(const byte *) xvid_dec_frame.output.plane[1],
				(const byte *) xvid_dec_frame.output.plane[2], _width, _height,
				xvid_dec_frame.output.stride[0], xvid_dec_frame.output.stride[1]);
}

} // End of namespace Video
