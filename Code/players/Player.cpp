#include "Player.h"


// #include <CryCore/StaticInstanceList.h>
// #include <CrySchematyc/Env/IEnvRegistrar.h>
// #include <CrySchematyc/Env/Elements/EnvComponent.h>


using namespace blankfs::players;


static void RegisterPlayerEntity(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(
            SCHEMATYC_MAKE_ENV_COMPONENT(Player)); // EnvComponent.h
    } // IEnvRegistrar.h
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayerEntity); // StaticInstanceList.h
Player::Player(/* args */)
{
}
Player::~Player()
{
}


#pragma region player init
void Player::Initialize()
{
    // nothing to do for now

    #ifndef NDEBUG
    CryLog("### players::Player Initialize");
    #endif
}

void Player::InitializeLocalPlayer()
{
    #ifndef NDEBUG
    CryLog("### players::Player InitializeLocalPlayer");
    #endif
}
#pragma endregion


#pragma region event listener
Cry::Entity::EventFlags Player::GetEventMask() const
{
    return
        Cry::Entity::EEvent::Reset |
        Cry::Entity::EEvent::Update
        ;
}

void Player::ProcessEvent(const SEntityEvent& e)
{

}
#pragma endregion


#pragma region player listener
#pragma endregion