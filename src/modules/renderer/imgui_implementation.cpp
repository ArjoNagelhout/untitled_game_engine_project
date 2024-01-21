//
// Created by Arjo Nagelhout on 20/01/2024.
//

#include "imgui_implementation.h"

#include "graphics/render_pipeline_state.h"
#include "graphics/texture.h"
#include "graphics/render_pass.h"

namespace renderer
{
    struct FramebufferDescriptor
    {
        unsigned long sampleCount;
        graphics::PixelFormat colorPixelFormat;
        graphics::PixelFormat depthPixelFormat;
        graphics::PixelFormat stencilPixelFormat;

        // render pass descriptor should have valid first color attachment, depth attachment and stencil attachment
        explicit FramebufferDescriptor(graphics::RenderPassDescriptor const& renderPassDescriptor)
        {
            assert(!renderPassDescriptor.colorAttachments.empty() && renderPassDescriptor.colorAttachments[0].pTexture);
            assert(renderPassDescriptor.depthAttachment.pTexture);
            assert(renderPassDescriptor.stencilAttachment.pTexture);

            sampleCount = renderPassDescriptor.colorAttachments[0].pTexture->getSampleCount();
            colorPixelFormat = renderPassDescriptor.colorAttachments[0].pTexture->getPixelFormat();
            depthPixelFormat = renderPassDescriptor.depthAttachment.pTexture->getPixelFormat();
            stencilPixelFormat = renderPassDescriptor.stencilAttachment.pTexture->getPixelFormat();
        }
    };

    [[nodiscard]] bool operator==(FramebufferDescriptor const& lhs, FramebufferDescriptor const& rhs)
    {
        return (lhs.sampleCount == rhs.sampleCount &&
                lhs.colorPixelFormat == rhs.colorPixelFormat &&
                lhs.depthPixelFormat == rhs.depthPixelFormat &&
                lhs.stencilPixelFormat == rhs.stencilPixelFormat
        );
    }

    struct ImGui_ImplShapeReality_Data
    {
        graphics::IDevice* pDevice;
        std::unique_ptr<graphics::IDepthStencilState> pDepthStencilState;
        std::unique_ptr<graphics::ITexture> pFontTexture;
        std::unique_ptr<FramebufferDescriptor> pFramebufferDescriptor;
//        std::unordered_map<int, std::unique_ptr<graphics::RenderPipelineState>> renderPipelineStateCache;

        ImGui_ImplShapeReality_Data() {}
    };

    static ImGui_ImplShapeReality_Data* ImGui_ImplShapeReality_GetBackendData()
    {
        if (ImGui::GetCurrentContext())
        {
            return (ImGui_ImplShapeReality_Data*)ImGui::GetIO().BackendRendererUserData;
        }
        else
        {
            return nullptr;
        }
    }

    bool ImGui_ImplShapeReality_Init(graphics::IDevice* pDevice)
    {
        auto* bd = new ImGui_ImplShapeReality_Data();
        ImGuiIO& io = ImGui::GetIO();
        io.BackendRendererUserData = (void*)bd;
        io.BackendRendererName = "imgui_impl_shapereality";
        //io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

        bd->pDevice = pDevice;

        return true;
    }

    void ImGui_ImplShapeReality_Shutdown()
    {
        ImGui_ImplShapeReality_Data* bd = ImGui_ImplShapeReality_GetBackendData();
        IM_ASSERT(bd != nullptr && "No renderer backend to shutdown, or already shut down?");
        ImGui_ImplShapeReality_DestroyDeviceObjects();
        delete bd;

        ImGuiIO& io = ImGui::GetIO();
        io.BackendRendererName = nullptr;
        io.BackendRendererUserData = nullptr;
//        io.BackendFlags &= ~ImGuiBackendFlags_RendererHasVtxOffset;
    }

    void ImGui_ImplShapeReality_NewFrame(graphics::RenderPassDescriptor const& renderPassDescriptor)
    {
        ImGui_ImplShapeReality_Data* bd = ImGui_ImplShapeReality_GetBackendData();
        IM_ASSERT(bd != nullptr && "No ShapeReality context. Did you call ImGui_ImplShapeReality_Init() ?");

        if (!bd->pDepthStencilState)
        {
            ImGui_ImplShapeReality_CreateDeviceObjects(bd->pDevice);
        }
    }

    static void ImGui_ImplShapeReality_SetupRenderState(ImDrawData* drawData,
                                                        graphics::ICommandBuffer* pCommandBuffer,
                                                        graphics::IRenderPipelineState* pRenderPipelineState,
                                                        graphics::IBuffer* pVertexBuffer,
                                                        size_t vertexBufferOffset)
    {
        ImGui_ImplShapeReality_Data* bd = ImGui_ImplShapeReality_GetBackendData();
        pCommandBuffer->setCullMode(graphics::CullMode::None);
        pCommandBuffer->setDepthStencilState(bd->pDepthStencilState.get());

        // Setup viewport, orthographic projection matrix
        // Our visible imgui space lies from draw_data->DisplayPos (top left) to
        // draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayMin is typically (0,0) for single viewport apps.
        graphics::Viewport viewport = {
            .originX = 0.0,
            .originY = 0.0,
            .width = drawData->DisplaySize.x * drawData->FramebufferScale.x,
            .height = drawData->DisplaySize.y * drawData->FramebufferScale.y,
            .zNear = 0.0,
            .zFar = 1.0
        };
        pCommandBuffer->setViewport(viewport);

        float L = drawData->DisplayPos.x;
        float R = drawData->DisplayPos.x + drawData->DisplaySize.x;
        float T = drawData->DisplayPos.y;
        float B = drawData->DisplayPos.y + drawData->DisplaySize.y;
        float N = viewport.zNear;
        float F = viewport.zFar;
        const float ortho_projection[4][4] = {
            {2.0f / (R - L),    0.0f,              0.0f,        0.0f},
            {0.0f,              2.0f / (T - B),    0.0f,        0.0f},
            {0.0f,              0.0f,              1 / (F - N), 0.0f},
            {(R + L) / (L - R), (T + B) / (B - T), N / (F - N), 1.0f},
        };

    }

    void ImGui_ImplShapeReality_RenderDrawData(ImDrawData* drawData,
                                               graphics::ICommandBuffer* pCommandBuffer)
    {
        ImGui_ImplShapeReality_Data* bd = ImGui_ImplShapeReality_GetBackendData();

        // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
        int fb_width = (int)(drawData->DisplaySize.x * drawData->FramebufferScale.x);
        int fb_height = (int)(drawData->DisplaySize.y * drawData->FramebufferScale.y);
        if (fb_width <= 0 || fb_height <= 0 || drawData->CmdListsCount == 0)
        {
            return;
        }
    }

    bool ImGui_ImplShapeReality_CreateFontsTexture(graphics::IDevice* pDevice)
    {
        return false;
    }

    void ImGui_ImplShapeReality_DestroyFontsTexture()
    {

    }

    bool ImGui_ImplShapeReality_CreateDeviceObjects(graphics::IDevice* pDevice)
    {
        return false;
    }

    void ImGui_ImplShapeReality_DestroyDeviceObjects()
    {

    }
}