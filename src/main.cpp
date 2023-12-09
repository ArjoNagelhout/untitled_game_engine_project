#include "application.h"

#include "graphics/device.h"
#include "graphics/command_queue.h"
#include "graphics/render_pass.h"

#include <iostream>

// high level implementation of what the app should be doing
class App final : public engine::IApplicationDelegate, public graphics::IWindowDelegate
{
public:
	explicit App() = default;

	~App() = default;

	void applicationDidFinishLaunching() override
	{
		graphics::CommandQueueDescriptor commandQueueDescription{};
		pCommandQueue = pDevice->createCommandQueue(commandQueueDescription);

		graphics::RenderPassDescriptor renderPassDescription{};
		pRenderPass = pDevice->createRenderPass(renderPassDescription);
	}

	void render(graphics::IWindow* window) override
	{
		// get command buffer from pCommandQueue
		std::unique_ptr<graphics::ICommandBuffer> cmd = pCommandQueue->getCommandBuffer();

		std::cout << "rendererer" << std::endl;

//		cmd->beginRenderPass();

//		cmd->endRenderPass();
	}

	// todo: find a better way to make the device accessible
	void setDevice(graphics::IDevice* device)
	{
		pDevice = device;
	}

private:
	graphics::IDevice* pDevice{nullptr};
	std::unique_ptr<graphics::ICommandQueue> pCommandQueue;
	std::unique_ptr<graphics::IRenderPass> pRenderPass;
};

int main(int argc, char* argv[] )
{
	// create application, should be done first
	engine::Application application{};

	App app{};
	application.setDelegate(&app);

	graphics::GraphicsBackend backend = graphics::GraphicsBackend::Metal;
	std::unique_ptr<graphics::IDevice> device = graphics::createDevice(backend);
	app.setDevice(device.get());

	graphics::WindowDescriptor desc{
		.x = 500,
		.y = 500,
		.width = 500,
		.height = 500,
		.flags = graphics::WindowFlags_Default,
		.clearColor = math::vec4{{0.5f, 1.f, 1.f, 1.f}}
	};
	std::unique_ptr<graphics::IWindow> window = device->createWindow(desc);
	window->setTitle("bored engine");
	window->setMinSize(300, 100);
	window->setDelegate(&app);

	// run application
	application.run();

	return 0;
}