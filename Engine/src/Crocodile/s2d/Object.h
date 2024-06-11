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

		struct CollisionData {
			inline bool hasCollided() const { return on_floor || on_ceiling || on_wall_left || on_wall_right; }
			bool on_floor = false;
			bool on_ceiling = false;
			bool on_wall_left = false;
			bool on_wall_right = false;
			CollisionData() {};
			void reset()
			{
				on_floor = false;
				on_ceiling = false;
				on_wall_left = false;
				on_wall_right = false;
			};
		};

		struct CollisionLayer {
			std::string name;
			bool dynamic;
		};

		class CROCODILE_API Object
		{

		public:
			Object();
			virtual ~Object();

			// label
			std::string m_label = "";
			std::string m_layer = "";
			double m_id = glfwGetTime();

			// movement
			glm::vec2 m_velocity = glm::vec2(0.f);

			// vertex
			glm::vec2 m_size = glm::vec2(0.0f);
			glm::vec3 m_modelScale = glm::vec3(1.0f);
			float m_rotation = 0.0f;

			// fragment
			glm::vec3 m_color = glm::vec3(0.0f);
			float m_alpha = 1.0f;
			TextureData m_texture = {};
			bool m_useTexture = false;
			bool m_useColorTexture = false;
			float m_numberOfRows = 1.f;
			float m_numberOfCols = 1.f;
			glm::vec2 m_textureOffset = glm::vec2(0.0f);
			bool m_useDistortion = false;
			bool m_scrollDistortionX = false;
			bool m_scrollDistortionY = false;
			float m_distortionSpeed = 0.02f;
			bool m_flipX = false;
			bool m_flipY = false;

			// rendering
			std::string m_renderMethod = "";

			// filters
			bool m_outline = false;
			bool m_showBoundingBox = false;
			bool m_showAxes = false;

			// animation
			Animation* m_animation = nullptr;

			// collisions
			void resetCollisionData(unsigned int layer);
			std::vector<unsigned int> m_collisionLayers = {};
			std::map<unsigned int, CollisionData> m_collisionData = {
				{0, CollisionData()},
				{1, CollisionData()},
				{2, CollisionData()}
			};

		private:
			glm::vec2 m_position = glm::vec2(0.0f);
			bool m_animated = false;

		public:
			std::vector<Object*> m_children = {};
			void addChildObject(Object* obj);

			glm::mat4 calculateModelMatrix(glm::vec2 pos, float layerDepth);
			void move(float dx, float dy);
			void moveTowards(glm::vec2 targetPosition, float distance);
			void scale(glm::vec2 s);
			void rotate(float v);
			glm::vec2 getScaledSize();
			glm::vec2 getScaledPosition();
			glm::vec2 getPosition() const;
			glm::vec2 getCenteredPosition();
			glm::vec2 getScaledCenteredPosition();
			glm::vec2 getScreenPosition(bool centre, glm::mat4 view, glm::mat4 projection, float width, float height, float layerDepth);
			glm::vec2 getShiftedScreenPosition(glm::vec2 offset, glm::mat4 view, glm::mat4 projection, float width, float height, float layerDepth);
			float getDistanceFrom(glm::vec2 targetPosition) const;
			float getYSortValue() const;
			void setPosition(glm::vec2 pos);

			// textures, animations, vfx
			void setTexture(TextureData texture);
			void setTileMapTexture(
				TextureData texture,
				float gridSize,
				unsigned int x,
				unsigned int y);
			void setAnimation(Animation *animation);
			void updateAnimation(float dt);
			void setDistortionProperties(bool useDistortion, bool scrollX, bool scrollY, float distortionSpeed);

			// squeeze & stretch vfx
			void setSqueezeEffect(glm::vec2 maxDeformation, float squeezeDuration);
			void updateSqueezeEffect(float dt);
			float m_currentSqueezeElapsed = 0.0f;
			float m_squeezeDuration = 0.0f;
			bool m_useSqueeze = false;
			bool m_squeezeOut = true;
			glm::vec2 m_deformationMagnitude = glm::vec2(0.f);
			glm::vec2 m_maxDeformation = glm::vec2(0.f);

			bool intersects(Object* obj);
			s2d::col::BoundingBox getScreenBoundingBox(glm::mat4 view, glm::mat4 projection, float zoom, float width, float height, float layerDepth);
			s2d::col::BoundingBox getBoundingBox();
			s2d::col::BoundingBox getShiftedBoundingBox(float dx, float dy);

		private:
			glm::mat4 applyRotation(glm::mat4 model) const;

		};
	}
}
