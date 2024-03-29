#pragma once

#include "Object.h"
#include "../Core.h"
#include "../graphics/Window.h"

namespace Crocodile
{
	namespace s2d
	{
		class CROCODILE_API Camera
		{
		public:
			Camera(graphics::Window *win);
			~Camera();

			Object *target = nullptr;

			glm::vec3 cameraScaledPosition = glm::vec3(0.0f);
			glm::vec2 screenPos = glm::vec2(0.0f);

			bool capMinimumZoom = true;
			bool transitioning = false;
			bool centreOnTarget = true;

			double distance = 0.0f;
			float v = 0.f;

			float zoom = 1.0f;
			void setZoom(float z);
			void changeZoom(float z);

			void setTargetScreenPosition(float x, float y);
			glm::vec2 getTargetScreenPosition(bool pixels);

			void setTarget(Object *t, bool transition);
			void setLevelBounds(glm::vec2 levelBounds, bool clampX, bool clampY);
			void removeLevelBounds();

			glm::vec2 getDifferenceFromTarget(glm::vec2 pos);

			glm::vec2 getWorldfromScreenPosition(glm::vec2 screenPos, float screenWidth, float screenHeight, float layerDepth);
			glm::mat4 getViewMatrix(float depth);
			glm::mat4 getProjectionMatrix(bool applyZoom);

			void update(glm::vec2 viewportScale);

		private:
			bool clampX = false;
			bool clampY = false;
			glm::vec2 levelBounds = glm::vec2(0.f);
			glm::vec2 scaledLevelBounds = glm::vec2(0.f);

			void clamp();

			glm::vec2 viewportScale = glm::vec2(1.f);

			graphics::Window *window;
			glm::vec2 calculateTransition(glm::vec2 pos);
		};
	}
}
