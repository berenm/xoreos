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

/** @file engines/nwn/gui/widgets/slider.cpp
 *  A NWN slider widget.
 */

#include "common/util.h"
#include "common/ustring.h"

#include "graphics/aurora/modelnode.h"
#include "graphics/aurora/model.h"

#include "engines/nwn/gui/widgets/slider.h"

namespace Engines {

namespace NWN {

WidgetSlider::WidgetSlider(::Engines::GUI &gui, const Common::UString &tag,
                           const Common::UString &model) :
	ModelWidget(gui, tag, model), _position(0.0), _steps(0), _state(0) {

	_model->setClickable(true);

	_width = getSize().x;

	_thumb = _model->getNode("thumb");

	assert(_thumb);

	changePosition(0.0);
}

WidgetSlider::~WidgetSlider() {
}

void WidgetSlider::setPosition(const glm::vec3 &position) {
	ModelWidget::setPosition(position);
}

void WidgetSlider::setSteps(int steps) {
	_steps = steps;
}

int WidgetSlider::getState() const {
	return _state;
}

void WidgetSlider::setState(int state) {
	_state = state;

	changePosition(CLIP(((float) _state) / _steps, 0.0f, 1.0f));
}

void WidgetSlider::mouseMove(uint8 state, const glm::vec2 &point) {
	if (isDisabled())
		return;

	if (state != SDL_BUTTON_LMASK)
		// We only care about moves with the left mouse button pressed
		return;

	changedValue(point);
}

void WidgetSlider::mouseDown(uint8 state, const glm::vec2 &point) {
	if (isDisabled())
		return;

	if (state != SDL_BUTTON_LMASK)
		return;

	changedValue(point);
}

void WidgetSlider::changedValue(const glm::vec2 &point) {
	const glm::vec3 cur = getPosition();

	float pX    = CLIP(point.x - cur.x, 0.0f, _width) / _width;
	int   state = roundf(pX * _steps);

	if (state == _state)
		// No change
		return;

	_state = state;

	if (_steps == 0) {
		changePosition(0.0);
		return;
	}

	changePosition(((float) _state) / _steps);

	setActive(true);
}

void WidgetSlider::changePosition(float value) {
	value = (value * _width) - (_thumb->getSize().x / 2.0);

	_thumb->move(glm::vec3(-_position + value, 0.0, 0.0));

	_position = value;
}

} // End of namespace NWN

} // End of namespace Engines
