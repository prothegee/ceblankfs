#include "CPCamera.h"


using namespace ceblankfs::players;


#pragma region component and registrar
static void RegisterCPCameraComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPCamera));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterCPCameraComponent);
CPCamera::CPCamera(/* args */)
{
}
CPCamera::~CPCamera()
{
}
#pragma endregion




#pragma region initialize and binding
void CPCamera::Initialize()
{
    RegisterCPPovDefaultValue();
    RegisterCPPovPointer();

    if (GameConfig::IS_DEBUG)
    {
        CryLog("### players::CPPov::Initialize");
    }
}
#pragma endregion




#pragma region event listener
Cry::Entity::EventFlags CPCamera::GetEventMask() const
{
    return
        Cry::Entity::EEvent::GameplayStarted |
        Cry::Entity::EEvent::Reset |
        Cry::Entity::EEvent::Update
        ;
}


void CPCamera::ProcessEvent(const SEntityEvent& e)
{
    switch (e.event)
    {
        case Cry::Entity::EEvent::GameplayStarted:
        {
            // log base on game config
            if (GameConfig::IS_DEBUG)
            {
                CryLog("### CPCamera::m_orientParentX is %s", m_orientParentX ? "true" : "false");
            }
        }
        break;


        case Cry::Entity::EEvent::Reset:
        {
            m_isAlive = e.nParam[0] != 0;

            // log base on game config
            if (GameConfig::IS_DEBUG)
            {
                CryLog("### CPPov::m_isAlive: %s", m_isAlive ? "true" : "false");
            }
        }
        break;


        case Cry::Entity::EEvent::Update:
        {
            if (!m_isAlive) return;

            const float dt = e.fParam[0]; // delta time/frame time

            MainCameraLogic(dt);
            OrientParentRotationOnX(dt);
        }
        break;
    }
}
#pragma endregion




#pragma region functions or methods
void CPCamera::RegisterCPPovPointer()
{
    m_pCamera = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();

    m_pController = m_pEntity->GetOrCreateComponent<ceblankfs::players::CPController>();
}


void CPCamera::RegisterCPPovDefaultValue()
{
    m_fov = DVCPCamera::fov;
    m_orientParentX = DVCPCamera::orientParentX;
    m_rotationLimitsMinPitch = DVCPCamera::rotationLimitsMinPitch;
    m_rotationLimitsMaxPitch = DVCPCamera::rotationLimitsMaxPitch;
}


void CPCamera::MainCameraLogic(float dt)
{
    // Matrix34 transform = IDENTITY;
    Matrix34 transform = m_pEntity->GetWorldTM();
    Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_pController->m_lookOrientation));

    ypr.x += m_pController->m_mouseDeltaRotation.x * m_pController->m_rotationSpeed;
    ypr.y = CLAMP(ypr.y + m_pController->m_mouseDeltaRotation.y * m_pController->m_rotationSpeed, m_rotationLimitsMinPitch, m_rotationLimitsMaxPitch);
    ypr.z = 0;

    m_pController->m_lookOrientation = Quat(CCamera::CreateOrientationYPR(ypr));
    m_pController->m_mouseDeltaRotation = ZERO;

    transform.SetRotation33(CCamera::CreateOrientationYPR(ypr));

    m_pCamera->GetEntity()->SetWorldTM(transform);
}


void CPCamera::OrientParentRotationOnX(float dt)
{
    if (m_orientParentX)
    {
        auto parent = m_pEntity->GetParent();

        auto parentTranform = parent->GetWorldTM();

        Ang3 pYpr = CCamera::CreateAnglesYPR(Matrix33(m_pController->m_parentLookOrientation));
        pYpr.x += m_pController->m_parentDeltaRotation.x * m_pController->m_rotationSpeed;
        pYpr.y = 0;
        pYpr.z = 0;

        m_pController->m_parentLookOrientation = Quat(CCamera::CreateOrientationYPR(pYpr));
        m_pController->m_parentDeltaRotation = ZERO;

        parentTranform.SetRotation33(CCamera::CreateOrientationYPR(pYpr));

        parent->SetWorldTM(parentTranform);
    }
    else
    {
        // what?
    }
}
#pragma endregion
