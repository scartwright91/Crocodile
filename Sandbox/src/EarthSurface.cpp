#include "EarthSurface.h"

EarthSurface::EarthSurface(s3d::HeightMap heightMap, graphics::Shader* shader) : s3d::Surface(heightMap, shader)
{
}

EarthSurface::~EarthSurface()
{

}

void EarthSurface::customRender(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection,
    glm::vec3 cameraPosition,
    float ambientLighting,
    glm::vec3 lightPosition,
    glm::vec3 lightColour
)
{
    shader->use();
    shader->setMat4("u_Model", model);
    shader->setMat4("u_View", view);
    shader->setMat4("u_Projection", projection);
    shader->setFloat("u_AmbientLighting", ambientLighting);
    shader->setVec3("u_LightPosition", lightPosition);
    shader->setVec3("u_LightColour", lightColour);
    shader->setVec3("u_CameraPosition", cameraPosition);
    shader->setVec3("u_SurfaceColour", colour);
    shader->setFloat("u_Alpha", alpha);
    shader->setFloat("u_MinHeight", heightMap.minHeight);
    shader->setFloat("u_MaxHeight", heightMap.maxHeight);
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
