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

/** @file engines/nwn/gui/widgets/textwidget.h
 *  A NWN text widget.
 */

#ifndef ENGINES_NWN_GUI_WIDGETS_TEXTWIDGET_H
#define ENGINES_NWN_GUI_WIDGETS_TEXTWIDGET_H

#include "graphics/aurora/types.h"

#include "engines/nwn/gui/widgets/nwnwidget.h"

namespace Common {
	class UString;
}

namespace Engines {

namespace NWN {

/** A NWN text widget.
 *
 *  One of the base NWN widget classes, the TextWidget consists of a
 *  single Aurora Text.
 */
class TextWidget : public NWNWidget {
public:
	TextWidget(::Engines::GUI &gui, const Common::UString &tag,
	           const Common::UString &font, const Common::UString &text);
	~TextWidget();

	void show();
	void hide();

	void setPosition(const glm::vec3 &position);
	void setColor(float r, float g, float b, float a);
	void setText(const Common::UString &text);

	glm::vec2 getSize() const;

	void setDisabled(bool disabled);

protected:
	Graphics::Aurora::Text *_text;

	float _r;
	float _g;
	float _b;
	float _a;
};

} // End of namespace NWN

} // End of namespace Engines

#endif // ENGINES_NWN_GUI_WIDGETS_TEXTWIDGET_H
