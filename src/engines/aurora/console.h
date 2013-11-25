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

/** @file engines/aurora/console.h
 *  Generic Aurora engines (debug) console.
 */

#ifndef ENGINES_AURORA_CONSOLE_H
#define ENGINES_AURORA_CONSOLE_H

#include <boost/function.hpp>

#include "common/types.h"
#include "common/error.h"
#include "common/ustring.h"
#include "common/file.h"

#include "events/types.h"
#include "events/notifyable.h"

#include "graphics/types.h"
#include "graphics/guifrontelement.h"

#include "graphics/aurora/types.h"
#include "graphics/aurora/fontman.h"

namespace Common {
	class ReadLine;
}

namespace Engines {

class ConsoleWindow : public Graphics::GUIFrontElement, public Events::Notifyable {
public:
	ConsoleWindow(const Common::UString &font, uint32 lines, uint32 history,
	              int fontHeight = 0);
	~ConsoleWindow();


	void show();
	void hide();

	void showPrompt();
	void hidePrompt();

	bool isIn(const glm::vec2 &point) const;
	bool isIn(const glm::vec3 &point) const;


	// Dimensions

	glm::vec2 getSize() const;
	glm::vec2 getContentSize() const;

	uint32 getLines  () const;
	uint32 getColumns() const;


	// Input

	void setPrompt(const Common::UString &prompt);
	void setInput(const Common::UString &input, uint32 cursorPos, bool overwrite);


	// Output

	void clear();
	void print(const Common::UString &line);
	bool setRedirect(Common::UString redirect = "");


	// Highlight

	void clearHighlight();

	void startHighlight(const glm::ivec2 &cursor);
	void stopHighlight(const glm::ivec2 &cursor);

	void highlightWord(const glm::ivec2 &cursor);
	void highlightLine(const glm::ivec2 &cursor);

	Common::UString getHighlight() const;


	// Scrolling

	void scrollUp(uint32 n = 1);
	void scrollDown(uint32 n = 1);
	void scrollTop();
	void scrollBottom();


	// Renderable
	void calculateDistance();
	void render(Graphics::RenderPass pass);


private:
	Graphics::Aurora::FontHandle _font;

	Graphics::Aurora::Text    *_prompt;
	Graphics::Aurora::GUIQuad *_cursor;
	Graphics::Aurora::GUIQuad *_highlight;

	uint32 _historySizeMax;
	uint32 _historySizeCurrent;
	std::list<Common::UString> _history;

	uint32 _historyStart;

	std::vector<Graphics::Aurora::Text *> _lines;
	Graphics::Aurora::Text *_input;


	Common::UString _inputText;
	uint32 _cursorPosition;
	bool _overwrite;


	float _lineHeight;

	glm::vec2 _position;
	glm::vec2 _size;

	bool _cursorBlinkState;
	uint32 _lastCursorBlink;

	float _scrollbarLength;
	float _scrollbarPosition;

	glm::uvec2 _highlightPosition;
	int32      _highlightLength;

	Common::DumpFile _redirect;


	void recalcCursor();
	void redrawLines();

	void printLine(const Common::UString &line);

	void updateHighlight();
	glm::vec2  getPosition(const glm::ivec2 &cursor);
	glm::uvec2 highlightClip(const glm::uvec2 &position) const;

	void updateScrollbarLength();
	void updateScrollbarPosition();

	void notifyResized(const glm::ivec2 &oldSize, const glm::ivec2 &newSize);

	static uint32 findWordStart(const Common::UString &line, uint32 pos);
	static uint32 findWordEnd  (const Common::UString &line, uint32 pos);
};

class Console {
public:
	Console(const Common::UString &font, int fontHeight = 0);
	virtual ~Console();

	void show();
	void hide();

	bool isVisible() const;

	glm::vec2 getSize   () const;
	uint32    getLines  () const;
	uint32    getColumns() const;

	bool processEvent(Events::Event &event);

	void clear();
	void print(const Common::UString &line);
	void printf(const char *s, ...);


protected:
	struct CommandLine {
		Common::UString cmd;
		Common::UString args;
	};

	typedef boost::function<void (const CommandLine &cl)> CommandCallback;


	void printException(Common::Exception &e, const Common::UString &prefix = "ERROR: ");

	bool registerCommand(const Common::UString &cmd, const CommandCallback &callback,
	                     const Common::UString &help);

	void printCommandHelp(const Common::UString &cmd);
	void printList(const std::list<Common::UString> &list, uint32 maxSize = 0);

	void setArguments(const Common::UString &cmd, const std::list<Common::UString> &args);
	void setArguments(const Common::UString &cmd);

	virtual void updateCaches();
	virtual void showCallback();


private:
	struct Command {
		Common::UString cmd;
		Common::UString help;

		CommandCallback callback;
	};

	typedef std::map<Common::UString, Command, Common::UString::iless> CommandMap;


	bool _neverShown;
	bool _visible;

	Common::ReadLine *_readLine;
	ConsoleWindow *_console;

	CommandMap _commands;

	uint32 _tabCount;
	bool _printedCompleteWarning;

	 int8  _lastClickCount;
	uint8  _lastClickButton;
	uint32 _lastClickTime;
	uint32 _lastClickX;
	uint32 _lastClickY;


	std::list<Common::UString> _videos;
	std::list<Common::UString> _sounds;

	uint32 _maxSizeVideos;
	uint32 _maxSizeSounds;


	void updateVideos();
	void updateSounds();

	void cmdHelp       (const CommandLine &cl);
	void cmdClear      (const CommandLine &cl);
	void cmdExit       (const CommandLine &cl);
	void cmdQuit       (const CommandLine &cl);
	void cmdDumpResList(const CommandLine &cl);
	void cmdDumpRes    (const CommandLine &cl);
	void cmdDumpTGA    (const CommandLine &cl);
	void cmdDump2DA    (const CommandLine &cl);
	void cmdDumpAll2DA (const CommandLine &cl);
	void cmdListVideos (const CommandLine &cl);
	void cmdPlayVideo  (const CommandLine &cl);
	void cmdListSounds (const CommandLine &cl);
	void cmdPlaySound  (const CommandLine &cl);
	void cmdSilence    (const CommandLine &cl);

	void updateHelpArguments();

	void printFullHelp();
	bool printHints(const Common::UString &command);

	void execute(const Common::UString &line);
};

} // End of namespace Engines

#endif // ENGINES_AURORA_CONSOLE_H
