#include "precompiled.h"

CSpread gSpread;

void CSpread::ServerActivate()
{
    // Activate Debug Mode
    //
    // 0 Disabled
    // 1 Enabled
    //
    // Default "1"
    // sc_debug "0"
    static cvar_t tDebug = {"sc_debug", "0", FCVAR_SERVER, 0.0f, NULL};

    // Activate Plugin
    //
    // 0 Disabled
    // 1 Enabled
    //
    // Default "1"
    // sc_active "1"
    static cvar_t tActive = {"sc_active", "1", FCVAR_SERVER, 1.0f, NULL};

    // Check if the player is on ground to apply fix
    //
    // 0 Disable Check
    // 1 Enable Check
    //
    // Default "1"
    // sc_ground_check "1"
    static cvar_t tGroundCheck = {"sc_ground_check", "1", FCVAR_SERVER, 1.0f, NULL};

    // Maximum player speed divisor:
    // Checks if the player moves above the speed:
    // Formula of this: PLAYER_VELOCITY <= (WEAPON_MAX_SPEED / sc_max_speed)
    // Weapons Table: https://wiki.alliedmods.net/CS_Weapons_Information
    //
    // > 0.0 (Greater than zero activates the check)
    //
    // Default "2.0"
    // sc_max_speed "2.0"
    static cvar_t tMaxSpeed = {"sc_max_speed", "2.0", FCVAR_SERVER, 2.0f, NULL};

    // Maximum player punch angle while shotting:
    // The first shot always have 0.0 (Zero) of punch angle
    // The second shot have other value deppending each weapon
    // 
    // >= 0.0 (Greater than or equal to zero activates the check)
    // <  0.0 (Less than zero disables checking)
    //
    // Default "2.0"
    // sc_max_punch_angle "2.0"
    static cvar_t tMaxPunchAngle = {"sc_max_punch_angle", "0.0", FCVAR_SERVER, 0.0f, NULL};

    // Spread value applied when all checks has passed
    //
    // >= 0.0 (Greater than or equal to zero enables the adjustment)
    // <  0.0 (Less than zero disables adjustment)
    //
    // Default "0.0"
    // sc_spread "0.0"
    static cvar_t tSpread = {"sc_spread", "0.0", FCVAR_SERVER, 0.0f, NULL};

    // Weapons blocked from spread control fix
    //
    // To block weapon: Put 1 on weapon position
    // To allow weapon: Put 0 on weapon slot
    //
	// 0 Not Used
	// 1 P228
	// 2 GLOCK
	// 3 SCOUT
	// 4 HEGRENADE
	// 5 XM1014
	// 6 C4
	// 7 MAC10
	// 8 AUG
	// 9 SMOKEGRENADE
	// 10 ELITE
	// 11 FIVESEVEN
	// 12 UMP45
	// 13 SG550
	// 14 GALIL
	// 15 FAMAS
	// 16 USP
	// 17 GLOCK18
	// 18 AWP
	// 19 MP5N
	// 20 M249
	// 21 M3
	// 22 M4A1
	// 23 TMP
	// 24 G3SG1
	// 25 FLASHBANG
	// 26 DEAGLE
	// 27 SG552
	// 28 AK47
	// 29 KNIFE
	// 30 P90
    //
    // Default "000000000000000000000000000000"
    // sc_weapons_block "000000000000000000000000000000"
    static cvar_t tWeapons = {"sc_weapons_block", "000000000000000000000000000000", FCVAR_SERVER, 0.0f, NULL};

    g_engfuncs.pfnCVarRegister(&tDebug);
    this->m_Debug = g_engfuncs.pfnCVarGetPointer(tDebug.name);

    g_engfuncs.pfnCVarRegister(&tActive);
    this->m_Active = g_engfuncs.pfnCVarGetPointer(tActive.name);

    g_engfuncs.pfnCVarRegister(&tGroundCheck);
    this->m_GroundCheck = g_engfuncs.pfnCVarGetPointer(tGroundCheck.name);

    g_engfuncs.pfnCVarRegister(&tMaxSpeed);
    this->m_MaxSpeed = g_engfuncs.pfnCVarGetPointer(tMaxSpeed.name);

    g_engfuncs.pfnCVarRegister(&tMaxPunchAngle);
    this->m_MaxPunchAngle = g_engfuncs.pfnCVarGetPointer(tMaxPunchAngle.name);

    g_engfuncs.pfnCVarRegister(&tSpread);
    this->m_Spread = g_engfuncs.pfnCVarGetPointer(tSpread.name);

    g_engfuncs.pfnCVarRegister(&tWeapons);
    this->m_Weapons = g_engfuncs.pfnCVarGetPointer(tWeapons.name);
}

float CSpread::GetSpread(CBaseEntity *pEntity, Vector &vecSrc, Vector &vecDirShooting, float vecSpread, float flDistance, int iPenetration, int iBulletType, int iDamage, float flRangeModifier, entvars_t *pevAttacker, bool bPistol, int shared_rand)
{
    if (this->m_Active)
    {
        if (this->m_Active->value > 0.0f)
        {
            auto Player = static_cast<CBasePlayer*>(pEntity);

            if (Player)
            {
                bool DebugMode = (this->m_Debug && this->m_Debug->value > 0.0f);

                // If player is not on ground, block spread fix
                if (this->m_GroundCheck)
                {
                    if (this->m_GroundCheck->value > 0.0f)
                    {
                        if (DebugMode)
                        {
                            this->ClientPrint
                            (
                                Player->edict(),
                                E_PRINT::CHAT,
                                "[%s] Ground Check: %2.2f (OnGround: %s) (Pass: %s)",
                                Plugin_info.logtag,
                                this->m_GroundCheck->value,
                                (Player->pev->flags & FL_ONGROUND) ? "Yes" : "No",
                                (Player->pev->flags & FL_ONGROUND) ? "Yes" : "No"
                            );
                        }

                        if (!(Player->pev->flags & FL_ONGROUND))
                        {
                            return vecSpread;
                        }
                    }
                }

                // If player is moving: Block if player speed is greater than: (weapon-max-speed / max-speed-cvar-value)
                if (this->m_MaxSpeed)
                {
                    if (this->m_MaxSpeed->value > 0.0f)
                    {
                        if (Player->pev->maxspeed > 0.0f)
                        {
                            if (DebugMode)
                            {
                                this->ClientPrint
                                (
                                    Player->edict(),
                                    E_PRINT::CHAT,
                                    "[%s] Max Speed Divisor: %2.2f (Player Velocity: %2.2f) (MaxSpeed: %2.2f) (Pass: %s)",
                                    Plugin_info.logtag,
                                    this->m_MaxSpeed->value,
                                    Player->pev->velocity.Length2D(),
                                    Player->pev->maxspeed,
                                    (Player->pev->velocity.Length2D() <= (Player->pev->maxspeed / this->m_MaxSpeed->value)) ? "Yes" : "No"
                                );
                            }

                            if (Player->pev->velocity.Length2D() > (Player->pev->maxspeed / this->m_MaxSpeed->value))
                            {
                                return vecSpread;
                            }
                        }
                    }
                }

                // If player has fired: Do not remove spread if player is shoting a lot
                if (this->m_MaxPunchAngle)
                {
                    if (this->m_MaxPunchAngle->value >= 0.0f)
                    {
                        if (DebugMode)
                        {
                            this->ClientPrint
                            (
                                Player->edict(),
                                E_PRINT::CHAT,
                                "[%s] Max Punch Angle: %2.2f (Player Punch Angle: %2.2f) (Pass: %s)",
                                Plugin_info.logtag,
                                this->m_MaxPunchAngle->value,
                                Player->pev->punchangle.Length2D(),
                                (Player->pev->punchangle.Length2D() <= this->m_MaxPunchAngle->value) ? "Yes" : "No"
                            );
                        }
                        
                        if (Player->pev->punchangle.Length2D() > this->m_MaxPunchAngle->value)
                        {
                            return vecSpread;
                        }
                    }
                }

                // Check Blocked Weapons
                if (this->m_Weapons)
                {
                    if (this->m_Weapons->string[0u] != '\0')
                    {
                        if (Player->m_pActiveItem && Player->m_pActiveItem->m_iId >= WEAPON_P228 && Player->m_pActiveItem->m_iId <= WEAPON_P90)
                        {
                            if (DebugMode)
                            {
                                this->ClientPrint
                                (
                                    Player->edict(),
                                    E_PRINT::CHAT,
                                    "[%s] Weapon Position: %d (Is Blocked: %s)",
                                    Plugin_info.logtag,
                                    Player->m_pActiveItem->m_iId,
                                    (this->m_Weapons->string[Player->m_pActiveItem->m_iId] != '0') ? "Yes" : "No"
                                );
                            }
                            
                            if (this->m_Weapons->string[Player->m_pActiveItem->m_iId] != '0')
                            {
                                return vecSpread;
                            }
                        }
                    }
                }

                // Do not remove spred if player is not aimming his weapons like AWP, SCOUT or other
                if (!Player->m_bResumeZoom && Player->m_pActiveItem)
                {
                    if (Player->m_pActiveItem->m_iId == WEAPON_SCOUT || Player->m_pActiveItem->m_iId == WEAPON_SG550 || Player->m_pActiveItem->m_iId == WEAPON_AWP || Player->m_pActiveItem->m_iId == WEAPON_G3SG1)
                    {
                        return vecSpread;
                    }
                }

                // Spread value to return
                if (this->m_Spread)
                {
                    if (this->m_Spread->value >= 0.0f)
                    {
                        return this->m_Spread->value;
                    }
                }
            }
        }
    }

    return vecSpread;
}

void CSpread::ClientPrint(edict_t *pEntity, int iMsgDest, const char *Format, ...)
{
	va_list argList;

	va_start(argList, Format);

	char Buffer[188] = { 0 };

	int Length = vsnprintf(Buffer, sizeof(Buffer), Format, argList);

	va_end(argList);

	if (iMsgDest == E_PRINT::CONSOLE)
	{
		if (Length > 125)
		{
			Length = 125;
		}

		Buffer[Length++] = '\n';
		Buffer[Length++] = '\n';
		Buffer[Length] = 0;
	}

	static int iMsgTextMsg;

	if (iMsgTextMsg || (iMsgTextMsg = gpMetaUtilFuncs->pfnGetUserMsgID(PLID, "TextMsg", NULL)))
	{
		if (!FNullEnt(pEntity))
		{
			g_engfuncs.pfnMessageBegin(MSG_ONE, iMsgTextMsg, nullptr, pEntity);
		}
		else
		{
			g_engfuncs.pfnMessageBegin(MSG_BROADCAST, iMsgTextMsg, nullptr, nullptr);
		}

		g_engfuncs.pfnWriteByte(iMsgDest);
		g_engfuncs.pfnWriteString("%s");
		g_engfuncs.pfnWriteString(Buffer);
		g_engfuncs.pfnMessageEnd();
	}
}
