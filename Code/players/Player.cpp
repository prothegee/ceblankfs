#include "Player.h"


// #include <CryCore/StaticInstanceList.h>
// #include <CrySchematyc/Env/IEnvRegistrar.h>
// #include <CrySchematyc/Env/Elements/EnvComponent.h>


using namespace ceblankfs::players;


#pragma region player init
void Player::Initialize()
{
    pPm = m_pEntity->GetOrCreateComponent<PlayerMovement>(); // PlayerMovement component
    pPm->initializePlayerMovement(); // don't forget to initialize it

    #ifndef NDEBUG
    CryLog("### Player::Initialize");
    #endif
}


void Player::InitializeLocalPlayer()
{
    #ifndef NDEBUG
    CryLog("### Player::InitializeLocalPlayer");
    #endif
}
#pragma endregion




#pragma region event listener
Cry::Entity::EventFlags Player::GetEventMask() const
{
    return
        Cry::Entity::EEvent::GameplayStarted |
        Cry::Entity::EEvent::Reset |
        Cry::Entity::EEvent::Update
        ;
}


void Player::ProcessEvent(const SEntityEvent& e)
{
    switch (e.event)
    {
        case Cry::Entity::EEvent::GameplayStarted:
        {
            
        }
        break;


        case Cry::Entity::EEvent::Reset:
        {
            m_isAlive = e.nParam[0] != 0;

            #ifndef NDEBUG
            CryLog("### Player m_isAlive: %s", m_isAlive ? "true" : "false");
            #endif
        }
        break;


        case Cry::Entity::EEvent::Update:
        {
            if (!m_isAlive) return;

            const float dt = e.fParam[0]; // frametime or deltatime

            pPm->HanldePlayerMovementRequest(dt);
            pPm->HandlePlayerJumpRequest(dt);
        }
        break;
    }
}
#pragma endregion


#pragma region player listener
#pragma endregion