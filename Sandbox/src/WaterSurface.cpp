#include "WaterSurface.h"

WaterSurface::WaterSurface(
    graphics::Window* window,
    s3d::HeightMap heightMap,
    graphics::Shader* shader
    ) : s3d::Surface(heightMap, shader)
{
    shader->use();
    shader->setInt("u_Reflection", 0);
    shader->setInt("u_Refraction", 1);
    shader->setInt("u_DuDv", 2);
}

WaterSurface::~WaterSurface()
{
}

void WaterSurface::customRender(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection,
    unsigned int reflectionTexture,
    unsigned int refractionTexture,
    unsigned int dudvTexture,
    float timer,
    glm::vec3 cameraPosition
)
{
    shader->use();
    shader->setMat4("u_Model", model);
    shader->setMat4("u_View", view);
    shader->setMat4("u_Projection", projection);
    shader->setVec3("u_SurfaceColour", colour);
    shader->setFloat("u_Alpha", alpha);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, reflectionTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, refractionTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, dudvTexture);
    shader->setFloat("u_Time", timer);
    shader->setVec3("u_CameraPosition", cameraPosition);
    // render the cube
    glBindVertexArray(terrainVAO);
    for(int strip = 0; strip < numStrips; strip++)
    {
        glDrawElements(
            GL_TRIANGLE_STRIP,   // primitive type
            numTrisPerStrip+2,   // number of indices to render
            GL_UNSIGNED_INT,     // index data type
            (void*)(sizeof(unsigned) * (numTrisPerStrip+2) * strip)
        ); // offset to starting index
    }
    glBindVertexArray(0);



}
