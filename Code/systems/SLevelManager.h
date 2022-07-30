#pragma once
#include "StdAfx.h"

#include <CryAction.h>
#include <LevelSystem.h>


/**
 * @brief system level manager class
 * 
 */
class SLevelManager
    :   public IEntityComponent
{
public:
    SLevelManager(/* args */);
    virtual ~SLevelManager();

    static void ReflectType(Schematyc::CTypeDesc<SLevelManager>& desc)
    {
        desc.SetGUID("{fe61b671-9519-4624-af40-eab269c6a500}"_cry_guid);
        desc.SetLabel("SLevelManager");
        desc.SetEditorCategory("_systems");
        desc.SetDescription("System level manager");
    }

    virtual void Initialize() override
    {
        #ifndef NDEBUG
        CryLog("# SLevelManager Initialize");
        #else
        #endif
    }

#pragma region public level manager functions
    // do something on level load start
    static void DSOLLS()
    {
        #ifndef NDEBUG
        CryLog("# SLevelManager DSOLLS");
        #else
        #endif
    }

    // do something on level load end
    static void DSOLLE()
    {
        #ifndef NDEBUG
        CryLog("# SLevelManager DSOLLE:");
        #else
        #endif
    }

    // get current level name
    const static string CurrentLevel()
    {
        return gEnv->pConsole->GetCVar("sv_map")->GetString();
    }
#pragma endregion


protected:
    // 
};
