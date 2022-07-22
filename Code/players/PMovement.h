#pragma once
#include "GameConfig.h"
#include "PData.h"
#include "PController.h"


namespace ceblankfs
{
namespace players
{

/**
 * @brief player movement core class
 * 
 */
class PMovement
    :   public IEntityComponent
{
private:
    bool m_isAlive = false;
    bool m_canJumpNow = false;




public:
    // playr movement and action pointer from PMovementAndAction
    ceblankfs::players::PMovement* m_pMAA = nullptr;

public:
    PMovement(/* args */);
    virtual ~PMovement();


    virtual void Initialize() override;


    virtual Cry::Entity::EventFlags GetEventMask() const override;
    virtual void ProcessEvent(const SEntityEvent& e) override;


    static void ReflectType(Schematyc::CTypeDesc<PMovement>& desc)
    {
        desc.SetGUID("{c95b48f9-4f9f-4bd1-95f0-f828a1831fa4}"_cry_guid);
        desc.SetLabel("PMovement");
        desc.SetEditorCategory("_players");
        desc.SetDescription("Players movement");
    }




protected:
    // player character controller pointer from PMovementAndAction
    Cry::DefaultComponents::CCharacterControllerComponent* m_pCC = nullptr;

    // PController pointer from PMovementAndAction
    ceblankfs::players::PController* m_pControllerPtr = nullptr;

    // PController pointer from PMovementAndAction
    ceblankfs::players::PData* m_pDataPtr = nullptr;

protected:
    /**
     * @brief RegisterPMovementPointer register pointer
     * 
     */
    void RegisterPMovementPointer();


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
