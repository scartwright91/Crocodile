#include "Object.h"

namespace Crocodile
{
	namespace s2d
	{

		Object::Object() : m_renderMethod("sprite")
		{
		}

		Object::~Object()
		{
		}

		void Object::addChildObject(Object* obj)
		{
			obj->setPosition(m_position + obj->getPosition());
			m_children.push_back(obj);
		}

		glm::mat4 Object::calculateModelMatrix(glm::vec2 pos, float layerDepth)
		{
			glm::mat4 model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(pos, 0.0f));
			model = applyRotation(model);
			model = glm::scale(model, glm::vec3(getScaledSize() * layerDepth, 1.0f));
			return model;
		}

		void Object::move(float dx, float dy)
		{
			m_position += glm::vec2(dx, dy);
			for (Object* obj : m_children)
				obj->move(dx, dy);
		}

		void Object::moveTowards(glm::vec2 targetPosition, float distance)
		{
			float theta = std::atan2(targetPosition.y - m_position.y, targetPosition.x - m_position.x);
			m_velocity.x = std::cos(theta) * distance;
			m_velocity.y = std::sin(theta) * distance;
			for (Object* obj : m_children)
				obj->m_velocity = m_velocity;
		}

		void Object::scale(glm::vec2 s)
		{
			m_modelScale = glm::vec3(s, 1.0f);
		}

		void Object::rotate(float v)
		{
			m_rotation += v;
			if (m_rotation > 2 * 3.14159265358979323846f)
				m_rotation -= (2 * 3.14159265358979323846f);
		}

		glm::mat4 Object::applyRotation(glm::mat4 model) const
		{
			model = glm::translate(model, glm::vec3(m_size.x * m_modelScale.x / 2, m_size.y * m_modelScale.y / 2, 0.0f));
			model = glm::rotate(model, m_rotation, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-m_size.x * m_modelScale.x / 2, -m_size.y * m_modelScale.y / 2, 0.0f));
			return model;
		}

		glm::vec2 Object::getScaledSize()
		{
			glm::vec2 scaledSize = glm::vec2(m_size.x * m_modelScale.x, m_size.y * m_modelScale.y);
			return scaledSize;
		}

		glm::vec2 Object::getScaledPosition()
		{
			glm::vec2 scaledPosition = glm::vec2(m_position.x * m_modelScale.x, m_position.y * m_modelScale.y);
			return scaledPosition;
		}

		glm::vec2 Object::getPosition() const
		{
			return m_position;
		}

		glm::vec2 Object::getCenteredPosition()
		{
			glm::vec2 centeredPos = glm::vec2(m_position.x + m_size.x / 2, m_position.y + m_size.y / 2);
			return centeredPos;
		}

		glm::vec2 Object::getScaledCenteredPosition()
		{
			glm::vec2 scaledCenteredPosition = glm::vec2((m_position.x + m_size.x / 2) * m_modelScale.x, (m_position.y + m_size.y / 2) * m_modelScale.y);
			return scaledCenteredPosition;
		}

		float Object::getDistanceFrom(glm::vec2 targetPosition) const
		{
			float sqDifference = (float)std::pow(targetPosition.x - m_position.x, 2) + (float)std::pow(targetPosition.y - m_position.y, 2);
			return std::sqrt(sqDifference);
		}

		float Object::getYSortValue() const
		{
			return getPosition().y + m_size.y;
		}

		void Object::setTexture(TextureData texture)
		{
			this->m_texture = texture;
			m_useTexture = true;
		}

		void Object::setTileMapTexture(
			TextureData texture,
			float gridSize,
			unsigned int x,
			unsigned int y)
		{
			this->m_texture = texture;
			m_useTexture = true;
			m_numberOfRows = texture.height / gridSize;
			m_numberOfCols = texture.width / gridSize;
			m_textureOffset = glm::vec2((int)x / texture.width, (int)y / texture.height);
		}

		void Object::updateAnimation(float dt)
		{
			if (m_animated)
			{
				m_animation->updateAnimation(dt);
				if (m_animation->m_spritesheetAnimation)
					m_textureOffset = m_animation->getTextureOffset();
				else
					m_texture = m_animation->m_textures[m_animation->m_currentFrame];
			}
		}

		void Object::setAnimation(Animation *animation)
		{
			m_animated = true;
			m_useTexture = true;
			m_animation = animation;
			if (animation->m_spritesheetAnimation)
			{
				m_texture = m_animation->m_spritesheet;
				m_numberOfRows = 1.0f;
				m_numberOfCols = (float)m_animation->m_totalFrames;
				m_textureOffset = m_animation->getTextureOffset();
			}
			else
			{
				m_texture = m_animation->m_textures[animation->m_currentFrame];
			}
		}

		void Object::resetCollisionData(unsigned int layer)
		{
			m_collisionData[layer].reset();
		}

		void Object::setDistortionProperties(bool useDistortion, bool scrollX, bool scrollY, float distortionSpeed)
		{
			m_useDistortion = useDistortion;
			m_scrollDistortionX = scrollX;
			m_scrollDistortionY = scrollY;
			m_distortionSpeed = distortionSpeed;
		}

		void Object::setSqueezeEffect(glm::vec2 maxDeformation, float squeezeDuration)
		{
			if (!m_useSqueeze)
			{
				m_maxDeformation = maxDeformation;
				m_squeezeDuration = squeezeDuration;
				m_useSqueeze = true;
			}
		}

		glm::vec2 Object::getScreenPosition(bool centre, glm::mat4 view, glm::mat4 projection, float width, float height, float layerDepth)
		{
			glm::mat4 model = calculateModelMatrix(centre ? getCenteredPosition() : getPosition(), layerDepth);
			glm::mat4 mvp = projection * view * model;
			glm::vec2 pos = mvp * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			return glm::vec2((pos.x * 0.5 + 0.5) * width, height - (pos.y * 0.5 + 0.5) * height);
		}

		glm::vec2 Object::getShiftedScreenPosition(glm::vec2 offset, glm::mat4 view, glm::mat4 projection, float width, float height, float layerDepth)
		{
			glm::mat4 model = calculateModelMatrix(m_position + offset, layerDepth);
			glm::mat4 mvp = projection * view * model;
			glm::vec2 pos = mvp * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			return glm::vec2((pos.x * 0.5 + 0.5) * width, height - (pos.y * 0.5 + 0.5) * height);
		}

		s2d::col::BoundingBox Object::getScreenBoundingBox(glm::mat4 view, glm::mat4 projection, float zoom, float width, float height, float layerDepth)
		{
			glm::mat4 model = calculateModelMatrix(m_position, layerDepth);
			glm::mat4 mvp = projection * view * model;
			glm::vec2 pos = mvp * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			glm::vec2 scaledSize = getScaledSize();
			s2d::col::BoundingBox bbox = s2d::col::BoundingBox(
				(pos.x * 0.5f + 0.5f) * width,
				height - (pos.y * 0.5f + 0.5f) * height,
				scaledSize.x / zoom,
				scaledSize.y / zoom,
				m_rotation);
			return bbox;
		}

		bool Object::intersects(Object* obj)
		{
			return getBoundingBox().intersectsBounds(obj->getBoundingBox());
		}

		s2d::col::BoundingBox Object::getBoundingBox()
		{
			// Returns a bounding box with the same position and size as the entity in world coordinates
			return s2d::col::BoundingBox(m_position.x, m_position.y, m_size.x, m_size.y, m_rotation);
		}

		s2d::col::BoundingBox Object::getShiftedBoundingBox(float dx, float dy)
		{
			return s2d::col::BoundingBox(m_position.x + dx, m_position.y + dy, m_size.x, m_size.y, m_rotation);
		}

		void Object::updateSqueezeEffect(float dt)
		{
			if (!m_useSqueeze)
				return;

			if (m_squeezeOut)
			{
				m_currentSqueezeElapsed += dt;
				if (m_currentSqueezeElapsed > m_squeezeDuration)
				{
					m_squeezeOut = false;
				}
			}
			else
			{
				m_currentSqueezeElapsed -= dt;
				if (m_currentSqueezeElapsed < 0.0f)
				{
					m_useSqueeze = false;
					m_squeezeOut = true;
					m_currentSqueezeElapsed = 0.0f;
				}
			}
			float r =  m_currentSqueezeElapsed / m_squeezeDuration;
			m_deformationMagnitude.x = m_maxDeformation.x * r;
			m_deformationMagnitude.y = m_maxDeformation.y * r;
		}

	}
}
