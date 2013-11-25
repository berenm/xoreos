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

/** @file events/notifications.cpp
 *  The notification manager, handling all notifications.
 */

#include "events/notifications.h"
#include "events/notifyable.h"

DECLARE_SINGLETON(Events::NotificationManager)

namespace Events {

NotificationManager::NotificationManager() {
}

NotificationManager::~NotificationManager() {
}

void NotificationManager::init() {
}

std::list<Notifyable *>::iterator NotificationManager::registerNotifyable(Notifyable &notifyable) {
	Common::StackLock lock(_mutex);

	_notifyables.push_back(&notifyable);

	return --_notifyables.end();
}

void NotificationManager::unregisterNotifyable(const std::list<Notifyable *>::iterator &it) {
	Common::StackLock lock(_mutex);

	_notifyables.erase(it);
}

void NotificationManager::resized(const glm::ivec2 &oldSize, const glm::ivec2 &newSize) {
	Common::StackLock lock(_mutex);

	for (std::list<Notifyable *>::iterator it = _notifyables.begin(); it != _notifyables.end(); ++it)
		(*it)->notifyResized(oldSize, newSize);
}

void NotificationManager::cameraMoved() {
	Common::StackLock lock(_mutex);

	for (std::list<Notifyable *>::iterator it = _notifyables.begin(); it != _notifyables.end(); ++it)
		(*it)->notifyCameraMoved();
}

} // End of namespace Events
