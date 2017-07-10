#pragma once

#include "Entity.h"
#include "GameInfo.h"
#include "Maths.h"

#include <vector>


class EntityManager
{
public:
	EntityManager(TerrainManager& aTerrainManager)
		: myTerrainManager(aTerrainManager)
	{
	}
	Entity& AddEntity(TexturedModel& aModel, glm::vec3 aPosition, glm::vec3 aRotation, float aScale)
	{
		myEntities.push_back(Entity(aModel, aPosition, aRotation, aScale));
		return myEntities.back();
	}

	void AddEntityRandom(TexturedModel& aModel, vec3 aMin, vec3 aMax, vec2 aScaleRange)
	{
		vec3 randPos;
		do {
			 randPos = RandVector(aMin, aMax);
			 randPos.y = myTerrainManager.GetHeight(randPos.x, randPos.z);
		} while (randPos.y < GameInfo::ourWaterHeight);
		float rY = (float)(rand() % 180);
		float rS = RandFloat(aScaleRange.x, aScaleRange.y);

		AddEntity(aModel, randPos, vec3(0, rY, 0), rS);
	}

	std::vector<Entity> myEntities;
	TerrainManager& myTerrainManager;
};