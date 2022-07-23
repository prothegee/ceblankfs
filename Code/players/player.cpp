#include "player.h"


#pragma region component and registrar
static void RegisterPlayerComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(
            SCHEMATYC_MAKE_ENV_COMPONENT(ceblankfs::players::player));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayerComponent);
ceblankfs::players::player::player(/* args */)
{
}
ceblankfs::players::player::~player()
{
}
#pragma endregion



void ceblankfs::players::player::Initialize()
{
    DefaultValue();
    PointerComponentsRegistrar();
    RegisterInputActionAndBinding();

    #ifndef NDEBUG
    CryLog("# players::player::Initialize");
    #else
    #endif
}




#pragma region events listener
Cry::Entity::EventFlags ceblankfs::players::player::GetEventMask() const
{
    return
        Cry::Entity::EEvent::GameplayStarted |
        Cry::Entity::EEvent::Reset |
        Cry::Entity::EEvent::Update
        ;
}


void ceblankfs::players::player::ProcessEvent(const SEntityEvent& e)
{
    switch (e.event)
    {
        case Cry::Entity::EEvent::GameplayStarted:
        {
            #ifndef NDEBUG
            CryLog("# players::player::GameplayStarted");
            #else
            #endif
        }
        break;

        case Cry::Entity::EEvent::Reset:
        {
            m_isAlive = e.nParam[0] != 0;

            #ifndef NDEBUG
            CryLog("# players::player::ProcessEvent m_isAlive: %s", m_isAlive ? "true" : "false");
            #else
            #endif
        }
        break;

        case Cry::Entity::EEvent::Update:
        {
            if (!m_isAlive) return;

            ValuePolicy();

            const float dt = e.fParam[0];

            MovementHandler(dt);
            JumpHandler(dt);
            CharacterControllerHandlerRotationX(dt);
            CameraViewHandler(dt);
        }
        break;
    }
}
#pragma endregion




#pragma region functions
void ceblankfs::players::player::DefaultValue()
{
    if (m_username.length() <= 3) { m_username = "username"; }
    if (m_character.length() <= 3) { m_character = "character"; }

    m_velocity = DVplayer::velocity;
    m_health = DVplayer::health;
    m_stamina = DVplayer::stamina;
    m_movementSpeed = DVplayer::movementSpeed;
    m_jumpForce = DVplayer::jumpForce;
    m_jumpCharge = DVplayer::jumpCharge;
    m_jumpChargeMultiplier = DVplayer::jumpChargeMultiplier;
    m_jumpDurationOnHold = DVplayer::jumpDurationOnHold;
    m_weight = DVplayer::weight;
    m_sprintMultiplier = DVplayer::sprintMultiplier;
}


void ceblankfs::players::player::ValuePolicy()
{
    (m_health < 0.f) ? m_health = 0.f : m_health = this->m_health;
    (m_armor < 0.f) ? m_armor = 0.f : m_armor = this->m_armor;
    (m_stamina < 0.f) ? m_stamina = 0.f : m_stamina = this->m_stamina;
    (m_movementSpeed < 0.f) ? m_movementSpeed = 0.f : m_movementSpeed = this->m_movementSpeed;
    (m_jumpForce < 0.f) ? m_jumpForce = 0.f : m_jumpForce = this->m_jumpForce;
    (m_jumpCharge < 0.f) ? m_jumpCharge = 0.f : m_jumpCharge = this->m_jumpCharge;
    (m_jumpChargeMultiplier < 0.f) ? m_jumpChargeMultiplier = 0.f : m_jumpChargeMultiplier = this->m_jumpChargeMultiplier;
    (m_jumpDurationOnHold < 0.f) ? m_jumpDurationOnHold = 0.f : m_jumpDurationOnHold = this->m_jumpDurationOnHold;
    (m_weight < 0.f) ? m_weight = 0.f : m_weight = this->m_weight;
    (m_sprintMultiplier < 0.f) ? m_sprintMultiplier = 0.f : m_sprintMultiplier = this->m_sprintMultiplier;
}


void ceblankfs::players::player::PointerComponentsRegistrar()
{
    m_pCC = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
    m_pCC->Physicalize();

    m_pCamera = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();

    #ifndef NDEBUG
    CryLog("# players::player::PointerComponentsRegistrar");
    #else
    #endif
}

void ceblankfs::players::player::RegisterInputActionAndBinding()
{
    m_pInput = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();

    #pragma region input register and binding
    m_pInput->RegisterAction(
        "player",
        "forward",
        [this](int activationMode, float value)
        {
            HandleInputFlagChange(
                EInputFlag::forward,
                (EActionActivationMode)activationMode,
                EInputFlagType::Hold
            );
        });
        m_pInput->BindAction(
            "player",
            "forward",
            eAID_KeyboardMouse,
            EKeyId::eKI_W);

    m_pInput->RegisterAction(
        "player",
        "backward",
        [this](int activationMode, float value)
        {
            HandleInputFlagChange(
                EInputFlag::backward,
                (EActionActivationMode)activationMode,
                EInputFlagType::Hold
            );
        });
        m_pInput->BindAction(
            "player",
            "backward",
            eAID_KeyboardMouse,
            EKeyId::eKI_S);

    m_pInput->RegisterAction(
        "player",
        "left",
        [this](int activationMode, float value)
        {
            HandleInputFlagChange(
                EInputFlag::left,
                (EActionActivationMode)activationMode,
                EInputFlagType::Hold
            );
        });
        m_pInput->BindAction(
            "player",
            "left",
            eAID_KeyboardMouse,
            EKeyId::eKI_A);

    m_pInput->RegisterAction(
        "player",
        "right",
        [this](int activationMode, float value)
        {
            HandleInputFlagChange(
                EInputFlag::right,
                (EActionActivationMode)activationMode,
                EInputFlagType::Hold
            );
        });
        m_pInput->BindAction(
            "player",
            "right",
            eAID_KeyboardMouse,
            EKeyId::eKI_D);

    m_pInput->RegisterAction(
        "player",
        "jump",
        [this](int activationMode, float value)
        {
            JumpHandleInputFlagChange(
                EInputFlag::jump,
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
        "sprint",
        [this](int activationMode, float value)
        {
            HandleInputFlagChange(
                EInputFlag::sprint,
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


void ceblankfs::players::player::HandleInputFlagChange(CEnumFlags<EInputFlag> flags, CEnumFlags<EActionActivationMode> activationMode, EInputFlagType type)
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
            m_inputFlags ^= flags;
        }
        break;
    }
}


void ceblankfs::players::player::JumpHandleInputFlagChange(CEnumFlags<EInputFlag> flags, CEnumFlags<EActionActivationMode> activationMode, EInputFlagType type)
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
}
#pragma endregion


#pragma region player handlers
void ceblankfs::players::player::MovementHandler(float dt)
{
    if (!m_pCC->IsOnGround()) return;

    Vec3 velocity = ZERO;

    if (m_inputFlags & EInputFlag::forward)
    {
        velocity.y += m_movementSpeed * dt;
    }

    if (m_inputFlags & EInputFlag::backward)
    {
        velocity.y -= m_movementSpeed * dt;
    }

    if (m_inputFlags & EInputFlag::left)
    {
        velocity.x -= m_movementSpeed * dt;
    }

    if (m_inputFlags & EInputFlag::right)
    {
        velocity.x += m_movementSpeed * dt;
    }

    if (m_inputFlags & EInputFlag::sprint)
    {
        m_pCC->AddVelocity(GetEntity()->GetWorldRotation() * (velocity * m_sprintMultiplier));
        m_isSprint = true;
    }
    else
    {
        m_pCC->AddVelocity(GetEntity()->GetWorldRotation() * velocity);
        m_isSprint = false;
    }
}


void ceblankfs::players::player::JumpHandler(float dt)
{
    if (!m_pCC->IsOnGround()) return;

    if (m_jumpInputFlags & EInputFlag::jump)
    {
        m_jumpDurationOnHold += 1.f * dt;

        (m_jumpDurationOnHold >= m_minJumpCharge)
            ? m_jumpCharge = m_jumpDurationOnHold
            : m_jumpCharge = 0.f;

        (m_pCC->IsOnGround())
            ? m_canJumpNow = true
            : m_canJumpNow = false;
    }
    else
    {
        m_jumpDurationOnHold = 0.f;
    }

    if (m_canJumpNow && m_jumpDurationOnHold == 0.f)
    {
        if (m_canJumpNow && m_jumpCharge < m_minJumpCharge)
        {
            // bouncy policy when on air then land #1
            (m_pCC->GetVelocity() != Vec3(0, 0, 0))
                ? m_pCC->AddVelocity(Vec3(0, 0, (m_jumpForce * m_jumpChargeMultiplier)))
                : m_pCC->AddVelocity(Vec3(0, 0, (m_jumpForce * 0.f)));

            m_canJumpNow = false;
            m_jumpDurationOnHold = 0.f;

            m_jumpCharge = DVplayer::jumpCharge;
            m_jumpChargeMultiplier = DVplayer::jumpChargeMultiplier;
        }
        if (m_canJumpNow && m_jumpCharge > m_minJumpCharge)
        {
            // bouncy policy when on air then land #1
            (m_pCC->GetVelocity() != Vec3(0, 0, 0))
                ? m_pCC->AddVelocity(Vec3(0, 0, ((m_jumpForce * m_jumpCharge) * m_jumpChargeMultiplier)))
                : m_pCC->AddVelocity(Vec3(0, 0, ((m_jumpForce * m_jumpCharge) * 0.f)));
            
            m_canJumpNow = false;
            m_jumpDurationOnHold = 0.f;

            m_jumpCharge = DVplayer::jumpCharge;
            m_jumpChargeMultiplier = DVplayer::jumpChargeMultiplier;
        }
    }

    // #ifndef NDEBUG
    // CryLog("%s | %i | %f | %f", m_pCC->GetVelocity() == Vec3(0, 0, 0) ? "VZERO" : "NZERO", m_canJumpNow, m_jumpDurationOnHold, m_jumpCharge);
    // #else
    // #endif
}


void ceblankfs::players::player::CharacterControllerHandlerRotationX(float dt)
{
    Matrix34 etf = m_pEntity->GetWorldTM();
    Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_entityLookOrientation));

    ypr.x += m_entityDeltaRotation.x * m_rotationSpeed;
    ypr.y = 0;
    ypr.z = 0;

    m_entityLookOrientation = Quat(CCamera::CreateOrientationYPR(ypr));
    m_entityDeltaRotation = ZERO;

    etf.SetRotation33(CCamera::CreateOrientationYPR(ypr));

    m_pEntity->SetWorldTM(etf);
}


void ceblankfs::players::player::CameraViewHandler(float dt)
{
    Matrix34 ctf = m_pCamera->GetTransformMatrix();
    Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));

    ypr.x = 0;
    ypr.y = CLAMP(ypr.y + m_mouseDeltaRotation.y * m_rotationSpeed, m_rotationLimitsMinPitch, m_rotationLimitsMaxPitch);
    ypr.z = 0;

    m_lookOrientation = Quat(CCamera::CreateOrientationYPR(ypr));
    m_mouseDeltaRotation = ZERO;

    ctf.SetRotation33(CCamera::CreateOrientationYPR(ypr));

    m_pCamera->SetTransformMatrix(ctf);
}
#pragma endregion
