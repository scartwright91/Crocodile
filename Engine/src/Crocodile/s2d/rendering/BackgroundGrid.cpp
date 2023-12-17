#include "BackgroundGrid.h"

namespace Crocodile
{
	namespace s2d
	{

		BackgroundGrid::BackgroundGrid(graphics::Shader *shader)
		{
			this->shader = shader;
			init();
		}

		BackgroundGrid::~BackgroundGrid()
		{
		}

		void BackgroundGrid::render(
			float width,
			float height,
			float offsetX,
			float offsetY,
			float zoom)
		{
			shader->use();
			shader->setFloat("u_ScreenWidth", width);
			shader->setFloat("u_ScreenHeight", height);
			shader->setFloat("u_OffsetX", offsetX);
			shader->setFloat("u_OffsetY", offsetY);
			shader->setFloat("u_GridSize", (float)gridSize);
			shader->setFloat("u_Zoom", 1.0f);
			shader->setVec3("u_Color1", color1);
			shader->setVec3("u_Color2", color2);
			shader->setFloat("u_Alpha", alpha);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}

		void BackgroundGrid::init()
		{
			// configure VAO/VBO
			unsigned int VBO;
			float vertices[] = {
				// pos       // tex
				1.0f, 1.0f, 0.0f, 1.0f,
				1.0f, -1.0f, 1.0f, 0.0f,
				-1.0f, 1.0f, 0.0f, 0.0f,

				1.0f, -1.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 1.0f, 1.0f,
				-1.0f, 1.0f, 1.0f, 0.0f};

			glGenVertexArrays(1, &this->VAO);
			glGenBuffers(1, &VBO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glBindVertexArray(this->VAO);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

	}
}
