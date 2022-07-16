#include "Rotator.h"


// #include <CryCore/StaticInstanceList.h>
// #include <CrySchematyc/Env/IEnvRegistrar.h>
// #include <CrySchematyc/Env/Elements/EnvComponent.h>


using namespace ceblankfs::entities;


static void RegisterRotatorEntity(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(
            SCHEMATYC_MAKE_ENV_COMPONENT(Rotator)); // EnvComponent.h
    } // IEnvRegistrar.h
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterRotatorEntity); // StaticInstanceList.h
Rotator::Rotator(/* args */)
{
}
Rotator::~Rotator()
{
}


void Rotator::Initialize()
{
    #ifndef NDEBUG
    CryLog("### entities::Rotator Initialize");
    #endif
}


#pragma region event listener
Cry::Entity::EventFlags Rotator::GetEventMask() const
{
    return
        Cry::Entity::EEvent::Reset |
        Cry::Entity::EEvent::Update
        ;
}

void Rotator::ProcessEvent(const SEntityEvent& e)
{
    switch (e.event)
    {
        case Cry::Entity::EEvent::Reset:
        {
            m_isRotating = e.nParam[0] != 0;

            #ifndef NDEBUG
            CryLog("### entities::Rotator event: Reset");
            CryLog("### m_isRotating: %s", m_isRotating ? "true" : "false");
            #endif
        }
        break;


        case Cry::Entity::EEvent::Update:
        {
            if (!m_isRotating) return;

            const float dt = e.fParam[0]; // deltatime or frametime

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
#pragma endregion