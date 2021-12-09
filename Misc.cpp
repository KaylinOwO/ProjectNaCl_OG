#include "Misc.h"
#include "Util.h"
#include <chrono>
#include <sstream>

#include "Radar.h"
CMisc gMisc;

std::string repeat(int n, const char* str)
{
	std::ostringstream os;
	for (int i = 0; i < n; i++)
		os << str;
	return os.str();
}


void CMisc::Run(CBaseEntity* pLocal, CUserCmd* pCommand)
{
	if (pLocal->GetLifeState() != LIFE_ALIVE)
		return;

	auto wepid = pLocal->GetActiveWeapon()->GetItemDefinitionIndex();

	if (gCvars.triggerbot_autobackstab)
	{
		CBaseCombatWeapon* pWep = pLocal->GetActiveWeapon();
		if (pLocal->szGetClass() == "Spy" && Util->IsOtherSlot(pLocal, pLocal->GetActiveWeapon()) && pWep->CanBackStab())
			pCommand->buttons |= IN_ATTACK;
	}

	if (!gCvars.misc_enabled)
	   return;
	
	if (gCvars.misc_tauntslide) //This is for some reason broken, I have an idea why just haven't got to trying to fix it.
	{
		if ((pLocal->GetCond() & tf_cond::TFCond_Taunting))
		{

			Vector vLocal = pLocal->GetEyePosition();
			Vector vAngs;

			float forward = 0;
			float side = 0;

			if (pCommand->buttons |= IN_FORWARD)
				pCommand->forwardmove += 450;
			if (pCommand->buttons |= IN_BACK)
				pCommand->forwardmove -= 450;
			if (pCommand->buttons |= IN_MOVELEFT)
				pCommand->sidemove -= 450;
			if (pCommand->buttons |= IN_MOVERIGHT)
				pCommand->sidemove += 450;
				
			static Vector camera_angle;
			gInts.Engine->GetViewAngles(camera_angle);
				
			if (!(gCvars.hvh_aaswitch &&
				gCvars.hvh_aay &&
				!(side || forward)))
				pCommand->viewangles.y = camera_angle[1];
		}
	}

	if (gCvars.misc_speedcrouch && !(pCommand->buttons & IN_ATTACK) && (pCommand->buttons & IN_DUCK)) // who changed my comment >:(
	{
		Vector vLocalAngles = pCommand->viewangles;
		float speed = pCommand->forwardmove;
		if (fabs(speed) > 0.0f) {
			pCommand->forwardmove = -speed;
			pCommand->sidemove = 0.0f;
			pCommand->viewangles.y = vLocalAngles.y;
			pCommand->viewangles.y -= 180.0f;
			if (pCommand->viewangles.y < -180.0f) pCommand->viewangles.y += 360.0f;
			pCommand->viewangles.z = 90.0f;
		}
	}

	if (!(pLocal->GetFlags() & FL_ONGROUND) && pCommand->buttons & IN_JUMP)
	{
		//Autostrafe	
		if (gCvars.misc_autostrafe)
			if (pCommand->mousedx > 1 || pCommand->mousedx < -1)  //> 1 < -1 so we have some wiggle room
				pCommand->sidemove = pCommand->mousedx > 1 ? 450.f : -450.f;

		//Bunnyhop
		if (gCvars.misc_bunnyhop)
			pCommand->buttons &= ~IN_JUMP;
	}

	if (gCvars.misc_noisemaker_spam)
	{
		PVOID kv = Util->InitKeyValue();
		if (kv != NULL)
		{
			NoisemakerSpam(kv);
			gInts.Engine->ServerCmdKeyValues(kv);
		}
	}

	if (gCvars.misc_nopush)
	{
		ConVar* tf_avoidteammates_pushaway = gInts.cvar->FindVar("tf_avoidteammates_pushaway");
		if (tf_avoidteammates_pushaway->GetInt() == 1)
			tf_avoidteammates_pushaway->SetValue(0);
	}


	if (gCvars.misc_lagexploit && Util->IsKeyPressed(gCvars.misc_lagexploit_key))
	{
		gInts.Engine->ClientCmd("voicemenu 0 0 ");
		gInts.Engine->ClientCmd("voicemenu 0 1");
		gInts.Engine->ClientCmd("voicemenu 0 2 ");
		gInts.Engine->ClientCmd("voicemenu 0 3 ");
		gInts.Engine->ClientCmd("voicemenu 0 4");
		gInts.Engine->ClientCmd("voicemenu 0 5");
		gInts.Engine->ClientCmd("voicemenu 0 6 ");
		gInts.Engine->ClientCmd("voicemenu 0 7 ");
		gInts.Engine->ClientCmd("voicemenu 1 0 ");
		gInts.Engine->ClientCmd("voicemenu 1 1 ");
		gInts.Engine->ClientCmd("voicemenu 1 2");
		gInts.Engine->ClientCmd("voicemenu 1 3");
		gInts.Engine->ClientCmd("voicemenu 1 4 ");
		gInts.Engine->ClientCmd("voicemenu 1 5");
		gInts.Engine->ClientCmd("voicemenu 1 6 ");
		gInts.Engine->ClientCmd("voicemenu 2 0 ");
		gInts.Engine->ClientCmd("voicemenu 2 1 ");
		gInts.Engine->ClientCmd("voicemenu 2 2");
		gInts.Engine->ClientCmd("voicemenu 2 3");
		gInts.Engine->ClientCmd("voicemenu 2 4 ");
		gInts.Engine->ClientCmd("voicemenu 2 5 ");
		gInts.Engine->ClientCmd("voicemenu 2 6");
		gInts.Engine->ClientCmd("voicemenu 2 7");
		gInts.Engine->ClientCmd("voicemenu 0 0 ");
		gInts.Engine->ClientCmd("voicemenu 0 1");
		gInts.Engine->ClientCmd("voicemenu 0 2 ");
		gInts.Engine->ClientCmd("voicemenu 0 3 ");
		gInts.Engine->ClientCmd("voicemenu 0 4");
		gInts.Engine->ClientCmd("voicemenu 0 5");
		gInts.Engine->ClientCmd("voicemenu 0 6 ");
		gInts.Engine->ClientCmd("voicemenu 0 7 ");
		gInts.Engine->ClientCmd("voicemenu 1 0 ");
		gInts.Engine->ClientCmd("voicemenu 1 1 ");
		gInts.Engine->ClientCmd("voicemenu 1 2");
		gInts.Engine->ClientCmd("voicemenu 1 3");
		gInts.Engine->ClientCmd("voicemenu 1 4 ");
		gInts.Engine->ClientCmd("voicemenu 1 5");
		gInts.Engine->ClientCmd("voicemenu 1 6 ");
		gInts.Engine->ClientCmd("voicemenu 2 0 ");
		gInts.Engine->ClientCmd("voicemenu 2 1 ");
		gInts.Engine->ClientCmd("voicemenu 2 2");
		gInts.Engine->ClientCmd("voicemenu 2 3");
		gInts.Engine->ClientCmd("voicemenu 2 4 ");
		gInts.Engine->ClientCmd("voicemenu 2 5 ");
		gInts.Engine->ClientCmd("voicemenu 2 6");
		gInts.Engine->ClientCmd("voicemenu 2 7");
	}


	if (gCvars.misc_chatspam)
	{
		const std::vector<std::string> chatlines = {
			"NaCl",
			"NaCl Rage/Legit Cheat"
		};
		std::chrono::time_point<std::chrono::system_clock> last_spam_point{};
		int current_index{ 0 };
		const std::vector<std::string>* source = nullptr;
		source = &chatlines;
		if (current_index >= source->size()) current_index = 0;
		current_index = rand() % source->size();
		std::string spamString = source->at(current_index);
		//char msg[256];
		std::string msg;
		msg.append("say ");
		if (gCvars.misc_chatspam_newlines)//\x0D
		{
			msg.append(" ");
			msg.append(repeat(gCvars.misc_chatspam_newlines, "\x0D"));
		}
		msg.append(spamString);

		gInts.Engine->ClientCmd_Unrestricted(msg.c_str());
		current_index++;

		last_spam_point = std::chrono::system_clock::now();
	}

	static ConVar* mat_fullbright = gInts.cvar->FindVar("mat_fullbright");
	if (gCvars.misc_claymode)
	{
		if (mat_fullbright->GetInt() == 0) mat_fullbright->SetValue(2);
	}
	else
	{
		mat_fullbright->SetValue(0);
	}

	static ConVar* sv_cheats = gInts.cvar->FindVar("sv_cheats");
	if (gCvars.misc_cheatsbypass)
	{
		if (sv_cheats->GetInt() == 0) sv_cheats->SetValue(1);
	}
	else
	{
		sv_cheats->SetValue(0);
	}

}
		  //Could be much simpler, but I don't want keyvals class
void CMisc::NoisemakerSpam(PVOID kv) //Credits gir https://www.unknowncheats.me/forum/team-fortress-2-a/141108-infinite-noisemakers.html
{
	char chCommand[30] = "use_action_slot_item_server";
	typedef int(__cdecl* HashFunc_t)(const char*, bool);
	static DWORD dwHashFunctionLocation = gSignatures.GetClientSignature("FF 15 ? ? ? ? 83 C4 08 89 06 8B C6");
	static HashFunc_t s_pfGetSymbolForString = (HashFunc_t)**(PDWORD*)(dwHashFunctionLocation + 0x2);
	*(PDWORD)((DWORD)kv + 0x4) = 0;
	*(PDWORD)((DWORD)kv + 0x8) = 0;
	*(PDWORD)((DWORD)kv + 0xC) = 0;
	*(PDWORD)((DWORD)kv + 0x10) = /*0x10000*/0xDEADBEEF;
	*(PDWORD)((DWORD)kv + 0x14) = 0;
	*(PDWORD)((DWORD)kv + 0x18) = 0; //Extra one the game isn't doing, but if you don't zero this out, the game crashes.
	*(PDWORD)((DWORD)kv + 0x1C) = 0;
	*(PDWORD)((DWORD)kv + 0) = s_pfGetSymbolForString(chCommand, 1);
}

//pAAs
void CMisc::AntiAim()
{

}
