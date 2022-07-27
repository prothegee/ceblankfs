#pragma once
#include "PCH_CEBLANKFS.h"


/**
 * @brief core player projectile header
 * 
 */
class CProjectile
    :   public IEntityComponent
{
public:
    CProjectile(/* args */);
    virtual ~CProjectile();

    virtual void Initialize() override
    {
        #ifndef NDEBUG
        CryLog("# CProjectile::Initialize");
        #else
        #endif
    }

    static void ReflectType(Schematyc::CTypeDesc<CProjectile>& desc)
    {
        desc.SetGUID("{a372a85f-4956-45a2-849f-776fec044fd2}"_cry_guid);
        desc.SetLabel("CProjectile");
        desc.SetEditorCategory("_players");
        desc.SetDescription("player projectile entity");
    }


protected:
    // handler on launch
    // handler on impact
};
