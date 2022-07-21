#include "CPMovement.h"


using namespace ceblankfs::players;


#pragma region component and registrar
static void RegisterCPMovementComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPMovement));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterCPMovementComponent);
CPMovement::CPMovement(/* args */)
{
}
CPMovement::~CPMovement()
{
}
#pragma endregion




#pragma region initialize and binding
void CPMovement::Initialize()
{
    RegisterCPMovementPointer();

    if (GameConfig::IS_DEBUG)
    {
        CryLog("### players::CPMovementAndAction::Initialize");
    }
}
#pragma endregion




#pragma region event listener
Cry::Entity::EventFlags CPMovement::GetEventMask() const
{
    return
        Cry::Entity::EEvent::Reset |
        Cry::Entity::EEvent::Update
        ;
}


void CPMovement::ProcessEvent(const SEntityEvent& e)
{
    switch (e.event)
    {
        case Cry::Entity::EEvent::Reset:
        {
            m_isAlive = e.nParam[0] != 0;

            // log base on game config
            if (GameConfig::IS_DEBUG)
            {
                CryLog("### CPMovementAndAction::m_isAlive: %s", m_isAlive ? "true" : "false");
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
void CPMovement::RegisterCPMovementPointer()
{
    m_pCC = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();

    m_pMAA = m_pEntity->GetOrCreateComponent<ceblankfs::players::CPMovement>();

    m_pControllerPtr = m_pEntity->GetOrCreateComponent<ceblankfs::players::CPController>();

    m_pDataPtr = m_pEntity->GetOrCreateComponent<ceblankfs::players::CPData>();
}


void CPMovement::GroundMovementHandler(float dt)
{
    if (!m_pCC->IsOnGround()) return;

    Vec3 velocity = ZERO;

    if (m_pControllerPtr->m_inputFlags & CPController::EInputFlag::forward)
    {
        velocity.y += m_pDataPtr->m_movementSpeed * dt;
    }

    if (m_pControllerPtr->m_inputFlags & CPController::EInputFlag::backward)
    {
        velocity.y -= m_pDataPtr->m_movementSpeed * dt;
    }

    if (m_pControllerPtr->m_inputFlags & CPController::EInputFlag::left)
    {
        velocity.x -= m_pDataPtr->m_movementSpeed * dt;
    }

    if (m_pControllerPtr->m_inputFlags & CPController::EInputFlag::right)
    {
        velocity.x += m_pDataPtr->m_movementSpeed * dt;
    }

    if (m_pControllerPtr->m_inputFlags & CPController::EInputFlag::sprint)
    {
        m_pCC->AddVelocity(GetEntity()->GetWorldRotation() * (velocity * m_pDataPtr->m_sprintMultiplier));
    }
    else
    {
        m_pCC->AddVelocity(GetEntity()->GetWorldRotation() * velocity);
    }
}


void CPMovement::GroundJumpHandler(float dt)
{
    if (!m_pCC->IsOnGround()) return;

    if (m_pControllerPtr->m_inputFlags & CPController::EInputFlag::jump)
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
