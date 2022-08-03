#pragma once
#include "StdAfx.h"
#include "PCH.h"


class EColorSwitch
    :   public IEntityComponent
{
private:
    bool m_isAlive = false;


protected:
    Cry::DefaultComponents::CInputComponent* objInputPtr = nullptr;


public:
    bool m_switchColor = false;

    #pragma region this object materials
    IMaterial* matBlue = nullptr;
    IMaterial* matRed = nullptr;
    #pragma endregion

public:
    EColorSwitch(/* args */);
    virtual ~EColorSwitch();

    virtual void Initialize() override
    {
        const int geometrySlot = 0;
		m_pEntity->LoadGeometry(geometrySlot, "%ENGINE%/EngineAssets/Objects/primitive_sphere.cgf");

        objInputPtr = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();

        objInputPtr->RegisterAction(
        "others",
        "change",
        [this](int activationMode, float value)
        {
            if (activationMode == eAAM_OnPress)
            {
                // set material and reverse condition
                if (m_switchColor)
                {
                    m_pEntity->SetMaterial(matBlue);
                    m_switchColor = false;
                }
                else
                {
                    m_pEntity->SetMaterial(matRed);
                    m_switchColor = true;
                }

                #ifndef NDEBUG
                CryLog("# EColorSwitch m_switchColor %s", m_switchColor ? "true" : "false");
                #else
                #endif
            }
        });
        objInputPtr->BindAction(
            "others",
            "change",
            eAID_KeyboardMouse,
            EKeyId::eKI_Mouse1);

        matBlue = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial("materials/test-blue");
        matRed = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial("materials/test-red");

        m_pEntity->SetMaterial(matBlue);

        GetEntity()->SetViewDistRatio(255);

        #ifndef NDEBUG
        CryLog("### EColorSwitch Initialize");
        #else
        #endif
    }

    static void ReflectType(Schematyc::CTypeDesc<EColorSwitch>& desc)
    {
        desc.SetGUID("{c8b94193-461d-48f4-9048-5ae12698fbbb}"_cry_guid);
        desc.SetLabel("EColorSwitch");
        desc.SetEditorCategory("_entities");
        desc.SetDescription("Entity test color switch");
    }

    virtual Cry::Entity::EventFlags GetEventMask() const override
    {
        return
            Cry::Entity::EEvent::Reset |
            Cry::Entity::EEvent::Update
            ;
    }
    virtual void ProcessEvent(const SEntityEvent& e) override
    {
        switch (e.event)
        {
            case Cry::Entity::EEvent::Reset:
            {
                m_isAlive = e.nParam[0] != 0;
            }
            break;

            case Cry::Entity::EEvent::Update:
            {
                if (!m_isAlive) return;
            }
            break;
        }
    }

    void ChangeColor()
    {
        if (m_switchColor)
        {
            m_switchColor = false;
            m_pEntity->SetMaterial(matBlue);
        }
        else
        {
            m_switchColor = true;
            m_pEntity->SetMaterial(matRed);
        }
    }
};
