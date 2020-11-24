#include "SDK.h"
#include "Client.h"
#include <string>
#define IsCorrectFrame(x) (gInts.globals->framecount % x == 0)
bool switchj = false;

CBaseCombatWeapon* CBaseEntity::GetActiveWeapon() //wah whyd you put it here wah(Cause I couldn't put it in the sdk obviously, not making a whole new .cpp file just to store this.)
{
	DYNVAR(pHandle, DWORD, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	return (CBaseCombatWeapon *)gInts.EntList->GetClientEntityFromHandle(pHandle.GetValue(this));
}

//============================================================================================
bool __fastcall Hooked_CreateMove(PVOID ClientMode, int edx, float input_sample_frametime, CUserCmd* pCommand)
{
	//If you want taunt slide, you will need to hook CHLClient::CreateMove and do it there. If you do it here, you'll just shimmy forward.
	VMTManager& hook = VMTManager::GetHook(ClientMode); //Get a pointer to the instance of your VMTManager with the function GetHook.
	bool bReturn = hook.GetMethod<bool(__thiscall*)(PVOID, float, CUserCmd*)>(gOffsets.iCreateMoveOffset)(ClientMode, input_sample_frametime, pCommand); //Call the original.
	try
	{
		CBaseEntity* pLocal = GetBaseEntity(me); //Grab the local player's entity pointer.

		if (pLocal == NULL || pLocal->IsDormant())
			return bReturn;

		Vector vOldAngles = pCommand->viewangles;
		Vector angles = pCommand->viewangles;

		gInts.cvar->FindVar("cl_pitchup")->SetValue(360); //Unlock view angles for extra weirdness
		gInts.cvar->FindVar("cl_pitchdown")->SetValue(360);

		if (IsCorrectFrame(1000) && pLocal->IsAlive() && pLocal->szGetClass() == "Engineer") { //Engineer troll(Destroy buildings randomly)
			gInts.Engine->ClientCmd_Unrestricted(string(string("destroy ") + to_string(rand() % 5)).c_str()); //For some reason nothing else does it correctly, so go ahead and cry about me spamming "string"
			gInts.Engine->ClientCmd_Unrestricted("taunt 0");
		}

		if (pLocal->IsAlive() && pLocal->szGetClass() == "Spy") { //Spy troll(If cloaked die, also make the charging sound, also good luck backstabbing )
			if ((pLocal->GetCond() & TFCond_Cloaked) || (pLocal->GetCond() & TFCond_DeadRingered)) {
				DYNVAR_SET(int, pLocal, TFCond_Charging, "DT_TFPlayer", "m_Shared", "m_nPlayerCond");
				gInts.Engine->ClientCmd_Unrestricted("kill");
			}
			if (pLocal->GetActiveWeapon() && pLocal->GetActiveWeapon()->IsReadyToBackstab(pLocal)) {
				angles.y += 40.f;
				pCommand->tick_count = INT_MAX; //no backstabbing trololol
			}
		}

		if (pLocal->IsAlive() && ((pLocal->szGetClass() == "Demoman") || (pLocal->szGetClass() == "Soldier")) && pCommand->buttons & IN_ATTACK && pLocal->GetHealth() <= 85) { //Soldier/Demo troll(Aim at the ground so the player dies when <= 85.)
			angles.x = 90.f;
		}

		if (pLocal->IsAlive() && pLocal->szGetClass() == "Heavy" && (pCommand->buttons & IN_ATTACK || pCommand->buttons & IN_ATTACK2)) { //Heavy troll(If rev'd then jitter around)
			if (switchj == false) {
				angles.y += 30.f;
			}
			else {
				angles.y -= 30.f;
			}
			switchj = !switchj;
		}

		if (pLocal->IsAlive() && pLocal->szGetClass() == "Scout") { //Scout troll(SlowWalk)
			auto max_speed = 150.f == -1.0f ? 0.3f : 150.f;
			auto forwardmove = pCommand->forwardmove;
			auto sidemove = pCommand->sidemove;

			auto move_length = sqrt(sidemove * sidemove + forwardmove * forwardmove);
			auto move_length_backup = move_length;

			if (move_length > max_speed)
			{
				pCommand->forwardmove = forwardmove / move_length_backup * max_speed;
				move_length = sidemove / move_length_backup * max_speed;
				pCommand->sidemove = sidemove / move_length_backup * max_speed;
			}
		}

		if (pLocal->IsAlive() && pLocal->szGetClass() == "Pyro" && (pCommand->buttons & IN_ATTACK || pCommand->buttons & IN_ATTACK2)) { //Pyro troll(Switch to melee if fire or airblast)
			gInts.Engine->ClientCmd_Unrestricted("slot3"); //melee
		}

		if (pLocal->IsAlive() && pLocal->szGetClass() == "Sniper" && pLocal->GetCond() & TFCond_Zoomed) { //Sniper troll(Good luck zooming in, also primary only)
			pCommand->buttons |= IN_ATTACK2;
		}

		if (pLocal->GetCond() & TFCond_Ubercharged || pLocal->GetCond() & TFCond_UberchargeFading || pLocal->GetCond() & TFCond_OnFire || pLocal->GetCond() & TFCond_Bleeding || pLocal->GetCond() & TFCond_Bonked || pLocal->GetCond() & TFCond_Buffed || pLocal->GetCond() & TFCond_Charging || pLocal->GetCond() & TFCond_Jarated || pLocal->GetCond() & TFCond_Disguising || pLocal->GetCond() & TFCond_Milked) { //If any of these happen then do the troll :trollface:
			auto max_speed = 0 == -1.0f ? 0.3f : -1.0f;
			auto forwardmove = pCommand->forwardmove;
			auto sidemove = pCommand->sidemove;

			auto move_length = sqrt(sidemove * sidemove + forwardmove * forwardmove);
			auto move_length_backup = move_length;

			if (move_length > max_speed)
			{
				pCommand->forwardmove = forwardmove / move_length_backup * max_speed;
				move_length = sidemove / move_length_backup * max_speed;
				pCommand->sidemove = sidemove / move_length_backup * max_speed;
			}
			angles.y += 0.05f;
		}

		if (IsCorrectFrame(1000)) {
			switch (rand() % 30 + 1)
			{
			//Disabled code below cause can actually be annoying to the eye.
			/*case 10:
				gInts.cvar->FindVar("mat_hsv")->SetValue(!gInts.cvar->FindVar("mat_hsv")->GetInt()); //You can now only see black and white.

			case 5:
				gInts.cvar->FindVar("mat_luxels")->SetValue(!gInts.cvar->FindVar("mat_luxels")->GetInt()); //Luxels(I don't know how to explain but it puts some weird box shit everywhere)

			case 9:
				gInts.cvar->FindVar("mat_wireframe")->SetValue(!gInts.cvar->FindVar("mat_wireframe")->GetInt()); //Wireframe*/

			case 15:
				gInts.Engine->ClientCmd_Unrestricted("screenshot");
			default:
				break;
			}
		}
		pCommand->viewangles = angles;
	}
	catch(...)
	{
		Log::Fatal("Failed Hooked_CreateMove");
	}
	return bReturn; //If bReturn is true, CInput::CreateMove will call CEngine::SetViewAngles(pCommand->viewangles). If you want silent aim, return false, but make sure to call SetViewAngles manually.
}
//============================================================================================