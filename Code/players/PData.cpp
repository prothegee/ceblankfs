#include "PData.h"


using namespace ceblankfs::players;


#pragma region component and registrar
static void RegisterPDataComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(PData));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPDataComponent);
PData::PData(/* args */)
{
}
PData::~PData()
{
}
#pragma endregion




#pragma region initialize and binding
void PData::Initialize()
{
    RegisterPDataPointer();
    RegisterPDataDefaultValue();

    if (GameConfig::IS_DEBUG)
    {
        CryLog("### players::PData::Initialize");
    }
}
#pragma endregion




#pragma region event listener
Cry::Entity::EventFlags PData::GetEventMask() const
{
    return
        Cry::Entity::EEvent::GameplayStarted |
        Cry::Entity::EEvent::Reset |
        Cry::Entity::EEvent::Update
        ;
}


void PData::ProcessEvent(const SEntityEvent& e)
{
    switch (e.event)
    {
        case Cry::Entity::EEvent::GameplayStarted:
        {
            // log base on game config
            if (GameConfig::IS_DEBUG)
            {
                CryLog("### PData::ProcessEvent::GameplayStarted");
                CryLog("### PData username: %s", m_username.c_str());
                CryLog("### PData character name: %s", m_charactername.c_str());
            }
        }
        break;


        case Cry::Entity::EEvent::Reset:
        {
            m_isAlive = e.nParam[0] != 0;

            // log base on game config
            if (GameConfig::IS_DEBUG)
            {
                CryLog("### PData::m_isAlive: %s", m_isAlive ? "true" : "false");
            }
        }
        break;


        case Cry::Entity::EEvent::Update:
        {
            if (!m_isAlive) return;

            PDataValueRules();
        }
        break;
    }
}
#pragma endregion




#pragma region functions or methods
void PData::PDataValueRules()
{
    if (m_hitPoint <= 0.f) { m_hitPoint = 0.f; }

    if (m_manaPoint <= 0.f) { m_manaPoint = 0.f; }

    if (m_energyPoint <= 0.f) { m_energyPoint = 0.f; }
}


void PData::RegisterPDataPointer()
{
    m_pData = m_pEntity->GetOrCreateComponent<ceblankfs::players::PData>();
}


void PData::RegisterPDataDefaultValue()
{
    m_username = "TESTUSER";
    m_charactername = "TESTCHARACTER";

    m_spawnOnCamera = DVPData::spawnOnCamera;
    m_ableToDoubleJump = DVPData::ableToDoubleJump;

    m_hitPoint = DVPData::hitPoint;
    m_manaPoint = DVPData::manaPoint;
    m_energyPoint = DVPData::energyPoint;

    m_weight = DVPData::weight;
    m_movementSpeed = DVPData::movementSpeed;

    m_jumpForce = DVPData::jumpForce;
    m_jumpCharge = DVPData::jumpCharge;

    m_sprintMultiplier = DVPData::sprintMultiplier;

    m_jumpChargeMultiplier = DVPData::jumpChargeMultiplier;
    m_jumpDurationOnHold = DVPData::jumpDurationOnHold;
}
#pragma endregion
