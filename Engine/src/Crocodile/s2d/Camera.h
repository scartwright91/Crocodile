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

			Object *m_target = nullptr;

			glm::vec3 m_cameraScaledPosition = glm::vec3(0.0f);
			glm::vec2 m_screenPos = glm::vec2(0.0f);

			bool m_capMinimumZoom = true;
			bool m_transitioning = false;
			bool m_centreOnTarget = true;

			double m_distance = 0.0f;
			float m_v = 0.f;

			float m_zoom = 1.0f;
			void setZoom(float z);
			void changeZoom(float z);

			void setTargetScreenPosition(float x, float y);
			glm::vec2 getTargetScreenPosition(bool pixels) const;

			void setTarget(Object *t, bool transition);
			void setLevelBounds(glm::vec2 levelBounds, bool clampX, bool clampY);
			void removeLevelBounds();

			glm::vec2 getDifferenceFromTarget(glm::vec2 pos) const;

			glm::vec2 getWorldfromScreenPosition(glm::vec2 screenPos, float screenWidth, float screenHeight, float layerDepth);
			glm::mat4 getViewMatrix(float depth);
			glm::mat4 getProjectionMatrix(bool applyZoom);

			void update(glm::vec2 viewportScale);

		private:
			bool m_clampX = false;
			bool m_clampY = false;
			glm::vec2 m_levelBounds = glm::vec2(0.f);
			glm::vec2 m_scaledLevelBounds = glm::vec2(0.f);

			void clamp();

			glm::vec2 m_viewportScale = glm::vec2(1.f);

			graphics::Window *m_window;
			glm::vec2 calculateTransition(glm::vec2 pos);
		};
	}
}
