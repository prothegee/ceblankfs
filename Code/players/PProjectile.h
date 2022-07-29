#pragma once
#include "StdAfx.h"


/**
 * @brief POSTPONE: player projectile
 * 
 */
class PProjectile
    :   public IEntityComponent
{
public:
    PProjectile(/* args */);
    virtual ~PProjectile();

    virtual void Initialize() override
    {
        #ifndef NDEBUG
        CryLog("# PProjectile::Initialize");
        #else
        #endif
    }

    static void ReflectType(Schematyc::CTypeDesc<PProjectile>& desc)
    {
        desc.SetGUID("{a372a85f-4956-45a2-849f-776fec044fd2}"_cry_guid);
        desc.SetLabel("PProjectile");
        desc.SetEditorCategory("_players");
        desc.SetDescription("player projectile entity");
    }


protected:
    // handler on launch
    // handler on impact
};
