#pragma once


#include <Component.h>
#include <Singleton.h>


namespace pd
{
	struct InputArgs : public ComponentArgs
	{

	};

	class Input : public SingletonComponent, public Singleton<Input>
	{
	friend class Singleton<Input>;
	friend class EntityManager;

	private:
		Input() = default;
		~Input() = default;

	private:
		void init(ObjectArgs* pArgs) override {}

    public:
        //// Input-related functions: gamepads
        //bool IsGamepadAvailable(int gamepad);                   // Check if a gamepad is available
        //const char* GetGamepadName(int gamepad);                // Get gamepad internal name id
        //bool IsGamepadButtonPressed(int gamepad, int button);   // Check if a gamepad button has been pressed once
        //bool IsGamepadButtonDown(int gamepad, int button);      // Check if a gamepad button is being pressed
        //bool IsGamepadButtonReleased(int gamepad, int button);  // Check if a gamepad button has been released once
        //bool IsGamepadButtonUp(int gamepad, int button);        // Check if a gamepad button is NOT being pressed
        //int GetGamepadButtonPressed(void);                      // Get the last gamepad button pressed
        //int GetGamepadAxisCount(int gamepad);                   // Get gamepad axis count for a gamepad
        //float GetGamepadAxisMovement(int gamepad, int axis);    // Get axis movement value for a gamepad axis
        //int SetGamepadMappings(const char* mappings);           // Set internal gamepad mappings (SDL_GameControllerDB)

        bool isGamepadAvailable(s32 gamepadId) { IsGamepadAvailable(gamepadId); }                                               // Check if a gamepad is available
        std::string_view gamepadName(s32 gamepadId) { return GetGamepadName(gamepadId); }                                       // Get gamepad internal name id
        bool controllerButtonPressed(s32 gamepadId, s32 button) const { return IsGamepadButtonPressed(gamepadId, button); }     // Get gamepad internal name id
        bool controllerButtonReleased(s32 gamepadId, s32 button) const { return IsGamepadButtonReleased(gamepadId, button); }   // Get gamepad internal name id
        bool controllerButtonDown(s32 gamepadId, s32 button) const { return IsGamepadButtonDown(gamepadId, button); }           // Get gamepad internal name id
        bool controllerButtonUp(s32 gamepadId, s32 button) const { return IsGamepadButtonUp(gamepadId, button); }               // Get gamepad internal name id
        f32 controllerAxis(s32 gamepadId, s32 axis) const { GetGamepadAxisMovement(gamepadId, axis); }                          // Get axis movement value for a gamepad axis


        //// Input-related functions: mouse
        //bool IsMouseButtonPressed(int button);                  // Check if a mouse button has been pressed once
        //bool IsMouseButtonDown(int button);                     // Check if a mouse button is being pressed
        //bool IsMouseButtonReleased(int button);                 // Check if a mouse button has been released once
        //bool IsMouseButtonUp(int button);                       // Check if a mouse button is NOT being pressed
        //int GetMouseX(void);                                    // Get mouse position X
        //int GetMouseY(void);                                    // Get mouse position Y
        //Vector2 GetMousePosition(void);                         // Get mouse position XY
        //Vector2 GetMouseDelta(void);                            // Get mouse delta between frames
        //void SetMousePosition(int x, int y);                    // Set mouse position XY
        //void SetMouseOffset(int offsetX, int offsetY);          // Set mouse offset
        //void SetMouseScale(float scaleX, float scaleY);         // Set mouse scaling
        //float GetMouseWheelMove(void);                          // Get mouse wheel movement for X or Y, whichever is larger
        //Vector2 GetMouseWheelMoveV(void);                       // Get mouse wheel movement for both X and Y
        //void SetMouseCursor(int cursor);                        // Set mouse cursor

        bool mouseButtonPressed(s32 mouseButton) const { return IsMouseButtonPressed(mouseButton); }    // Check if a mouse button has been pressed once
        bool mouseButtonReleased(s32 mouseButton) const { return IsMouseButtonReleased(mouseButton); }  // Check if a mouse button has been released once
        bool mouseButtonDown(s32 mouseButton) const { return IsMouseButtonDown(mouseButton); }          // Check if a mouse button is being pressed
        bool mouseButtonUp(s32 mouseButton) const { return IsMouseButtonUp(mouseButton); }              // Check if a mouse button is NOT being pressed
        s32 mouseX() const { return GetMouseX(); }                                                      // Get mouse position X
        s32 mouseY() const { return GetMouseY(); }                                                      // Get mouse position Y
        Vector2 mousePosition() const { return GetMousePosition(); }                                    // Get mouse position XY
        Vector2 mouseDelta() const { return GetMouseDelta(); }                                          // Get mouse delta between frames
        Vector2 mouseWheel() const { return GetMouseWheelMoveV(); }                                     // Get mouse wheel movement for both X and Y
        void setMousePosition(s32 x, s32 y) { setMousePosition(x, y); }                                 // Set mouse position XY


        // Cursor-related functions
        void showCursor() { ShowCursor(); }              // Shows cursor
        void hideCursor() { HideCursor(); }              // Hides cursor
        bool isCursorHidden() { IsCursorHidden(); }      // Check if cursor is not visible
        void enableCursor() { EnableCursor(); }          // Enables cursor (unlock cursor)
        void disableCursor() { DisableCursor(); }        // Disables cursor (lock cursor)
        bool isCursorOnScreen() { IsCursorOnScreen(); }  // Check if cursor is on the screen


        //// Input-related functions: keyboard
        //bool IsKeyPressed(int key);                             // Check if a key has been pressed once
        //bool IsKeyDown(int key);                                // Check if a key is being pressed
        //bool IsKeyReleased(int key);                            // Check if a key has been released once
        //bool IsKeyUp(int key);                                  // Check if a key is NOT being pressed
        //void SetExitKey(int key);                               // Set a custom key to exit program (default is ESC)
        //int GetKeyPressed(void);                                // Get key pressed (keycode), call it multiple times for keys queued, returns 0 when the queue is empty
        //int GetCharPressed(void);                               // Get char pressed (unicode), call it multiple times for chars queued, returns 0 when the queue is empty

        bool keyPressed(s32 keyCode) const { return IsKeyPressed(keyCode); }    // Check if a key has been pressed once
        bool keyReleased(s32 keyCode) const { return IsKeyReleased(keyCode); }  // Check if a key has been released once
        bool keyDown(s32 keyCode) const { return IsKeyDown(keyCode); }          // Check if a key is being pressed
        bool keyUp(s32 keyCode) const { return IsKeyUp(keyCode); }              // Check if a key is NOT being pressed
        void setExitKey(s32 keyCode) { return SetExitKey(keyCode); }            // Set a custom key to exit program (default is ESC)
	};
}
