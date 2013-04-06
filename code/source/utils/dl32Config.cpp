#include "dl32Config.h"

//Global debugging settings:
//--------------------------
	const bool dl32GlobalConfig::debugging = true; ///< cpp_lib32 is in debug-mode.

//dl32Console for debugging settings:
//-----------------------------------
	const bool  dl32GlobalConfig::dl32Console_AutoOpen       = false;                          ///< If its closed, opens debug console if this recieves a debug write instruction.
	const char* dl32GlobalConfig::dl32Console_AutoOpen_Title = "cpp_lib32 (Debug) [autoopen]"; ///< Console title for autoOpen.

//dl32Window debugging settings:
//------------------------------
	const bool dl32GlobalConfig::dl32Window_PromptUnhandledMessages  = false;  ///< Prints unhandled window messages (Messages that are not part of the dl32Window events set) in debug console.
	const bool dl32GlobalConfig::dl32Window_PromptCallbackedMessages = false;  ///< Prints windows messages that are processed by "WindowProcedure()" callback instead of "dl32Window::ProcessMessage()" (Standard messaging loop) in debug console.

//dl32Timing configuration:
//-------------------------
	const bool dl32GlobalConfig::dl32Timing_APITiming_Enabled = false;        ///< Internal timing (Few API functions that have timing instructions) enabled.
	const bool dl32GlobalConfig::dl32Timing_PromptPush        = false;        ///< Prints dl32Timing::push() calls in debug console.
	const bool dl32GlobalConfig::dl32Timing_Prompt_Pop        = false;        ///< Prints dl32Timing::pop() calls in debug console.

//dl32Math configuration:
//-----------------------
	const float dl32GlobalConfig::float_epsilon = 0.0001f;                    ///< cpp_lib32 flotating-point comparations epsilon.

//dl32Graphics configuration:
//---------------------------
	const bool dl32GlobalConfig::dl32Graphics_defaults_windowed            = true;	 ///< Default value for Windowed param of dl32Graphics ctors.
	const bool dl32GlobalConfig::dl32Graphics_defaults_tripleBuffer        = false;	 ///< Default value for tripleBuffer param of dl32Graphics ctors.
	const bool dl32GlobalConfig::dl32Graphics_defaults_vSync               = false;	 ///< Default value for vSync param of dl32Graphics ctors.
	const unsigned int dl32GlobalConfig::dl32Graphics_defaults_refreshRate = 60;     ///< Default value for refreshRate param of dl32Graphics ctors.

	const bool dl32GlobalConfig::dl32Graphics_Performance_DrawCallsMerging = true;   ///< Graphics engine tries to merge consecutives draw calls in unique GPU draw call.

	const float dl32GlobalConfig::dl32Graphics_d3dVertex_Z                 = 0.5f;   ///< Default z value of _d3dVertex instances
	const float dl32GlobalConfig::dl32Graphics_d3dVertex_RHW               = 1.0f;   ///< RHW value of _d3dVertex instances