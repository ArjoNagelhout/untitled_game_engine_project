//
// Created by Arjo Nagelhout on 27/12/2023.
//

#ifndef SHAPEREALITY_MESH_RENDERER_H
#define SHAPEREALITY_MESH_RENDERER_H

namespace rendering
{
    class Mesh;

    class Material;

    struct MeshRendererComponent final
    {
        Mesh* mesh; // unowned pointer
        Material* material; // unowned pointer
    };
}

#endif //SHAPEREALITY_MESH_RENDERER_H