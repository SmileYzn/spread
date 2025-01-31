#include "precompiled.h"

CSpread gSpread;

void CSpread::ServerActivate()
{
    static cvar_t tActive = {"sc_active", strdup("1"), FCVAR_SERVER | FCVAR_PROTECTED, 2.0f, NULL};

    g_engfuncs.pfnCVarRegister(&tActive);

    this->m_Active = g_engfuncs.pfnCVarGetPointer(tActive.name);
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
                if (Player->pev->flags & FL_ONGROUND)
                {
                    if (Player->pev->velocity.Length2D() < (Player->pev->maxspeed / 2.0f))
                    {
                        if (Player->pev->punchangle.IsZero())
                        {
                            if (!Player->m_bResumeZoom)
                            {
                                if (Player->m_pActiveItem->m_iId == WEAPON_SCOUT || Player->m_pActiveItem->m_iId == WEAPON_SG550 || Player->m_pActiveItem->m_iId == WEAPON_AWP || Player->m_pActiveItem->m_iId == WEAPON_G3SG1)
                                {
                                    return vecSpread;
                                }
                            }

                            return 0.0f;
                        }
                    }
                }
            }
        }
    }

    return vecSpread;
}
