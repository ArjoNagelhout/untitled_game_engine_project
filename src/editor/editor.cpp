//
// Created by Arjo Nagelhout on 29/02/2024.
//

#include "editor.h"
#include "axes.h"

#include <iostream>
#include <common/logger.h>
#include <reflection/enum.h>

namespace editor
{
    struct MeshRendererNew
    {
        asset::Asset mesh;
        renderer::Material* material;
    };

    void createObjectNew(entity::EntityRegistry& r, entity::EntityId entityId,
                         MeshRendererNew const& meshRenderer, bool visible = true)
    {
        r.createEntity(entityId);
        r.addComponent<entity::HierarchyComponent>(entityId);
        if (visible)
        {
            r.addComponent<renderer::VisibleComponent>(entityId);
        }
        r.addComponent<renderer::TransformComponent>(entityId);
        r.addComponent<renderer::TransformDirtyComponent>(entityId); // to make sure the transform gets calculated on start
        r.addComponent<MeshRendererNew>(entityId, meshRenderer);
    }

    Editor::Editor(asset::AssetDatabase& assets_) : assets(assets_) {}

    Editor::~Editor() = default;

    void Editor::onEvent(graphics::InputEvent const& event, graphics::Window* _window)
    {
        input->onEvent(event);
        ui->onEvent(event);
    }

    void Editor::applicationDidFinishLaunching()
    {
        mesh0 = assets.get(asset::AssetId{"models/sea_house/scene.gltf", "Object_0_0.mesh"});
        mesh1 = assets.get(asset::AssetId{"models/sea_house/scene.gltf", "Object_1_0.mesh"});
        mesh2 = assets.get(asset::AssetId{"models/sea_house/scene.gltf", "Object_2_0.mesh"});
        mesh3 = assets.get(asset::AssetId{"models/sea_house/scene.gltf", "Object_3_0.mesh"});
        mesh4 = assets.get(asset::AssetId{"models/sea_house/scene.gltf", "Object_4_0.mesh"});

        assets.importFile("models/city/city_2.glb");

        // Manual creation of a triangle
        renderer::MeshDescriptor descriptor{
            .primitiveType = graphics::PrimitiveType::Triangle,
            .attributes{
                renderer::VertexAttributeDescriptor{
                    .index = 0,
                    .type = renderer::VertexAttribute_Position,
                    .elementType = renderer::ElementType::Vector3,
                    .componentType = renderer::ComponentType::Float
                },
                renderer::VertexAttributeDescriptor{
                    .index = 0,
                    .type = renderer::VertexAttribute_Color,
                    .elementType = renderer::ElementType::Vector3,
                    .componentType = renderer::ComponentType::Float
                }
            },
            .vertexCount = 3,
            .hasIndexBuffer = true,
            .indexCount = 3,
            .indexType = renderer::ComponentType::UnsignedInt,
            .writable = true,
        };
        dummyMesh = asset::makeAsset<renderer::Mesh>(asset::AssetId{"test", "test.mesh"}, device, descriptor);
        {
            auto& mesh = dummyMesh->get<renderer::Mesh>();
            std::vector<unsigned int> indices{
                0, 1, 2
            };
            mesh.setIndexData(indices.data());
            std::vector<math::Vector3> positions{
                math::Vector3{{0.5f, -0.5f, 0.0f}},
                math::Vector3{{-0.5f, -0.5f, 0.0f}},
                math::Vector3{{0.0f, 0.5f, 0.0f}},
            };
            mesh.setAttributeData(renderer::VertexAttribute_Position, positions.data(), 0);
            std::vector<math::Vector3> colors{
                math::Vector3{{1.0f, 0.0f, 0.0f}},
                math::Vector3{{0.0f, 0.0f, 1.0f}},
                math::Vector3{{1.0f, 1.0f, 0.0f}},
            };
            mesh.setAttributeData(renderer::VertexAttribute_Color, colors.data(), 0);
        }

        // command queue
        graphics::CommandQueueDescriptor commandQueueDescriptor{};
        commandQueue = device->createCommandQueue(commandQueueDescriptor);

        // shader library
        shaderLibrary = device->createShaderLibrary(
            "/Users/arjonagelhout/Documents/ShapeReality/shapereality/build/shaders/library.metallib");

        // depth stencil state
        graphics::DepthStencilDescriptor depthStencilDescriptor{
            .depthCompareFunction = graphics::CompareFunction::LessEqual,
            .depthWriteEnabled = true,
        };
        depthStencilState = device->createDepthStencilState(depthStencilDescriptor);

        // input handler
        input = std::make_unique<input::Input>();

        // camera
        camera = std::make_unique<renderer::Camera>(device);
        cameraController = std::make_unique<CameraController>(*camera, *input);

        // shaders
        newShader = std::make_unique<renderer::Shader>(device, shaderLibrary.get(), "new_vertex", "new_fragment");
        newColorShader = std::make_unique<renderer::Shader>(device, shaderLibrary.get(), "new_color_vertex", "new_color_fragment");
        newCityShader = std::make_unique<renderer::Shader>(device, shaderLibrary.get(), "new_city_vertex", "new_city_fragment");
        breakerRoomShader = std::make_unique<renderer::Shader>(device, shaderLibrary.get(), "breaker_room_vertex", "breaker_room_fragment");
        axesShader = std::make_unique<renderer::Shader>(device, shaderLibrary.get(), "axes_vertex", "axes_fragment");

        // textures
        textureBaseColor = assets.get(asset::AssetId{"models/sea_house/textures/default_baseColor.png", "texture.texture"});
        textureMaterial25 = assets.get(asset::AssetId{"models/sea_house/textures/11112_sheet_Material__25_baseColor.png", "texture.texture"});
        textureMaterial37 = assets.get(asset::AssetId{"models/sea_house/textures/11112_sheet_Material__37_baseColor.png", "texture.texture"});

        // materials
        materialBaseColor = {newShader.get(), textureBaseColor};
        material25 = {newShader.get(), textureMaterial25};
        material37 = {newShader.get(), textureMaterial37};
        newColorMaterial = {newColorShader.get(), textureMaterial37};
        newCityMaterial = {newCityShader.get(), textureBaseColor};
        breakerRoomMaterial = {breakerRoomShader.get(), textureBaseColor};
        axesMaterial = {axesShader.get(), nullptr};

        axesMesh = createAxesMesh(assets, asset::AssetId{"temp", "axes.mesh"});

        // scene
        scene = std::make_unique<scene::Scene>();

        // create objects
        createObjectNew(scene->entities, 0, MeshRendererNew{mesh0, &material25}, true);
        createObjectNew(scene->entities, 1, MeshRendererNew{mesh1, &material25}, true);
        createObjectNew(scene->entities, 2, MeshRendererNew{mesh2, &material37}, true);
        createObjectNew(scene->entities, 3, MeshRendererNew{mesh3, &material37}, true);
        createObjectNew(scene->entities, 4, MeshRendererNew{mesh4, &materialBaseColor}, true);
        createObjectNew(scene->entities, 5, MeshRendererNew{dummyMesh, &newColorMaterial}, false);
        createObjectNew(scene->entities, 6, MeshRendererNew{axesMesh, &axesMaterial}, true);

        std::vector<std::string> meshNames{
            "building_0.mesh",
            "building_1.mesh",
            "building_2.mesh",
            "building_3.mesh",
            "building_4.mesh",
            "building_5.mesh",
            "building_6.mesh",
            "building_7.mesh",
            "building_8.mesh",
            "building_9.mesh",
            "building_10.mesh",
            "building_11.mesh",
            "building_12.mesh",
            "building_13.mesh",
            "building_14.mesh",
            "building_15.mesh",
            "building_16.mesh"
        };

        size_t index = 6;
        for (auto& meshName: meshNames)
        {
            asset::Asset a = assets.get(asset::AssetId{"models/city/city_2.gltf", meshName});
            createObjectNew(scene->entities, index, MeshRendererNew{a, &newCityMaterial});
            index++;
        }

        // editor UI
        ui = std::make_unique<editor::UI>(device, window, shaderLibrary.get());
        ui->setRegistry(&scene->entities);
    }

    void Editor::applicationWillTerminate()
    {
        ui.reset();
    }

    void Editor::render(graphics::Window* _window)
    {
        std::unique_ptr<graphics::RenderPassDescriptor> renderPassDescriptor = _window->getRenderPassDescriptor();

        // todo: move into one render function that takes a scene render function as an argument
        ui->update(*renderPassDescriptor);

        //-------------------------------------------------
        // Update camera transform
        //-------------------------------------------------

        // set camera aspect ratio based on the current size of the window
        graphics::Size size = _window->getContentViewSize();
        camera->parameters().aspectRatio = size.width / size.height;

        // only update camera transform if UI didn't capture keyboard input
        if (!ui->getCapturedKeyboard())
        {
            cameraController->update();
        }

        //-------------------------------------------------
        // Update transforms of all objects (should be refactored into renderer / scene abstraction)
        //-------------------------------------------------

        // updates the transform components based on the hierarchy
        renderer::computeLocalToWorldMatrices(scene->entities);

        //-------------------------------------------------
        // Draw objects with MeshRenderers on the screen (should be refactored into renderer / scene abstraction)
        //-------------------------------------------------

        std::unique_ptr<graphics::ICommandBuffer> cmd = commandQueue->getCommandBuffer();

        cmd->beginRenderPass(*renderPassDescriptor);
        cmd->setWindingOrder(graphics::WindingOrder::Clockwise);
        cmd->setCullMode(graphics::CullMode::None);
        cmd->setTriangleFillMode(graphics::TriangleFillMode::Fill);
        cmd->setDepthStencilState(depthStencilState.get());

        for (auto [entityId, meshRenderer, transform, visible]:
            scene->entities.view<MeshRendererNew, renderer::TransformComponent, renderer::VisibleComponent>(
                entity::IterationPolicy::UseFirstComponent))
        {
            if (!meshRenderer.mesh->success() || !meshRenderer.mesh->valid<renderer::Mesh>())
            {
                continue;
            }

            auto& mesh = meshRenderer.mesh->get<renderer::Mesh>();

            renderer::Material* material = meshRenderer.material;
            cmd->setRenderPipelineState(material->shader->getRenderPipelineState());

            // bind vertex attributes
            for (auto& attribute: mesh)
            {
                cmd->setVertexStageBuffer(mesh.vertexBuffer(), /*offset*/ attribute.offset, /*atIndex*/ attribute.index);
                //common::log::infoDebug("bound {} to index {}", reflection::enumToString(attribute.descriptor->type), attribute.index);
            }

            // bind one after the last vertex attribute
            cmd->setVertexStageBuffer(
                camera->getCameraDataBuffer(),
                0, /*offset*/
                3); /*atIndex*/

            // set small constant data that is different for each object
            math::Matrix4 localToWorldTransform = transform.localToWorldTransform.getTranspose();
            cmd->setVertexStageBytes(
                static_cast<void const*>(&localToWorldTransform),
                sizeof(math::Matrix4), /*length*/
                4); /*atIndex*/

            // check if texture is loaded
            if (material->texture && material->texture->success() && material->texture->valid<graphics::ITexture>())
            {
                cmd->setFragmentStageTexture(&material->texture->get<graphics::ITexture>(), 0);
            }

            if (mesh.descriptor().hasIndexBuffer)
            {
                // draw with index buffer
                cmd->drawIndexedPrimitives(
                    mesh.descriptor().primitiveType,
                    mesh.descriptor().indexCount, /*indexCount*/
                    mesh.indexBuffer(), /*indexBuffer*/
                    0, /*indexBufferOffset*/
                    1, /*instanceCount*/
                    0, /*baseVertex*/
                    0); /*baseInstance*/
            }
            else
            {
                cmd->drawPrimitives(
                    mesh.descriptor().primitiveType,
                    0, // vertex start
                    mesh.descriptor().vertexCount);
            }
        }

        //-------------------------------------------------
        // Draw ImGui user interface
        //-------------------------------------------------

        ui->render(cmd.get());

        cmd->endRenderPass();

        //-------------------------------------------------
        // Submit to window
        //-------------------------------------------------

        std::unique_ptr<graphics::ITexture> drawable = window->getDrawable();
        cmd->present(drawable.get());
        cmd->commit();
    }
}