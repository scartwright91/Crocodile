#include "Object.h"

namespace Crocodile
{
	namespace s2d
	{

		Object::Object() : renderMethod("sprite")
		{
		}

		Object::Object(float x, float y) : renderMethod("sprite")
		{
			move(x, y);
		}

		Object::~Object()
		{
		}

		void Object::addChild(Object *o)
		{
			if (parent == NULL)
			{
				children.push_back(o);
				o->addParent(this);
				nChildren++;
			}
			else
				std::cout << "Cannot be added: object has parent and recursive relationships not implemented.";
		}

		void Object::addParent(Object *o)
		{
			this->parent = o;
			syncParentPosition();
		}

		void Object::removeChild(Object *o)
		{
			o->removeParent();
			children.erase(std::remove(children.begin(), children.end(), o), children.end());
			nChildren--;
		}

		void Object::removeParent()
		{
			this->parent = nullptr;
		}

		void Object::clear()
		{
			children.clear();
		}

		glm::mat4 Object::calculateModelMatrix(glm::vec2 pos)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(pos, 0.0f));
			model = applyRotation(model);
			model = glm::scale(model, glm::vec3(getScaledSize(), 1.0f));
			return model;
		}

		void Object::move(float dx, float dy)
		{
			startingPosition += glm::vec2(dx, dy);
			position += glm::vec2(dx, dy);
			for (Object *child : children)
			{
				child->move(dx, dy);
			}
		}

		void Object::moveTowards(glm::vec2 targetPosition, float speed)
		{
			float theta = std::atan2(targetPosition.y - position.y, targetPosition.x - position.x);
			move(std::cos(theta) * speed, std::sin(theta) * speed);
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

		glm::vec2 Object::getStartingPosition()
		{
			return startingPosition;
		}

		glm::vec2 Object::getScaledStartingPosition()
		{
			glm::vec2 startingPos = glm::vec2(startingPosition.x * modelScale.x, startingPosition.y * modelScale.y);
			return startingPos;
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

		void Object::setStartingPosition(glm::vec2 pos)
		{
			startingPosition = pos;
			position = pos;
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
			float width,
			float height,
			unsigned int gridSize,
			unsigned int x,
			unsigned int y)
		{
			this->texture = texture;
			useTexture = true;
			numberOfRows = (float)height / gridSize;
			numberOfCols = (float)width / gridSize;
			textureOffset = glm::vec2(x / width, y / height);
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

		glm::vec2 Object::getAbsolutePosition()
		{
			bool parentExists = true;
			Object *currentParent = parent;
			glm::vec2 absPosition = position;
			while (parentExists)
			{
				if (currentParent == NULL)
					parentExists = false;
				else
				{
					absPosition += currentParent->position;
					currentParent = currentParent->parent;
				}
			}
			return absPosition;
		}

		glm::vec2 Object::getScreenPosition(glm::mat4 view, glm::mat4 projection, float width, float height)
		{
			glm::mat4 model = calculateModelMatrix(position);
			glm::mat4 mvp = projection * view * model;
			glm::vec2 pos = mvp * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			return glm::vec2((pos.x * 0.5 + 0.5) * width, height - (pos.y * 0.5 + 0.5) * height);
		}

		s2d::col::BoundingBox Object::getScreenBoundingBox(glm::mat4 view, glm::mat4 projection, float zoom, float width, float height)
		{
			glm::mat4 model = calculateModelMatrix(position);
			glm::mat4 mvp = projection * view * model;
			glm::vec2 pos = mvp * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			glm::vec2 scaledSize = getScaledSize();
			s2d::col::BoundingBox bbox = s2d::col::BoundingBox(
				(pos.x * 0.5 + 0.5) * width,
				height - (pos.y * 0.5 + 0.5) * height,
				scaledSize.x / zoom,
				scaledSize.y / zoom);
			return bbox;
		}

		s2d::col::BoundingBox Object::getBoundingBox()
		{
			// Returns a bounding box with the same position and size as the entity in world coordinates
			return s2d::col::BoundingBox(position.x, position.y, size.x, size.y);
		}

		s2d::col::BoundingBox Object::getShiftedBoundingBox(float dx, float dy)
		{
			return s2d::col::BoundingBox(position.x + dx, position.y + dy, size.x, size.y);
		}

		void Object::syncParentPosition()
		{
			position += parent->position;
		}

	}
}
