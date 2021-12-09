#include "SDK.h"
#include "Panels.h"
#include "CDrawManager.h"
#include "Util.h"
#include "ESP.h"

#include "Radar.h"
CScreenSize gScreenSize;
//===================================================================================
void __fastcall Hooked_PaintTraverse( PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	try
	{
		CBaseEntity* pLocal = gInts.EntList->GetClientEntity(me);
		auto crosshair = gInts.cvar->FindVar("crosshair");
		if (!strcmp("HudScope", gInts.Panels->GetName(vguiPanel)) && gCvars.misc_noscope && gCvars.vismisc_active)
		{
			if (gCvars.misc_noscope_disablecrosshair)
			{
				if (pLocal->GetCond() & tf_cond::TFCond_Zoomed)
				{
					crosshair->SetValue(0);
				}
				else
				{
					crosshair->SetValue(1);
				}

			}

			return;
		}
		VMTManager& hook = VMTManager::GetHook(pPanels); 
		hook.GetMethod<void(__thiscall*)(PVOID, unsigned int, bool, bool)>(gOffsets.iPaintTraverseOffset)(pPanels, vguiPanel, forceRepaint, allowForce); //Call the original.
		static unsigned int vguiFocusOverlayPanel;

		if (vguiFocusOverlayPanel == NULL)
		{											//FocusOverlayPanel
			const char* szName = gInts.Panels->GetName(vguiPanel);
			if( szName[0] == 'F' && szName[5] == 'O' &&  szName[12] == 'P' )
			{
				vguiFocusOverlayPanel = vguiPanel;
				Intro();
			}
		}

		if (vguiFocusOverlayPanel == vguiPanel )
		{
			if (gCvars.misc_cleanscreenshots && gInts.Engine->IsTakingScreenshot() || gCvars.misc_cleanscreenshots && GetAsyncKeyState(VK_F12) || gCvars.misc_cleanscreenshots && GetAsyncKeyState(VK_SNAPSHOT))
			    return;


			int iWidth, iHeight; //Resolution fix, so this can work in Fullscreen
			gInts.Engine->GetScreenSize(iWidth, iHeight);
			if (gScreenSize.iScreenWidth != iWidth || gScreenSize.iScreenHeight != iHeight)
				gInts.Engine->GetScreenSize(gScreenSize.iScreenWidth, gScreenSize.iScreenHeight);

			//gInts.Panels->SetTopmostPopup(vguiPanel, true);

			if (gInts.Engine->IsInGame()) 
			{
				gDrawManager.DrawString(5, 5, Color::Green(), "%s", CHEATNAME);
				gDrawManager.DrawString(5, 15, Color::Green(), "Build Date: %s", __DATE__);
				gDrawManager.DrawString(5, 25, Color::Green(), "Build Status: User");
			}
			else //The reason this is done is because the party "menu" at the top of your screen is in the way, so it draws under if not in game
			{
				gDrawManager.DrawString(5, 55, Color::Green(), "%s", CHEATNAME);
				gDrawManager.DrawString(5, 65, Color::Green(), "Build Date: %s", __DATE__);
				gDrawManager.DrawString(5, 75, Color::Green(), "Build Status: User");
			}

			if (!pLocal)
				return;

			if (gCheatMenu.bMenuActive)
			{
				gCheatMenu.DrawMenu();
				gCheatMenu.Render();
			}


			gRadar.DrawRadarBack();
			gESP.Run(pLocal);



			if (gInts.Engine->IsInGame() && gInts.Engine->IsConnected() && gCvars.misc_noscope_drawlines)
			{
			   if (!gCvars.misc_noscope || !gCvars.vismisc_active)
			     return;

				if (pLocal->GetLifeState() == LIFE_ALIVE && pLocal->GetCond() & tf_cond::TFCond_Zoomed)
				{
					int width, height;
					gInts.Engine->GetScreenSize(width, height);
					gDrawManager.DrawLine(width / 2, 0, width / 2, height, Color(0, 0, 0, 255));
					gDrawManager.DrawLine(0, height / 2, width, height / 2, Color(0, 0, 0, 255));
				}

			}
		}
	}
	catch(...)
	{
		Log::Fatal("Failed PaintTraverse");
	}
}
//===================================================================================
void Intro( void )
{
	try
	{
		gDrawManager.Initialize(); //Initalize the drawing class.

		gNetVars.Initialize();
	}
	catch(...)
	{
		Log::Fatal("Failed Intro");
	}
}
