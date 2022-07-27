#pragma once

#include "PCH_CEBLANKFS.h"


/**
 * @brief core player class
 * 
 */
class CPlayer
    :   public IEntityComponent
{
	enum class EInputFlagType
	{
		Hold = 0,
		Toggle
	};

	enum class EInputFlag : uint8
	{
		MoveLeft    = 1 << 0,
		MoveRight   = 1 << 1,
		MoveForward = 1 << 2,
		MoveBack    = 1 << 3,
		DoSprint    = 1 << 4,
		DoJump      = 1 << 5,
	};

	static constexpr EEntityAspects InputAspect = eEA_GameClientD;


public:
    // default value data for CPlayer class
    struct DVCPlayer
    {
        static constexpr float health = 100.f;
        static constexpr float stamina = 100.0f;
        static constexpr float movementSpeed = 50.f;
        static constexpr float jumpForce = 6.f;
        static constexpr float jumpCharge = 0.f;
        static constexpr float jumpChargeMultiplier = 1.f;
        static constexpr float jumpDurationOnHold = 0.f;
        static constexpr float weight = 60.f;
        static constexpr float sprintMultiplier = 2.f;

        static constexpr float sensitivity = 1.f;
    };

public:
	CPlayer();
	virtual ~CPlayer();

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& e) override;
	
	virtual bool NetSerialize(TSerialize ser, EEntityAspects aspect, uint8 profile, int flags) override;
	virtual NetworkAspectType GetNetSerializeAspectMask() const override { return InputAspect; }
	// ~IEntityComponent

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CPlayer>& desc)
	{
		desc.SetGUID("{7cdcdaa7-7f11-4801-9cb6-a674dd691dc0}"_cry_guid);
        desc.SetLabel("CPlayer");
        desc.SetEditorCategory("_players");
        desc.SetDescription("core player entity");
        #pragma region player data member
        desc.AddMember(
            &CPlayer::m_username,
            'punm',
            "player-username",
            "username",
            "username value",
            ""
        );
        desc.AddMember(
            &CPlayer::m_character,
            'pcnm',
            "player-character-name",
            "character",
            "character value",
            ""
        );
        desc.AddMember(
            &CPlayer::m_health,
            'phv',
            "player-health",
            "health",
            "health value",
            DVCPlayer::health
        );
        desc.AddMember(
            &CPlayer::m_stamina,
            'psv',
            "player-stamina",
            "stamina",
            "stamina value",
            DVCPlayer::stamina
        );
        desc.AddMember(
            &CPlayer::m_stamina,
            'psv',
            "player-stamina",
            "stamina",
            "stamina value",
            DVCPlayer::stamina
        );
        desc.AddMember(
            &CPlayer::m_movementSpeed,
            'pmsv',
            "player-movement-peed",
            "movement speed",
            "movement speed value",
            DVCPlayer::movementSpeed
        );
        desc.AddMember(
            &CPlayer::m_jumpForce,
            'pjfv',
            "player-jump-force",
            "jump force",
            "jump force value",
            DVCPlayer::jumpForce
        );
        desc.AddMember(
            &CPlayer::m_jumpCharge,
            'pjcv',
            "player-jump-charge",
            "jump charge",
            "jump charge value",
            DVCPlayer::jumpCharge
        );
        desc.AddMember(
            &CPlayer::m_jumpChargeMultiplier,
            'pjcm',
            "player-jump-charge-multiplier",
            "jump charge multiplier",
            "jump charge multiplier value",
            DVCPlayer::jumpChargeMultiplier
        );
        desc.AddMember(
            &CPlayer::m_jumpDurationOnHold,
            'pjdh',
            "player-jump-duration-on-hold",
            "jump duration on hold",
            "jump duration on hold value",
            DVCPlayer::jumpDurationOnHold
        );
        desc.AddMember(
            &CPlayer::m_jumpDurationOnHold,
            'pjdh',
            "player-jump-duration-on-hold",
            "jump duration on hold",
            "jump duration on hold value",
            DVCPlayer::jumpDurationOnHold
        );
        desc.AddMember(
            &CPlayer::m_jumpDurationOnHold,
            'pwv',
            "player-weight",
            "weight",
            "weight value",
            DVCPlayer::weight
        );
        desc.AddMember(
            &CPlayer::m_sprintMultiplier,
            'psmv',
            "player-sprint-multiplier",
            "sprint multiplier",
            "sprint multiplier value",
            DVCPlayer::sprintMultiplier
        );
        desc.AddMember(
            &CPlayer::m_sensitivity,
            'pcs',
            "player-controller-sesnsitivity",
            "sesnsitivity",
            "sesnsitivity value",
            DVCPlayer::sensitivity
        );
        #pragma endregion
	}

	void OnReadyForGameplayOnServer();
	bool IsLocalClient() const { return (m_pEntity->GetFlags() & ENTITY_FLAG_LOCAL_PLAYER) != 0; }

    // pub get current player character value
    string PubGetCharacter() const
    {
        return m_character.c_str();
    }


protected:
	void Revive(const Matrix34& transform);

	// Called when this entity becomes the local player, to create client specific setup such as the Camera
	void InitializeLocalPlayer();
	
	// Start remote method declarations
protected:
	// Parameters to be passed to the RemoteReviveOnClient function
	struct RemoteReviveParams
	{
		// Called once on the server to serialize data to the other clients
		// Then called once on the other side to deserialize
		void SerializeWith(TSerialize ser)
		{
			// Serialize the position with the 'wrld' compression policy
			ser.Value("pos", position, 'wrld');
			// Serialize the rotation with the 'ori0' compression policy
			ser.Value("rot", rotation, 'ori0');
		}
		
		Vec3 position;
		Quat rotation;
	};
	// Remote method intended to be called on all remote clients when a player spawns on the server
	bool RemoteReviveOnClient(RemoteReviveParams&& params, INetChannel* pNetChannel);

    void InitializeDefaultData()
    {
        (m_username.length() <= 3)
            ? m_username = "username"
            : m_username = this->m_username;

        (m_character.length() <= 3)
            ? m_character = "character"
            : m_character = this->m_character;

        m_health = DVCPlayer::health;
        m_stamina = DVCPlayer::stamina;
        m_movementSpeed = DVCPlayer::movementSpeed;
        m_jumpForce = DVCPlayer::jumpForce;
        m_jumpCharge = DVCPlayer::jumpCharge;
        m_jumpChargeMultiplier = DVCPlayer::jumpChargeMultiplier;
        m_jumpDurationOnHold = DVCPlayer::jumpDurationOnHold;
        m_weight = DVCPlayer::weight;
        m_sprintMultiplier = DVCPlayer::sprintMultiplier;

        m_sensitivity = DVCPlayer::sensitivity;
    }

    // value policy for non-unsigned data type
    void ValuePolicy()
    {
        // health
        (m_health < 0.f)
            ? m_health = 0.f
            : m_health = this->m_health;
        
        // stamina
        (m_stamina < m_staminaMinLimit)
            ? m_stamina = m_staminaMinLimit
            : m_stamina = this->m_stamina;
        (m_stamina > m_staminaMaxLimit)
            ? m_stamina = m_staminaMaxLimit
            : m_stamina = this->m_stamina;
        
        // movement speed
        (m_movementSpeed < 0.f)
            ? m_movementSpeed = 0.f
            : m_movementSpeed = this->m_movementSpeed;
        
        // jumpforce
        (m_jumpForce < 0.f)
            ? m_jumpForce = 0.f
            : m_jumpForce = this->m_jumpForce;
        
        // jumpcharge
        (m_jumpCharge < 0.f)
            ? m_jumpCharge = 0.f
            : m_jumpCharge = this->m_jumpCharge;
        
        // jumpcharge multiplier
        (m_jumpChargeMultiplier < 0.f)
            ? m_jumpChargeMultiplier = 0.f
            : m_jumpChargeMultiplier = this->m_jumpChargeMultiplier;
        
        // jump on hold
        (m_jumpDurationOnHold < 0.f)
            ? m_jumpDurationOnHold = 0.f
            : m_jumpDurationOnHold = this->m_jumpDurationOnHold;

        // weight
        (m_weight < 0.f)
            ? m_weight = 0.f
            : m_weight = this->m_weight;
        
        // sprint multiplier
        (m_sprintMultiplier < 0.f)
            ? m_sprintMultiplier = 0.f
            : m_sprintMultiplier = this->m_sprintMultiplier;

        // sensitivity
        (m_sensitivity < 0.f)
            ? m_sensitivity = 0.f
            : m_sensitivity = this->m_sensitivity;
    }

protected:
    #pragma region player data variables
    Schematyc::CSharedString m_username;
    Schematyc::CSharedString m_character;

    float m_health;
    float m_stamina;
    float m_movementSpeed;
    float m_jumpForce;
    float m_jumpCharge;
    float m_jumpChargeMultiplier;
    float m_jumpDurationOnHold;
    float m_weight;
    float m_sprintMultiplier;

    float m_sensitivity;

    const float m_minJumpCharge = 0.3f;
    const float m_maxJumpCharge = 3.0f;

    const float m_staminaMinLimit = 0.0f;
    const float m_staminaMaxLimit = 100.0f;
    const float m_staminaReductionRate = 30.0f;
    const float m_staminaRegenerationRate = 15.0f;

    const float m_rotationSpeed = 0.002f;

    float m_rotationLimitsMinPitch = -0.85f; // dfr : -.84f;
    float m_rotationLimitsMaxPitch = 1.6f; // dfr: 1.5f;
    #pragma endregion

    CEnumFlags<EInputFlag> m_inputFlags, m_jumpInputFlags;
	Vec2 m_mouseDeltaRotation;
    Quat m_lookOrientation;
    Vec2 m_entityDeltaRotation;
    Quat m_entityLookOrientation;

	bool m_isAlive = false;
    bool m_canJumpNow = false;
    bool m_aimStance = false;

    // player camera
	Cry::DefaultComponents::CCameraComponent* m_pCamera = nullptr;
	// player input
    Cry::DefaultComponents::CInputComponent* m_pInput = nullptr;
	// player audio listener
    Cry::Audio::DefaultComponents::CListenerComponent* m_pAudio = nullptr;
    // player character controller
    Cry::DefaultComponents::CCharacterControllerComponent* m_pCC = nullptr;

protected:
    void HandleInputFlagChange(CEnumFlags<EInputFlag> flags, CEnumFlags<EActionActivationMode> activationMode, EInputFlagType type = EInputFlagType::Hold);
    void JumpHandleInputFlagChange(CEnumFlags<EInputFlag> flags, CEnumFlags<EActionActivationMode> activationMode, EInputFlagType type = EInputFlagType::Hold);


    // ground movement logic handler
    void GroundMovementHandler(float dt);
    // camera movement logic handler
    void CameraMovementHandler(float dt);
    // player stamina reduction & regeneration handler
    void StaminaHanlder(float dt);
    // player ground jump handler
    void GroundJumpHandler(float dt);
    // player aim stance handler
    void AimStanceHandler();


private:
    Vec3 m_defaultCameraPosition = Vec3(0, 0, 1.75f);
};
