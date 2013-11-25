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

/** @file engines/nwn/gui/legal.cpp
 *  The legal billboard.
 */

#include "common/maths.h"
#include "common/ustring.h"
#include "common/stream.h"

#include "aurora/resman.h"

#include "events/events.h"

#include "graphics/graphics.h"

#include "graphics/aurora/model_nwn.h"

#include "engines/aurora/model.h"

#include "engines/nwn/gui/legal.h"

namespace Engines {

namespace NWN {

class FadeModel : public Graphics::Aurora::Model_NWN {
private:
	bool _fade;

	uint32 _fadeStart;

	float _fadeValue;
	float _fadeStep;

	void updateFade() {
		if (!_fade)
			return;

		uint32 now = EventMan.getTimestamp();

		if ((now - _fadeStart) >= 10) {
			// Get new fade value every 10ms

			_fadeValue += _fadeStep * ((now - _fadeStart) / 10.0);
			_fadeStart = now;
		}

		if        (_fadeValue > 1.0) {
			// Fade in finished
			_fade      = false;
			_fadeValue = 1.0;
		} else if (_fadeValue < 0.0) {
			// Fade out finished
			_fade      = false;
			_fadeValue = 0.0;
			hide();
		}

	}

public:
	FadeModel(const Common::UString &name) :
		Graphics::Aurora::Model_NWN(name, Graphics::Aurora::kModelTypeGUIFront),
		_fade(false), _fadeStart(0), _fadeValue(1.0), _fadeStep(0.0) {

	}

	~FadeModel() {
	}

	void fadeIn(uint32 length) {
		GfxMan.lockFrame();

		_fade      = true;
		_fadeStart = EventMan.getTimestamp();
		_fadeValue = 0.0;
		_fadeStep  = 10.0 / length;

		show();

		GfxMan.unlockFrame();
	}

	void fadeOut(uint32 length) {
		GfxMan.lockFrame();

		_fade      = true;
		_fadeStart = EventMan.getTimestamp();
		_fadeValue = 1.0;
		_fadeStep  = - (10.0 / length);

		GfxMan.unlockFrame();
	}

	void render(Graphics::RenderPass pass) {
		bool isTransparent = _fadeValue < 1.0;
		if (((pass == Graphics::kRenderPassOpaque     ) &&  isTransparent) ||
		    ((pass == Graphics::kRenderPassTransparent) && !isTransparent))
			return;

		glColor4f(1.0, 1.0, 1.0, _fadeValue);
		Graphics::Aurora::Model_NWN::render(Graphics::kRenderPassAll);
		glColor4f(1.0, 1.0, 1.0, 1.0);

		updateFade();
	}
};


static FadeModel *createFade(const Common::UString &name) {
	FadeModel *model = 0;

	try {
		model = new FadeModel(name);
	} catch (...) {
		delete model;
		throw;
	}

	return model;
}


Legal::Legal() : _billboard(0) {
	_billboard = createFade("load_legal");

	_billboard->setPosition(glm::vec3(0.0, 0.0, -1000.0));
	_billboard->setTag("Legal");
}

Legal::~Legal() {
	delete _billboard;
}

void Legal::fadeIn() {
	_billboard->fadeIn(1000);

	bool abort = false;
	uint32 start = EventMan.getTimestamp();
	while ((EventMan.getTimestamp() - start) < 1000) {
		Events::Event event;
		while (EventMan.pollEvent(event))
			if (event.type == Events::kEventMouseDown)
				abort = true;

		if (abort || EventMan.quitRequested())
			break;

		EventMan.delay(10);
	}

	if (abort || EventMan.quitRequested()) {
		delete _billboard;
		_billboard = 0;
	}
}

void Legal::show() {
	if (!_billboard)
		return;

	uint32 start   = EventMan.getTimestamp();
	bool   fadeOut = false;
	while (!EventMan.quitRequested()) {
		Events::Event event;

		// Mouse click => abort
		bool abort = false;
		while (EventMan.pollEvent(event))
			if (event.type == Events::kEventMouseDown)
				abort = true;
		if (abort)
			break;

		if (!fadeOut && (EventMan.getTimestamp() - start) >= 5000) {
			_billboard->fadeOut(1000);
			fadeOut = true;
		}

		// Display and fade-out time's up
		if ((EventMan.getTimestamp() - start) >= 6000)
			break;
	}

	delete _billboard;
	_billboard = 0;
}

} // End of namespace NWN

} // End of namespace Engines
