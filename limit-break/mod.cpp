#include "stdafx.h"

static short last_level = 0;
static short last_act = 0;

static const Uint32 SPRITE_COUNT = 4096;
static Uint8 table[80 * SPRITE_COUNT];

static void __cdecl InitSpriteTable_r(void*, Uint32)
{
	InitSpriteTable((QueuedModelParticle*)table, SPRITE_COUNT);

#ifdef _DEBUG
	Display_SPR_TASK = 1;
#endif
}

extern "C"
{
	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };

	__declspec(dllexport) void __cdecl Init()
	{
		WriteCall((void*)0x00415A60, InitSpriteTable_r);

		object_init();
		collision_init();
		clip_init();
		textures_init();
		set_init();
	}

	__declspec(dllexport) void __cdecl OnFrame()
	{
		const auto pad = ControllerPointers[0];

		// TODO: fix; this pulls the draw distance from the *last* stage, if any
		if (last_level != CurrentLevel || last_act != CurrentAct
			|| (pad && pad->PressedButtons & Buttons_C)
			|| abs(clip_current) < FLT_EPSILON)
		{
			last_level = CurrentLevel;
			last_act = CurrentAct;
			clip_reset();
		}

		DisplayDebugStringFormatted(NJM_LOCATION(1, 5), "CLIP: %f", clip_current);

		object_OnFrame();
		textures_OnFrame();
	}
}
