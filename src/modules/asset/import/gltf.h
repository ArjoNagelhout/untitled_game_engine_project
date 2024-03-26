//
// Created by Arjo Nagelhout on 22/12/2023.
//

#ifndef SHAPEREALITY_GLTF_H
#define SHAPEREALITY_GLTF_H

#include <filesystem>

#include <renderer/mesh.h>

#include <asset/import_registry.h>

namespace asset
{
    // metadata for how to import the gltf object hierarchy
    struct GltfImportDescriptor
    {

    };

    struct GltfImportResult
    {
        bool success;
    };

    // how do we handle texture importing? do import a "model" and then import the textures
    [[nodiscard]] GltfImportResult importGltf(graphics::IDevice* device,
                                              std::filesystem::path const& source,
                                              GltfImportDescriptor const& descriptor,
                                              std::vector<std::unique_ptr<renderer::Mesh>>& outMeshes);

    /**
     * todo: import:
     *      - skinning,
     *      - animation
     *      - lights
     *      - cameras
     *      - material variants
     */
    [[nodiscard]] ImportResult importGltfNew(AssetDatabase& assets, std::filesystem::path const& inputFile);

    /**
     * This is a simple dummy asset so that I can test out the AssetHandle system
     */
    class DummyAsset
    {
    public:
        explicit DummyAsset(std::string name);

        ~DummyAsset();

        [[nodiscard]] std::string_view name() const;

    private:
        std::string name_;
    };
}

#endif //SHAPEREALITY_GLTF_H
