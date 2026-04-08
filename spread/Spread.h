#pragma once

enum E_PRINT
{
    NOTIFY = 1,
    CONSOLE = 2,
    CHAT = 3,
    CENTER = 4,
    RADIO = 5
};

class CSpread
{
public:
    void ServerActivate();
    float GetSpread(CBaseEntity *pEntity, Vector &vecSrc, Vector &vecDirShooting, float vecSpread, float flDistance, int iPenetration, int iBulletType, int iDamage, float flRangeModifier, entvars_t *pevAttacker, bool bPistol, int shared_rand);
    void ClientPrint(edict_t *pEntity, int iMsgDest, const char *Format, ...);

    cvar_t* m_Debug         = nullptr;
    cvar_t* m_Active        = nullptr;
    cvar_t* m_GroundCheck   = nullptr;
    cvar_t* m_MaxSpeed      = nullptr;
    cvar_t* m_MaxPunchAngle = nullptr;
    cvar_t* m_Spread        = nullptr;
    cvar_t* m_Weapons       = nullptr;  // was missing = nullptr — uninitialized pointer
                                        //      could cause UB on any read before ServerActivate()
};

extern CSpread gSpread;
