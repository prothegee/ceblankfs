#pragma once
#include "GameConfig.h"


namespace ceblankfs
{
namespace players
{

class PlayerCamera
    :   public IEntityComponent
{
private:
    bool cam_isAlive = false;


public:
    // default value for PlayerCamera
    struct DVPlayerCamera
    {
        static constexpr float rotationLimitsMinPitch = -0.9f; // df : -.84f
        static constexpr float rotationLimitsMaxPitch = 1.3f; // df: 1.5f
    };

public:
    PlayerCamera(/* args */);
    virtual ~PlayerCamera();


    virtual void Initialize() override
    {
        initializePlayerCamera();
    }


    // initialize PlayerCamera
    void initializePlayerCamera()
    {
        m_pcPc = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();

        m_cInput = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();

    #pragma region action register
        m_cInput->RegisterAction(
            "player",
            "mouse_rotateyaw",
            [this](int activationMode, float value)
            {
                m_mouseDeltaRotation.x -= value;
                m_parentDeltaRotation.x -= value;
            });
            m_cInput->BindAction(
                "player",
                "mouse_rotateyaw",
                eAID_KeyboardMouse,
                eKI_MouseX);

        m_cInput->RegisterAction(
            "player",
            "mouse_rotatepitch",
            [this](int activationMode, float value)
            {
                m_mouseDeltaRotation.y -= value;
                m_parentDeltaRotation.y -= value;
            });
            m_cInput->BindAction(
                "player",
                "mouse_rotatepitch",
                eAID_KeyboardMouse,
                eKI_MouseY);
    #pragma endregion
    
        #ifndef NDEBUG
        CryLog("### PlayerCamera::initializePlayerCamera");
        #endif
    }


#pragma region event listener
    virtual Cry::Entity::EventFlags GetEventMask() const override
    {
        return
            Cry::Entity::EEvent::GameplayStarted |
            Cry::Entity::EEvent::Reset |
            Cry::Entity::EEvent::Update
            ;
    }


    virtual void ProcessEvent(const SEntityEvent& e) override
    {
        switch (e.event)
        {
            case Cry::Entity::EEvent::GameplayStarted:
            {
                
            }
            break;


            case Cry::Entity::EEvent::Reset:
            {
                cam_isAlive = e.nParam[0] != 0;
                #ifndef NDEBUG
                CryLog("### PlayerCamera cam_isAlive: %s", cam_isAlive ? "true" : "false");
                #endif
            }
            break;


            case Cry::Entity::EEvent::Update:
            {
                if (!cam_isAlive) return;

                const float dt = e.fParam[0]; // frametime or deltatime

                PC_CameraViewLogic(dt);
                PC_ParentRotationLogic(dt);
            }
            break;
        }
    }
#pragma endregion


    static void ReflectType(Schematyc::CTypeDesc<PlayerCamera>& desc)
    {
        desc.SetGUID("{90085a54-7f05-46aa-b203-d614a938c0cc}"_cry_guid);
        desc.SetLabel("PlayerCamera");
        desc.SetEditorCategory("_players");
        desc.SetDescription("Core player camera entity");
        desc.AddMember(
            &PlayerCamera::m_rotationLimitsMinPitch,
            'rlmn',
            "playerCameraRotationLimitsMinPitch",
            "rotation min pitch",
            "rotation min pitch value",
            DVPlayerCamera::rotationLimitsMinPitch
        );
        desc.AddMember(
            &PlayerCamera::m_rotationLimitsMaxPitch,
            'rlmx',
            "playerCameraRotationLimitsMaxPitch",
            "rotation max pitch",
            "rotation max pitch value",
            DVPlayerCamera::rotationLimitsMaxPitch
        );
    }


protected:
    Vec2 m_mouseDeltaRotation, m_parentDeltaRotation;
    Quat m_lookOrientation, m_parentLookOrientation;
    const float m_rotationSpeed = 0.002f;
    
    // camera min rotation limit
    float m_rotationLimitsMinPitch
        = DVPlayerCamera::rotationLimitsMinPitch; 
    // camera max rotation limit
    float m_rotationLimitsMaxPitch
        = DVPlayerCamera::rotationLimitsMaxPitch;

    // camera input controller from PlayerCamera
    Cry::DefaultComponents::CInputComponent* m_cInput = nullptr;

    // player camera pointer from PlayerCamera
    Cry::DefaultComponents::CCameraComponent* m_pcPc = nullptr;

protected:
    /**
     * @brief player camera view logic
     * 
     * @param dt 
     */
    void PC_CameraViewLogic(float dt)
    {
        Matrix34 transformation = m_pEntity->GetWorldTM();
        Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));

        ypr.x += m_mouseDeltaRotation.x * m_rotationSpeed;
		ypr.y = CLAMP(ypr.y + m_mouseDeltaRotation.y * m_rotationSpeed, m_rotationLimitsMinPitch, m_rotationLimitsMaxPitch);
        ypr.z = 0;

        m_lookOrientation = Quat(CCamera::CreateOrientationYPR(ypr));
        m_mouseDeltaRotation = ZERO;

        transformation.SetRotation33(CCamera::CreateOrientationYPR(ypr));

        m_pcPc->GetEntity()->SetWorldTM(transformation);
    }


    /**
     * @brief player parent rotation logic on x axis only
     * 
     * @param dt 
     */
    void PC_ParentRotationLogic(float dt)
    {
        auto parent = m_pEntity->GetParent();
        auto parentTransform = parent->GetWorldTM();

        Ang3 pYpr = CCamera::CreateAnglesYPR(Matrix33(m_parentLookOrientation));
        pYpr.x += m_parentDeltaRotation.x * m_rotationSpeed;
        pYpr.y = 0;
        pYpr.z = 0;

        m_parentLookOrientation = Quat(CCamera::CreateOrientationYPR(pYpr));
        m_parentDeltaRotation = ZERO;
        
        parentTransform.SetRotation33(CCamera::CreateOrientationYPR(pYpr));

        parent->SetWorldTM(parentTransform);
    }
};
 

} // namespace players
} // namespace ceblankfs