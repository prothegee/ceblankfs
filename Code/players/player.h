#pragma once
#include "GameConfig.h"


namespace ceblankfs
{
namespace players
{


/**
 * @brief core player class from scratch
 * 
 */
class player final
    :   public IEntityComponent
{
private:
    bool m_isAlive = false;
    bool m_canJumpNow = false;
    bool m_isSprint = false;
    bool m_readyStance = false;
    bool m_aimStance = false;

    enum class EInputFlagType
    {
        Hold = 0,
        Toggle,
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




public:
    // default value for player class
    struct DVplayer
    {
        static constexpr float velocity = 0.f;

        static constexpr float health = 100.f;
        static constexpr float armor = 0.f;
        static constexpr float stamina = 100.f;
        static constexpr float movementSpeed = 50.f;
        static constexpr float jumpForce = 6.f;
        static constexpr float jumpCharge = 0.f;
        static constexpr float jumpChargeMultiplier = 1.f;
        static constexpr float jumpDurationOnHold = 0.f;
        static constexpr float weight = 60.f;
        static constexpr float sprintMultiplier = 2.f;
    };

public:
    player(/* args */);
    virtual ~player();


    virtual void Initialize() override;


    virtual Cry::Entity::EventFlags GetEventMask() const override;
    virtual void ProcessEvent(const SEntityEvent& e) override;


    static void ReflectType(Schematyc::CTypeDesc<player>& desc)
    {
        desc.SetGUID("{b39fbff6-f9bf-4597-9cf9-a8f76c2ed878}"_cry_guid);
        desc.SetLabel("player");
        desc.SetEditorCategory("_players");
        desc.SetDescription("Player entity component");
        #pragma region player data member
        desc.AddMember(
            &player::m_username,
            'punm',
            "player-username",
            "username",
            "username value",
            ""
        );
        desc.AddMember(
            &player::m_character,
            'pcnm',
            "player-character-name",
            "character",
            "character value",
            ""
        );
        desc.AddMember(
            &player::m_health,
            'phv',
            "player-health",
            "health",
            "health value",
            DVplayer::health
        );
        desc.AddMember(
            &player::m_armor,
            'pav',
            "player-armor",
            "armor",
            "armor value",
            DVplayer::armor
        );
        desc.AddMember(
            &player::m_stamina,
            'psv',
            "player-stamina",
            "stamina",
            "stamina value",
            DVplayer::stamina
        );
        desc.AddMember(
            &player::m_stamina,
            'psv',
            "player-stamina",
            "stamina",
            "stamina value",
            DVplayer::stamina
        );
        desc.AddMember(
            &player::m_movementSpeed,
            'pmsv',
            "player-movement-peed",
            "movement speed",
            "movement speed value",
            DVplayer::movementSpeed
        );
        desc.AddMember(
            &player::m_jumpForce,
            'pjfv',
            "player-jump-force",
            "jump force",
            "jump force value",
            DVplayer::jumpForce
        );
        desc.AddMember(
            &player::m_jumpCharge,
            'pjcv',
            "player-jump-charge",
            "jump charge",
            "jump charge value",
            DVplayer::jumpCharge
        );
        desc.AddMember(
            &player::m_jumpChargeMultiplier,
            'pjcm',
            "player-jump-charge-multiplier",
            "jump charge multiplier",
            "jump charge multiplier value",
            DVplayer::jumpChargeMultiplier
        );
        desc.AddMember(
            &player::m_jumpDurationOnHold,
            'pjdh',
            "player-jump-duration-on-hold",
            "jump duration on hold",
            "jump duration on hold value",
            DVplayer::jumpDurationOnHold
        );
        desc.AddMember(
            &player::m_jumpDurationOnHold,
            'pjdh',
            "player-jump-duration-on-hold",
            "jump duration on hold",
            "jump duration on hold value",
            DVplayer::jumpDurationOnHold
        );
        desc.AddMember(
            &player::m_jumpDurationOnHold,
            'pwv',
            "player-weight",
            "weight",
            "weight value",
            DVplayer::weight
        );
        desc.AddMember(
            &player::m_sprintMultiplier,
            'psmv',
            "player-sprint-multiplier",
            "sprint multiplier",
            "sprint multiplier value",
            DVplayer::sprintMultiplier
        );
        #pragma endregion
    }




protected:
    Schematyc::CSharedString m_username;
    Schematyc::CSharedString m_character;

    float m_velocity;

    float m_health;
    float m_armor;
    float m_stamina;
    float m_movementSpeed;
    float m_jumpForce;
    float m_jumpCharge;
    float m_jumpChargeMultiplier;
    float m_jumpDurationOnHold;
    float m_weight;
    float m_sprintMultiplier;

    const float m_minJumpCharge = 0.3f;
    const float m_maxJumpCharge = 3.0f;

    CEnumFlags<EInputFlag> m_inputFlags;
    CEnumFlags<EInputFlag> m_jumpInputFlags;

    Quat m_entityLookOrientation;
    Vec2 m_entityDeltaRotation;
    Quat m_lookOrientation;
    Vec2 m_mouseDeltaRotation;
    float m_sensitivity = 1.f;
    const float m_rotationSpeed = 0.002f;
    float m_rotationLimitsMinPitch = -0.9f; // df : -.84f;
    float m_rotationLimitsMaxPitch = 1.3f; // df: 1.5f;


    // player character controller
    Cry::DefaultComponents::CCharacterControllerComponent* m_pCC = nullptr;
    // player input
    Cry::DefaultComponents::CInputComponent* m_pInput = nullptr;
    // player camera
    Cry::DefaultComponents::CCameraComponent* m_pCamera = nullptr;

protected:
    // set default value
    void DefaultValue();
    // value policy for non unsigned type
    void ValuePolicy();
    // register class member pointer
    void PointerComponentsRegistrar();
    // register and bind player input
    void RegisterInputActionAndBinding();
    // input flag change
    void HandleInputFlagChange(CEnumFlags<EInputFlag> flags, CEnumFlags<EActionActivationMode> activationMode, EInputFlagType type);
    // jump input flag change
    void JumpHandleInputFlagChange(CEnumFlags<EInputFlag> flags, CEnumFlags<EActionActivationMode> activationMode, EInputFlagType type);

    // ground movement handler
    void MovementHandler(float dt);
    // ground jump handler
    void JumpHandler(float dt);
    // player character/mesh handler
    void CharacterControllerHandlerRotationX(float dt);
    // main player camera handler
    void CameraViewHandler(float dt);
};


} // namespace players
} // namespace ceblankfs
