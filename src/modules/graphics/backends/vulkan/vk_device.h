//
// Created by Arjo Nagelhout on 12/12/2023.
//

#ifndef SHAPEREALITY_VK_DEVICE_H
#define SHAPEREALITY_VK_DEVICE_H

#include "graphics/device.h"

namespace graphics
{
    class VulkanDevice final : public IDevice
    {
    public:
        explicit VulkanDevice();

        ~VulkanDevice() override;

        [[nodiscard]] std::unique_ptr<Window>
        createWindow(WindowDescriptor const& descriptor) const override;

        [[nodiscard]] std::unique_ptr<ICommandQueue>
        createCommandQueue(CommandQueueDescriptor const& descriptor) const override;

        [[nodiscard]] std::unique_ptr<IRenderPass>
        createRenderPass(RenderPassDescriptor const& descriptor) const override;

        [[nodiscard]] std::unique_ptr<IRenderPipelineState>
        createRenderPipelineState(RenderPipelineDescriptor const& descriptor) const override;

        [[nodiscard]] std::unique_ptr<IDepthStencilState>
        createDepthStencilState(DepthStencilDescriptor const& descriptor) const override;

        [[nodiscard]] std::unique_ptr<IShaderLibrary>
        createShaderLibrary(std::filesystem::path const& path) const override;

        [[nodiscard]] std::unique_ptr<IBuffer>
        createBuffer(BufferDescriptor const& descriptor) const override;

        [[nodiscard]] std::unique_ptr<ITexture>
        createTexture(TextureDescriptor const& descriptor) const override;
    };
}

#endif //SHAPEREALITY_VK_DEVICE_H
