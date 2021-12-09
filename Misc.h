#pragma once
#include "SDK.h"

class CMisc
{
public:

	void Run(CBaseEntity* pLocal, CUserCmd* pCommand);

private:

	//Other funcs

	void NoisemakerSpam(PVOID kv);
	void AntiAim();
	void AntiAim2();
};

extern CMisc gMisc;