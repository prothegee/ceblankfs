#include "CPData.h"


using namespace ceblankfs::players;


#pragma region component and registrar
static void RegisterCPDataComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPData));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterCPDataComponent);
CPData::CPData(/* args */)
{
}
CPData::~CPData()
{
}
#pragma endregion




#pragma region initialize and binding
void CPData::Initialize()
{
    RegisterCPDataPointer();
    RegisterCPDataDefaultValue();

    if (GameConfig::IS_DEBUG)
    {
        CryLog("### players::CPData::Initialize");
    }
}
#pragma endregion




#pragma region event listener
Cry::Entity::EventFlags CPData::GetEventMask() const
{
    return
        Cry::Entity::EEvent::GameplayStarted |
        Cry::Entity::EEvent::Reset |
        Cry::Entity::EEvent::Update
        ;
}


void CPData::ProcessEvent(const SEntityEvent& e)
{
    switch (e.event)
    {
        case Cry::Entity::EEvent::GameplayStarted:
        {
            // log base on game config
            if (GameConfig::IS_DEBUG)
            {
                CryLog("### CPData::ProcessEvent::GameplayStarted");
                CryLog("### CPData username: %s", m_username.c_str());
                CryLog("### CPData character name: %s", m_charactername.c_str());
            }
        }
        break;


        case Cry::Entity::EEvent::Reset:
        {
            m_isAlive = e.nParam[0] != 0;

            // log base on game config
            if (GameConfig::IS_DEBUG)
            {
                CryLog("### CPData::m_isAlive: %s", m_isAlive ? "true" : "false");
            }
        }
        break;


        case Cry::Entity::EEvent::Update:
        {
            if (!m_isAlive) return;

            CPDataValueRules();
        }
        break;
    }
}
#pragma endregion




#pragma region functions or methods
void CPData::CPDataValueRules()
{
    if (m_hitPoint <= 0.f) { m_hitPoint = 0.f; }

    if (m_manaPoint <= 0.f) { m_manaPoint = 0.f; }

    if (m_energyPoint <= 0.f) { m_energyPoint = 0.f; }
}


void CPData::RegisterCPDataPointer()
{
    m_pData = m_pEntity->GetOrCreateComponent<ceblankfs::players::CPData>();
}


void CPData::RegisterCPDataDefaultValue()
{
    m_username = "TESTUSER";
    m_charactername = "TESTCHARACTER";

    m_spawnOnCamera = DVCPData::spawnOnCamera;
    m_ableToDoubleJump = DVCPData::ableToDoubleJump;

    m_hitPoint = DVCPData::hitPoint;
    m_manaPoint = DVCPData::manaPoint;
    m_energyPoint = DVCPData::energyPoint;

    m_weight = DVCPData::weight;
    m_movementSpeed = DVCPData::movementSpeed;

    m_jumpForce = DVCPData::jumpForce;
    m_jumpCharge = DVCPData::jumpCharge;

    m_sprintMultiplier = DVCPData::sprintMultiplier;

    m_jumpChargeMultiplier = DVCPData::jumpChargeMultiplier;
    m_jumpDurationOnHold = DVCPData::jumpDurationOnHold;
}
#pragma endregion
