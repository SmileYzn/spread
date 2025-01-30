#include "precompiled.h"

CSpread gSpread;

void CSpread::ServerActivate()
{
    static cvar_t tActive = {"sc_active", strdup("1"), FCVAR_SERVER | FCVAR_PROTECTED, 2.0f, NULL};

    g_engfuncs.pfnCVarRegister(&tActive);

    this->m_Active = g_engfuncs.pfnCVarGetPointer(tActive.name);
}

float CSpread::GetSpread(CBasePlayer *Player, float vecSpread)
{
    if (this->m_Active)
    {
        if (this->m_Active->value > 0.0f)
        {
            if (Player->m_pActiveItem)
            {
                if (Player->pev->flags & FL_ONGROUND)
                {
                    if (Player->pev->punchangle.IsZero())
                    {
                        if (Player->pev->velocity.Length2D() < (Player->m_pActiveItem->GetMaxSpeed() / 2.0f))
                        {
                            vecSpread = 0.0f;
                        }
                    }
                }
            }
        }
    }

    return vecSpread;
}
