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
            if (pEntity)
            {
                if (pEntity->pev->flags & FL_ONGROUND)
                {
                    if (pEntity->pev->velocity.Length2D() < (pEntity->pev->maxspeed / 2.0f))
                    {
                        if (pEntity->pev->punchangle.IsZero())
                        {
                            if (pEntity->pev->fov == DEFAULT_FOV)
                            {
                                if (bPistol)
                                {
                                    if (flDistance > 4096.0f)
                                    {
                                        return vecSpread;
                                    }
                                }
                            }

                            return 0.0f;
                        }
                    }
                }
        }
    }

    return vecSpread;
}
