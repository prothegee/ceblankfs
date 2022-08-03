// ONLY for player event listener
#include "PCore.h"
#include "systems/SLevelManager.h"
#include "systems/SGraphicManager.h"


#pragma region register PCore
PCore::PCore(/* args */)
{
}
PCore::~PCore()
{
}
#pragma endregion


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
            #ifndef NDEBUG
            // check current map
            CryLog("# current level: %s", SLevelManager::CurrentLevel());

            // get current editor camera?
            auto cameditor = gEnv->pSystem->GetViewCamera();

            auto campos = cameditor.GetPosition();
            auto camangle = cameditor.GetAngles();

            CryLog("# editor campos: %f, %f, %f", campos.x, campos.y, campos.z);
            CryLog("# editor camangle: %f, %f, %f", camangle.x, camangle.y, camangle.z);
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
            OrientHandler(dt);

            ValuePolicy(dt);
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
