// ONLY for player event listener
#include "PCore.h"


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
            // 
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

            ValuePolicy();
            AimStanceHandler();

            StaminaHanlder(dt);

            GroundMovementHandler(dt);
            CameraMovementHandler(dt);
            GroundJumpHandler(dt);
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
