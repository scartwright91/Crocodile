#pragma once

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Core.h"
#include "Mesh.h"

namespace Crocodile
{
    namespace s3d
    {
        class CROCODILE_API MeshLoader
        {
        public:
            Mesh loadMesh(const std::string& filePath);

        private:
            Assimp::Importer m_importer;
        };
    }
}
