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

/** @file aurora/lytfile.cpp
 *  Handling BioWare's LYTs (Layout files).
 */

#include "common/stream.h"
#include "common/streamtokenizer.h"
#include "common/util.h"

#include "aurora/lytfile.h"
#include "aurora/error.h"

namespace Aurora {

LYTFile::LYTFile() {
}

LYTFile::~LYTFile() {
}

void LYTFile::clear() {
	_rooms.clear();
	_doorHooks.clear();
	_fileDependency.clear();
}

void LYTFile::load(Common::SeekableReadStream &lyt) {
	clear();

	Common::StreamTokenizer tokenizer(Common::StreamTokenizer::kRuleIgnoreAll);
	tokenizer.addSeparator(' ');
	tokenizer.addChunkEnd('\n');
	tokenizer.addIgnore('\r');

	while (!lyt.eos() && !lyt.err()) {
		std::vector<Common::UString> strings;
		tokenizer.getTokens(lyt, strings);

		if (strings.empty()) {
			// Empty line?
		} else if (*strings[0].begin() == '#') {
			// Comment line
		} else if (strings[0] == "filedependancy") {
			// A clone2727 note: It's spelled "dependency", BioWare.
			_fileDependency = strings[1];
		} else if (strings[0] == "roomcount") {
			int roomCount = atoi(strings[1].c_str());
			_rooms.resize(roomCount);

			for (int i = 0; i < roomCount; i++) {
				tokenizer.nextChunk(lyt);
				tokenizer.getTokens(lyt, strings);

				_rooms[i].model = strings[0];
				strings[1].parse(_rooms[i].position.x);
				strings[2].parse(_rooms[i].position.y);
				strings[3].parse(_rooms[i].position.z);
			}
		} else if (strings[0] == "trackcount") {
			int trackCount = atoi(strings[1].c_str());

			// TODO! Just ignore for now
			for (int i = 0; i < trackCount; i++) {
				tokenizer.nextChunk(lyt);
				//tokenizer.getTokens(lyt, strings);
			}
		} else if (strings[0] == "obstaclecount") {
			int obstacleCount = atoi(strings[1].c_str());

			// TODO! Just ignore for now
			for (int i = 0; i < obstacleCount; i++) {
				tokenizer.nextChunk(lyt);
				//tokenizer.getTokens(lyt, strings);
			}
		} else if (strings[0] == "artplaceablecount") {
			int obstacleCount = atoi(strings[1].c_str());

			// TODO! Just ignore for now
			for (int i = 0; i < obstacleCount; i++) {
				tokenizer.nextChunk(lyt);
				//tokenizer.getTokens(lyt, strings);
			}
		} else if (strings[0] == "walkmeshRooms") {
			int obstacleCount = atoi(strings[1].c_str());

			// TODO! Just ignore for now
			for (int i = 0; i < obstacleCount; i++) {
				tokenizer.nextChunk(lyt);
				//tokenizer.getTokens(lyt, strings);
			}
		} else if (strings[0] == "doorhookcount") {
			int doorHookCount = atoi(strings[1].c_str());
			_doorHooks.resize(doorHookCount);

			for (int i = 0; i < doorHookCount; i++) {
				tokenizer.nextChunk(lyt);
				tokenizer.getTokens(lyt, strings);

				_doorHooks[i].name = strings[0];
				strings[1].parse(_doorHooks[i].unk0);
				strings[2].parse(_doorHooks[i].position.x);
				strings[3].parse(_doorHooks[i].position.y);
				strings[4].parse(_doorHooks[i].position.z);
				strings[5].parse(_doorHooks[i].unk1);
				strings[6].parse(_doorHooks[i].unk2);
				strings[7].parse(_doorHooks[i].unk3);
				strings[8].parse(_doorHooks[i].unk4);
				strings[9].parse(_doorHooks[i].unk5);
			}
		} else if (strings[0] == "beginlayout") {
			// Ignore, we don't need it
		} else if (strings[0] == "donelayout") {
			// End parsing
			break;
		} else {
			throw Common::Exception("Unknown LYT token %s", strings[0].c_str());
		}

		tokenizer.nextChunk(lyt);
	}
}

const LYTFile::RoomArray &LYTFile::getRooms() const {
	return _rooms;
}

const LYTFile::DoorHookArray &LYTFile::getDoorHooks() const {
	return _doorHooks;
}

Common::UString LYTFile::getFileDependency() const {
	return _fileDependency;
}

} // End of namespace Aurora
