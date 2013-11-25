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

/** @file engines/nwn/gui/widgets/modelwidget.cpp
 *  A NWN model widget.
 */

#include "common/error.h"
#include "common/ustring.h"

#include "graphics/aurora/model.h"

#include "engines/aurora/model.h"

#include "engines/nwn/gui/widgets/modelwidget.h"

namespace Engines {

namespace NWN {

ModelWidget::ModelWidget(::Engines::GUI &gui, const Common::UString &tag,
                         const Common::UString &model) : NWNWidget(gui, tag) {

	if (!(_model = loadModelGUI(model)))
		throw Common::Exception("Can't load model \"%s\" for widget \"%s\"",
				model.c_str(), tag.c_str());

	_model->setTag(tag);
}

ModelWidget::~ModelWidget() {
	freeModel(_model);
}

void ModelWidget::show() {
	if (isVisible())
		return;

	if (!isInvisible())
		_model->show();

	NWNWidget::show();
}

void ModelWidget::hide() {
	if (!isVisible())
		return;

	_model->hide();
	NWNWidget::hide();
}

void ModelWidget::setPosition(const glm::vec3 &position) {
	NWNWidget::setPosition(position);

	const glm::vec3 p = getPosition();
	_model->setPosition(p);
}

glm::vec2 ModelWidget::getSize() const {
	return glm::vec2(_model->getSize());
}

} // End of namespace NWN

} // End of namespace Engines
