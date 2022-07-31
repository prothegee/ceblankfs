#pragma once
#include "StdAfx.h"
#include "PCH.h"


/**
 * @brief player spawn point
 * 
 */
class PSpawnPoint
    :   public IEntityComponent
{
public:
    PSpawnPoint(/* args */);
    virtual ~PSpawnPoint();

    static void ReflectType(Schematyc::CTypeDesc<PSpawnPoint>& desc)
    {
        desc.SetGUID("{7d0bca14-6b96-4436-af81-169ed1a35fab}"_cry_guid);
        desc.SetLabel("PSpawnPoint");
        desc.SetEditorCategory("_players");
        desc.SetDescription("Player spawn point component");
        desc.SetComponentFlags({
            IEntityComponent::EFlags::Transform,
            IEntityComponent::EFlags::Socket,
            IEntityComponent::EFlags::Attach
        });
    }

    virtual void Initialize() override
    {
        #ifndef NDEBUG
        CryLog("# PSpawnPoint Initialize");
        #else
        #endif
    }

    static Matrix34 GetFirstSpawnPointTranform()
    {
        IEntityItPtr pEntitIterator = gEnv->pEntitySystem->GetEntityIterator();
        pEntitIterator->MoveFirst();

        while (!pEntitIterator->IsEnd())
        {
            IEntity* pEntity = pEntitIterator->Next();

            if (PSpawnPoint* pSpawner = pEntity->GetComponent<PSpawnPoint>())
            {
                return pSpawner->GetWorldTransformMatrix();
            }
        }

        return IDENTITY;
    }
};
