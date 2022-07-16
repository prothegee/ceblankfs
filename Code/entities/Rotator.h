#pragma once
#include "StdAfx.h"
#include "GameConfig.h"


// #include <CryEntitySystem/IEntityComponent.h>


namespace blankfs
{
namespace entities
{

/**
 * @brief entity rotator component class
 * 
 */
class Rotator
    : public IEntityComponent
{
public:
    // default value for Rotator
    struct DVRotator
    {
        static constexpr bool isRotating = false; // default value for do rotate
        static constexpr float rotationX = 1.f; // default value for rotation x axis
        static constexpr float rotationY = 1.f; // default value for rotation y axis
        static constexpr float rotationZ = 1.f; // default value for rotation z axis
    };

    bool m_isRotating = DVRotator::isRotating; // object status is rotating or not
    float m_rotationX = DVRotator::rotationX; // object rotation x axis value
    float m_rotationY = DVRotator::rotationY; // object rotation y axis value
    float m_rotationZ = DVRotator::rotationZ; // object rotation z axis value

public:
    Rotator(/* args */);
    virtual ~Rotator();


    virtual void Initialize() override;


    virtual Cry::Entity::EventFlags GetEventMask() const override;
    virtual void ProcessEvent(const SEntityEvent& e) override;


    static void ReflectType(Schematyc::CTypeDesc<Rotator>& desc)
    {
        desc.SetGUID("{62e968de-c441-4e8c-9f89-032ee6fb59b8}"_cry_guid);
        desc.SetLabel("Rotator");
        desc.SetEditorCategory("_entities");
        desc.SetDescription("Entity rotator for objects");
        desc.SetComponentFlags({
            IEntityComponent::EFlags::Transform,
            IEntityComponent::EFlags::Socket,
            IEntityComponent::EFlags::Attach
        });
        desc.AddMember(
            &Rotator::m_rotationX,
            'rvlx',
            "rotationX",
            "rotation x",
            "rotation x value",
            DVRotator::rotationX
        );
        desc.AddMember(
            &Rotator::m_rotationY,
            'rvly',
            "rotationY",
            "rotation y",
            "rotation y value",
            DVRotator::rotationY
        );
        desc.AddMember(
            &Rotator::m_rotationZ,
            'rvlz',
            "rotationZ",
            "rotation z",
            "rotation z value",
            DVRotator::rotationZ
        );
    }
};


} // namespace entities
} // namespace blankfs