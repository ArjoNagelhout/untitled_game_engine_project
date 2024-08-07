//
// Created by Arjo Nagelhout on 21/12/2023.
//

#ifndef SHAPEREALITY_INPUT_H
#define SHAPEREALITY_INPUT_H

#include <string>
#include <sstream>

namespace graphics
{
    enum class MouseButton : int
    {
        None = 0,
        Left,
        Right,
        Middle
    };
    constexpr static const char* mouseButtonStrings[]{
        "None",
        "Left",
        "Right",
        "Middle"
    };

    constexpr static const char* string(MouseButton value)
    {
        return mouseButtonStrings[static_cast<int>(value)];
    }

    enum class MouseEventType : int
    {
        Up = 0,
        Down,
        Moved,
        Entered,
        Exited,
        Dragged, // when down and over drag threshold
    };
    constexpr static const char* mouseEventTypeStrings[]{
        "Up",
        "Down",
        "Moved",
        "Entered",
        "Exited",
        "Dragged"
    };

    constexpr static const char* string(MouseEventType value)
    {
        return mouseEventTypeStrings[static_cast<int>(value)];
    }

    struct MouseEvent final
    {
        MouseEventType type;
        MouseButton mouseButton; // which mouse button is pressed
        float x;
        float y;
    };

    struct ScrollEvent final
    {
        float x;
        float y;
    };

    enum class KeyCode : int
    {
        None = 0,
        Space,
        Enter,
        Tab,
        Backtick,
        Quote,
        Semicolon,
        Comma,
        Period,
        Slash,
        Backslash,
        LeftBracket,
        RightBracket,
        Minus,
        Equals,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        Digit1,
        Digit2,
        Digit3,
        Digit4,
        Digit5,
        Digit6,
        Digit7,
        Digit8,
        Digit9,
        Digit0,
        LeftShift,
        LeftControl,
        LeftOption,
        LeftMeta,
        RightShift,
        RightControl,
        RightOption,
        RightMeta,
        ContextMenu,
        Escape,
        LeftArrow,
        RightArrow,
        UpArrow,
        DownArrow,
        Backspace,
        PageDown,
        PageUp,
        Home,
        End,
        Insert,
        Delete,
        CapsLock,
        NumLock,
        PrintScreen,
        ScrollLock,
        Pause,
        NumpadEnter,
        NumpadDivide,
        NumpadMultiply,
        NumpadPlus,
        NumpadMinus,
        NumpadPeriod,
        NumpadEquals,
        Numpad0,
        Numpad1,
        Numpad2,
        Numpad3,
        Numpad4,
        Numpad5,
        Numpad6,
        Numpad7,
        Numpad8,
        Numpad9,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        OEM1,
        OEM2,
        OEM3,
        OEM4,
        OEM5,

        Last = OEM5
    };
    constexpr static const char* keyStrings[]{
        "None",
        "Space",
        "Enter",
        "Tab",
        "Backtick",
        "Quote",
        "Semicolon",
        "Comma",
        "Period",
        "Slash",
        "Backslash",
        "LeftBracket",
        "RightBracket",
        "Minus",
        "Equals",
        "A",
        "B",
        "C",
        "D",
        "E",
        "F",
        "G",
        "H",
        "I",
        "J",
        "K",
        "L",
        "M",
        "N",
        "O",
        "P",
        "Q",
        "R",
        "S",
        "T",
        "U",
        "V",
        "W",
        "X",
        "Y",
        "Z",
        "Digit1",
        "Digit2",
        "Digit3",
        "Digit4",
        "Digit5",
        "Digit6",
        "Digit7",
        "Digit8",
        "Digit9",
        "Digit0",
        "LeftShift",
        "LeftControl",
        "LeftOption",
        "LeftMeta",
        "RightShift",
        "RightControl",
        "RightOption",
        "RightMeta",
        "ContextMenu",
        "Escape",
        "LeftArrow",
        "RightArrow",
        "UpArrow",
        "DownArrow",
        "Backspace",
        "PageDown",
        "PageUp",
        "Home",
        "End",
        "Insert",
        "Delete",
        "CapsLock",
        "NumLock",
        "PrintScreen",
        "ScrollLock",
        "Pause",
        "NumpadEnter",
        "NumpadDivide",
        "NumpadMultiply",
        "NumpadPlus",
        "NumpadMinus",
        "NumpadPeriod",
        "NumpadEquals",
        "Numpad0",
        "Numpad1",
        "Numpad2",
        "Numpad3",
        "Numpad4",
        "Numpad5",
        "Numpad6",
        "Numpad7",
        "Numpad8",
        "Numpad9",
        "F1",
        "F2",
        "F3",
        "F4",
        "F5",
        "F6",
        "F7",
        "F8",
        "F9",
        "F10",
        "F11",
        "F12",
        "F13",
        "F14",
        "F15",
        "F16",
        "F17",
        "F18",
        "F19",
        "F20",
        "OEM1",
        "OEM2",
        "OEM3",
        "OEM4",
        "OEM5",
        "IMESelected"
    };

    constexpr static const char* string(KeyCode value)
    {
        return keyStrings[static_cast<int>(value)];
    }

    enum class KeyboardEventType
    {
        Up,
        Down,
        Repeat,
        ModifiersChanged
    };
    constexpr static char const* keyboardEventTypeStrings[]{
        "Up",
        "Down",
        "Repeat",
        "ModifiersChanged"
    };

    constexpr static char const* string(KeyboardEventType value)
    {
        return keyboardEventTypeStrings[static_cast<int>(value)];
    }

    enum KeyboardModifier_ : int
    {
        KeyboardModifier_None = 0,
        KeyboardModifier_CapsLock = 1 << 0,
        KeyboardModifier_Shift = 1 << 1,
        KeyboardModifier_Control = 1 << 2,
        KeyboardModifier_Option = 1 << 3,
        KeyboardModifier_Meta = 1 << 4, // on macOS: Command key, windows: Windows key
        KeyboardModifier_Last = KeyboardModifier_Meta
    };
    constexpr static char const* keyboardModifierStrings[]{
        "None",
        "CapsLock",
        "Shift",
        "Control",
        "Option",
        "Meta"
    };

    static std::string string(KeyboardModifier_ value)
    {
        std::stringstream s{};
        int j = 1;
        for (int i = 1; i <= KeyboardModifier_Last; i <<= 1)
        {
            if ((value & i) != 0)
            {
                s << keyboardModifierStrings[j];
            }
            if (i < KeyboardModifier_Last)
            {
                s << ", ";
            }
            j++;
        }
        return s.str();
    }

    // see https://wiki.libsdl.org/SDL2/Tutorials-TextInput
    struct TextInputEvent
    {
        std::string text;
    };

    // the point of a text editing event is that when the user
    // is using IME (Input Method Editor), the user can type
    // three characters and then select the right character.
    //
    // This is used for typing Japanese, Chinese etc. which have
    // more characters than what would fit on a keyboard.
    //
    // after the editing is completed, a TextInputEvent is created
    // with the resulting utf-8 unicode character.
    struct TextEditingEvent
    {
        // in UTF-8, by convention this is displayed with a line underneath
        std::string composition;

        // the location to begin editing from
        unsigned int start;

        // the number of characters to edit from the start point
        unsigned int length;
    };

    struct KeyboardEvent final
    {
        KeyboardEventType type;

        // non-localized keycode.
        // prefer this for game input and keyboard shortcuts
        KeyCode keyCode;

        // modifier-keys
        KeyboardModifier_ modifiers;
    };

    enum class InputEventType
    {
        None,
        Mouse,
        Scroll,
        Keyboard,
        TextInput,
        TextEditing
    };
    constexpr static const char* inputEventTypeStrings[]{
        "None",
        "Mouse",
        "Scroll",
        "Keyboard",
        "TextInput",
        "TextEditing"
    };

    constexpr static const char* string(InputEventType value)
    {
        return inputEventTypeStrings[static_cast<int>(value)];
    }

    class InputEvent
    {
    public:
        explicit InputEvent(MouseEvent _mouse);

        explicit InputEvent(ScrollEvent _scroll);

        explicit InputEvent(KeyboardEvent _keyboard);

        explicit InputEvent(TextInputEvent _textInput);

        explicit InputEvent(TextEditingEvent _textEditing);

        ~InputEvent();

        // explicitly delete copy constructor

        InputEvent(InputEvent const&) = delete;

        InputEvent& operator=(InputEvent const&) = delete;

        // get data
        [[nodiscard]] InputEventType getType() const;

        [[nodiscard]] MouseEvent getMouse() const;

        [[nodiscard]] ScrollEvent getScroll() const;

        [[nodiscard]] KeyboardEvent getKeyboard() const;

        [[nodiscard]] TextInputEvent getTextInput() const;

        [[nodiscard]] TextEditingEvent getTextEditing() const;

        [[nodiscard]] std::string string() const;

    private:
        InputEventType type;

        union
        {
            MouseEvent mouse;
            ScrollEvent scroll;
            KeyboardEvent keyboard;
            TextInputEvent textInput;
            TextEditingEvent textEditing;
        };
    };
}

#endif //SHAPEREALITY_INPUT_H
