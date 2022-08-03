#pragma once
#include "StdAfx.h"
#include "PCH.h"


/**
 * @brief system global environment in game class to manage & control in game environment
 * 
 */
class SGlobalEnvironment
    :   public IEntityComponent
{
public:
    SGlobalEnvironment(/* args */);
    virtual ~SGlobalEnvironment();

    static void ReflectType(Schematyc::CTypeDesc<SGlobalEnvironment>& desc)
    {
        desc.SetGUID("{08ddd93d-ae33-4735-bf8f-d3fb13a1abaf}"_cry_guid);
        desc.SetLabel("SGlobalEnvironment");
        desc.SetEditorCategory("_systems");
        desc.SetDescription("System global environment in game manager");
    }

    virtual void Initialize() override
    {
        #ifndef NDEBUG
        CryLog("# SGlobalEnvironment Initialize");
        #else
        #endif
    }


protected:
    // 
};
