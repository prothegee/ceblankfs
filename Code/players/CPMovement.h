#pragma once
#include "GameConfig.h"
#include "CPData.h"
#include "CPController.h"


namespace ceblankfs
{
namespace players
{

/**
 * @brief class player movement and action logics
 * 
 */
class CPMovement
    :   public IEntityComponent
{
private:
    bool m_isAlive = false;
    bool m_canJumpNow = false;




public:
    // playr movement and action pointer from CPMovementAndAction
    ceblankfs::players::CPMovement* m_pMAA = nullptr;

public:
    CPMovement(/* args */);
    virtual ~CPMovement();


    virtual void Initialize() override;


    virtual Cry::Entity::EventFlags GetEventMask() const override;
    virtual void ProcessEvent(const SEntityEvent& e) override;


    static void ReflectType(Schematyc::CTypeDesc<CPMovement>& desc)
    {
        desc.SetGUID("{c95b48f9-4f9f-4bd1-95f0-f828a1831fa4}"_cry_guid);
        desc.SetLabel("CPMovement");
        desc.SetEditorCategory("_players");
        desc.SetDescription("Players movement");
    }




protected:
    // player character controller pointer from CPMovementAndAction
    Cry::DefaultComponents::CCharacterControllerComponent* m_pCC = nullptr;

    // CPController pointer from CPMovementAndAction
    ceblankfs::players::CPController* m_pControllerPtr = nullptr;

    // CPController pointer from CPMovementAndAction
    ceblankfs::players::CPData* m_pDataPtr = nullptr;

protected:
    /**
     * @brief RegisterCPMovementPointer register pointer
     * 
     */
    void RegisterCPMovementPointer();


    /**
     * @brief player ground movement handler
     * 
     * @param dt float
     */
    void GroundMovementHandler(float dt);


    /**
     * @brief player ground jump handler
     * 
     * @param dt float
     */
    void GroundJumpHandler(float dt);
};


} // namespace players
} // namespace ceblankfs
