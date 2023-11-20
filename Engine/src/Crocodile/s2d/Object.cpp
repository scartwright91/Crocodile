#include "Object.h"

namespace Crocodile
{
	namespace s2d
	{

		Object::Object() : renderMethod("sprite")
		{
		}

		Object::~Object()
		{
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
			position += glm::vec2(dx, dy);
		}

		void Object::moveTowards(glm::vec2 targetPosition, float distance)
		{
			float theta = std::atan2(targetPosition.y - position.y, targetPosition.x - position.x);
			move(std::cos(theta) * distance, std::sin(theta) * distance);
		}

		void Object::scale(glm::vec2 s)
		{
			modelScale = glm::vec3(s, 1.0f);
		}

		void Object::rotate(float v)
		{
			rotation += v;
			if (rotation > 2 * 3.14159265358979323846)
				rotation -= (2 * 3.14159265358979323846);
		}

		glm::mat4 Object::applyRotation(glm::mat4 model)
		{
			model = glm::translate(model, glm::vec3(size.x * modelScale.x / 2, size.y * modelScale.y / 2, 0.0f));
			model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-size.x * modelScale.x / 2, -size.y * modelScale.y / 2, 0.0f));
			return model;
		}

		glm::vec2 Object::getScaledSize()
		{
			glm::vec2 scaledSize = glm::vec2(size.x * modelScale.x, size.y * modelScale.y);
			return scaledSize;
		}

		glm::vec2 Object::getScaledPosition()
		{
			glm::vec2 scaledPosition = glm::vec2(position.x * modelScale.x, position.y * modelScale.y);
			return scaledPosition;
		}

		glm::vec2 Object::getPosition()
		{
			return position;
		}

		glm::vec2 Object::getCenteredPosition()
		{
			glm::vec2 centeredPos = glm::vec2(position.x + size.x / 2, position.y + size.y / 2);
			return centeredPos;
		}

		glm::vec2 Object::getScaledCenteredPosition()
		{
			glm::vec2 scaledCenteredPosition = glm::vec2((position.x + size.x / 2) * modelScale.x, (position.y + size.y / 2) * modelScale.y);
			return scaledCenteredPosition;
		}

		float Object::getDistanceFrom(glm::vec2 targetPosition)
		{
			float sqDifference = std::pow(targetPosition.x - position.x, 2) + std::pow(targetPosition.y - position.y, 2);
			return std::sqrt(sqDifference);
		}

		float Object::getYSortValue()
		{
			return getPosition().y + size.y;
		}

		void Object::setPosition(glm::vec2 pos)
		{
			position = pos;
		}

		void Object::setTexture(ResourceManager::TextureData texture)
		{
			this->texture = texture;
			useTexture = true;
		}

		void Object::setTileMapTexture(
			ResourceManager::TextureData texture,
			unsigned int gridSize,
			unsigned int x,
			unsigned int y)
		{
			this->texture = texture;
			useTexture = true;
			numberOfRows = (int)texture.height / gridSize;
			numberOfCols = (int)texture.width / gridSize;
			textureOffset = glm::vec2((int)x / texture.width, (int)y / texture.height);
		}

		void Object::updateAnimation(float dt)
		{
			if (animated)
			{
				animation->updateAnimation(dt);
				if (animation->spritesheetAnimation)
					textureOffset = animation->getTextureOffset();
				else
					texture = animation->textures[animation->currentFrame];
			}
		}

		void Object::setAnimation(Animation *animation)
		{
			animated = true;
			useTexture = true;
			this->animation = animation;
			if (animation->spritesheetAnimation)
			{
				this->texture = this->animation->spritesheet;
				numberOfRows = 1.0f;
				numberOfCols = (float)this->animation->totalFrames;
				textureOffset = this->animation->getTextureOffset();
			}
			else
			{
				this->texture = this->animation->textures[animation->currentFrame];
			}
		}

		void Object::setDistortionProperties(bool useDistortion, bool scrollX, bool scrollY, float distortionSpeed)
		{
			this->useDistortion = useDistortion;
			this->scrollDistortionX = scrollX;
			this->scrollDistortionY = scrollY;
			this->distortionSpeed = distortionSpeed;
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
			glm::mat4 model = calculateModelMatrix(position + offset, layerDepth);
			glm::mat4 mvp = projection * view * model;
			glm::vec2 pos = mvp * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			return glm::vec2((pos.x * 0.5 + 0.5) * width, height - (pos.y * 0.5 + 0.5) * height);
		}

		s2d::col::BoundingBox Object::getScreenBoundingBox(glm::mat4 view, glm::mat4 projection, float zoom, float width, float height, float layerDepth)
		{
			glm::mat4 model = calculateModelMatrix(position, layerDepth);
			glm::mat4 mvp = projection * view * model;
			glm::vec2 pos = mvp * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			glm::vec2 scaledSize = getScaledSize();
			s2d::col::BoundingBox bbox = s2d::col::BoundingBox(
				(pos.x * 0.5 + 0.5) * width,
				height - (pos.y * 0.5 + 0.5) * height,
				scaledSize.x / zoom,
				scaledSize.y / zoom,
				rotation);
			return bbox;
		}

		s2d::col::BoundingBox Object::getBoundingBox()
		{
			// Returns a bounding box with the same position and size as the entity in world coordinates
			return s2d::col::BoundingBox(position.x, position.y, size.x, size.y, rotation);
		}

		s2d::col::BoundingBox Object::getShiftedBoundingBox(float dx, float dy)
		{
			return s2d::col::BoundingBox(position.x + dx, position.y + dy, size.x, size.y, rotation);
		}

	}
}
