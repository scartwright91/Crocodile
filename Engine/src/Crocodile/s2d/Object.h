#pragma once

#include <iostream>
#include "GL/glew.h"
#include <vector>
#include <map>
#include <stack>
#include <algorithm>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core.h"
#include "col/BoundingBox.h"
#include "Animation.h"

namespace Crocodile
{
	namespace s2d
	{
		class CROCODILE_API Object
		{

		public:
			enum RenderTypes
			{
				NONE,	  // Will not be rendered
				SPRITE,	  // Sprite renderer
				PARTICLE, // Particle renderer
				TEXT,	  // Text renderer
				UI		  // UI renderer
			};

			Object();
			Object(float x, float y);
			~Object();

			// label
			std::string label = "";

			// vertex
			glm::vec2 size = glm::vec2(0.0f);
			glm::vec3 modelScale = glm::vec3(1.0f);
			float rotation = 0.0f;

			// fragment
			glm::vec3 color = glm::vec3(0.0f);
			float alpha = 1.0f;
			unsigned int texture = 0;
			bool useTexture = false;
			float numberOfRows = 1.f;
			float numberOfCols = 1.f;
			glm::vec2 textureOffset = glm::vec2(0.0f);
			bool useDistortion = false;
			bool scrollDistortionX = false;
			bool scrollDistortionY = false;
			float distortionSpeed = 0.02f;
			bool flipX = false;
			bool flipY = false;

			// children
			Object *parent = nullptr;
			std::vector<Object *> children = {};
			unsigned int nChildren = 0;

			// rendering
			RenderTypes renderType;

			// filters
			bool outline = false;

		private:
			glm::vec2 startingPosition = glm::vec2(0.0f);
			glm::vec2 position = glm::vec2(0.0f);
			bool animated = false;
			Animation *animation = nullptr;

		public:
			void addChild(Object *o);
			void addParent(Object *o);
			void removeChild(Object *o);
			void removeParent();
			void clear();

			glm::mat4 calculateModelMatrix(glm::vec2 pos);
			void move(float dx, float dy);
			void moveTowards(glm::vec2 targetPosition, float speed);
			void scale(glm::vec2 s);
			void rotate(float v);
			glm::vec2 getScaledSize();
			glm::vec2 getScaledPosition();
			glm::vec2 getPosition();
			glm::vec2 getStartingPosition();
			glm::vec2 getScaledStartingPosition();
			glm::vec2 getAbsolutePosition();
			glm::vec2 getCenteredPosition();
			glm::vec2 getScaledCenteredPosition();
			float getDistanceFrom(glm::vec2 targetPosition);
			float getYSortValue();
			void setStartingPosition(glm::vec2 pos);
			void setPosition(glm::vec2 pos);

			void setTexture(unsigned int textureID);
			void setTileMapTexture(
				unsigned int textureID,
				float width,
				float height,
				unsigned int gridSize,
				unsigned int x,
				unsigned int y);
			void setAnimation(Animation *animation);
			void updateAnimation(float dt);
			void setDistortionProperties(bool useDistortion, bool scrollX, bool scrollY, float distortionSpeed);

			s2d::col::BoundingBox getScreenBoundingBox(glm::mat4 view, glm::mat4 projection, float zoom, float width, float height);
			s2d::col::BoundingBox getBoundingBox();
			s2d::col::BoundingBox getShiftedBoundingBox(float dx, float dy);

		private:
			void syncParentPosition();
			glm::mat4 applyRotation(glm::mat4 model);
		};
	}
}
