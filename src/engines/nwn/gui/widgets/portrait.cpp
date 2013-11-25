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

/** @file engines/nwn/gui/widgets/portrait.cpp
 *  A portrait model and widget.
 */

#include "common/util.h"
#include "common/maths.h"
#include "common/ustring.h"

#include "graphics/graphics.h"

#include "graphics/aurora/texture.h"

#include "engines/nwn/gui/widgets/portrait.h"

static const char *kSuffix[] = {"h"  , "l"  , "m"  , "s"  , "t"  };
static const float kWidth [] = {256.0, 128.0,  64.0,  32.0,  16.0};
static const float kHeight[] = {400.0, 200.0, 100.0,  50.0,  25.0};

namespace Engines {

namespace NWN {

Portrait::Portrait(const Common::UString &name, Size size,
		float border, float bR, float bG, float bB, float bA) :
	_size(size), _border(border), _bR(bR), _bG(bG), _bB(bB), _bA(bA) {

	assert((_size >= kSizeHuge) && (_size < kSizeMAX));

	setSize();
	setPortrait(name);

	setPosition(glm::vec3(0.0, 0.0, -FLT_MAX));
}

Portrait::~Portrait() {
}

void Portrait::createBorder() {
	if (_border <= 0.0)
		return;

	_qBorder.clear();

	const float x = _qPortrait.vX[0];
	const float y = _qPortrait.vY[0];

	const float width  = ABS(_qPortrait.vX[0] - _qPortrait.vX[1]);
	const float height = ABS(_qPortrait.vY[0] - _qPortrait.vY[2]);

	_qBorder.resize(4);

	_qBorder[0].vX[0] = x         - _border; _qBorder[0].vY[0] = y - _border;
	_qBorder[0].vX[1] = x + width + _border; _qBorder[0].vY[1] = y - _border;
	_qBorder[0].vX[2] = x + width + _border; _qBorder[0].vY[2] = y          ;
	_qBorder[0].vX[3] = x         - _border; _qBorder[0].vY[3] = y          ;

	_qBorder[1].vX[0] = x         - _border; _qBorder[1].vY[0] = y + height;
	_qBorder[1].vX[1] = x + width + _border; _qBorder[1].vY[1] = y + height;
	_qBorder[1].vX[2] = x + width + _border; _qBorder[1].vY[2] = y + height + _border;
	_qBorder[1].vX[3] = x         - _border; _qBorder[1].vY[3] = y + height + _border;

	_qBorder[2].vX[0] = x - _border; _qBorder[2].vY[0] = y          - _border;
	_qBorder[2].vX[1] = x          ; _qBorder[2].vY[1] = y          - _border;
	_qBorder[2].vX[2] = x          ; _qBorder[2].vY[2] = y + height + _border;
	_qBorder[2].vX[3] = x - _border; _qBorder[2].vY[3] = y + height + _border;

	_qBorder[3].vX[0] = x + width          ; _qBorder[3].vY[0] = y          - _border;
	_qBorder[3].vX[1] = x + width + _border; _qBorder[3].vY[1] = y          - _border;
	_qBorder[3].vX[2] = x + width + _border; _qBorder[3].vY[2] = y + height + _border;
	_qBorder[3].vX[3] = x + width          ; _qBorder[3].vY[3] = y + height + _border;
}

glm::vec2 Portrait::getSize() const {
	return glm::abs(glm::vec2(_qPortrait.vX[0] - _qPortrait.vX[1], _qPortrait.vY[0] - _qPortrait.vY[2])) + _border + _border;
}

void Portrait::setPosition(const glm::vec3 &position) {
	GfxMan.lockFrame();

	float width  = ABS(_qPortrait.vX[0] - _qPortrait.vX[1]);
	float height = ABS(_qPortrait.vY[0] - _qPortrait.vY[2]);

	glm::vec3 p = position;
	p.x += _border;
	p.y += _border;

	_qPortrait.vX[0] = p.x        ; _qPortrait.vY[0] = p.y         ;
	_qPortrait.vX[1] = p.x + width; _qPortrait.vY[1] = p.y         ;
	_qPortrait.vX[2] = p.x + width; _qPortrait.vY[2] = p.y + height;
	_qPortrait.vX[3] = p.x        ; _qPortrait.vY[3] = p.y + height;

	createBorder();

	_distance = p.z;
	resort();

	GfxMan.unlockFrame();
}

glm::vec3 Portrait::getPosition() const {
	return glm::vec3(_qPortrait.vX[0], _qPortrait.vY[0], _distance);
}

bool Portrait::isIn(const glm::vec2 &point) const {
	const glm::vec2 position = glm::vec2(_qPortrait.vX[0], _qPortrait.vY[0]);
	return Common::insideOf(point, position, position + getSize());
}

void Portrait::calculateDistance() {
}

void Portrait::render(Graphics::RenderPass pass) {
	bool isTransparent = (_bA < 1.0) ||
	                     (!_texture.empty() && _texture.getTexture().hasAlpha());
	if (((pass == Graphics::kRenderPassOpaque)      &&  isTransparent) ||
			((pass == Graphics::kRenderPassTransparent) && !isTransparent))
		return;


	// Border

	TextureMan.set();
	glColor4f(_bR, _bG, _bB, _bA);

	glBegin(GL_QUADS);
	for (std::vector<Quad>::const_iterator b = _qBorder.begin(); b != _qBorder.end(); ++b)
		for (int i = 0; i < 4; i++)
			glVertex2f(b->vX[i], b->vY[i]);
	glEnd();


	// Portrait

	TextureMan.set(_texture);
	glColor4f(1.0, 1.0, 1.0, 1.0);

	glBegin(GL_QUADS);
	for (int i = 0; i < 4; i++) {
		glTexCoord2f(_qPortrait.tX[i], _qPortrait.tY[i]);
		glVertex2f  (_qPortrait.vX[i], _qPortrait.vY[i]);
	}
	glEnd();
}

void Portrait::setPortrait(const Common::UString &name) {
	Size curSize = _size;

	_texture.clear();
	while (_texture.empty() && (curSize < kSizeMAX)) {
		try {
			_texture = TextureMan.get(name + kSuffix[curSize]);
		} catch (...) {
			_texture.clear();
		}

		curSize = (Size) (curSize + 1);
	}

	if (_texture.empty()) {
		try {
			_texture = TextureMan.get(name);
		} catch (...) {
			_texture.clear();
		}
	}
}

void Portrait::setSize() {
	_qPortrait.vX[0] = 0.0                ; _qPortrait.vY[0] = 0.0                 ;
	_qPortrait.vX[1] = 0.0 + kWidth[_size]; _qPortrait.vY[1] = 0.0                 ;
	_qPortrait.vX[2] = 0.0 + kWidth[_size]; _qPortrait.vY[2] = 0.0 + kHeight[_size];
	_qPortrait.vX[3] = 0.0                ; _qPortrait.vY[3] = 0.0 + kHeight[_size];

	_qPortrait.tX[0] = 0.0; _qPortrait.tY[0] = 112.0 / 512.0;
	_qPortrait.tX[1] = 1.0; _qPortrait.tY[1] = 112.0 / 512.0;
	_qPortrait.tX[2] = 1.0; _qPortrait.tY[2] = 1.0;
	_qPortrait.tX[3] = 0.0; _qPortrait.tY[3] = 1.0;
}


PortraitWidget::PortraitWidget(::Engines::GUI &gui, const Common::UString &tag,
               const Common::UString &name, Portrait::Size size, float border,
               float bR, float bG, float bB, float bA) : NWNWidget(gui, tag),
	_portrait(name, size, border, bR, bG, bB, bA) {

	_portrait.setTag(tag);
	_portrait.setClickable(true);
}

PortraitWidget::~PortraitWidget() {
}

void PortraitWidget::show() {
	if (isVisible())
		return;

	if (!isInvisible())
		_portrait.show();

	NWNWidget::show();
}

void PortraitWidget::hide() {
	if (!isVisible())
		return;

	_portrait.hide();
	NWNWidget::hide();
}

void PortraitWidget::setPosition(const glm::vec3 &position) {
	NWNWidget::setPosition(position);

	const glm::vec3 p = getPosition();
	_portrait.setPosition(p);
}

void PortraitWidget::setPortrait(const Common::UString &name) {
	_portrait.setPortrait(name);
}

glm::vec2 PortraitWidget::getSize() const {
	return _portrait.getSize();
}

void PortraitWidget::setTag(const Common::UString &tag) {
	NWNWidget::setTag(tag);

	_portrait.setTag(getTag());
}

} // End of namespace NWN

} // End of namespace Engines
