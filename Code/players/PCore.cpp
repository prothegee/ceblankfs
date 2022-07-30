// ONLY for player event listener
#include "PCore.h"
#include "systems/SLevelManager.h"
#include "systems/SGraphicManager.h"


#pragma region event listener
Cry::Entity::EventFlags PCore::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::BecomeLocalPlayer |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset
        ;
}

void PCore::ProcessEvent(const SEntityEvent& e)
{
	switch (e.event)
	{
        case Cry::Entity::EEvent::GameplayStarted:
        {
            // check current map
            #ifndef NDEBUG
            CryLog("# current level: %s", SLevelManager::CurrentLevel());
            #else
            #endif
        }
        break;

        case Cry::Entity::EEvent::BecomeLocalPlayer:
        {
            InitializeLocalPlayer();

            InitializeDefaultData();
        }
        break;
        
        case Cry::Entity::EEvent::Update:
        {
            // Don't update the player if we haven't spawned yet
            if(!m_isAlive) return;
            
            const float dt = e.fParam[0];

            GroundMovementHandler(dt);
            CameraMovementHandler(dt);
            GroundJumpHandler(dt);

            StaminaHanlder(dt);

            ValuePolicy();
            AimStanceHandler();
        }
        break;

        case Cry::Entity::EEvent::Reset:
        {
            // Disable player when leaving game mode.
            m_isAlive = e.nParam[0] != 0;
        }
        break;
	}
}
#pragma endregion
