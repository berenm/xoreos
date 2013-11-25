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

/** @file engines/nwn/gui/widgets/listbox.cpp
 *  A NWN listbox widget.
 */

#include "common/util.h"
#include "common/error.h"
#include "common/ustring.h"

#include "graphics/graphics.h"
#include "graphics/font.h"

#include "graphics/aurora/modelnode.h"
#include "graphics/aurora/model.h"
#include "graphics/aurora/text.h"
#include "graphics/aurora/fontman.h"

#include "engines/nwn/gui/widgets/button.h"
#include "engines/nwn/gui/widgets/scrollbar.h"
#include "engines/nwn/gui/widgets/listbox.h"

namespace Engines {

namespace NWN {

WidgetListItem::WidgetListItem(::Engines::GUI &gui) : NWNWidget(gui, ""), _itemNumber(0xFFFFFFFF), _state(false) {
}

WidgetListItem::~WidgetListItem() {
}

void WidgetListItem::mouseUp(uint8 state, const glm::vec2 &point) {
	if (isDisabled())
		return;

	if (state != SDL_BUTTON_LMASK)
		return;

	activate();
}

void WidgetListItem::mouseDown(uint8 state, const glm::vec2 &point) {
	if (isDisabled())
		return;

	if ((state == SDL_BUTTON_WHEELUP) || (state == SDL_BUTTON_WHEELDOWN)) {
		if (_owner)
			_owner->mouseDown(state, point);
		return;
	}
}

void WidgetListItem::mouseDblClick(uint8 state, const glm::vec2 &point) {
	if (isDisabled())
		return;

	if (state == SDL_BUTTON_LMASK) {
		WidgetListBox *ownerList = dynamic_cast<WidgetListBox *>(_owner);
		if (ownerList)
			ownerList->itemDblClicked();
	}
}

void WidgetListItem::select() {
	if (isDisabled())
		return;

	activate();
}

bool WidgetListItem::getState() {
	return _state;
}

bool WidgetListItem::activate() {
	if (_state)
		return false;

	WidgetListBox *ownerList = dynamic_cast<WidgetListBox *>(_owner);
	if (!ownerList || (ownerList->getMode() != WidgetListBox::kModeSelectable))
		return false;

	_state = true;

	setActive(true);

	return true;
}

bool WidgetListItem::deactivate() {
	if (!_state)
		return false;

	WidgetListBox *ownerList = dynamic_cast<WidgetListBox *>(_owner);
	if (!ownerList || (ownerList->getMode() != WidgetListBox::kModeSelectable))
		return false;

	_state = false;

	return true;
}

void WidgetListItem::signalGroupMemberActive() {
	NWNWidget::signalGroupMemberActive();

	deactivate();
}


WidgetListItemTextLine::WidgetListItemTextLine(::Engines::GUI &gui,
    const Common::UString &font, const Common::UString &text, float spacing) :
	WidgetListItem(gui),
	_uR(1.0), _uG(1.0), _uB(1.0), _uA(1.0),
	_sR(1.0), _sG(1.0), _sB(0.0), _sA(1.0), _spacing(spacing) {

	Graphics::Aurora::FontHandle f = FontMan.get(font);

	_fontHeight = f.getFont().getHeight();

	_text = new Graphics::Aurora::Text(f, text, _uR, _uG, _uB, _uA, 0.0);

	_text->setClickable(true);
}

WidgetListItemTextLine::~WidgetListItemTextLine() {
	delete _text;
}

void WidgetListItemTextLine::show() {
	_text->show();
}

void WidgetListItemTextLine::hide() {
	_text->hide();
}

void WidgetListItemTextLine::setPosition(const glm::vec3 &position) {
	NWNWidget::setPosition(position);

	const glm::vec3 p = getPosition();
	_text->setPosition(p);
}

void WidgetListItemTextLine::setUnselectedColor(float r, float g, float b, float a) {
	_uR = r;
	_uG = g;
	_uB = b;
	_uA = a;

	if (!getState())
		_text->setColor(_uR, _uG, _uB, _uA);
}

void WidgetListItemTextLine::setSelectedColor(float r, float g, float b, float a) {
	_sR = r;
	_sG = g;
	_sB = b;
	_sA = a;

	if (getState())
		_text->setColor(_sR, _sG, _sB, _sA);
}

glm::vec2 WidgetListItemTextLine::getSize() const {
	const glm::vec2 size = _text->getSize();

	if (_text->isEmpty())
		return glm::vec2(size.x, _fontHeight + _spacing);

	return glm::vec2(size.x, size.y + _spacing);
}

void WidgetListItemTextLine::setTag(const Common::UString &tag) {
	WidgetListItem::setTag(tag);

	_text->setTag(tag);
}

bool WidgetListItemTextLine::activate() {
	if (!WidgetListItem::activate())
		return false;

	_text->setColor(_sR, _sG, _sB, _sA);

	return true;
}

bool WidgetListItemTextLine::deactivate() {
	if (!WidgetListItem::deactivate())
		return false;

	_text->setColor(_uR, _uG, _uB, _uA);

	return true;
}


WidgetListBox::WidgetListBox(::Engines::GUI &gui, const Common::UString &tag,
                             const Common::UString &model) :
	ModelWidget(gui, tag, model),
	_mode(kModeStatic), _content(0.0, 0.0, 0.0),
	_hasScrollbar(false), _up(0), _down(0), _scrollbar(0), _dblClicked(false),
	_startItem(0), _selectedItem(0xFFFFFFFF), _locked(false) {

	_model->setClickable(true);

	getProperties();

	createScrollbar();
}

WidgetListBox::~WidgetListBox() {
}

void WidgetListBox::getProperties() {
	// Do we have a scroll bar?
	_hasScrollbar = _model->hasNode("scrollmin") && _model->hasNode("scrollmax");

	// Calculate content region

	Graphics::Aurora::ModelNode *node = 0;

	glm::vec3 top = glm::vec3(8.0, getSize().y - 6.0, 0.0);
	if ((node = _model->getNode("text0")))
		top = node->getPosition();

	glm::vec3 bottom = glm::vec3(getSize().x - (_hasScrollbar ? 25.0 : 3.0), 3.0, 0.0);
	if ((node = _model->getNode("text1")))
		bottom = node->getPosition();

	_content.x = top.x;
	_content.y = top.y;

	_contentWidth  = bottom.x - top.x;
	_contentHeight = top.y - bottom.y;
}

void WidgetListBox::createScrollbar() {
	if (!_hasScrollbar)
		return;

	// Get top position
	const glm::vec3 min = _model->getNode("scrollmin")->getPosition();

	// Create the "up" button
	_up = new WidgetButton(*_gui, getTag() + "#Up", "pb_scrl_up", "gui_scroll");
	_up->setPosition(glm::vec3(min.x, min.y, -50.0));
	addSub(*_up);

	// Get bottom position
	const glm::vec3 max = _model->getNode("scrollmax")->getPosition();

	// Create the "down" button
	_down = new WidgetButton(*_gui, getTag() + "#Down", "pb_scrl_down", "gui_scroll");
	_down->setPosition(glm::vec3(max.x, max.y - 10, -50.0));
	addSub(*_down);

	// Scroll bar range (max length)
	float scrollRange = min.y - (max.y - 10) - _up->getSize().y - 1;

	// Create the scrollbar
	_scrollbar =
		new WidgetScrollbar(*_gui, getTag() + "#Bar", Scrollbar::kTypeVertical, scrollRange);

	// Center the bar within the scrollbar area
	float scrollX = max.x + (_up->getSize().x - _scrollbar->getSize().x) / 2;
	// Move it to the base position
	float scrollY = max.y - 10 + _up->getSize().y;

	_scrollbar->setPosition(glm::vec3(scrollX, scrollY, -50.0));
	addSub(*_scrollbar);
}

WidgetListBox::Mode WidgetListBox::getMode() const {
	return _mode;
}

void WidgetListBox::setMode(Mode mode) {
	_mode = mode;
}

void WidgetListBox::show() {
	ModelWidget::show();

	// Show the scrollbar
	if (_hasScrollbar) {
		_up->show();
		_down->show();
		_scrollbar->show();
	}

	// Show the visible items
	for (std::vector<WidgetListItem *>::iterator v = _visibleItems.begin(); v != _visibleItems.end(); ++v)
		(*v)->show();
}

void WidgetListBox::hide() {
	ModelWidget::hide();

	// Hide the scrollbar
	if (_hasScrollbar) {
		_up->hide();
		_down->hide();
		_scrollbar->hide();
	}

	// Hide the visible items
	for (std::vector<WidgetListItem *>::iterator v = _visibleItems.begin(); v != _visibleItems.end(); ++v)
		(*v)->hide();
}

void WidgetListBox::setPosition(const glm::vec3 &position) {
	const glm::vec3 op = getPosition();

	ModelWidget::setPosition(position);

	const glm::vec3 np = getPosition();

	for (std::list<Widget *>::iterator it = _subWidgets.begin(); it != _subWidgets.end(); ++it) {
		const glm::vec3 sp = (*it)->getPosition() - op;
		(*it)->setPosition(sp + np);
	}

	_content = _content - op + np;
}

float WidgetListBox::getContentWidth() const {
	return _contentWidth;
}

float WidgetListBox::getContentHeight() const {
	return _contentHeight;
}

void WidgetListBox::lock() {
	assert(!_locked);
	_locked = true;

	GfxMan.lockFrame();
}

void WidgetListBox::clear() {
	assert(_locked);

	for (std::vector<WidgetListItem *>::iterator v = _visibleItems.begin(); v != _visibleItems.end(); ++v)
		(*v)->hide();
	_visibleItems.clear();

	for (std::vector<WidgetListItem *>::iterator i = _items.begin(); i != _items.end(); ++i)
		(*i)->remove();
	_items.clear();

	_startItem    = 0;
	_selectedItem = 0xFFFFFFFF;

	updateScrollbarLength();
}

void WidgetListBox::reserve(uint n) {
	assert(_locked);

	_items.reserve(n);
}

void WidgetListBox::add(WidgetListItem *item) {
	assert(_locked);

	if (!_items.empty())
		if (round(item->getSize().y) != round(_items.front()->getSize().y))
			throw Common::Exception("WidgetListBox item sizes mismatch");

	item->_itemNumber = _items.size();

	item->setTag(Common::UString::sprintf("%s#Item%d", getTag().c_str(), (int)_items.size()));

	for (std::vector<WidgetListItem *>::iterator i = _items.begin(); i != _items.end(); ++i) {
		(*i)->addGroupMember(*item);
		item->addGroupMember(**i);
	}

	_items.push_back(item);

	addSub(*item);
}

void WidgetListBox::unlock() {
	assert(_locked);
	_locked = false;

	if (_items.empty()) {
		GfxMan.unlockFrame();
		return;
	}

	uint count = MIN<uint>(_contentHeight / _items.front()->getSize().y, _items.size());
	if ((count == 0) || (count == _visibleItems.size())) {
		GfxMan.unlockFrame();
		return;
	}

	assert(_visibleItems.size() < count);

	_visibleItems.reserve(count);

	uint start = _startItem + _visibleItems.size();

	float itemHeight = _items.front()->getSize().y;
	while (_visibleItems.size() < count) {
		WidgetListItem *item = _items[start++];

		float itemY = _content.y - (_visibleItems.size() + 1) * itemHeight;
		item->setPosition(glm::vec3(_content.x, itemY, _content.z - 5.0));

		_visibleItems.push_back(item);
		if (isVisible())
			_visibleItems.back()->show();
	}

	updateScrollbarLength();
	updateScrollbarPosition();

	GfxMan.unlockFrame();
}

void WidgetListBox::setText(const Common::UString &font,
                            const Common::UString &text, float spacing) {

	lock();
	clear();

	Graphics::Aurora::FontHandle f = FontMan.get(font);
	std::vector<Common::UString> lines;
	f.getFont().split(text, lines, getContentWidth());

	for (std::vector<Common::UString>::iterator l = lines.begin(); l != lines.end(); ++l)
		add(new WidgetListItemTextLine(*_gui, font, *l, spacing));

	unlock();
}

void WidgetListBox::updateScrollbarLength() {
	if (!_scrollbar)
		return;

	if (_visibleItems.empty())
		_scrollbar->setLength(1.0);
	else
		_scrollbar->setLength(((float) _visibleItems.size()) / _items.size());
}

void WidgetListBox::updateScrollbarPosition() {
	if (!_scrollbar)
		return;

	int max = _items.size() - _visibleItems.size();
	if (max > 0)
		_scrollbar->setState(((float) _startItem) / max);
	else
		_scrollbar->setState(0.0);
}

void WidgetListBox::updateVisible() {
	if (_visibleItems.empty())
		return;

	GfxMan.lockFrame();

	for (uint i = 0; i < _visibleItems.size(); i++)
		_visibleItems[i]->hide();

	float itemHeight = _items.front()->getSize().y;
	float itemY      = _content.y;
	for (uint i = 0; i < _visibleItems.size(); i++) {
		WidgetListItem *item = _items[_startItem + i];

		itemY -= itemHeight;

		item->setPosition(glm::vec3(_content.x, itemY, _content.z - 5.0));
		_visibleItems[i] = item;

		if (isVisible())
			_visibleItems[i]->show();
	}

	GfxMan.unlockFrame();
}

void WidgetListBox::itemDblClicked() {
	_dblClicked = true;

	setActive(true);
}

void WidgetListBox::scrollUp(uint n) {
	if (_visibleItems.empty())
		return;

	if (_startItem == 0)
		return;

	_startItem -= MIN<uint>(n, _startItem);

	updateVisible();
	updateScrollbarPosition();
}

void WidgetListBox::scrollDown(uint n) {
	if (_visibleItems.empty())
		return;

	if (_startItem + _visibleItems.size() >= _items.size())
		return;

	_startItem += MIN<uint>(n, _items.size() - _visibleItems.size() - _startItem);

	updateVisible();
	updateScrollbarPosition();
}

void WidgetListBox::select(uint item) {
	if (item >= _items.size())
		return;

	_items[item]->select();
	_selectedItem = item;
}

uint WidgetListBox::getSelected() const {
	return _selectedItem;
}

bool WidgetListBox::wasDblClicked() {
	bool dblClicked = _dblClicked;

	_dblClicked = false;

	return dblClicked;
}

void WidgetListBox::subActive(Widget &widget) {
	if (widget.getTag().endsWith("#Up")) {
		scrollUp(1);
		return;
	}

	if (widget.getTag().endsWith("#Down")) {
		scrollDown(1);
		return;
	}

	if (widget.getTag().endsWith("#Bar")) {
		int max = _items.size() - _visibleItems.size();
		if (max <= 0)
			return;

		uint startItem = _scrollbar->getState() * max;
		if (startItem == _startItem)
			return;

		_startItem = startItem;
		updateVisible();
		return;
	}

	WidgetListItem *listItem = dynamic_cast<WidgetListItem *>(&widget);
	if (listItem) {
		if (_selectedItem != listItem->_itemNumber) {
			_selectedItem = listItem->_itemNumber;
			setActive(true);
		}
	}

}

void WidgetListBox::mouseDown(uint8 state, const glm::vec2 &point) {
	if (isDisabled())
		return;

	if (state == SDL_BUTTON_WHEELUP) {
		scrollUp(1);
		return;
	}

	if (state == SDL_BUTTON_WHEELDOWN) {
		scrollDown(1);
		return;
	}

	const glm::vec3 p = getPosition();

	// Check if we clicked on the scrollbar area
	if (_scrollbar) {
		if (point.x > (p.x + getSize().x - 20)) {
			if (point.y > _scrollbar->getBarPosition())
				scrollUp(_visibleItems.size());
			else
				scrollDown(_visibleItems.size());

			return;
		}
	}
}

} // End of namespace NWN

} // End of namespace Engines
