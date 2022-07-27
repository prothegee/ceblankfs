#pragma once

#include "PCH_CEBLANKFS.h"


/**
 * @brief entity rotator class
 * 
 */
class ERotator
    :   public IEntityComponent
{
public:
    // default value for Rotator
    struct DVERotator
    {
        static constexpr bool isRotating = false; // default value for do rotate
        static constexpr float rotationX = 1.f; // default value for rotation x axis
        static constexpr float rotationY = 1.f; // default value for rotation y axis
        static constexpr float rotationZ = 1.f; // default value for rotation z axis
    };

    bool m_isRotating = DVERotator::isRotating; // object status is rotating or not
    float m_rotationX = DVERotator::rotationX; // object rotation x axis value
    float m_rotationY = DVERotator::rotationY; // object rotation y axis value
    float m_rotationZ = DVERotator::rotationZ; // object rotation z axis value

public:
    ERotator(/* args */);
    virtual ~ERotator();

    virtual void Initialize() override
    {
        #ifndef NDEBUG
        CryLog("### ERotator Initialize");
        #endif
    }

    static void ReflectType(Schematyc::CTypeDesc<ERotator>& desc)
    {
        desc.SetGUID("{6705341c-f532-444b-84c7-b73fc3a9ed4f}"_cry_guid);
        desc.SetLabel("ERotator");
        desc.SetEditorCategory("_entities");
        desc.SetDescription("Entity rotator for object");
        desc.AddMember(
            &ERotator::m_rotationX,
            'rvlx',
            "rotation-x",
            "rotation x",
            "rotation x value",
            DVERotator::rotationX
        );
        desc.AddMember(
            &ERotator::m_rotationY,
            'rvly',
            "rotation-y",
            "rotation y",
            "rotation y value",
            DVERotator::rotationY
        );
        desc.AddMember(
            &ERotator::m_rotationZ,
            'rvlz',
            "rotation-z",
            "rotation z",
            "rotation z value",
            DVERotator::rotationZ
        );
    }

    virtual Cry::Entity::EventFlags GetEventMask() const override
    {
        return
            Cry::Entity::EEvent::GameplayStarted |
            Cry::Entity::EEvent::Reset |
            Cry::Entity::EEvent::Update
            ;
    }
    virtual void ProcessEvent(const SEntityEvent& e) override
    {
        switch (e.event)
        {
            case Cry::Entity::EEvent::GameplayStarted:
            {
                (!m_isRotating) // force to rotate only on game mode
                    ? m_isRotating = true
                    : m_isRotating = this->m_isRotating;
            }
            break;

            case Cry::Entity::EEvent::Reset:
            {
                m_isRotating = e.nParam[0] != 0;
            }
            break;

            case Cry::Entity::EEvent::Update:
            {
                if (!m_isRotating) return;

                const float dt = e.fParam[0];

                Ang3 ar = ZERO;

                ar.x += m_rotationX * dt;
                ar.y += m_rotationY * dt;
                ar.z += m_rotationZ * dt;

                auto r = GetEntity()->GetRotation() * Quat::CreateRotationXYZ(ar);

                m_pEntity->SetRotation(r);
            }
            break;
        }
    }
};
