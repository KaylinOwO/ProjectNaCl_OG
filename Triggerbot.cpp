#include "Triggerbot.h"
#include "Util.h"
#include "Aimbot.h"

CTriggerbot gTrigger;

void CTriggerbot::Run(CBaseEntity* pLocal, CUserCmd* pCommand)
{
	float flDistToBest = 99999.f;
	float minimalDistance = 99999.0;

	if (!gCvars.triggerbot_active)
		return;

	if (!Util->IsKeyPressed(gCvars.triggerbot_key))
	 return;

	if (pLocal->GetLifeState() != LIFE_ALIVE) 
	 return;
	 
	Ray_t ray;
	trace_t trace;
	CTraceFilter filt;

	Vector vAim;
	gInts.Engine->GetViewAngles(vAim); //We use getviewangles so that this can work with anti-aim when it gets added



	Vector vForward;
	AngleVectors(vAim, &vForward);
	vForward = vForward * minimalDistance + pLocal->GetEyePosition();//vForward * 9999;

	auto pWep = pLocal->GetActiveWeapon();
	auto pClass = pWep->GetItemDefinitionIndex();
	if (pWep->GetSlot() == 2)
		vForward = vForward * (float)8.4 + pLocal->GetEyePosition();
	if (pLocal->szGetClass() == "Pyro" &&
		(pClass == pyroweapons::WPN_Backburner
			|| pClass == pyroweapons::WPN_Degreaser
			|| pClass == pyroweapons::WPN_FestiveBackburner
			|| pClass == pyroweapons::WPN_FestiveFlamethrower
			|| pClass == pyroweapons::WPN_Flamethrower
			|| pClass == pyroweapons::WPN_Phlogistinator
			|| pClass == pyroweapons::WPN_Rainblower))
		vForward = vForward * 17.0 + pLocal->GetEyePosition();
	

	filt.pSkip = pLocal;

	ray.Init(pLocal->GetEyePosition(), vForward);
	gInts.EngineTrace->TraceRay(ray, 0x46004003, &filt, &trace);

	if (!trace.m_pEnt)
		return;

	if (trace.hitgroup < 1)
		return;

	if (trace.m_pEnt->GetTeamNum() == pLocal->GetTeamNum())
		return;

	if (!gCvars.PlayerMode[trace.m_pEnt->GetIndex()])
		return;

	if (trace.m_pEnt->GetCond() & TFCond_Ubercharged ||
		trace.m_pEnt->GetCond() & TFCond_UberchargeFading ||
		trace.m_pEnt->GetCond() & TFCond_Bonked)
		return;

	if (pLocal->szGetClass() == "Sniper" && pLocal->GetActiveWeapon()->GetSlot() == 0 && !(pLocal->GetCond() & tf_cond::TFCond_Zoomed) && gCvars.triggerbot_zoomedonly)
		return;

	if (gCvars.triggerbot_headonly && trace.hitbox != 0)
		return;

	if (!gCvars.triggerbot_melee && pWep->GetSlot() == 2)
	   return;


	pCommand->buttons |= IN_ATTACK;
}