#pragma once
#include "StdAfx.h"
#include "PCH.h"


/**
 * @brief system graphic manager class
 * 
 * @helper:
 * - https://docs.cryengine.com/display/CEPROG/CVar+Tutorial
 * - https://docs.cryengine.com/pages/viewpage.action?pageId=15010931
 * 
 */
class SGraphicManager
    :   public IEntityComponent
{
public:
    SGraphicManager(/* args */);
    virtual ~SGraphicManager();

    virtual void Initialize() override
    {
        #ifndef NDEBUG
        CryLog("# SGraphicManager Initialize");
        #else
        #endif
    }

    static void ReflectType(Schematyc::CTypeDesc<SGraphicManager>& desc)
    {
        desc.SetGUID("{044e18f6-3e72-4105-99eb-27e30cc7aa06}"_cry_guid);
        desc.SetLabel("SGraphicManager");
        desc.SetEditorCategory("_systems");
        desc.SetDescription("System graphic manager");
    }

#pragma region public graphic manager functions
    /**
     * set graphic system base on int param through pConsole
     * 
     * min 0
     * max 7
     * 
     * ---
     * 
     * 0 = custom
     * 1 = low
     * 2 = med
     * 3 = high
     * 4 = very hight
     * 5 = Xbox One
     * 6 = Xbox One X
     * 7 = PS4
     * 
     * ---
     * 
     * param num - int
     * 
     */
    static void SetGraphycSys(int& num)
    {
        switch (num)
        {
            case 0:
            {
                gEnv->pConsole->ExecuteString("sys_spec 0", false, false);
            }
            break;

            case 1:
            {
                gEnv->pConsole->ExecuteString("sys_spec 1", false, false);
            }
            break;

            case 2:
            {
                gEnv->pConsole->ExecuteString("sys_spec 2", false, false);
            }
            break;

            case 3:
            {
                gEnv->pConsole->ExecuteString("sys_spec 3", false, false);
            }
            break;

            case 4:
            {
                gEnv->pConsole->ExecuteString("sys_spec 4", false, false);
            }
            break;

            case 5:
            {
                gEnv->pConsole->ExecuteString("sys_spec 5", false, false);
            }
            break;

            case 6:
            {
                gEnv->pConsole->ExecuteString("sys_spec 6", false, false);
            }
            break;

            case 7:
            {
                gEnv->pConsole->ExecuteString("sys_spec 7", false, false);
            }
            break;

            default:
            {
                CryLog("# SGraphicManager: Unknown sys_spec");
            }
            break;
        }
    }
#pragma endregion


protected:
    // 
};
