#include "CPlayer.h"

#include <CryNetwork/Rmi.h> // required on *.cc or *.cpp file


#pragma region register, constructor and desctructor
static void RegisterCPlayerComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlayer));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterCPlayerComponent);
CPlayer::CPlayer(/* args */)
{
}
CPlayer::~CPlayer()
{
}
#pragma endregion




#pragma region initialization
void CPlayer::Initialize()
{
	// Mark the entity to be replicated over the network
	m_pEntity->GetNetEntity()->BindToNetwork();
	
	// Register the RemoteReviveOnClient function as a Remote Method Invocation (RMI) that can be executed by the server on clients
	SRmi<RMI_WRAP(&CPlayer::RemoteReviveOnClient)>::Register(this, eRAT_NoAttach, false, eNRT_ReliableOrdered);
}

void CPlayer::InitializeLocalPlayer()
{
    #pragma region pointer components
	m_pCamera = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();

	m_pAudio = m_pEntity->GetOrCreateComponent<Cry::Audio::DefaultComponents::CListenerComponent>();

	m_pInput = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();

    m_pCC = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
    m_pCC->SetTransformMatrix(Matrix34::Create(Vec3(.5f), IDENTITY, Vec3(0, 0, .5f)));
    #pragma endregion


    #pragma region input register and binding
	m_pInput->RegisterAction(
        "player",
        "moveleft",
        [this](int activationMode, float value)
        {
            HandleInputFlagChange(EInputFlag::MoveLeft, (EActionActivationMode)activationMode);
        });
        m_pInput->BindAction(
            "player",
            "moveleft",
            eAID_KeyboardMouse,
            EKeyId::eKI_A);

	m_pInput->RegisterAction(
        "player",
        "moveright",
        [this](int activationMode, float value)
        {
            HandleInputFlagChange(EInputFlag::MoveRight, (EActionActivationMode)activationMode);
        });
	    m_pInput->BindAction(
            "player",
            "moveright",
            eAID_KeyboardMouse,
            EKeyId::eKI_D);

	m_pInput->RegisterAction(
        "player",
        "moveforward",
        [this](int activationMode, float value)
        {
            HandleInputFlagChange(EInputFlag::MoveForward, (EActionActivationMode)activationMode);
        });
        m_pInput->BindAction(
            "player",
            "moveforward",
            eAID_KeyboardMouse,
            EKeyId::eKI_W);

	m_pInput->RegisterAction(
        "player",
        "moveback",
        [this](int activationMode, float value)
        {
            HandleInputFlagChange(EInputFlag::MoveBack, (EActionActivationMode)activationMode);
        }); 
        m_pInput->BindAction(
            "player",
            "moveback", 
            eAID_KeyboardMouse,
            EKeyId::eKI_S);

	m_pInput->RegisterAction(
        "player",
        "mouse_rotateyaw",
        [this](int activationMode, float value)
        {
            m_mouseDeltaRotation.x -= value;
            m_entityDeltaRotation.x -= value;
        });
        m_pInput->BindAction(
            "player",
            "mouse_rotateyaw",
            eAID_KeyboardMouse,
            EKeyId::eKI_MouseX);

	m_pInput->RegisterAction(
        "player",
        "mouse_rotatepitch",
        [this](int activationMode, float value)
        {
            m_mouseDeltaRotation.y -= value;
        });
        m_pInput->BindAction(
            "player",
            "mouse_rotatepitch",
            eAID_KeyboardMouse,
            EKeyId::eKI_MouseY);

    m_pInput->RegisterAction(
        "player",
        "sprint",
        [this](int activationMode, float value)
        {
            HandleInputFlagChange(
                EInputFlag::DoSprint,
                (EActionActivationMode)activationMode,
                EInputFlagType::Hold
            );
        });
        m_pInput->BindAction(
            "player",
            "sprint",
            eAID_KeyboardMouse,
            EKeyId::eKI_LShift);

    m_pInput->RegisterAction(
        "player",
        "jump",
        [this](int activationMode, float value)
        {
            JumpHandleInputFlagChange(
                EInputFlag::DoJump,
                (EActionActivationMode)activationMode,
                EInputFlagType::Hold
            );
        });
        m_pInput->BindAction(
            "player",
            "jump",
            eAID_KeyboardMouse,
            EKeyId::eKI_Space);

    m_pInput->RegisterAction(
        "player",
        "shoot",
        [this](int activationMode, float value)
        {
            if (activationMode == eAAM_OnPress)
            {
                #ifndef NDEBUG
                CryLog("# players::player shoot");
                #else
                #endif
            }
        });
        m_pInput->BindAction(
            "player",
            "shoot",
            eAID_KeyboardMouse,
            EKeyId::eKI_Mouse1);

    m_pInput->RegisterAction(
        "player",
        "aim",
        [this](int activationMode, float value)
        {
            if (activationMode == eAAM_OnPress)
            {
                if (m_aimStance)
                {
                    m_aimStance = false;
                }
                else
                {
                    m_aimStance = true;
                }

                #ifndef NDEBUG
                CryLog("# players::player m_aimStance %s", m_aimStance ? "true" : "false");
                #else
                #endif
            }
        });
        m_pInput->BindAction(
            "player",
            "aim",
            eAID_KeyboardMouse,
            EKeyId::eKI_Mouse2);
    #pragma endregion
}
#pragma endregion




#pragma region event listener
Cry::Entity::EventFlags CPlayer::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::BecomeLocalPlayer |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset
        ;
}

void CPlayer::ProcessEvent(const SEntityEvent& e)
{
	switch (e.event)
	{
        case Cry::Entity::EEvent::GameplayStarted:
        {
            // 
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

            ValuePolicy();
            AimStanceHandler();

            StaminaHanlder(dt);

            GroundMovementHandler(dt);
            CameraMovementHandler(dt);
            GroundJumpHandler(dt);

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



#pragma region player misc1
bool CPlayer::NetSerialize(TSerialize ser, EEntityAspects aspect, uint8 profile, int flags)
{
	if(aspect == InputAspect)
	{
		ser.BeginGroup("PlayerInput");

		const CEnumFlags<EInputFlag> prevInputFlags = m_inputFlags;

		ser.Value("m_inputFlags", m_inputFlags.UnderlyingValue(), 'ui8');

		if (ser.IsReading())
		{
			const CEnumFlags<EInputFlag> changedKeys = prevInputFlags ^ m_inputFlags;

			const CEnumFlags<EInputFlag> pressedKeys = changedKeys & prevInputFlags;
			if (!pressedKeys.IsEmpty())
			{
				HandleInputFlagChange(pressedKeys, eAAM_OnPress);
			}

			const CEnumFlags<EInputFlag> releasedKeys = changedKeys & prevInputFlags;
			if (!releasedKeys.IsEmpty())
			{
				HandleInputFlagChange(pressedKeys, eAAM_OnRelease);
			}
		}

		ser.EndGroup();
	}
	
	return true;
}

void CPlayer::OnReadyForGameplayOnServer()
{
	CRY_ASSERT(gEnv->bServer, "This function should only be called on the server!");
	
	Vec3 playerScale = Vec3(1.f);
	Quat playerRotation = IDENTITY;

	// Position the player in the center of the map
	const float heightOffset = 20.f;
	const float terrainCenter = gEnv->p3DEngine->GetTerrainSize() / 2.f;
	const float height = gEnv->p3DEngine->GetTerrainZ(terrainCenter, terrainCenter);
	const Vec3 playerPosition = Vec3(terrainCenter, terrainCenter, height + heightOffset);

	const Matrix34 newTransform = Matrix34::Create(playerScale, playerRotation, playerPosition);
	
	Revive(newTransform);
	
	// Invoke the RemoteReviveOnClient function on all remote clients, to ensure that Revive is called across the network
	SRmi<RMI_WRAP(&CPlayer::RemoteReviveOnClient)>::InvokeOnOtherClients(this, RemoteReviveParams{ playerPosition, playerRotation });
	
	// Go through all other players, and send the RemoteReviveOnClient on their instances to the new player that is ready for gameplay
	const int channelId = m_pEntity->GetNetEntity()->GetChannelId();
	CGamePlugin::GetInstance()->IterateOverPlayers([this, channelId](CPlayer& player)
	{
		// Don't send the event for the player itself (handled in the RemoteReviveOnClient event above sent to all clients)
		if (player.GetEntityId() == GetEntityId())
			return;

		// Only send the Revive event to players that have already respawned on the server
		if (!player.m_isAlive)
			return;

		// Revive this player on the new player's machine, on the location the existing player was currently at
		const QuatT currentOrientation = QuatT(player.GetEntity()->GetWorldTM());
		SRmi<RMI_WRAP(&CPlayer::RemoteReviveOnClient)>::InvokeOnClient(&player, RemoteReviveParams{ currentOrientation.t, currentOrientation.q }, channelId);
	});
}

bool CPlayer::RemoteReviveOnClient(RemoteReviveParams&& params, INetChannel* pNetChannel)
{
	// Call the Revive function on this client
	Revive(Matrix34::Create(Vec3(1.f), params.rotation, params.position));

	return true;
}

void CPlayer::Revive(const Matrix34& transform)
{
	m_isAlive = true;
	
	// Set the entity transformation, except if we are in the editor
	// In the editor case we always prefer to spawn where the viewport is
	if(!gEnv->IsEditor())
	{
		m_pEntity->SetWorldTM(transform);
	}

    m_pCC->Physicalize();
	
	// Reset input now that the player respawned
	m_inputFlags.Clear();
	NetMarkAspectsDirty(InputAspect);
	
	m_mouseDeltaRotation = ZERO;
	m_lookOrientation = IDENTITY;
	m_entityDeltaRotation = ZERO;
	m_entityLookOrientation = IDENTITY;
}
#pragma endregion




#pragma region player handler
void CPlayer::HandleInputFlagChange(const CEnumFlags<EInputFlag> flags, const CEnumFlags<EActionActivationMode> activationMode, const EInputFlagType type)
{
	switch (type)
	{
        case EInputFlagType::Hold:
        {
            if (activationMode == eAAM_OnRelease)
            {
                m_inputFlags &= ~flags;
            }
            else
            {
                m_inputFlags |= flags;
            }
        }
        break;
        case EInputFlagType::Toggle:
        {
            if (activationMode == eAAM_OnRelease)
            {
                // Toggle the bit(s)
                m_inputFlags ^= flags;
            }
        }
        break;
	}
	
	if(IsLocalClient())
	{
		NetMarkAspectsDirty(InputAspect);
	}
}


void CPlayer::JumpHandleInputFlagChange(CEnumFlags<EInputFlag> flags, CEnumFlags<EActionActivationMode> activationMode, EInputFlagType type)
{
    switch (type)
    {
        case EInputFlagType::Hold:
        {
            if (activationMode == eAAM_OnRelease)
            {
                m_jumpInputFlags &= ~flags;
            }
            else
            {
                m_jumpInputFlags |= flags;
            }
        }
        break;


        case EInputFlagType::Toggle:
        {
            m_jumpInputFlags ^= flags;
        }
        break;
    }

    if(IsLocalClient())
	{
		NetMarkAspectsDirty(InputAspect);
	}
}


void CPlayer::GroundMovementHandler(float dt)
{
    if (!m_pCC->IsOnGround()) return;

    Vec3 velocity = ZERO;

    // Check input to calculate local space velocity
    if (m_inputFlags & EInputFlag::MoveLeft)
    {
        velocity.x -= m_movementSpeed * dt;
    }
    if (m_inputFlags & EInputFlag::MoveRight)
    {
        velocity.x += m_movementSpeed * dt;
    }
    if (m_inputFlags & EInputFlag::MoveForward)
    {
        velocity.y += m_movementSpeed * dt;
    }
    if (m_inputFlags & EInputFlag::MoveBack)
    {
        velocity.y -= m_movementSpeed * dt;
    }

    if (m_inputFlags & EInputFlag::DoSprint)
    {
        if (m_stamina > m_staminaMinLimit)
            m_pCC->AddVelocity(GetEntity()->GetWorldRotation() * (velocity * m_sprintMultiplier));

            // reduce stamina if moving
            if (m_pCC->GetVelocity() != Vec3(0, 0, 0))
            {
                m_stamina -= m_staminaReductionRate * dt;
            }
    }
    else
    {
        m_pCC->AddVelocity(GetEntity()->GetWorldRotation() * velocity);
    }
}


void CPlayer::CameraMovementHandler(float dt)
{
    #pragma region root or parent section
    // core/root entity world tranformation
    Matrix34 etf = m_pEntity->GetWorldTM();
    Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_entityLookOrientation));

    ypr.x += m_entityDeltaRotation.x * (m_rotationSpeed * m_sensitivity);
    ypr.y = 0;
    ypr.z = 0;

    m_entityLookOrientation = Quat(CCamera::CreateOrientationYPR(ypr));
    m_entityDeltaRotation = ZERO;

    etf.SetRotation33(CCamera::CreateOrientationYPR(ypr));

    m_pEntity->SetWorldTM(etf);
    #pragma endregion


    #pragma region camera section
    // camera entity world tranformation
    Matrix34 ctf = m_pCamera->GetTransformMatrix();
    Ang3 rypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));

    rypr.x = 0; // x axis already handle #pragma region root or parent section
    rypr.y = CLAMP(rypr.y + m_mouseDeltaRotation.y * (m_rotationSpeed * m_sensitivity), m_rotationLimitsMinPitch, m_rotationLimitsMaxPitch);
    rypr.z = 0;

    m_lookOrientation = Quat(CCamera::CreateOrientationYPR(rypr));
    m_mouseDeltaRotation = ZERO;

    ctf.SetRotation33(CCamera::CreateOrientationYPR(rypr));
    
    ctf.SetTranslation(m_defaultCameraPosition); // camera adjustment

    m_pCamera->SetTransformMatrix(ctf);
    m_pAudio->SetOffset(ctf.GetTranslation());
    #pragma endregion
}


void CPlayer::StaminaHanlder(float dt)
{
    m_stamina += m_staminaRegenerationRate * dt;
}


void CPlayer::GroundJumpHandler(float dt)
{
    float tmpjumpcharge = 0.0f; // temporal jump charge

    if (m_jumpInputFlags & EInputFlag::DoJump)
    {
        m_jumpDurationOnHold += 1.f * dt;

        (m_jumpDurationOnHold >= m_minJumpCharge)
            ?   
                tmpjumpcharge = m_jumpDurationOnHold,
                m_canJumpNow = true,
                m_stamina -= m_staminaReductionRate * dt
            :   
                tmpjumpcharge = 1.f,
                m_canJumpNow = true,
                m_stamina -= m_staminaReductionRate * dt;

        (tmpjumpcharge >= m_maxJumpCharge)
            ? m_jumpCharge = m_maxJumpCharge
            : m_jumpCharge = tmpjumpcharge;
    }
    else
    {
        m_jumpDurationOnHold = 0.f;
    }

    if (m_canJumpNow && m_jumpDurationOnHold == 0.0f && m_stamina >= m_staminaMinLimit)
    {
        if (m_pCC->IsOnGround())
        {
            m_pCC->AddVelocity(Vec3(0, 0, ((m_jumpForce * m_jumpCharge) * m_jumpChargeMultiplier)));
        }

        m_canJumpNow = false;
        m_jumpDurationOnHold = 0.f;
        m_jumpCharge = DVCPlayer::jumpCharge;
        m_jumpChargeMultiplier = DVCPlayer::jumpChargeMultiplier;
        tmpjumpcharge = 0.0f;
    }
}


void CPlayer::AimStanceHandler()
{
    (m_aimStance)
        ? m_movementSpeed = DVCPlayer::movementSpeed/2
        : m_movementSpeed = DVCPlayer::movementSpeed;
}
#pragma endregion
