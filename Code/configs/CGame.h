#pragma once
#include "StdAfx.h"


/**
 * @brief config game project class
 * 
 */
class CGame
    :   public IEntityComponent
{
public:
    static const uint m_major = 0; // major semantic version
    static const uint m_minor = 0; // minor semantic version
    static const uint m_patch = 0; // patch semantic version

public:
    CGame(/* args */) = default;
    ~CGame() = default;

    virtual void Initialize() override
    {
        #ifndef NDEBUG
        CryLog("# ceblankfs GConfig::version v.%i.%i.%i", CGame::m_major, CGame::m_minor, CGame::m_patch);
        #else
        #endif
    }

    static void ReflectType(Schematyc::CTypeDesc<CGame>& desc)
    {
        desc.SetGUID("{42b3f78a-89eb-477e-a23c-36b0e8a54f15}"_cry_guid);
        desc.SetLabel("CGame");
        desc.SetEditorCategory("_configs");
        desc.SetDescription("ceblankfs game config");
    }
};
