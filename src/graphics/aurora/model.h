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

/** @file graphics/aurora/model.h
 *  A 3D model of an object.
 */

#ifndef GRAPHICS_AURORA_NEWMODEL_H
#define GRAPHICS_AURORA_NEWMODEL_H

#include <vector>
#include <list>
#include <map>

#include "common/ustring.h"
#include "common/boundingbox.h"

#include "graphics/types.h"
#include "graphics/glcontainer.h"
#include "graphics/renderable.h"

#include "graphics/aurora/types.h"

namespace Common {
	class SeekableReadStream;
}

namespace Graphics {

namespace Aurora {

class ModelNode;
class Animation;

class Model : public GLContainer, public Renderable {
public:
	Model(ModelType type = kModelTypeObject);
	~Model();

	ModelType getType() const; ///< Return the model's type.

	/** Get the model's name. */
	const Common::UString &getName() const;

	glm::vec3 getSize() const; ///< Get the size of the model's bounding box.

	/** Should a bounding box be drawn around this model? */
	void drawBound(bool enabled);


	/** Is that point within the model's bounding box? */
	bool isIn(const glm::vec2 &point) const;
	/** Is that point within the model's bounding box? */
	bool isIn(const glm::vec3 &point) const;
	/** Does the line from x1.y1.z1 to x2.y2.z2 intersect with model's bounding box? */
	bool isIn(const std::pair<glm::vec3, glm::vec3> &line) const;


	// Positioning

	/** Get the current position of the model. */
	glm::vec3 getPosition() const;
	/** Get the current rotation of the model. */
	glm::vec3 getRotation() const;

	/** Get the position of the node after translate/rotate. */
	glm::vec3 getAbsolutePosition() const;

	/** Set the current position of the model. */
	void setPosition(const glm::vec3 &position);
	/** Set the current rotation of the model. */
	void setRotation(const glm::vec3 &rotation);

	/** Move the model, relative to its current position. */
	void move  (const glm::vec3 &position);
	/** Rotate the model, relative to its current rotation. */
	void rotate(const glm::vec3 &rotation);

	/** Get the point where the feedback tooltip is anchored. */
	glm::vec3 getTooltipAnchor() const;

	// States

	/** Return a list of all animation state names. */
	const std::list<Common::UString> &getStates() const;
	/** Set the current animation state. */
	void setState(const Common::UString &name = "");
	/** Return the name of the current state. */
	const Common::UString &getState() const;


	// Nodes

	/** Does the specified node exist in the current state? */
	bool hasNode(const Common::UString &node) const;

	/** Get the specified node, from the current state. */
	ModelNode *getNode(const Common::UString &node);
	/** Get the specified node, from the current state. */
	const ModelNode *getNode(const Common::UString &node) const;


	// Animation

	/** Determine what animation scaling applies. */
	float getAnimationScale(const Common::UString &anim);

	/** Play a named animation.
	 *
	 *  @param anim      The animation to play.
	 *  @param restart   Whether to restart the animation if it's already playing.
	 *  @param loopCount Number of times to loop the animation. Negative for loop indefinitely.
	 */
	void playAnimation(const Common::UString &anim, bool restart = true, int32 loopCount = 0);
	/** Play a default idle animation. */
	void playDefaultAnimation();


	// Renderable
	void calculateDistance();
	void render(RenderPass pass);
	void advanceTime(float dt);


protected:
	typedef std::list<ModelNode *> NodeList;
	typedef std::map<Common::UString, ModelNode *, Common::UString::iless> NodeMap;
	typedef std::map<Common::UString, Animation *, Common::UString::iless> AnimationMap;

	/** A model state. */
	struct State {
		Common::UString name; ///< The state's name.

		NodeList nodeList; ///< The nodes within the state.
		NodeMap  nodeMap;  ///< The nodes within the state, indexed by name.

		NodeList rootNodes; ///< The nodes in the state without a parent.
	};

	typedef std::list<State *> StateList;
	typedef std::map<Common::UString, State *> StateMap;

	/** A default animation. */
	struct DefaultAnimation {
		Animation *animation; ///< The animation.

		uint8 probability; ///< The probability (in percent) this animation is selected.
	};

	typedef std::list<DefaultAnimation> DefaultAnimations;


	ModelType _type; ///< The model's type.

	Common::UString _fileName; ///< The model's file name.

	Common::UString _name; ///< The model's name.

	Common::UString _superModelName; ///< Name of the supermodel.
	Model *_supermodel; ///< The actual supermodel.

	StateList _stateList;   ///< All states within this model.
	StateMap  _stateMap;    ///< All states within this model, index by name.
	State   *_currentState; ///< The current state.

	std::list<Common::UString> _stateNames; ///< All state names.

	AnimationMap _animationMap; ///< Map of all animations in this model.

	Animation *_currentAnimation; ///< The currently playing animations.
	Animation *_nextAnimation;    ///< The animation that's scheduled next.

	int32 _loopAnimation; ///< Number of times to loop the current animation.

	float _animationScale; ///< The scale of the animation.

	/** All default animations, sorted from least to most probable. */
	DefaultAnimations _defaultAnimations;

	glm::vec3 _modelScale; ///< The model's scale.

	glm::vec3 _position; ///< Model's position.
	glm::vec3 _rotation; ///< Model's rotation.
	glm::vec3 _center; ///< Model's center.

	glm::mat4 _absolutePosition;

	/** The model's bounding box. */
	Common::BoundingBox _boundBox;
	/** The model's box after translate/rotate. */
	Common::BoundingBox _absoluteBoundBox;


	// Animation

	/** Get the animation from its name. */
	Animation *getAnimation(const Common::UString &anim);


	/** Finalize the loading procedure. */
	void finalize();
	/** Signal that the nodes changed and the OpenGL list needs to be rebuild. */
	void needRebuild();


	// GLContainer
	void doRebuild();
	void doDestroy();


private:
	bool _needBuild[kRenderPassAll];
	bool _drawBound;
	float _elapsedTime; ///< Track animation duration

	ListID _lists; ///< OpenGL display lists for the model


	bool buildList(RenderPass pass);

	void createStateNamesList(); ///< Create the list of all state names.
	void createBound();          ///< Create the model's bounding box.

	void createAbsolutePosition();

	void doDrawBound();
	void manageAnimations(float dt);

	Animation *selectDefaultAnimation() const;


public:
	// General loading helpers

	static void readValue(Common::SeekableReadStream &stream, uint32 &value);
	static void readValue(Common::SeekableReadStream &stream, float  &value);

	static void readArrayDef(Common::SeekableReadStream &stream,
	                         uint32 &offset, uint32 &count);

	template<typename T>
	static void readArray(Common::SeekableReadStream &stream,
	                      uint32 offset, uint32 count, std::vector<T> &values);

	friend class ModelNode;
};

} // End of namespace Aurora

} // End of namespace Graphics

#endif // GRAPHICS_AURORA_NEWMODEL_H
