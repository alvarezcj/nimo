#pragma once

#include "KeyCodes.h"
#include <utility>

namespace nimo {
	class Input
	{
	public:
        //Mouse
		static bool GetMouseButton(MouseButton button);
		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePosition();
        //Key
		static bool GetKey(KeyCode keycode);
        // Cursor mode
		static void SetCursorMode(CursorMode mode);
		static CursorMode GetCursorMode();
	};

}