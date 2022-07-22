#include "PMovement.h"


using namespace ceblankfs::players;


#pragma region component and registrar
static void RegisterPMovementComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(PMovement));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPMovementComponent);
PMovement::PMovement(/* args */)
{
}
PMovement::~PMovement()
{
}
#pragma endregion




#pragma region initialize and binding
void PMovement::Initialize()
{
    RegisterPMovementPointer();

    if (GameConfig::IS_DEBUG)
    {
        CryLog("### players::PMovementAndAction::Initialize");
    }
}
#pragma endregion




#pragma region event listener
Cry::Entity::EventFlags PMovement::GetEventMask() const
{
    return
        Cry::Entity::EEvent::Reset |
        Cry::Entity::EEvent::Update
        ;
}


void PMovement::ProcessEvent(const SEntityEvent& e)
{
    switch (e.event)
    {
        case Cry::Entity::EEvent::Reset:
        {
            m_isAlive = e.nParam[0] != 0;

            // log base on game config
            if (GameConfig::IS_DEBUG)
            {
                CryLog("### PMovementAndAction::m_isAlive: %s", m_isAlive ? "true" : "false");
            }
        }
        break;


        case Cry::Entity::EEvent::Update:
        {
            if (!m_isAlive) return;

            const float dt = e.fParam[0]; // delta time/frame time

            GroundMovementHandler(dt);
            GroundJumpHandler(dt);
        }
        break;
    }
}
#pragma endregion




#pragma region functions or methods
void PMovement::RegisterPMovementPointer()
{
    m_pCC = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();

    m_pMAA = m_pEntity->GetOrCreateComponent<ceblankfs::players::PMovement>();

    m_pControllerPtr = m_pEntity->GetOrCreateComponent<ceblankfs::players::PController>();

    m_pDataPtr = m_pEntity->GetOrCreateComponent<ceblankfs::players::PData>();
}


void PMovement::GroundMovementHandler(float dt)
{
    if (!m_pCC->IsOnGround()) return;

    Vec3 velocity = ZERO;

    if (m_pControllerPtr->m_inputFlags & PController::EInputFlag::forward)
    {
        velocity.y += m_pDataPtr->m_movementSpeed * dt;
    }

    if (m_pControllerPtr->m_inputFlags & PController::EInputFlag::backward)
    {
        velocity.y -= m_pDataPtr->m_movementSpeed * dt;
    }

    if (m_pControllerPtr->m_inputFlags & PController::EInputFlag::left)
    {
        velocity.x -= m_pDataPtr->m_movementSpeed * dt;
    }

    if (m_pControllerPtr->m_inputFlags & PController::EInputFlag::right)
    {
        velocity.x += m_pDataPtr->m_movementSpeed * dt;
    }

    if (m_pControllerPtr->m_inputFlags & PController::EInputFlag::sprint)
    {
        m_pCC->AddVelocity(GetEntity()->GetWorldRotation() * (velocity * m_pDataPtr->m_sprintMultiplier));
    }
    else
    {
        m_pCC->AddVelocity(GetEntity()->GetWorldRotation() * velocity);
    }
}


void PMovement::GroundJumpHandler(float dt)
{
    if (!m_pCC->IsOnGround()) return;

    if (m_pControllerPtr->m_inputFlags & PController::EInputFlag::jump)
    {
        m_pDataPtr->m_jumpDurationOnHold += 1.f * dt;

        (m_pDataPtr->m_jumpDurationOnHold > m_pDataPtr->m_minJumpCharge)
            ? m_pDataPtr->m_jumpCharge = m_pDataPtr->m_jumpDurationOnHold
            : m_pDataPtr->m_jumpCharge = 0.f;

        m_canJumpNow = true;
    }
    else
    {
        m_pDataPtr->m_jumpDurationOnHold = 0.f;
    }

    if (m_canJumpNow && m_pDataPtr->m_jumpDurationOnHold == 0.f)
    {
        if (m_pDataPtr->m_jumpCharge == 0.f)
        {
            m_pCC->AddVelocity(Vec3(0, 0, m_pDataPtr->m_jumpForce));

            m_pDataPtr->m_jumpCharge = 0.f;
            m_canJumpNow = false;
        }
        else
        {
            m_pCC->AddVelocity(Vec3(0, 0, (m_pDataPtr->m_jumpForce * m_pDataPtr->m_jumpCharge) * m_pDataPtr->m_jumpChargeMultiplier));

            m_pDataPtr->m_jumpCharge = 0.f;
            m_canJumpNow = false;
        }
    }
}
#pragma endregion
