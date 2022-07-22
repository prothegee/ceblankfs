#pragma once
#include "GameConfig.h"


namespace ceblankfs
{
namespace players
{

/**
 * @brief player controller core class
 * 
 */
class PController
    :   public IEntityComponent
{
private:
    bool m_isAlive = false;




public:
    Vec2 m_mouseDeltaRotation, m_parentDeltaRotation;
    Quat m_lookOrientation, m_parentLookOrientation;
    float m_sensitivity = 1.f;
    const float m_rotationSpeed = 0.002f;

    enum class EInputFlagType
    {
        Hold = 0,
        Toggle
    };

    enum class EInputFlag : uint8
    {
        forward = 1 << 0,
        backward = 1 << 1,
        left = 1 << 2,
        right = 1 << 3,
        jump = 1 << 4,
        sprint = 1 << 5,
    };

    // input flags
    CEnumFlags<EInputFlag> m_inputFlags;

    // player controller pointer from PData
    ceblankfs::players::PController* m_pController = nullptr;

public:
    PController(/* args */);
    virtual ~PController();


    virtual void Initialize() override;


    virtual Cry::Entity::EventFlags GetEventMask() const override;
    virtual void ProcessEvent(const SEntityEvent& e) override;


    static void ReflectType(Schematyc::CTypeDesc<PController>& desc)
    {
        desc.SetGUID("{97193e3d-4359-4b6a-afd6-1b52d508844c}"_cry_guid);
        desc.SetLabel("PController");
        desc.SetEditorCategory("_players");
        desc.SetDescription("Players controller");
    }




protected:
    // player input pointer from PController
    Cry::DefaultComponents::CInputComponent* m_pInput = nullptr;

protected:
    /**
     * @brief assign input player controller
     * 
     */
    void AssignInputController();


    /**
     * @brief handle input change
     * 
     * @param flags CEnumFlags<EInputFlag>
     * @param activationMode CEnumFlags<EActionActivationMode>
     * @param type EInputFlagType
     */
    void HandleInputFlagChange(CEnumFlags<EInputFlag> flags, CEnumFlags<EActionActivationMode> activationMode, EInputFlagType type);


    /**
     * @brief PController register pointer
     * 
     */
    void RegisterPControllerPointer();
};

} // namespace players
} // namespace ceblankfs
