#ifndef SHAPEREALITY_WINDOW_H
#define SHAPEREALITY_WINDOW_H

#include <memory>

#include <math/vector.h>

#include <graphics/input.h>

namespace graphics
{
    enum WindowFlags_ : uint32_t
    {
        WindowFlags_None = 0,
        WindowFlags_Borderless = 1 << 0,
        WindowFlags_Titled = 1 << 1,
        WindowFlags_Closable = 1 << 2,
        WindowFlags_Miniaturizable = 1 << 3,
        WindowFlags_Resizable = 1 << 4,
        WindowFlags_UnifiedTitleAndToolbar = 1 << 5,

        WindowFlags_Default =
        WindowFlags_Titled | WindowFlags_Closable | WindowFlags_Miniaturizable | WindowFlags_Resizable
    };

    class Window;

    class IWindowRenderDelegate
    {
    public:
        virtual void render(Window* window) = 0;
    };

    class IWindowInputDelegate
    {
    public:
        virtual void onEvent(InputEvent const& event, Window* window) = 0;
    };

    struct WindowDescriptor
    {
        int x;
        int y;
        int width;
        int height;
        int flags;
        math::Vector4 clearColor;
    };

    // simple alternative to Vector<2> without any methods
    struct Size final
    {
        float width;
        float height;
    };

    // simple alternative to math::Rect without any methods
    struct Rect final
    {
        float x;
        float y;
        float width;
        float height;
    };

    // simple alternative to Vector<2> without any methods
    struct Position final
    {
        float x;
        float y;
    };

    struct RenderPassDescriptor;

    class ITexture;

    class Window final
    {
    public:
        explicit Window();

        ~Window();

        // get a render pass descriptor that can be used to draw to this window
        [[nodiscard]] std::unique_ptr<RenderPassDescriptor> getRenderPassDescriptor() const;

        [[nodiscard]] std::unique_ptr<ITexture> getDrawable() const;

        // set the title of this window
        void setTitle(std::string const& title);

        // show this window (on macOS: deminiaturize)
        void show();

        // hide this window (on macOS: minimize)
        void hide();

        // move and scale this window so that it fills the entire screen
        // note: this is not the same as `fullscreen()`
        void maximize();

        // move the window to the front
        void makeKeyAndOrderFront();

        // make this window fullscreen
        void makeFullscreen();

        //
        void setPosition(Position position);

        // set the size of this window (including the top bar)
        void setWindowSize(Size size);

        // set the minimum horizontal and vertical dimensions of this window when resizing
        void setMinSize(Size size);

        // set the maximum horizontal and vertical dimensions of this window when resizing
        void setMaxSize(Size size);

        // get the position and size of this window (including the top bar)
        [[nodiscard]] Rect getWindowRect() const;

        // set both position and size (including the top bar)
        void setWindowRect(Rect rect);

        // get the position and size of
        [[nodiscard]] Size getContentViewSize() const;

        // get the scale factor of the window (1.0 on non-retina displays, 2.0 on retina displays)
        [[nodiscard]] float getScaleFactor() const;

        // set the input rect (for IME)
        void setTextInputRect(graphics::Rect rect);

        // enables text input at the provided rect
        void enableTextInput();

        // disables text input
        void disableTextInput();

        // get the render delegate of this window
        [[nodiscard]] IWindowRenderDelegate* getRenderDelegate() const;

        // set the render delegate of this window
        void setRenderDelegate(IWindowRenderDelegate* renderDelegate);

        // get the input delegate of this window
        [[nodiscard]] IWindowInputDelegate* getInputDelegate() const;

        // set the input delegate of this window
        void setInputDelegate(IWindowInputDelegate* inputDelegate);

        // platform specific implementation that can be accessed by a graphics backend subclass of IWindow
        // to automatically create a surface / context that can be drawn to.
        class Implementation;

        [[nodiscard]] Implementation* getImplementation() const;

    protected:
        std::unique_ptr<Implementation> implementation;

        // the delegate is responsible for rendering this window's contents
        IWindowRenderDelegate* renderDelegate{nullptr};

        IWindowInputDelegate* inputDelegate{nullptr};

    public:
        // properties that should only be set by the implementation
        // but as we don't know the classes of the implementation yet, we would
        // have to expose setters, overcomplicating everything.
        bool isKeyWindow{false};
    };
}

#endif //SHAPEREALITY_WINDOW_H
