/* xoreos - A reimplementation of BioWare's Aurora engine
 *
 * xoreos is the legal property of its developers, whose names
 * can be found in the AUTHORS file distributed with this source
 * distribution.
 *
 * xoreos is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * xoreos is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with xoreos. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file
 *  A room within a Star Wars: Knights of the Old Republic area.
 */

#ifndef ENGINES_KOTOR_ROOM_H
#define ENGINES_KOTOR_ROOM_H

#include "src/common/scopedptr.h"

#include "src/graphics/aurora/types.h"

#include "src/engines/kotor/walkmesh.h"

namespace Common {
	class UString;
}

namespace Engines {

namespace KotOR {

class Room {
public:
	Room(const Common::UString &resRef, float x, float y, float z);
	~Room();

	Common::UString getResRef() const;
	float evaluateElevation(float x, float y, bool highlight = false);
	void disableWalkmeshHighlight();
	void setWalkmeshInvisible(bool invisible);

	void show();
	void hide();
	bool isVisible() const;

private:
	Common::UString _resRef;
	Common::ScopedPtr<Graphics::Aurora::Model> _model;
	Walkmesh _walkmesh;

	void load(const Common::UString &resRef, float x, float y, float z);
};

} // End of namespace KotOR

} // End of namespace Engines

#endif // ENGINES_KOTOR_ROOM_H
