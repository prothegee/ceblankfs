#pragma once
#include "GameConfig.h"
#include "players/PlayerCamera.h"
#include "players/PlayerData.h"
#include "players/PlayerMovement.h"


// #include <CryEntitySystem/IEntityComponent.h>


namespace ceblankfs
{
namespace players
{

/**
 * @brief entity core player component class
 * 
 */
class Player final
    :   public IEntityComponent
{
private:
    /* data */


public:
    Player(/* args */);
    virtual ~Player();


    virtual void Initialize() override;


    // core player event mask
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	// core player process event
    virtual void ProcessEvent(const SEntityEvent& e) override;


    static void ReflectType(Schematyc::CTypeDesc<Player>& desc)
    {
        desc.SetGUID("{41b53e15-0680-43d9-8b6b-6c1b3310c31b}"_cry_guid);
        desc.SetLabel("Player");
        desc.SetEditorCategory("_players");
        desc.SetDescription("Core player entity");
    }


protected:
    bool m_isAlive = false;

    // PlayerCamera pPc; // player camera pointer from Player class
    // PlayerData pPd; // player data pointer from Player class
    PlayerMovement* pPm = nullptr; // player movement pointer from Player class

protected:
    void InitializeLocalPlayer();
};

} // namespace players
} // namespace ceblankfs