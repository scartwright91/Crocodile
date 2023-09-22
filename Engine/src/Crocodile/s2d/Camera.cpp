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
			screenPos = glm::vec2(x * zoom, y * zoom);
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
				glm::vec2 pos = target->getScaledCenteredPosition();
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
					setTargetScreenPosition(0.5f, 0.5f);
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
					setTargetScreenPosition(0.5f, 0.5f);
				}
			}
		}

		glm::vec2 Camera::getWorldfromScreenPosition(glm::vec2 screenPos, float screenWidth, float screenHeight, float layerDepth)
		{
			float ndcX = (2.0f * screenPos.x) / screenWidth - 1.0f;
			float ndcY = 1.0f - (2.0f * screenPos.y) / screenHeight;
			glm::mat4 inverseProjection = glm::inverse(getProjectionMatrix(true));
			glm::vec4 viewPosition = inverseProjection * glm::vec4(ndcX, ndcY, 0.f, 1.0f);
			glm::vec3 normalizedViewPos = glm::vec3(viewPosition) / viewPosition.w;
			glm::mat4 inverseView = glm::inverse(getViewMatrix(layerDepth));
			glm::vec4 worldPosition = inverseView * glm::vec4(normalizedViewPos, 1.0f);
			return glm::vec2(worldPosition.x, worldPosition.y);
		}

		glm::mat4 Camera::getViewMatrix(float depth)
		{
			glm::vec3 pos = glm::vec3(
				depth * cameraScaledPosition.x - (window->getViewportWidth() * screenPos.x),
				depth * cameraScaledPosition.y - (window->getViewportHeight() * screenPos.y),
				0.0f);
			glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
			return glm::lookAt(pos - glm::vec3(v), pos + front, up);
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
			return glm::vec3(target->getScaledCenteredPosition() - pos, 0.f);
		}

		void Camera::update(glm::vec2 viewportScale)
		{
			scaledLevelBounds = levelBounds * viewportScale;
			// set camera position
			if (transitioning)
			{
				glm::vec2 pos;
				if (centreOnTarget)
					pos = target->getScaledCenteredPosition();
				else
					pos = target->getScaledCenteredPosition();
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
					pos = target->getScaledCenteredPosition();
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
				if (cameraScaledPosition.x + targetScreenPos.x > scaledLevelBounds.x)
					cameraScaledPosition.x = scaledLevelBounds.x - targetScreenPos.x;
			}
			if (clampY)
			{
				if (cameraScaledPosition.y - targetScreenPos.y < 0)
					cameraScaledPosition.y = targetScreenPos.y;
				if (cameraScaledPosition.y + targetScreenPos.y > scaledLevelBounds.y)
					cameraScaledPosition.y = scaledLevelBounds.y - targetScreenPos.y;
			}
		}

	}
}