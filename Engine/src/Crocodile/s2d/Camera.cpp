#include "Camera.h"

namespace Crocodile
{
	namespace s2d
	{
		Camera::Camera(graphics::Window *win) : m_window(win)
		{
			setTargetScreenPosition(0.5f, 0.5f);
		}

		Camera::~Camera()
		{
		}

		void Camera::setTargetScreenPosition(float x, float y)
		{
			m_screenPos = glm::vec2(x * m_zoom, y * m_zoom);
		}

		glm::vec2 Camera::getTargetScreenPosition(bool pixels) const
		{
			if (pixels)
				return glm::vec2(m_screenPos.x * m_window->getWidth(), m_screenPos.y * m_window->getHeight());
			return m_screenPos;
		}

		void Camera::setTarget(Object *t, bool transition)
		{
			m_target = t;
			m_transitioning = transition;
			if (m_transitioning)
			{
				glm::vec2 pos = m_target->getScaledCenteredPosition();
				m_distance = sqrt(pow(pos.x - m_cameraScaledPosition.x, 2) + pow(pos.y - m_cameraScaledPosition.y, 2));
			}
		}

		void Camera::setLevelBounds(glm::vec2 levelBounds, bool clampX, bool clampY)
		{
			m_clampX = clampX;
			m_clampY = clampY;
			m_levelBounds = levelBounds;
		}

		void Camera::removeLevelBounds()
		{
			m_clampX = false;
			m_clampY = false;
			m_levelBounds = glm::vec2(0.f);
		}

		void Camera::setZoom(float z)
		{
			if (m_capMinimumZoom)
			{
				if (z > 0.f)
				{
					m_zoom = z;
					setTargetScreenPosition(0.5f, 0.5f);
				}
			}
		}

		void Camera::changeZoom(float z)
		{
			if (m_capMinimumZoom)
			{
				if (m_zoom + z > 0.f)
				{
					m_zoom += z;
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
				depth * m_cameraScaledPosition.x - (m_window->getViewportWidth() * m_screenPos.x),
				depth * m_cameraScaledPosition.y - (m_window->getViewportHeight() * m_screenPos.y),
				0.0f);
			glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
			return glm::lookAt(pos - glm::vec3(m_v), pos + front, up);
		}

		glm::mat4 Camera::getProjectionMatrix(bool applyZoom)
		{
			float width;
			float height;
			width = (float)m_window->getViewportWidth();
			height = (float)m_window->getViewportHeight();
			if (applyZoom)
			{
				width *= m_zoom;
				height *= m_zoom;
			}
			float aspectRatio = (float)width / height;
			glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
			return projection;
		}

		glm::vec2 Camera::getDifferenceFromTarget(glm::vec2 pos) const
		{
			if (m_target == NULL)
				return glm::vec3(0.0f);
			return glm::vec3(m_target->getScaledCenteredPosition() - pos, 0.f);
		}

		void Camera::update(glm::vec2 viewportScale)
		{
			m_scaledLevelBounds = m_levelBounds * viewportScale;
			// set camera position
			if (m_transitioning)
			{
				glm::vec2 pos;
				if (m_centreOnTarget)
					pos = m_target->getScaledCenteredPosition();
				else
					pos = m_target->getScaledCenteredPosition();
				glm::vec2 transition = calculateTransition(pos);
				float dx = (pos.x - m_cameraScaledPosition.x);
				float dy = (pos.y - m_cameraScaledPosition.y);
				m_cameraScaledPosition.x += transition.x;
				m_cameraScaledPosition.y += transition.y;
				if ((abs(dx) < 0.01 * m_distance) && (abs(dy) < 0.01 * m_distance))
					m_transitioning = false;
			}
			else if (m_target != NULL)
			{
				glm::vec2 pos;
				if (m_centreOnTarget)
					pos = m_target->getScaledCenteredPosition();
				else
					pos = m_target->getScaledCenteredPosition();
				m_cameraScaledPosition = glm::vec3(pos, 0.0f);
			}

			if (m_clampX || m_clampY)
				clamp();
		}

		glm::vec2 Camera::calculateTransition(glm::vec2 pos)
		{
			float theta = atan2(pos.y - m_cameraScaledPosition.y, pos.x - m_cameraScaledPosition.x);
			double currentDistance = sqrt(pow(pos.x - m_cameraScaledPosition.x, 2) + pow(pos.y - m_cameraScaledPosition.y, 2));
			float transitionDistance = (float)std::max(0.05 * currentDistance, 0.01 * m_distance);
			return glm::vec2(transitionDistance * cos(theta), transitionDistance * sin(theta));
		}

		void Camera::clamp()
		{
			glm::vec2 targetScreenPos = getTargetScreenPosition(true);
			if (m_clampX)
			{
				if (m_cameraScaledPosition.x - targetScreenPos.x < 0)
					m_cameraScaledPosition.x = targetScreenPos.x;
				if (m_cameraScaledPosition.x + targetScreenPos.x > m_scaledLevelBounds.x)
					m_cameraScaledPosition.x = m_scaledLevelBounds.x - targetScreenPos.x;
			}
			if (m_clampY)
			{
				if (m_cameraScaledPosition.y - targetScreenPos.y < 0)
					m_cameraScaledPosition.y = targetScreenPos.y;
				if (m_cameraScaledPosition.y + targetScreenPos.y > m_scaledLevelBounds.y)
					m_cameraScaledPosition.y = m_scaledLevelBounds.y - targetScreenPos.y;
			}
		}

	}
}