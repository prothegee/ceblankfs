#pragma once
#include "StdAfx.h"
#include "GameConfig.h"


// #include <CryEntitySystem/IEntityComponent.h>


namespace blankfs
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
public:
    bool m_isOnGround; // player is on ground status

    float m_movementSpeed; // player movement speed value
    float m_jumpForce; // player jump force value

public:
    Player(/* args */);
    virtual ~Player();


    virtual void Initialize() override;


	// virtual Cry::Entity::EventFlags GetEventMask() const override;
	// virtual void ProcessEvent(const SEntityEvent& event) override;


    static void ReflectType(Schematyc::CTypeDesc<Player>& desc)
    {
        desc.SetGUID("{41b53e15-0680-43d9-8b6b-6c1b3310c31b}"_cry_guid);
        desc.SetLabel("Player");
        desc.SetEditorCategory("_players");
        desc.SetDescription("Core player entity");
        desc.SetComponentFlags({
            IEntityComponent::EFlags::Transform,
            IEntityComponent::EFlags::Socket,
            IEntityComponent::EFlags::Attach
        });
    }


protected:
    bool m_isAlive = false;

protected:
    void InitializeLocalPlayer();
};

} // namespace players
} // namespace blankfs