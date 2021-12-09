#include "Aimbot.h"
#include "Util.h"

bool CAimbot::CanAmbassadorHeadshot(CBaseEntity* pLocal)
{
	return gInts.globals->curtime - pLocal->GetActiveWeapon()->m_flLastFireTime() >= 0.930;//1.0;
}

CAimbot gAim;

void FixMove(CUserCmd* pCmd, Vector m_vOldAngles, float m_fOldForward, float m_fOldSidemove)
{
	float deltaView = pCmd->viewangles.y - m_vOldAngles.y;
	float f1;
	float f2;

	if (m_vOldAngles.y < 0.f)
		f1 = 360.0f + m_vOldAngles.y;
	else
		f1 = m_vOldAngles.y;

	if (pCmd->viewangles.y < 0.0f)
		f2 = 360.0f + pCmd->viewangles.y;
	else
		f2 = pCmd->viewangles.y;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);
	deltaView = 360.0f - deltaView;

	pCmd->forwardmove = cos(DEG2RAD(deltaView)) * m_fOldForward + cos(DEG2RAD(deltaView + 90.f)) * m_fOldSidemove;
	pCmd->sidemove = sin(DEG2RAD(deltaView)) * m_fOldForward + sin(DEG2RAD(deltaView + 90.f)) * m_fOldSidemove;
}

Vector CAimbot::calc_angle(Vector src, Vector dst)
{
	Vector AimAngles, delta;
	float hyp;
	delta = src - dst;
	hyp = sqrtf((delta.x * delta.x) + (delta.y * delta.y)); //SUPER SECRET IMPROVEMENT CODE NAME DONUT STEEL
	AimAngles.x = atanf(delta.z / hyp) * RADPI;
	AimAngles.y = atanf(delta.y / delta.x) * RADPI;
	AimAngles.z = 0.0f;
	if (delta.x >= 0.0)
		AimAngles.y += 180.0f;
	return AimAngles;
}
void CAimbot::MakeVector(Vector angle, Vector& vector)
{
	float pitch, yaw, tmp;
	pitch = float(angle[0] * PI / 180);
	yaw = float(angle[1] * PI / 180);
	tmp = float(cos(pitch));
	vector[0] = float(-tmp * -cos(yaw));
	vector[1] = float(sin(yaw)*tmp);
	vector[2] = float(-sin(pitch));
}
float CAimbot::GetFOV(Vector angle, Vector src, Vector dst)
{
	Vector ang, aim;
	float mag, u_dot_v;
	ang = calc_angle(src, dst);


	MakeVector(angle, aim);
	MakeVector(ang, ang);

	mag = sqrtf(pow(aim.x, 2) + pow(aim.y, 2) + pow(aim.z, 2));
	u_dot_v = aim.Dot(ang);

	return RAD2DEG(acos(u_dot_v / (pow(mag, 2))));
}

void CAimbot::w(bool silent, Vector vAngs, CUserCmd* pCommand)
{
	if (silent)
	{
		ClampAngle(vAngs);
		Util->SilentMovementFix(pCommand, vAngs);
		pCommand->viewangles = vAngs;

	}
	else
	{
		pCommand->viewangles = vAngs;
		gInts.Engine->SetViewAngles(pCommand->viewangles);
	}
}

void CAimbot::Run(CBaseEntity* pLocal, CUserCmd* pCommand)
{
	Vector m_vOldViewAngle = pCommand->viewangles;
	float m_fOldSideMove = pCommand->sidemove;
	float m_fOldForwardMove = pCommand->forwardmove;

	gCvars.iAimbotIndex = -1;



	if (!gCvars.aimbot_active)
		return;

	if (!Util->IsKeyPressed(gCvars.aimbot_key))
		return;

	if (!pLocal->GetActiveWeapon())
		return;

	auto id = pLocal->GetActiveWeapon()->GetItemDefinitionIndex(); //This ignores all projectile weapons, doesn't work for strange/killstreak/etc
	if (
#pragma region scout
		id == (int)scoutweapons::WPN_Sandman
		|| id == (int)scoutweapons::WPN_CritCola
		|| id == (int)scoutweapons::WPN_CritCola
		|| id == (int)scoutweapons::WPN_FlyingGuillotine1
		|| id == (int)scoutweapons::WPN_FlyingGuillotine2
		|| id == (int)scoutweapons::WPN_Milk
		|| id == (int)scoutweapons::WPN_MutatedMilk
		|| id == (int)scoutweapons::WPN_WrapAssassin
#pragma endregion
#pragma region pyro
		|| id == (int)pyroweapons::WPN_Detonator
		|| id == (int)pyroweapons::WPN_FestiveFlaregun
		|| id == (int)pyroweapons::WPN_Flaregun
		|| id == (int)pyroweapons::WPN_ScorchShot
#pragma endregion
#pragma region demo
		|| id == (int)demomanweapons::WPN_FestiveGrenadeLauncher
		|| id == (int)demomanweapons::WPN_FestiveStickyLauncher
		|| id == (int)demomanweapons::WPN_GrenadeLauncher
		|| id == (int)demomanweapons::WPN_IronBomber
		|| id == (int)demomanweapons::WPN_LochNLoad
		|| id == (int)demomanweapons::WPN_LoooseCannon
		|| id == (int)demomanweapons::WPN_QuickieBombLauncher
		|| id == (int)demomanweapons::WPN_StickyLauncher
		|| id == (int)demomanweapons::WPN_ScottsSkullctter
#pragma endregion
#pragma region heavy
		|| id == (int)heavyweapons::WPN_FestiveSandvich
		|| id == (int)heavyweapons::WPN_RobotSandvich
		|| id == (int)heavyweapons::WPN_Sandvich
		|| id == (int)heavyweapons::WPN_Steak
		|| id == (int)heavyweapons::WPN_CandyBar
		|| id == (int)heavyweapons::WPN_Fishcake
#pragma endregion
#pragma region engineer
		|| id == (int)engineerweapons::WPN_Builder
		|| id == (int)engineerweapons::WPN_Builder2
		|| id == (int)engineerweapons::WPN_Destructor
		|| id == (int)engineerweapons::WPN_FestiveWrangler
		|| id == (int)engineerweapons::WPN_Pomson
		|| id == (int)engineerweapons::WPN_RescueRanger
		|| id == (int)engineerweapons::WPN_ShortCircut
		|| id == (int)engineerweapons::WPN_Toolbox
		|| id == (int)engineerweapons::WPN_Wrangler
#pragma endregion
#pragma region medic
		|| id == (int)medicweapons::WPN_Blutsauger
		|| id == (int)medicweapons::WPN_FestiveCrossbow
		|| id == (int)medicweapons::WPN_FestiveMedigun
		|| id == (int)medicweapons::WPN_Kritzkrieg
		|| id == (int)medicweapons::WPN_Medigun
		|| id == (int)medicweapons::WPN_Overdose
		|| id == (int)medicweapons::WPN_QuickFix
		|| id == (int)medicweapons::WPN_SyringeGun
		|| id == (int)medicweapons::WPN_Vaccinator

#pragma endregion
#pragma region sniper
		|| id == (int)sniperweapons::WPN_CompoundBow
		|| id == (int)sniperweapons::WPN_FestiveJarate
		|| id == (int)sniperweapons::WPN_FestiveHuntsman
		|| id == (int)sniperweapons::WPN_Huntsman
		|| id == (int)sniperweapons::WPN_Jarate
#pragma endregion
#pragma region spy
		|| id == (int)spyweapons::WPN_ApSap
		|| id == (int)spyweapons::WPN_DisguiseKit
		|| id == (int)spyweapons::WPN_RedTape1
		|| id == (int)spyweapons::WPN_RedTape2
		|| id == (int)spyweapons::WPN_Sapper
		|| id == (int)spyweapons::WPN_Sapper2
		|| id == (int)spyweapons::WPN_FestiveSapper
#pragma endregion
		)
		return;



	CBaseEntity* pEntity = GetBaseEntity(GetBestTarget(pLocal, pCommand));

	if (!pEntity)
		return;

	int iBestHitbox = GetBestHitbox(pLocal, pEntity);

	if (iBestHitbox == -1)
		return;

	auto pWep = pLocal->GetActiveWeapon();
	if (gCvars.aimbot_waitforcharge) //Advanced Ambassador Wait For Charge!
		if (pWep->GetItemDefinitionIndex() == spyweapons::WPN_Ambassador || pWep->GetItemDefinitionIndex() == spyweapons::WPN_FestiveAmbassador)
			if (!CanAmbassadorHeadshot(pLocal)) return;


	if (iBestHitbox == -1)
		return;
	Vector vEntity = pEntity->GetHitboxPosition(iBestHitbox);

	Vector vLocal = pLocal->GetEyePosition();

	Vector vAngs;
	
	auto pWeap = pLocal->GetActiveWeapon(); if (!pWeap) return;
	auto pSoldier = (pLocal->GetClassNum() == TF2_Soldier && pWeap->GetSlot() == 0);
	if (pSoldier)
	{
		bool ponGround = pEntity->GetFlags() & FL_ONGROUND;

		vEntity = pEntity->GetAbsOrigin(); vEntity[2] += 10.0f;

		auto projectile_speed = pWeap->GetItemDefinitionIndex() == WPN_DirectHit ? 1980.0f : 1100.0f;
		auto predicted_time = ((pLocal->GetEyePosition().DistTo(vEntity) / projectile_speed) + gInts.globals->interval_per_tick);

		Vector AbsVel = Util->EstimateAbsVelocity(pEntity);
		AbsVel[2] += -gInts.cvar->FindVar("sv_gravity")->GetFloat() * gInts.globals->interval_per_tick * gInts.globals->interval_per_tick;

		vEntity[0] += (AbsVel[0] * predicted_time) + gInts.globals->interval_per_tick;
		vEntity[1] += (AbsVel[1] * predicted_time) + gInts.globals->interval_per_tick;
		vEntity[2] += (ponGround ? (AbsVel[2] * predicted_time) + gInts.globals->interval_per_tick :
			(0.5 * pow(predicted_time, 2) + AbsVel[2] * predicted_time)) + gInts.globals->interval_per_tick;
	}
	
	VectorAngles((vEntity - vLocal), vAngs);

	ClampAngle(vAngs);

	gCvars.iAimbotIndex = pEntity->GetIndex();

		if (gCvars.aimbot_smooth > 0.0 && !gCvars.aimbot_silent)
		{
			Vector vDelta(pCommand->viewangles - vAngs);
			AngleNormalize(vDelta);
			vAngs = pCommand->viewangles - vDelta / gCvars.aimbot_smooth;
		}

		if (gCvars.aimbot_silent)
		{
			if (pCommand->buttons & IN_ATTACK)
			{
				w(true, vAngs, pCommand);
			}
			else if (gCvars.aimbot_autoshoot)
			{
				w(true, vAngs, pCommand);
				pCommand->buttons |= IN_ATTACK;
			}
			else
			{
				if (pCommand->buttons & IN_ATTACK)
					w(true, vAngs, pCommand);
			}
		}
		else
		{
			w(gCvars.aimbot_silent, vAngs, pCommand);
			if (gCvars.aimbot_autoshoot)
				pCommand->buttons |= IN_ATTACK;
		}


	FixMove(pCommand, m_vOldViewAngle, m_fOldForwardMove, m_fOldSideMove);
}

int CAimbot::GetBestTarget(CBaseEntity* pLocal, CUserCmd* pCommand)
{
	int iBestTarget = -1;
	int bestTarget = -1;					 //this num could be smaller 
	float flDistToBest = 99999.f;
	double minimalDistance = 99999.0;

	Vector vLocal = pLocal->GetEyePosition();

	for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
	{
		if (i == me)
			continue;

		auto weap = pLocal->GetActiveWeapon()->GetItemDefinitionIndex();

		CBaseEntity* pEntity = GetBaseEntity(i);

		if (!pEntity)
			continue;

		if (pEntity->IsDormant())
			continue;

		if (pEntity->GetLifeState() != LIFE_ALIVE)
			continue;

		static ConVar* mp_friendlyfire = gInts.cvar->FindVar("mp_friendlyfire");

		if (mp_friendlyfire->GetInt() == 0 && pEntity->GetTeamNum() == pLocal->GetTeamNum())
			continue;

		int iBestHitbox = GetBestHitbox(pLocal, pEntity);

		if (iBestHitbox == -1)
			continue;

		Vector vEntity = pEntity->GetHitboxPosition(iBestHitbox);

		if (!gCvars.PlayerMode[i])
			continue;

		if (pEntity->GetCond() & TFCond_Ubercharged ||
			pEntity->GetCond() & TFCond_UberchargeFading ||
			pEntity->GetCond() & TFCond_Bonked)
			continue;

		if (pEntity->GetCond() & TFCond_Cloaked && gCvars.aimbot_ignorecloaked)
			continue;

		if (pEntity->GetCond() & TFCond_Taunting && gCvars.aimbot_ignoretaunting)
			continue;

		if (pEntity->GetCond() & TFCond_Disguised && gCvars.aimbot_ignoredisguised)
			continue;

       if (!gCvars.aimbot_melee && Util->IsOtherSlot(pLocal, pLocal->GetActiveWeapon()))
			continue;

		auto pWep = pLocal->GetActiveWeapon();
		auto pClass = pWep->GetItemDefinitionIndex();
		auto urmomgay = pClass == demomanweapons::WPN_Sword || pClass == demomanweapons::WPN_FestiveEyelander || pClass == demomanweapons::WPN_Golfclub || pClass == demomanweapons::WPN_ScottsSkullctter || pClass == demomanweapons::WPN_Headless;
		if (pWep->GetSlot() == 2 && !urmomgay)
			minimalDistance = 8.4;

		if (pClass == demomanweapons::WPN_Sword || pClass == demomanweapons::WPN_FestiveEyelander || pClass == demomanweapons::WPN_Golfclub || pClass == demomanweapons::WPN_ScottsSkullctter || pClass == demomanweapons::WPN_Headless)
			minimalDistance = 12.0;

		if (pLocal->szGetClass() == "Pyro" && (pClass == pyroweapons::WPN_Backburner || pClass == pyroweapons::WPN_Degreaser || pClass == pyroweapons::WPN_FestiveBackburner || pClass == pyroweapons::WPN_FestiveFlamethrower || pClass == pyroweapons::WPN_Flamethrower || pClass == pyroweapons::WPN_Phlogistinator || pClass == pyroweapons::WPN_Rainblower))
			minimalDistance = 17.0;


		if (pLocal->szGetClass() == "Sniper" && pLocal->GetActiveWeapon()->GetSlot() == 0)
		{
			float damage = pLocal->GetActiveWeapon()->GetChargeDamage();
			if (gCvars.aimbot_zoomedonly)// || gCvars.aimbot.waitforcharge)
			{
				if (!(pLocal->GetCond() & tf_cond::TFCond_Zoomed))
					return -1;

				if (damage < 10.f)
					return -1;


			}
			if (gCvars.aimbot_waitforcharge) //This isn't the best code but it works for raging.
			{
				if (gCvars.aimbot_waitforcharge && ZOOM_BASE_DAMAGE + damage < pEntity->GetHealth())//<
					return -1;
			}
		}

		float flFOV = GetFOV(pCommand->viewangles, vLocal, vEntity);
		float distance = Util->flGetDistance(vEntity, pLocal->GetEyePosition());

		if (distance < minimalDistance)//gCvars.aimbot.fov)
		{
			if (flFOV < flDistToBest && flFOV < gCvars.aimbot_fov)
			{
				if (gCvars.PlayerMode[i] == 2)
					return i;
				//flDistToBest = flDistToTarget;
				flDistToBest = flFOV;
				gCvars.iAimbotIndex = i;
				bestTarget = i;
			}
		}

		if (gCvars.PlayerMode[i] == 2) //always aim at rage targets first
			return i;
	}


	return bestTarget;
}

int CAimbot::GetBestHitbox(CBaseEntity* pLocal, CBaseEntity* pEntity)
{
	int iBestHitbox = -1;

	if (!gCvars.aimbot_hitbox)
	{
		if (Util->IsHeadshotWeapon(pLocal, pLocal->GetActiveWeapon()))
			iBestHitbox = 0;
		else
			iBestHitbox = 4;
		for (int i = iBestHitbox; i < 17; i++) // int i equals prioritized hitbux, so above we check the weapon so it prioritizes the proper hitbox.
		{
			if (Util->IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), pEntity->GetHitboxPosition(i)))
				return i;
		}
	}
	else if (gCvars.aimbot_hitbox == 1)
	{
		iBestHitbox = 0;
	}
	else if (gCvars.aimbot_hitbox == 2)
	{
		iBestHitbox = 4;
	}

	if (pEntity->GetHitboxPosition(iBestHitbox).IsZero())
		return -1;

	if (!Util->IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), pEntity->GetHitboxPosition(iBestHitbox)))
		return - 1;
	
	return iBestHitbox;
}
