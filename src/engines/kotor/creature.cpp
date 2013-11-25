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

/** @file engines/kotor/creature.cpp
 *  A creature.
 */

#include "engines/kotor/creature.h"

#include "common/util.h"
#include "common/maths.h"
#include "common/error.h"
#include "common/ustring.h"

#include "aurora/2dafile.h"
#include "aurora/2dareg.h"
#include "aurora/gfffile.h"
#include "aurora/locstring.h"

#include "graphics/aurora/modelnode.h"
#include "graphics/aurora/model.h"

#include "engines/aurora/util.h"
#include "engines/aurora/model.h"

namespace Engines {

namespace KotOR {

Creature::Creature() : _appearance(Aurora::kFieldIDInvalid), _model(0) {
}

Creature::~Creature() {
	delete _model;
}

void Creature::show() {
	if (_model)
		_model->show();
}

void Creature::hide() {
	if (_model)
		_model->hide();
}

void Creature::setPosition(const glm::vec3 &position) {
	Object::setPosition(position);
	const glm::vec3 p = Object::getPosition();

	_model->setPosition(p);
}

void Creature::setOrientation(const glm::vec3 &orientation) {
	Object::setOrientation(orientation);
	const glm::vec3 o = Object::getOrientation();

	_model->setRotation(glm::vec3(o.x, o.z, -o.y));
}

void Creature::load(const Aurora::GFFStruct &creature) {
	Common::UString temp = creature.getString("TemplateResRef");

	Aurora::GFFFile *utc = 0;
	if (!temp.empty()) {
		try {
			utc = new Aurora::GFFFile(temp, Aurora::kFileTypeUTC, MKTAG('U', 'T', 'C', ' '));
		} catch (...) {
			delete utc;
		}
	}

	load(creature, utc ? &utc->getTopLevel() : 0);

	if (!utc)
		warning("Creature \"%s\" has no blueprint", _tag.c_str());

	delete utc;
}

void Creature::load(const Aurora::GFFStruct &instance, const Aurora::GFFStruct *blueprint) {
	assert(!_loaded);

	// General properties

	if (blueprint)
		loadProperties(*blueprint); // Blueprint
	loadProperties(instance);    // Instance


	// Appearance

	if (_appearance == Aurora::kFieldIDInvalid)
		throw Common::Exception("Creature without an appearance");

	loadAppearance();

	// Position

	setPosition(glm::vec3(instance.getDouble("XPosition"),
	                      instance.getDouble("YPosition"),
	                      instance.getDouble("ZPosition")));

	// Orientation

	const glm::vec2 bearing = glm::vec2(instance.getDouble("XOrientation"),
	                                    instance.getDouble("YOrientation"));

	const glm::vec3 o = Common::vector2orientation(bearing);

	setOrientation(o);

	_loaded = true;
}

void Creature::loadProperties(const Aurora::GFFStruct &gff) {
	// Tag
	_tag = gff.getString("Tag", _tag);

	// Name
	if (gff.hasField("LocName")) {
		Aurora::LocString name;
		gff.getLocString("LocName", name);

		_name = name.getString();
	}

	// Description
	if (gff.hasField("Description")) {
		Aurora::LocString description;
		gff.getLocString("Description", description);

		_description = description.getString();
	}

	// Portrait
	loadPortrait(gff);

	// Appearance
	_appearance = gff.getUint("Appearance_Type", _appearance);

	// Static
	_static = gff.getBool("Static", _static);

	// Usable
	_usable = gff.getBool("Useable", _usable);
}

void Creature::loadPortrait(const Aurora::GFFStruct &gff) {
	uint32 portraitID = gff.getUint("PortraitId");
	if (portraitID != 0) {
		const Aurora::TwoDAFile &twoda = TwoDAReg.get("portraits");

		Common::UString portrait = twoda.getRow(portraitID).getString("BaseResRef");
		if (!portrait.empty())
			_portrait = "po_" + portrait;
	}

	_portrait = gff.getString("Portrait", _portrait);
}

void Creature::loadAppearance() {
	PartModels parts;

	getPartModels(parts);

	if ((parts.type == "P") || parts.body.empty()) {
		warning("TODO: Model \"%s\": ModelType \"%s\" (\"%s\")",
		        _tag.c_str(), parts.type.c_str(), parts.body.c_str());
		return;
	}

	loadBody(parts);
	loadHead(parts);
}

void Creature::getPartModels(PartModels &parts, uint32 state) {
	const Aurora::TwoDARow &appearance = TwoDAReg.get("appearance").getRow(_appearance);

	parts.type = appearance.getString("modeltype");

	parts.body = appearance.getString(Common::UString("model") + state);
	if (parts.body.empty())
		parts.body = appearance.getString("race");

	parts.bodyTexture = appearance.getString(Common::UString("tex") + state);
	if (!parts.bodyTexture.empty())
		parts.bodyTexture += "01";

	if (parts.bodyTexture.empty())
		parts.bodyTexture = appearance.getString("racetex");

	if ((parts.type == "B") || (parts.type == "P")) {
		const int headNormalID = appearance.getInt("normalhead");
		const int headBackupID = appearance.getInt("backuphead");

		const Aurora::TwoDAFile &heads = TwoDAReg.get("heads");

		if      (headNormalID >= 0)
			parts.head = heads.getRow(headNormalID).getString("head");
		else if (headBackupID >= 0)
			parts.head = heads.getRow(headBackupID).getString("head");
	}
}

void Creature::loadBody(PartModels &parts) {
	_model = loadModelObject(parts.body, parts.bodyTexture);
	if (!_model)
		return;

	_ids.push_back(_model->getID());

	_model->setTag(_tag);
	_model->setClickable(isClickable());
}

void Creature::loadHead(PartModels &parts) {
	if (!_model || parts.head.empty())
		return;

	Graphics::Aurora::ModelNode *headHook = _model->getNode("headhook");
	if (!headHook)
		return;

	headHook->addChild(loadModelObject(parts.head));
}

void Creature::enter() {
	highlight(true);
}

void Creature::leave() {
	highlight(false);
}

void Creature::highlight(bool enabled) {
	_model->drawBound(enabled);
}

} // End of namespace KotOR

} // End of namespace Engines
