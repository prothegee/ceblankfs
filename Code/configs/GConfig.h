#pragma once
#include "PCH_CEBLANKFS.h"


/**
 * @brief this game config project class
 * 
 */
class GConfig
    :   public IEntityComponent
{
public:
    static const uint m_major = 0; // major semantic version
    static const uint m_minor = 0; // minor semantic version
    static const uint m_patch = 0; // patch semantic version

public:
    GConfig(/* args */) = default;
    ~GConfig() = default;

    virtual void Initialize() override
    {
        #ifndef NDEBUG
        CryLog("# ceblankfs GConfig::version v.%i.%i.%i", GConfig::m_major, GConfig::m_minor, GConfig::m_patch);
        #else
        #endif
    }

    static void ReflectType(Schematyc::CTypeDesc<GConfig>& desc)
    {
        desc.SetGUID("{42b3f78a-89eb-477e-a23c-36b0e8a54f15}"_cry_guid);
        desc.SetLabel("GConfig");
        desc.SetEditorCategory("_configs");
        desc.SetDescription("ceblankfs game config");
    }
};
