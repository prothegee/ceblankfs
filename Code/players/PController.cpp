#include "PController.h"


using namespace ceblankfs::players;


#pragma region component and registrar
static void RegisterPControllerComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(PController));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPControllerComponent);
PController::PController(/* args */)
{
}
PController::~PController()
{
}
#pragma endregion




#pragma region initialize and binding
void PController::Initialize()
{
    AssignInputController();
    RegisterPControllerPointer();

    if (GameConfig::IS_DEBUG)
    {
        CryLog("### players::PController::Initialize");
    }
}
#pragma endregion




#pragma region event listener
Cry::Entity::EventFlags PController::GetEventMask() const
{
    return
        Cry::Entity::EEvent::Reset |
        Cry::Entity::EEvent::Update
        ;
}


void PController::ProcessEvent(const SEntityEvent& e)
{
    switch (e.event)
    {
        case Cry::Entity::EEvent::Reset:
        {
            m_isAlive = e.nParam[0] != 0;

            // log base on game config
            if (GameConfig::IS_DEBUG)
            {
                CryLog("### PController::m_isAlive: %s", m_isAlive ? "true" : "false");
            }
        }
        break;


        case Cry::Entity::EEvent::Update:
        {
            if (!m_isAlive) return;
        }
        break;
    }
}
#pragma endregion




#pragma region functions or methods
void PController::AssignInputController()
{
    m_pInput = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();

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
            HandleInputFlagChange(
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
                m_parentDeltaRotation.x -= value;
            });
            m_pInput->BindAction(
                "player",
                "mouse_rotateyaw",
                eAID_KeyboardMouse,
                eKI_MouseX);

    m_pInput->RegisterAction(
        "player",
        "mouse_rotatepitch",
        [this](int activationMode, float value)
        {
            m_mouseDeltaRotation.y -= value;
            m_parentDeltaRotation.y -= value;
        });
        m_pInput->BindAction(
            "player",
            "mouse_rotatepitch",
            eAID_KeyboardMouse,
            eKI_MouseY);
}


void PController::HandleInputFlagChange(CEnumFlags<EInputFlag> flags, CEnumFlags<EActionActivationMode> activationMode, EInputFlagType type)
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


void PController::RegisterPControllerPointer()
{
    m_pController = m_pEntity->GetOrCreateComponent<ceblankfs::players::PController>();
}
#pragma endregion
