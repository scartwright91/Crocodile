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

			glm::vec3 cameraPosition = glm::vec3(0.0f);		  // used for offseting parallax layers
			glm::vec3 cameraScaledPosition = glm::vec3(0.0f); // used for rendering/calculating view matrix
			glm::vec2 screenPos = glm::vec2(0.0f);

			bool capMinimumZoom = true;
			bool transitioning = false;
			bool centreOnTarget = true;

			double distance = 0.0f;

			float zoom = 1.0f;
			void setZoom(float z);
			void changeZoom(float z);

			void setTargetScreenPosition(float x, float y);
			glm::vec2 getTargetScreenPosition(bool pixels);

			void setTarget(Object *t, bool transition);
			void setLevelBounds(glm::vec2 levelBounds, bool clampX, bool clampY);
			void removeLevelBounds();

			glm::vec2 getDifferenceFromTarget(glm::vec2 pos);

			glm::vec2 getWorldfromScreenPosition(glm::vec2 screenPos, float screenWidth, float screenHeight);
			glm::mat4 getViewMatrix();
			glm::mat4 getProjectionMatrix(bool applyZoom);

			void update();

		private:
			bool clampX = false;
			bool clampY = false;
			glm::vec2 levelBounds = glm::vec2(0.f);
			void clamp();

			graphics::Window *window;
			glm::vec2 calculateTransition(glm::vec2 pos);
		};
	}
}
