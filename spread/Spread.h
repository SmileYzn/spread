#pragma once

class CSpread
{
public:
    void ServerActivate();
    float GetSpread(CBaseEntity *pEntity, Vector &vecSrc, Vector &vecDirShooting, float vecSpread, float flDistance, int iPenetration, int iBulletType, int iDamage, float flRangeModifier, entvars_t *pevAttacker, bool bPistol, int shared_rand);

    cvar_t* m_Active = nullptr;
};

extern CSpread gSpread;
