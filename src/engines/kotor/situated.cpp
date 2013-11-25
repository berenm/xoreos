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

/** @file engines/kotor/situated.cpp
 *  KotOR situated object.
 */

#include "common/error.h"
#include "common/maths.h"
#include "common/util.h"

#include "aurora/locstring.h"
#include "aurora/gfffile.h"
#include "aurora/2dafile.h"
#include "aurora/2dareg.h"

#include "graphics/aurora/model.h"

#include "engines/aurora/util.h"
#include "engines/aurora/model.h"

#include "engines/kotor/situated.h"

namespace Engines {

namespace KotOR {

Situated::Situated() : _appearanceID(Aurora::kFieldIDInvalid), _model(0) {
}

Situated::~Situated() {
	delete _model;
}

void Situated::show() {
	assert(_loaded);

	if (_model)
		_model->show();
}

void Situated::hide() {
	assert(_loaded);

	if (_model)
		_model->hide();
}

void Situated::setPosition(const glm::vec3 &position) {
	Object::setPosition(position);
	const glm::vec3 p = Object::getPosition();

	if (_model)
		_model->setPosition(p);
}

void Situated::setOrientation(const glm::vec3 &orientation) {
	Object::setOrientation(orientation);
	const glm::vec3 o = Object::getOrientation();

	if (_model)
		_model->setRotation(glm::vec3(o.x, o.z, -o.y));
}

void Situated::load(const Aurora::GFFStruct &instance, const Aurora::GFFStruct *blueprint) {
	assert(!_loaded);


	// General properties

	if (blueprint)
		loadProperties(*blueprint); // Blueprint
	loadProperties(instance);    // Instance


	// Specialized object properties

	if (blueprint)
		loadObject(*blueprint); // Blueprint
	loadObject(instance);    // Instance


	// Appearance

	if (_appearanceID != Aurora::kFieldIDInvalid)
		loadAppearance();

	// Model

	if (!_modelName.empty()) {
		_model = loadModelObject(_modelName);

		if (!_model)
			throw Common::Exception("Failed to load situated object model \"%s\"",
			                        _modelName.c_str());
	} else
		warning("Situated object \"%s\" (\"%s\") has no model", _name.c_str(), _tag.c_str());

	if (_model) {
		// Clickable
		_model->setTag(_tag);
		_model->setClickable(isClickable());

		// ID
		_ids.push_back(_model->getID());
	}

	// Position

	setPosition(glm::vec3(instance.getDouble("X"),
	                      instance.getDouble("Y"),
	                      instance.getDouble("Z")));

	// Orientation

	float bearing = instance.getDouble("Bearing");

	setOrientation(glm::vec3(0.0, Common::rad2deg(bearing), 0.0));

	_loaded = true;
}

void Situated::loadProperties(const Aurora::GFFStruct &gff) {
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
	_appearanceID = gff.getUint("Appearance", _appearanceID);

	// Static
	_static = gff.getBool("Static", _static);

	// Usable
	_usable = gff.getBool("Useable", _usable);
}

void Situated::loadPortrait(const Aurora::GFFStruct &gff) {
	uint32 portraitID = gff.getUint("PortraitId");
	if (portraitID != 0) {
		const Aurora::TwoDAFile &twoda = TwoDAReg.get("portraits");

		Common::UString portrait = twoda.getRow(portraitID).getString("BaseResRef");
		if (!portrait.empty())
			_portrait = "po_" + portrait;
	}

	_portrait = gff.getString("Portrait", _portrait);
}

} // End of namespace KotOR

} // End of namespace Engines
