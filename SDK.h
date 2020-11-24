#pragma once

#include <windows.h>
#include <math.h>
#include <xstring>
#include <cmath>
#include "Vector.h"
#include "getvfunc.h"
#include "dt_recv2.h"
#include "CSignature.h"
#include "WeaponList.h"
#include "CGlobalVars.h"
#include "VMTHooks.h"
#include "Log.h"
#include "Convar.h"
#include "Color.h"
#include "CNetVars.h"

using namespace std;

typedef void* ( __cdecl* CreateInterface_t )( const char*, int* );
typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

#define VMTManager toolkit::VMTManager
#define VMTBaseManager toolkit::VMTBaseManager

#define WIN32_LEAN_AND_MEAN
#pragma optimize("gsy",on)
#pragma warning( disable : 4244 ) //Possible loss of data
#define GETSPREADOFFSET 0x5E0
#define GETCONEOFFSET 0x5E4
#define WEAPONIDOFFSET 0x5A0

typedef float matrix3x4[3][4];
class CNetVars;
class CBaseCombatWeapon;

#define me gInts.Engine->GetLocalPlayer()
#define GetBaseEntity gInts.EntList->GetClientEntity
#define MASK_AIMBOT 0x200400B
#define	FL_ONGROUND (1<<0)
#define FL_DUCKING (1<<1)
#define CONTENTS_HITBOX 0x40000000
#define FLOW_OUTGOING 0
#define FLOW_INCOMING 1
#define PI 3.14159265358979323846f
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )
#define square( x ) ( x * x )
#define RADPI 57.295779513082f
#define SQUARE( a ) a*a
#define BLU_TEAM 3
#define RED_TEAM 2

typedef struct player_info_s
{
	char			name[32];
	int				userID;
	char			guid[33];
	unsigned long	friendsID;
	char			friendsName[32];
	bool			fakeplayer;
	bool			ishltv;
	unsigned long	customFiles[4];
	unsigned char	filesDownloaded;
} player_info_t;

class ClientClass
{
private:
	BYTE _chPadding[8];
public:
	char* chName;
	RecvTable* Table;
	ClientClass* pNextClass;
	int iClassID;
};

class CHLClient
{
public:
	ClientClass* GetAllClasses( void )
	{
		typedef ClientClass* ( __thiscall* OriginalFn )( PVOID ); //Anything inside a VTable is a __thiscall unless it completly disregards the thisptr. You can also call them as __stdcalls, but you won't have access to the __thisptr.
		return getvfunc<OriginalFn>( this, 8 )( this ); //Return the pointer to the head CClientClass.
	}
};

enum tf_classes
{
	TF2_Scout = 1,
	TF2_Soldier = 3,
	TF2_Pyro = 7,
	TF2_Demoman = 4,
	TF2_Heavy = 6,
	TF2_Engineer = 9,
	TF2_Medic = 5,
	TF2_Sniper = 2,
	TF2_Spy = 8,
};

enum source_lifestates
{
	LIFE_ALIVE,
	LIFE_DYING,
	LIFE_DEAD,
	LIFE_RESPAWNABLE,
	LIFE_DISCARDBODY,
};

class CGlobals
{
public:
	float realtime;
	int framecount;
	float absoluteframetime;
	float curtime;
	float frametime;
	int maxclients;
	int tickcount;
	float interval_per_tick;
	float interpolation_amount;
};

class CUserCmd
{
public:
	virtual ~CUserCmd() {}; //Destructor 0
	int command_number; //4
	int tick_count; //8
	Vector viewangles; //C
	float forwardmove; //18
	float sidemove; //1C
	float upmove; //20
	int	buttons; //24
	byte impulse; //28
	int weaponselect; //2C
	int weaponsubtype; //30
	int random_seed; //34
	short mousedx; //38
	short mousedy; //3A
	bool hasbeenpredicted; //3C;
};

class CBaseEntity
{
public:
	Vector& GetAbsOrigin( )
	{
		typedef Vector& ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>(this, 9)(this);
	}
	Vector& GetAbsAngles( )
	{
		typedef Vector& ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>(this, 10)(this);
	}
	void GetWorldSpaceCenter( Vector& vWorldSpaceCenter)
	{
		Vector vMin, vMax;
		this->GetRenderBounds( vMin, vMax );
		vWorldSpaceCenter = this->GetAbsOrigin();
		vWorldSpaceCenter.z += (vMin.z + vMax.z) / 2;
	}
	DWORD* GetModel( )
	{
		PVOID pRenderable = (PVOID)(this + 0x4);
		typedef DWORD* ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pRenderable, 9 )( pRenderable );
	}
	bool SetupBones( matrix3x4 *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime )
	{
		PVOID pRenderable = (PVOID)(this + 0x4);
		typedef bool ( __thiscall* OriginalFn )( PVOID, matrix3x4*, int, int, float );
		return getvfunc<OriginalFn>( pRenderable, 16 )( pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime );
	}
	ClientClass* GetClientClass( )
	{
		PVOID pNetworkable = (PVOID)(this + 0x8);
		typedef ClientClass* ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pNetworkable, 2 )( pNetworkable );
	}
	bool IsDormant( )
	{
		PVOID pNetworkable = (PVOID)(this + 0x8);
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pNetworkable, 8 )( pNetworkable );
	}
	int GetIndex( )
	{
		PVOID pNetworkable = (PVOID)(this + 0x8);
		typedef int ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pNetworkable, 9 )( pNetworkable );
	}
	void GetRenderBounds( Vector& mins, Vector& maxs )
	{
		PVOID pRenderable = (PVOID)(this + 0x4);
		typedef void ( __thiscall* OriginalFn )( PVOID, Vector& , Vector& );
		getvfunc<OriginalFn>( pRenderable, 20 )( pRenderable, mins, maxs );
	}
	bool IsAlive()
	{
		return *(byte*)(this + gNetVars.get_offset("DT_BasePlayer", "m_lifeState")) == LIFE_ALIVE;
	}
	int GetClassNum()
	{
		DYNVAR_RETURN(int, this, "DT_TFPlayer", "m_PlayerClass", "m_iClass");
	}
	char* szGetClass()
	{
		DYNVAR(iClass, int, "DT_TFPlayer", "m_PlayerClass", "m_iClass");

		switch (iClass.GetValue(this))
		{
		case TF2_Scout:
			return "Scout";
		case TF2_Soldier:
			return "Soldier";
		case TF2_Pyro:
			return "Pyro";
		case TF2_Demoman:
			return "Demoman";
		case TF2_Heavy:
			return "Heavy";
		case TF2_Engineer:
			return "Engineer";
		case TF2_Medic:
			return "Medic";
		case TF2_Sniper:
			return "Sniper";
		case TF2_Spy:
			return "Spy";
		default:
			return "Unknown class";
		}

		return "Unknown class"; //Just in case
	}
	int GetCond()
	{
		DYNVAR_RETURN(int, this, "DT_TFPlayer", "m_Shared", "m_nPlayerCond");
	}
	int GetHealth()
	{
		DYNVAR_RETURN(int, this, "DT_BasePlayer", "m_iHealth");
	}
	bool CanBackStab()
	{
		static int offset = gNetVars.get_offset("DT_TFWeaponKnife", "m_bReadyToBackstab");
		return *(bool*)(this + offset);
	}
	CBaseCombatWeapon* GetActiveWeapon();
};

class CBaseCombatWeapon : public CBaseEntity
{
public:
	int GetMaxClip1()
	{
		typedef int(__thiscall * OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 318)(this);
	}

	float NextPrimaryAttack()
	{
		return *reinterpret_cast<float*>(uintptr_t(this) + 0x878);
	}

	float NextAttack()
	{
		return *reinterpret_cast<float*>(uintptr_t(this) + 0xC38);
	}


	int ammo()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x8BC);
	}

	bool HasAmmo()
	{
		return ammo() > 0;
	}

	int GetMaxClip2()
	{
		typedef int(__thiscall * OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 319)(this);
	}

	int GetSlot()
	{
		typedef int(__thiscall *OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 327)(this);
	}

	char *GetName()
	{
		typedef char *(__thiscall * OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 329)(this);
	}

	char *GetPrintName()
	{
		typedef char *(__thiscall * OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 330)(this);
	}
	float GetSpread()
	{
		typedef float(__thiscall *GetSpread_t)(PVOID);
		return ((GetSpread_t)(*(PDWORD)(*(PDWORD)(this) + GETSPREADOFFSET)))(this);
	}

	float GetCone()
	{
		typedef float(__thiscall *GetCone_t)(PVOID);
		return ((GetCone_t)(*(PDWORD)(*(PDWORD)(this) + GETCONEOFFSET)))(this);
	}

	int GetWeaponID()
	{
		typedef int(__thiscall *GetWeaponID_t)(PVOID);
		return ((GetWeaponID_t)(*(PDWORD)(*(PDWORD)(this) + WEAPONIDOFFSET)))(this);
	}

	float GetChargeDamage()
	{
		DYNVAR_RETURN(float, this, "DT_TFSniperRifle", "SniperRifleLocalData", "m_flChargedDamage");
	}
	int GetItemDefinitionIndex()
	{
		DYNVAR_RETURN(int, this, "DT_EconEntity", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
	}
	float m_flLastFireTime()
	{
		DYNVAR_RETURN(float, this, "DT_TFWeaponBase", "LocalActiveTFWeaponData", "m_flLastFireTime");
	}

	bool IsBackstabWeapon(CBaseEntity* pLocal, CBaseCombatWeapon* pWep)
	{
		if (pWep->GetSlot() == 2 && pLocal->GetClassNum() == TF2_Spy)
			return true;

		return false;
	}

	bool IsReadyToBackstab(CBaseEntity* pLocal)
	{
		return IsBackstabWeapon(pLocal, this) && this->CanBackStab();
	}
};

class EngineClient
{
public:
	void GetScreenSize( int& width, int& height )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, int& , int& );
		return getvfunc<OriginalFn>( this, 5 )( this, width, height );
	}
	bool GetPlayerInfo( int ent_num, player_info_t *pinfo )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID, int, player_info_t * );
		return getvfunc<OriginalFn>(this, 8)(this, ent_num, pinfo );
	}
	bool Con_IsVisible( void )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 11 )( this );
	}
	int GetLocalPlayer( void )
	{
		typedef int ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 12 )( this );
	}
	float Time( void )
	{
		typedef float ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 14 )( this );
	}
	void GetViewAngles( Vector& va )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, Vector& va );
		return getvfunc<OriginalFn>( this, 19 )( this, va );
	}
	void SetViewAngles( Vector& va )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, Vector& va );
		return getvfunc<OriginalFn>( this, 20 )( this, va );
	}
	int GetMaxClients( void )
	{
		typedef int ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 21 )( this );
	}
	bool IsInGame( void )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 26 )( this );
	}
	bool IsConnected( void )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 27 )( this );
	}
	bool IsDrawingLoadingImage( void )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 28 )( this );
	}
	const matrix3x4& WorldToScreenMatrix( void )
	{
		typedef const matrix3x4& ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>(this, 36)(this);
	}
	bool IsTakingScreenshot( void )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 85 )( this );
	}
	DWORD* GetNetChannelInfo( void )
	{
		typedef DWORD* ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 72 )( this );
	}
	void ClientCmd_Unrestricted( const char* chCommandString )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, const char * );
		return getvfunc<OriginalFn>( this, 106 )( this, chCommandString );
	}
};

class IPanel
{
public:
	const char *GetName(unsigned int vguiPanel)
	{
		typedef const char* ( __thiscall* OriginalFn )( PVOID, unsigned int );
		return getvfunc<OriginalFn>( this, 36 )( this, vguiPanel );
	}
};

class ISurface
{
public:
	void DrawSetColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc<OriginalFn>(this, 11)(this, r, g, b, a);
	}
	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc<OriginalFn>(this, 12)(this, x0, y0, x1, y1);
	}
	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc<OriginalFn>(this, 14)(this, x0, y0, x1, y1);
	}
	void DrawSetTextFont(unsigned long font)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long);
		getvfunc<OriginalFn>(this, 17)(this, font);
	}
	void DrawSetTextColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc<OriginalFn>(this, 19)(this, r, g, b, a);
	}
	void DrawSetTextPos(int x, int y)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int);
		getvfunc<OriginalFn>(this, 20)(this, x, y);
	}
	void DrawPrintText(const wchar_t *text, int textLen)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, const wchar_t *, int, int);
		return getvfunc<OriginalFn>(this, 22)(this, text, textLen, 0);
	}
	unsigned long CreateFont()
	{
		typedef unsigned int(__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 66)(this);
	}
	void SetFontGlyphSet(unsigned long &font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const char*, int, int, int, int, int, int, int);
		getvfunc<OriginalFn>(this, 67)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}
	void GetTextSize(unsigned long font, const wchar_t *text, int &wide, int &tall)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const wchar_t *, int&, int&);
		getvfunc<OriginalFn>(this, 75)(this, font, text, wide, tall);
	}
};

class CEntList
{
public:
	CBaseEntity* GetClientEntity( int entnum )
	{
		typedef CBaseEntity* ( __thiscall* OriginalFn )( PVOID, int );
		return getvfunc<OriginalFn>( this, 3 )( this, entnum );
	}
	CBaseEntity* GetClientEntityFromHandle( int hEnt )
	{
		typedef CBaseEntity* ( __thiscall* OriginalFn )( PVOID, int );
		return getvfunc<OriginalFn>( this, 4 )( this, hEnt );
	}
	int GetHighestEntityIndex(void)
	{
		typedef int ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 6 )( this );
	}
};

enum playercontrols
{
	IN_ATTACK = (1 << 0),
	IN_JUMP	= (1 << 1),
	IN_DUCK = (1 << 2),
	IN_FORWARD = (1 << 3),
	IN_BACK = (1 << 4),
	IN_USE = (1 << 5),
	IN_CANCEL = (1 << 6),
	IN_LEFT = (1 << 7),
	IN_RIGHT = (1 << 8),
	IN_MOVELEFT = (1 << 9),
	IN_MOVERIGHT = (1 << 10),
	IN_ATTACK2 = (1 << 11),
	IN_RUN = (1 << 12),
	IN_RELOAD = (1 << 13),
	IN_ALT1 = (1 << 14),
	IN_ALT2 = (1 << 15),
	IN_SCORE = (1 << 16),	// Used by client.dll for when scoreboard is held down
	IN_SPEED = (1 << 17),	// Player is holding the speed key
	IN_WALK = (1 << 18),	// Player holding walk key
	IN_ZOOM	= (1 << 19),	// Zoom key for HUD zoom
	IN_WEAPON1 = (1 << 20),	// weapon defines these bits
	IN_WEAPON2 = (1 << 21),	// weapon defines these bits
	IN_BULLRUSH = (1 << 22),
};

enum tf_cond 
{ 
    TFCond_Slowed = (1 << 0), //Toggled when a player is slowed down. 
    TFCond_Zoomed = (1 << 1), //Toggled when a player is zoomed. 
    TFCond_Disguising = (1 << 2), //Toggled when a Spy is disguising.  
    TFCond_Disguised = (1 << 3), //Toggled when a Spy is disguised. 
    TFCond_Cloaked = (1 << 4), //Toggled when a Spy is invisible. 
    TFCond_Ubercharged = (1 << 5), //Toggled when a player is ÜberCharged. 
    TFCond_TeleportedGlow = (1 << 6), //Toggled when someone leaves a teleporter and has glow beneath their feet. 
    TFCond_Taunting = (1 << 7), //Toggled when a player is taunting. 
    TFCond_UberchargeFading = (1 << 8), //Toggled when the ÜberCharge is fading. 
    TFCond_CloakFlicker = (1 << 9), //Toggled when a Spy is visible during cloak. 
    TFCond_Teleporting = (1 << 10), //Only activates for a brief second when the player is being teleported; not very useful. 
    TFCond_Kritzkrieged = (1 << 11), //Toggled when a player is being crit buffed by the KritzKrieg. 
    TFCond_TmpDamageBonus = (1 << 12), //Unknown what this is for. Name taken from the AlliedModders SDK. 
    TFCond_DeadRingered = (1 << 13), //Toggled when a player is taking reduced damage from the Deadringer. 
    TFCond_Bonked = (1 << 14), //Toggled when a player is under the effects of The Bonk! Atomic Punch. 
    TFCond_Stunned = (1 << 15), //Toggled when a player's speed is reduced from airblast or a Sandman ball. 
    TFCond_Buffed = (1 << 16), //Toggled when a player is within range of an activated Buff Banner. 
    TFCond_Charging = (1 << 17), //Toggled when a Demoman charges with the shield. 
    TFCond_DemoBuff = (1 << 18), //Toggled when a Demoman has heads from the Eyelander. 
    TFCond_CritCola = (1 << 19), //Toggled when the player is under the effect of The Crit-a-Cola. 
    TFCond_InHealRadius = (1 << 20), //Unused condition, name taken from AlliedModders SDK. 
    TFCond_Healing = (1 << 21), //Toggled when someone is being healed by a medic or a dispenser. 
    TFCond_OnFire = (1 << 22), //Toggled when a player is on fire. 
    TFCond_Overhealed = (1 << 23), //Toggled when a player has >100% health. 
    TFCond_Jarated = (1 << 24), //Toggled when a player is hit with a Sniper's Jarate. 
    TFCond_Bleeding = (1 << 25), //Toggled when a player is taking bleeding damage. 
    TFCond_DefenseBuffed = (1 << 26), //Toggled when a player is within range of an activated Battalion's Backup. 
    TFCond_Milked = (1 << 27), //Player was hit with a jar of Mad Milk. 
    TFCond_MegaHeal = (1 << 28), //Player is under the effect of Quick-Fix charge. 
    TFCond_RegenBuffed = (1 << 29), //Toggled when a player is within a Concheror's range. 
    TFCond_MarkedForDeath = (1 << 30), //Player is marked for death by a Fan O'War hit. Effects are similar to TFCond_Jarated. 
	TFCond_NoHealingDamageBuff = (1 << 31), //Unknown what this is used for.

    TFCondEx_SpeedBuffAlly = (1 << 0), //Toggled when a player gets hit with the disciplinary action. 
    TFCondEx_HalloweenCritCandy = (1 << 1), //Only for Scream Fortress event maps that drop crit candy. 
	TFCondEx_CritCanteen = (1 << 2), //Player is getting a crit boost from a MVM canteen.
	TFCondEx_CritDemoCharge = (1 << 3), //From demo's shield
	TFCondEx_CritHype = (1 << 4), //Soda Popper crits. 
    TFCondEx_CritOnFirstBlood = (1 << 5), //Arena first blood crit buff. 
    TFCondEx_CritOnWin = (1 << 6), //End of round crits. 
    TFCondEx_CritOnFlagCapture = (1 << 7), //CTF intelligence capture crits. 
    TFCondEx_CritOnKill = (1 << 8), //Unknown what this is for. 
    TFCondEx_RestrictToMelee = (1 << 9), //Unknown what this is for. 
	TFCondEx_DefenseBuffNoCritBlock = ( 1 << 10 ), //MvM Buff.
	TFCondEx_Reprogrammed = (1 << 11), //MvM Bot has been reprogrammed.
    TFCondEx_PyroCrits = (1 << 12), //Player is getting crits from the Mmmph charge. 
    TFCondEx_PyroHeal = (1 << 13), //Player is being healed from the Mmmph charge. 
	TFCondEx_FocusBuff = (1 << 14), //Player is getting a focus buff.
	TFCondEx_DisguisedRemoved = (1 << 15), //Disguised remove from a bot.
	TFCondEx_MarkedForDeathSilent = (1 << 16), //Player is under the effects of the Escape Plan/Equalizer or GRU.
	TFCondEx_DisguisedAsDispenser = (1 << 17), //Bot is disguised as dispenser.
	TFCondEx_Sapped = (1 << 18), //MvM bot is being sapped.
	TFCondEx_UberchargedHidden = (1 << 19), //MvM Related
	TFCondEx_UberchargedCanteen = (1 << 20), //Player is receiving ÜberCharge from a canteen.
	TFCondEx_HalloweenBombHead = (1 << 21), //Player has a bomb on their head from Merasmus.
	TFCondEx_HalloweenThriller = (1 << 22), //Players are forced to dance from Merasmus.
	TFCondEx_BulletCharge = (1 << 26), //Player is receiving 75% reduced damage from bullets.
	TFCondEx_ExplosiveCharge = (1 << 27), //Player is receiving 75% reduced damage from explosives.
	TFCondEx_FireCharge = (1 << 28), //Player is receiving 75% reduced damage from fire.
	TFCondEx_BulletResistance = (1 << 29), //Player is receiving 10% reduced damage from bullets.
	TFCondEx_ExplosiveResistance = (1 << 30), //Player is receiving 10% reduced damage from explosives.
	TFCondEx_FireResistance = (1 << 31), //Player is receiving 10% reduced damage from fire.

	TFCondEx2_Stealthed = (1 << 0),
	TFCondEx2_MedigunDebuff = (1 << 1),
	TFCondEx2_StealthedUserBuffFade = (1 << 2),
	TFCondEx2_BulletImmune = (1 << 3),
	TFCondEx2_BlastImmune = (1 << 4),
	TFCondEx2_FireImmune = (1 << 5),
	TFCondEx2_PreventDeath = (1 << 6),
	TFCondEx2_MVMBotRadiowave = (1 << 7),
	TFCondEx2_HalloweenSpeedBoost = (1 << 8), //Wheel has granted player speed boost.
	TFCondEx2_HalloweenQuickHeal = (1 << 9), //Wheel has granted player quick heal.
	TFCondEx2_HalloweenGiant = (1 << 10), //Wheel has granted player giant mode.
	TFCondEx2_HalloweenTiny = (1 << 11), //Wheel has granted player tiny mode.
	TFCondEx2_HalloweenInHell = (1 << 12), //Wheel has granted player in hell mode.
	TFCondEx2_HalloweenGhostMode = (1 << 13), //Wheel has granted player ghost mode.
	TFCondEx2_Parachute = (1 << 16), //Player has deployed the BASE Jumper.
	TFCondEx2_BlastJumping = (1 << 17), //Player has sticky or rocket jumped.

    TFCond_MiniCrits = ( TFCond_Buffed | TFCond_CritCola ),
    TFCond_IgnoreStates = ( TFCond_Ubercharged | TFCond_Bonked ), 
    TFCondEx_IgnoreStates = ( TFCondEx_PyroHeal ) 
};

class ClientModeShared
{
public:
	bool IsChatPanelOutOfFocus(void)
	{
		typedef PVOID(__thiscall* OriginalFn)(PVOID);
		PVOID CHudChat = getvfunc<OriginalFn>(this, 19)(this);
		if (CHudChat)
		{
			return *(PFLOAT)((DWORD)CHudChat + 0xFC) == 0;
		}
		return false;
	}
};

enum ButtonCode_t
{
	BUTTON_CODE_INVALID = -1,
	BUTTON_CODE_NONE = 0,

	KEY_FIRST = 0,

	KEY_NONE = KEY_FIRST,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_PAD_0,
	KEY_PAD_1,
	KEY_PAD_2,
	KEY_PAD_3,
	KEY_PAD_4,
	KEY_PAD_5,
	KEY_PAD_6,
	KEY_PAD_7,
	KEY_PAD_8,
	KEY_PAD_9,
	KEY_PAD_DIVIDE,
	KEY_PAD_MULTIPLY,
	KEY_PAD_MINUS,
	KEY_PAD_PLUS,
	KEY_PAD_ENTER,
	KEY_PAD_DECIMAL,
	KEY_LBRACKET,
	KEY_RBRACKET,
	KEY_SEMICOLON,
	KEY_APOSTROPHE,
	KEY_BACKQUOTE,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_BACKSLASH,
	KEY_MINUS,
	KEY_EQUAL,
	KEY_ENTER,
	KEY_SPACE,
	KEY_BACKSPACE,
	KEY_TAB,
	KEY_CAPSLOCK,
	KEY_NUMLOCK,
	KEY_ESCAPE,
	KEY_SCROLLLOCK,
	KEY_INSERT,
	KEY_DELETE,
	KEY_HOME,
	KEY_END,
	KEY_PAGEUP,
	KEY_PAGEDOWN,
	KEY_BREAK,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_LALT,
	KEY_RALT,
	KEY_LCONTROL,
	KEY_RCONTROL,
	KEY_LWIN,
	KEY_RWIN,
	KEY_APP,
	KEY_UP,
	KEY_LEFT,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_CAPSLOCKTOGGLE,
	KEY_NUMLOCKTOGGLE,
	KEY_SCROLLLOCKTOGGLE,

	KEY_LAST = KEY_SCROLLLOCKTOGGLE,
	KEY_COUNT = KEY_LAST - KEY_FIRST + 1,

	// Mouse
	MOUSE_FIRST = KEY_LAST + 1,

	MOUSE_LEFT = MOUSE_FIRST,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_4,
	MOUSE_5,
	MOUSE_WHEEL_UP,		// A fake button which is 'pressed' and 'released' when the wheel is moved up 
	MOUSE_WHEEL_DOWN,	// A fake button which is 'pressed' and 'released' when the wheel is moved down

	MOUSE_LAST = MOUSE_WHEEL_DOWN,
	MOUSE_COUNT = MOUSE_LAST - MOUSE_FIRST + 1,
};


class IAppSystem
{
public:
	// Here's where the app systems get to learn about each other
	virtual bool Connect(CreateInterfaceFn factory) = 0;
	virtual void Disconnect() = 0;

	// Here's where systems can access other interfaces implemented by this object
	// Returns NULL if it doesn't implement the requested interface
	virtual void *QueryInterface(const char *pInterfaceName) = 0;

	// Init, shutdown
	virtual int Init() = 0;
	virtual void Shutdown(char* reason) = 0;

};

enum AnalogCode_t //needed but not rly
{

};

class InputEvent_t;

class IInputSystem : public IAppSystem
{
public:
	// Attach, detach input system from a particular window
	// This window should be the root window for the application
	// Only 1 window should be attached at any given time.
	virtual void AttachToWindow(void* hWnd) = 0;
	virtual void DetachFromWindow() = 0;

	// Enables/disables input. PollInputState will not update current 
	// button/analog states when it is called if the system is disabled.
	virtual void EnableInput(bool bEnable) = 0;

	// Enables/disables the windows message pump. PollInputState will not
	// Peek/Dispatch messages if this is disabled
	virtual void EnableMessagePump(bool bEnable) = 0;

	// Polls the current input state
	virtual void PollInputState() = 0;

	// Gets the time of the last polling in ms
	virtual int GetPollTick() const = 0;

	// Is a button down? "Buttons" are binary-state input devices (mouse buttons, keyboard keys)
	virtual bool IsButtonDown(ButtonCode_t code) const = 0;

	// Returns the tick at which the button was pressed and released
	virtual int GetButtonPressedTick(ButtonCode_t code) const = 0;
	virtual int GetButtonReleasedTick(ButtonCode_t code) const = 0;

	// Gets the value of an analog input device this frame
	// Includes joysticks, mousewheel, mouse
	virtual int GetAnalogValue(AnalogCode_t code) const = 0;

	// Gets the change in a particular analog input device this frame
	// Includes joysticks, mousewheel, mouse
	virtual int GetAnalogDelta(AnalogCode_t code) const = 0;

	// Returns the input events since the last poll
	virtual int GetEventCount() const = 0;
	virtual const InputEvent_t* GetEventData() const = 0;

	// Posts a user-defined event into the event queue; this is expected
	// to be called in overridden wndprocs connected to the root panel.
	virtual void PostUserEvent(const InputEvent_t &event) = 0;

	// Returns the number of joysticks
	virtual int GetJoystickCount() const = 0;

	// Enable/disable joystick, it has perf costs
	virtual void EnableJoystickInput(int nJoystick, bool bEnable) = 0;

	// Enable/disable diagonal joystick POV (simultaneous POV buttons down)
	virtual void EnableJoystickDiagonalPOV(int nJoystick, bool bEnable) = 0;

	// Sample the joystick and append events to the input queue
	virtual void SampleDevices(void) = 0;

	// FIXME: Currently force-feedback is only supported on the Xbox 360
	virtual void SetRumble(float fLeftMotor, float fRightMotor, int userId = -1) = 0;
	virtual void StopRumble(void) = 0;

	// Resets the input state
	virtual void ResetInputState() = 0;

	// Sets a player as the primary user - all other controllers will be ignored.
	virtual void SetPrimaryUserId(int userId) = 0;

	// Convert back + forth between ButtonCode/AnalogCode + strings
	virtual const char *ButtonCodeToString(ButtonCode_t code) const = 0;
	virtual const char *AnalogCodeToString(AnalogCode_t code) const = 0;
	virtual ButtonCode_t StringToButtonCode(const char *pString) const = 0;
	virtual AnalogCode_t StringToAnalogCode(const char *pString) const = 0;

	// Sleeps until input happens. Pass a negative number to sleep infinitely
	virtual void SleepUntilInput(int nMaxSleepTimeMS = -1) = 0;

	// Convert back + forth between virtual codes + button codes
	// FIXME: This is a temporary piece of code
	virtual ButtonCode_t VirtualKeyToButtonCode(int nVirtualKey) const = 0;
	virtual int ButtonCodeToVirtualKey(ButtonCode_t code) const = 0;
	virtual ButtonCode_t ScanCodeToButtonCode(int lParam) const = 0;

	// How many times have we called PollInputState?
	virtual int GetPollCount() const = 0;

	// Sets the cursor position
	virtual void SetCursorPosition(int x, int y) = 0;

	// NVNT get address to haptics interface
	virtual void *GetHapticsInterfaceAddress() const = 0;

	virtual void SetNovintPure(bool bPure) = 0;

	// read and clear accumulated raw input values
	virtual bool GetRawMouseAccumulators(int& accumX, int& accumY) = 0;

	// tell the input system that we're not a game, we're console text mode.
	// this is used for dedicated servers to not initialize joystick system.
	// this needs to be called before CInputSystem::Init (e.g. in PreInit of
	// some system) if you want ot prevent the joystick system from ever
	// being initialized.
	virtual void SetConsoleTextMode(bool bConsoleTextMode) = 0;
};
class ICvar : public IAppSystem
{
public:
	// Allocate a unique DLL identifier
	virtual int AllocateDLLIdentifier() = 0;

	// Register, unregister commands
	virtual void			RegisterConCommand(ConCommandBase *pCommandBase) = 0;
	virtual void			UnregisterConCommand(ConCommandBase *pCommandBase) = 0;
	virtual void			UnregisterConCommands(int id) = 0;

	// If there is a +<varname> <value> on the command line, this returns the value.
	// Otherwise, it returns NULL.
	virtual const char*		GetCommandLineValue(const char *pVariableName) = 0;

	// Try to find the cvar pointer by name
	virtual ConCommandBase *FindCommandBase(const char *name) = 0;
	virtual const ConCommandBase *FindCommandBase(const char *name) const = 0;
	virtual ConVar			*FindVar(const char *var_name) = 0;
	virtual const ConVar	*FindVar(const char *var_name) const = 0;
	virtual ConCommand		*FindCommand(const char *name) = 0;
	virtual const ConCommand *FindCommand(const char *name) const = 0;

	// Get first ConCommandBase to allow iteration
	virtual ConCommandBase	*GetCommands(void) = 0;
	virtual const ConCommandBase *GetCommands(void) const = 0;

	// Install a global change callback (to be called when any convar changes)
	virtual void			InstallGlobalChangeCallback(FnChangeCallback_t callback) = 0;
	virtual void			RemoveGlobalChangeCallback(FnChangeCallback_t callback) = 0;
	virtual void			CallGlobalChangeCallbacks(ConVar *var, const char *pOldString, float flOldValue) = 0;

	// Install a console printer
	virtual void			InstallConsoleDisplayFunc(void* pDisplayFunc) = 0;
	virtual void			RemoveConsoleDisplayFunc(void* pDisplayFunc) = 0;
	virtual void			ConsoleColorPrintf(const Color& clr, const char *pFormat, ...) const = 0;
	virtual void			ConsolePrintf(const char *pFormat, ...) const = 0;
	virtual void			ConsoleDPrintf(const char *pFormat, ...) const = 0;

	// Reverts cvars which contain a specific flag
	virtual void			RevertFlaggedConVars(int nFlag) = 0;

	// Method allowing the engine ICvarQuery interface to take over
	// A little hacky, owing to the fact the engine is loaded
	// well after ICVar, so we can't use the standard connect pattern
	virtual void			InstallCVarQuery(void *pQuery) = 0;

#if defined( _X360 )
	virtual void			PublishToVXConsole() = 0;
#endif
	virtual bool			IsMaterialThreadSetAllowed() const = 0;
	virtual void			QueueMaterialThreadSetValue(ConVar *pConVar, const char *pValue) = 0;
	virtual void			QueueMaterialThreadSetValue(ConVar *pConVar, int nValue) = 0;
	virtual void			QueueMaterialThreadSetValue(ConVar *pConVar, float flValue) = 0;
	virtual bool			HasQueuedMaterialThreadConVarSets() const = 0;
	virtual int				ProcessQueuedMaterialThreadConVarSets() = 0;

protected:	class ICVarIteratorInternal;
public:
	/// Iteration over all cvars.
	/// (THIS IS A SLOW OPERATION AND YOU SHOULD AVOID IT.)
	/// usage:
	/// { ICVar::Iterator iter(g_pCVar);
	///   for ( iter.SetFirst() ; iter.IsValid() ; iter.Next() )
	///   {
	///       ConCommandBase *cmd = iter.Get();
	///   }
	/// }
	/// The Iterator class actually wraps the internal factory methods
	/// so you don't need to worry about new/delete -- scope takes care
	//  of it.
	/// We need an iterator like this because we can't simply return a
	/// pointer to the internal data type that contains the cvars --
	/// it's a custom, protected class with unusual semantics and is
	/// prone to change.
	class Iterator
	{
	public:
		inline Iterator(ICvar *icvar);
		inline ~Iterator(void);
		inline void		SetFirst(void);
		inline void		Next(void);
		inline bool		IsValid(void);
		inline ConCommandBase *Get(void);
	private:
		ICVarIteratorInternal *m_pIter;
	};

protected:
	// internals for  ICVarIterator
	class ICVarIteratorInternal
	{
	public:
		// warning: delete called on 'ICvar::ICVarIteratorInternal' that is abstract but has non-virtual destructor [-Wdelete-non-virtual-dtor]
		virtual ~ICVarIteratorInternal()
		{
		}
		virtual void		SetFirst(void) = 0;
		virtual void		Next(void) = 0;
		virtual	bool		IsValid(void) = 0;
		virtual ConCommandBase *Get(void) = 0;
	};

	virtual ICVarIteratorInternal	*FactoryInternalIterator(void) = 0;
	friend class Iterator;
};

class CInterfaces
{
public:
	CEntList* EntList;
	EngineClient* Engine;
	IPanel* Panels;
	ISurface* Surface;
	ClientModeShared* ClientMode;
	CHLClient* Client;
	ICvar* cvar;
	CGlobals* globals;
};

extern CInterfaces gInts;
extern CPlayerVariables gPlayerVars;
extern COffsets gOffsets;