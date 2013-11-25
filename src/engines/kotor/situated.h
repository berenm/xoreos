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

/** @file engines/kotor/situated.h
 *  KotOR situated object.
 */

#ifndef ENGINES_KOTOR_SITUATED_H
#define ENGINES_KOTOR_SITUATED_H

#include "aurora/types.h"

#include "graphics/aurora/types.h"

#include "engines/kotor/object.h"

namespace Engines {

namespace KotOR {

/** KotOR situated object. */
class Situated : public Object {
public:
	Situated();
	~Situated();

	void load(const Aurora::GFFStruct &situated);

	void show();
	void hide();

	void setPosition(const glm::vec3 &position);
	void setOrientation(const glm::vec3 &orientation);

protected:
	Common::UString _modelName;

	uint32 _appearanceID;

	Graphics::Aurora::Model *_model;


	void load(const Aurora::GFFStruct &instance, const Aurora::GFFStruct *blueprint = 0);

	virtual void loadObject(const Aurora::GFFStruct &gff) = 0;
	virtual void loadAppearance() = 0;


private:
	void loadProperties(const Aurora::GFFStruct &gff);
	void loadPortrait(const Aurora::GFFStruct &gff);
};

} // End of namespace KotOR

} // End of namespace Engines

#endif // ENGINES_KOTOR_SITUATED_H
