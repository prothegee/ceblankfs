// ONLY for player initialization
#include "PCore.h"
#include <GamePlugin.h>
#include <CryNetwork/Rmi.h> // required on *.cc or *.cpp file


#pragma region initialize
void PCore::Initialize()
{
	// Mark the entity to be replicated over the network
	m_pEntity->GetNetEntity()->BindToNetwork();
	
	// Register the RemoteReviveOnClient function as a Remote Method Invocation (RMI) that can be executed by the server on clients
	SRmi<RMI_WRAP(&PCore::RemoteReviveOnClient)>::Register(this, eRAT_NoAttach, false, eNRT_ReliableOrdered);
}

void PCore::InitializeLocalPlayer()
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
                CryLog("# players PCore shoot");
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
                CryLog("# players PCore m_aimStance %s", m_aimStance ? "true" : "false");
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


#pragma region player misc1
bool PCore::NetSerialize(TSerialize ser, EEntityAspects aspect, uint8 profile, int flags)
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

void PCore::OnReadyForGameplayOnServer()
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
	SRmi<RMI_WRAP(&PCore::RemoteReviveOnClient)>::InvokeOnOtherClients(this, RemoteReviveParams{ playerPosition, playerRotation });
	
	// Go through all other players, and send the RemoteReviveOnClient on their instances to the new player that is ready for gameplay
	const int channelId = m_pEntity->GetNetEntity()->GetChannelId();
	CGamePlugin::GetInstance()->IterateOverPlayers([this, channelId](PCore& player)
	{
		// Don't send the event for the player itself (handled in the RemoteReviveOnClient event above sent to all clients)
		if (player.GetEntityId() == GetEntityId())
			return;

		// Only send the Revive event to players that have already respawned on the server
		if (!player.m_isAlive)
			return;

		// Revive this player on the new player's machine, on the location the existing player was currently at
		const QuatT currentOrientation = QuatT(player.GetEntity()->GetWorldTM());
		SRmi<RMI_WRAP(&PCore::RemoteReviveOnClient)>::InvokeOnClient(&player, RemoteReviveParams{ currentOrientation.t, currentOrientation.q }, channelId);
	});
}

bool PCore::RemoteReviveOnClient(RemoteReviveParams&& params, INetChannel* pNetChannel)
{
	// Call the Revive function on this client
	Revive(Matrix34::Create(Vec3(1.f), params.rotation, params.position));

	return true;
}

void PCore::Revive(const Matrix34& transform)
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
