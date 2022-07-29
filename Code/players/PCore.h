#pragma once
#include "StdAfx.h"
#include "PData.h"
#include "PInput.h"


/**
 * @brief player core class
 * 
 */
class PCore final
    :   public IEntityComponent
    ,   PData
    ,   PInput
{
public:
	PCore();
	virtual ~PCore();

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& e) override;
	
	virtual bool NetSerialize(TSerialize ser, EEntityAspects aspect, uint8 profile, int flags) override;
	virtual NetworkAspectType GetNetSerializeAspectMask() const override { return InputAspect; }
	// ~IEntityComponent

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<PCore>& desc)
	{
		desc.SetGUID("{7cdcdaa7-7f11-4801-9cb6-a674dd691dc0}"_cry_guid);
        desc.SetLabel("PCore");
        desc.SetEditorCategory("_players");
        desc.SetDescription("player core component");
        #pragma region player data member
        desc.AddMember(
            &PCore::m_username,
            'punm',
            "player-username",
            "username",
            "username value",
            ""
        );
        desc.AddMember(
            &PCore::m_character,
            'pcnm',
            "player-character-name",
            "character",
            "character value",
            ""
        );
        desc.AddMember(
            &PCore::m_health,
            'phv',
            "player-health",
            "health",
            "health value",
            DVPData::health
        );
        desc.AddMember(
            &PCore::m_stamina,
            'psv',
            "player-stamina",
            "stamina",
            "stamina value",
            DVPData::stamina
        );
        desc.AddMember(
            &PCore::m_stamina,
            'psv',
            "player-stamina",
            "stamina",
            "stamina value",
            DVPData::stamina
        );
        desc.AddMember(
            &PCore::m_movementSpeed,
            'pmsv',
            "player-movement-peed",
            "movement speed",
            "movement speed value",
            DVPData::movementSpeed
        );
        desc.AddMember(
            &PCore::m_jumpForce,
            'pjfv',
            "player-jump-force",
            "jump force",
            "jump force value",
            DVPData::jumpForce
        );
        desc.AddMember(
            &PCore::m_jumpCharge,
            'pjcv',
            "player-jump-charge",
            "jump charge",
            "jump charge value",
            DVPData::jumpCharge
        );
        desc.AddMember(
            &PCore::m_jumpChargeMultiplier,
            'pjcm',
            "player-jump-charge-multiplier",
            "jump charge multiplier",
            "jump charge multiplier value",
            DVPData::jumpChargeMultiplier
        );
        desc.AddMember(
            &PCore::m_jumpDurationOnHold,
            'pjdh',
            "player-jump-duration-on-hold",
            "jump duration on hold",
            "jump duration on hold value",
            DVPData::jumpDurationOnHold
        );
        desc.AddMember(
            &PCore::m_jumpDurationOnHold,
            'pjdh',
            "player-jump-duration-on-hold",
            "jump duration on hold",
            "jump duration on hold value",
            DVPData::jumpDurationOnHold
        );
        desc.AddMember(
            &PCore::m_jumpDurationOnHold,
            'pwv',
            "player-weight",
            "weight",
            "weight value",
            DVPData::weight
        );
        desc.AddMember(
            &PCore::m_sprintMultiplier,
            'psmv',
            "player-sprint-multiplier",
            "sprint multiplier",
            "sprint multiplier value",
            DVPData::sprintMultiplier
        );
        #pragma endregion
        #pragma region player input member
        desc.AddMember(
            &PCore::m_sensitivity,
            'pcs',
            "player-controller-sesnsitivity",
            "sesnsitivity",
            "sesnsitivity value",
            DVPInput::sensitivity
        );
        #pragma endregion
    }

	void OnReadyForGameplayOnServer();
    bool IsLocalClient() const { return (m_pEntity->GetFlags() & ENTITY_FLAG_LOCAL_PLAYER) != 0; }


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

protected:
    // initialize default data
    void InitializeDefaultData()
    {
        (m_username.length() <= 3)
            ? m_username = "USER_DEBUG"
            : m_username = this->m_username;

        (m_character.length() <= 3)
            ? m_character = "CHARACTERD_EBUG"
            : m_character = this->m_character;

        m_health = DVPData::health;
        m_stamina = DVPData::stamina;
        m_movementSpeed = DVPData::movementSpeed;
        m_jumpForce = DVPData::jumpForce;
        m_jumpCharge = DVPData::jumpCharge;
        m_jumpChargeMultiplier = DVPData::jumpChargeMultiplier;
        m_jumpDurationOnHold = DVPData::jumpDurationOnHold;
        m_weight = DVPData::weight;
        m_sprintMultiplier = DVPData::sprintMultiplier;

        m_sensitivity = DVPInput::sensitivity;
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
    void HandleInputFlagChange(CEnumFlags<EInputFlag> flags, CEnumFlags<EActionActivationMode> activationMode, EInputFlagType type = EInputFlagType::Hold);

    void JumpHandleInputFlagChange(CEnumFlags<EInputFlag> flags, CEnumFlags<EActionActivationMode> activationMode, EInputFlagType type = EInputFlagType::Hold);

protected:
    // player input component
    Cry::DefaultComponents::CInputComponent* m_pInput = nullptr;
    // player camera component
	Cry::DefaultComponents::CCameraComponent* m_pCamera = nullptr;
	// player audio listener component
    Cry::Audio::DefaultComponents::CListenerComponent* m_pAudio = nullptr;
    // player character controller component9
    Cry::DefaultComponents::CCharacterControllerComponent* m_pCC = nullptr;

protected:
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
};
