#include "Camera.h"

namespace Crocodile
{
	namespace s2d
	{
		Camera::Camera(graphics::Window *win) : window(win)
		{
			setTargetScreenPosition(0.5f, 0.5f);
		}

		Camera::~Camera()
		{
		}

		void Camera::setTargetScreenPosition(float x, float y)
		{
			screenPos = glm::vec2(x, y);
		}

		glm::vec2 Camera::getTargetScreenPosition(bool pixels)
		{
			if (pixels)
				return glm::vec2(screenPos.x * window->getWidth(), screenPos.y * window->getHeight());
			return screenPos;
		}

		void Camera::setTarget(Object *t, bool transition)
		{
			target = t;
			transitioning = transition;
			if (transitioning)
			{
				glm::vec2 pos = target->getScaledPosition();
				distance = sqrt(pow(pos.x - cameraScaledPosition.x, 2) + pow(pos.y - cameraScaledPosition.y, 2));
			}
		}

		void Camera::setLevelBounds(glm::vec2 levelBounds, bool clampX, bool clampY)
		{
			this->clampX = clampX;
			this->clampY = clampY;
			this->levelBounds = levelBounds;
		}

		void Camera::removeLevelBounds()
		{
			clampX = false;
			clampY = false;
			levelBounds = glm::vec2(0.f);
		}

		void Camera::setZoom(float z)
		{
			if (capMinimumZoom)
			{
				if (z > 0.f)
				{
					zoom = z;
					setTargetScreenPosition(0.5f * zoom, 0.5f * zoom);
				}
			}
		}

		void Camera::changeZoom(float z)
		{
			if (capMinimumZoom)
			{
				if (zoom + z > 0.f)
				{
					zoom += z;
					setTargetScreenPosition(0.5f * zoom, 0.5f * zoom);
				}
			}
		}

		glm::mat4 Camera::getViewMatrix()
		{
			glm::vec3 pos = glm::vec3(
				cameraScaledPosition.x - (window->getViewportWidth() * screenPos.x),
				cameraScaledPosition.y - (window->getViewportHeight() * screenPos.y),
				0.0f);
			glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
			return glm::lookAt(pos, pos + front, up);
		}

		glm::mat4 Camera::getProjectionMatrix(bool applyZoom)
		{
			float width;
			float height;
			width = window->getViewportWidth();
			height = window->getViewportHeight();
			if (applyZoom)
			{
				width *= zoom;
				height *= zoom;
			}
			float aspectRatio = (float)width / height;
			glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
			return projection;
		}

		glm::vec2 Camera::getDifferenceFromTarget(glm::vec2 pos)
		{
			if (target == NULL)
				return glm::vec3(0.0f);
			return glm::vec3(target->getScaledPosition() - pos, 0.f);
		}

		void Camera::update()
		{
			// set camera position
			if (transitioning)
			{
				glm::vec2 pos;
				if (centreOnTarget)
					pos = target->getScaledCenteredPosition();
				else
					pos = target->getScaledPosition();
				glm::vec2 transition = calculateTransition(pos);
				float dx = (pos.x - cameraScaledPosition.x);
				float dy = (pos.y - cameraScaledPosition.y);
				cameraScaledPosition.x += transition.x;
				cameraScaledPosition.y += transition.y;
				if ((abs(dx) < 0.01 * distance) && (abs(dy) < 0.01 * distance))
				{
					transitioning = false;
				}
			}
			else if (target != NULL)
			{
				glm::vec2 pos;
				if (centreOnTarget)
					pos = target->getScaledCenteredPosition();
				else
					pos = target->getScaledPosition();
				cameraScaledPosition = glm::vec3(pos, 0.0f);
				cameraPosition = glm::vec3(target->getPosition(), 0.0f);
			}

			if (clampX || clampY)
				clamp();
		}

		glm::vec2 Camera::calculateTransition(glm::vec2 pos)
		{
			float theta = atan2(pos.y - cameraScaledPosition.y, pos.x - cameraScaledPosition.x);
			double currentDistance = sqrt(pow(pos.x - cameraScaledPosition.x, 2) + pow(pos.y - cameraScaledPosition.y, 2));
			float transitionDistance = (float)std::max(0.05 * currentDistance, 0.01 * distance);
			return glm::vec2(transitionDistance * cos(theta), transitionDistance * sin(theta));
		}

		void Camera::clamp()
		{
			glm::vec2 targetScreenPos = getTargetScreenPosition(true);
			if (clampX)
			{
				if (cameraScaledPosition.x - targetScreenPos.x < 0)
					cameraScaledPosition.x = targetScreenPos.x;
				if (cameraScaledPosition.x + targetScreenPos.x > levelBounds.x)
					cameraScaledPosition.x = levelBounds.x - targetScreenPos.x;
			}
			if (clampY)
			{
				if (cameraScaledPosition.y - targetScreenPos.y < 0)
					cameraScaledPosition.y = targetScreenPos.y;
				if (cameraScaledPosition.y + targetScreenPos.y > levelBounds.y)
					cameraScaledPosition.y = levelBounds.y - targetScreenPos.y;
			}
		}

	}
}