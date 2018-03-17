/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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
 */

#ifndef BLADERUNNER_SCENE_OBJECTS_H
#define BLADERUNNER_SCENE_OBJECTS_H

#include "bladerunner/boundingbox.h"

#include "common/rect.h"

namespace BladeRunner {

class BladeRunnerEngine;
class SaveFile;
class View;

enum SceneObjectType {
	kSceneObjectTypeUnknown = -1,
	kSceneObjectTypeActor   = 0,
	kSceneObjectTypeObject  = 1,
	kSceneObjectTypeItem    = 2
};

class SceneObjects {
	friend class Debugger;

	static const int kSceneObjectCount = 115;

	struct SceneObject {
		int                 id;
		SceneObjectType     type;
		const BoundingBox  *boundingBox;
		const Common::Rect *screenRectangle;
		float               distanceToCamera;
		bool                isPresent;
		bool                isClickable;
		bool                isObstacle;
		int                 unknown1;
		bool                isTarget;
		bool                isMoving;
		bool                isRetired;
	};

	BladeRunnerEngine *_vm;

	View        *_view;
	int          _count;
	SceneObject  _sceneObjects[kSceneObjectCount];
	int          _sceneObjectsSortedByDistance[kSceneObjectCount];

public:
	SceneObjects(BladeRunnerEngine *vm, View *view);
	~SceneObjects();

	bool addActor(int sceneObjectId, BoundingBox *boundingBox, Common::Rect *screenRectangle, bool isClickable, bool isMoving, bool isTarget, bool isRetired);
	bool addObject(int sceneObjectId, BoundingBox *boundingBox, bool isClickable, bool isObstacle, uint8 unknown1, bool isTarget);
	bool addItem(int sceneObjectId, BoundingBox *boundingBox, Common::Rect *screenRectangle, bool isTarget, bool isObstacle);
	bool remove(int sceneObjectId);
	void clear();
	int findByXYZ(bool *isClickable, bool *isObstacle, bool *isTarget, Vector3 &position, bool findClickables, bool findObstacles, bool findTargets) const;
	bool existsOnXZ(int exceptSceneObjectId, float x, float z, bool movingActorIsObstacle, bool standingActorIsObstacle) const;
	void setMoving(int sceneObjectId, bool isMoving);
	void setRetired(int sceneObjectId, bool isRetired);
	bool isBetween(float sourceX, float sourceZ, float targetX, float targetZ, int sceneObjectId) const;
	bool isObstacleBetween(const Vector3 &source, const Vector3 &target, int exceptSceneObjectId) const;
	void setIsClickable(int sceneObjectId, bool isClickable);
	void setIsObstacle(int sceneObjectId, bool isObstacle);
	void setIsTarget(int sceneObjectId, bool isTarget);
	void updateObstacles();

	void save(SaveFile &f);
private:
	int findById(int sceneObjectId) const;
	bool addSceneObject(int sceneObjectId, SceneObjectType sceneObjectType, BoundingBox *boundingBox, Common::Rect *screenRectangle, bool isClickable, bool isObstacle, uint8 unknown1, bool isTarget, bool isMoving, bool isRetired);
	int findEmpty() const;
};

} // End of namespace BladeRunner

#endif
