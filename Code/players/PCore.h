#pragma once
#include "StdAfx.h"
#include "PCH.h"
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
private:
    // 


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
        desc.SetDescription("Player core component");
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
    void InitializeDefaultData();

protected:
    // common player handle input change
    void HandleInputFlagChange(CEnumFlags<EInputFlag> flags, CEnumFlags<EActionActivationMode> activationMode, EInputFlagType type = EInputFlagType::Hold);
    // jump player handle input change
    void JumpHandleInputFlagChange(CEnumFlags<EInputFlag> flags, CEnumFlags<EActionActivationMode> activationMode, EInputFlagType type = EInputFlagType::Hold);

protected:
    // player input component
    Cry::DefaultComponents::CInputComponent* m_pInput = nullptr;
    // player camera component
	Cry::DefaultComponents::CCameraComponent* m_pCamera = nullptr;
	// player audio listener component
    Cry::Audio::DefaultComponents::CListenerComponent* m_pAudio = nullptr;
    // player character controller component
    Cry::DefaultComponents::CCharacterControllerComponent* m_pCC = nullptr;

protected:
    // ground movement logic handler
    void GroundMovementHandler(float dt);
    // camera movement logic handler
    void OrientHandler(float dt);
    // value policy for non-unsigned data type
    void ValuePolicy(float dt);
};
