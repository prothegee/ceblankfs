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
    PlayerCamera(/* args */);
    virtual ~PlayerCamera();


    virtual void Initialize() override
    {
        initializePlayerCamera();
    }


    // initialize PlayerCamera
    void initializePlayerCamera()
    {
        pcPc = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();

        m_cInput = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();

    #pragma region action register
        m_cInput->RegisterAction(
            "player",
            "mouse_rotateyaw",
            [this](int activationMode, float value)
            {
                m_mouseDeltaRotation.x -= value;
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
            Cry::Entity::EEvent::Reset |
            Cry::Entity::EEvent::Update
            ;
    }


    virtual void ProcessEvent(const SEntityEvent& e) override
    {
        switch (e.event)
        {
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

                PC_ViewLogic(dt);
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
    }


protected:
    Vec2 m_mouseDeltaRotation;
    Quat m_lookOrientation;
    const float m_rotationSpeed = 0.002f;

    // camera input controller
    Cry::DefaultComponents::CInputComponent* m_cInput = nullptr;

    // player camera pointer from PlayerCamera
    Cry::DefaultComponents::CCameraComponent* pcPc = nullptr;

protected:
    // player camera view logic
    void PC_ViewLogic(float dt)
    {
    #pragma region v1
        Matrix34 transformation = m_pEntity->GetWorldTM();
        Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));

        ypr.x += m_mouseDeltaRotation.x * m_rotationSpeed;

        const float rotationLimitsMinPitch = -0.84f;
        const float rotationLimitsMaxPitch = 1.5f;
		ypr.y = CLAMP(ypr.y + m_mouseDeltaRotation.y * m_rotationSpeed, rotationLimitsMinPitch, rotationLimitsMaxPitch);

        ypr.z = 0;

        m_lookOrientation = Quat(CCamera::CreateOrientationYPR(ypr));
        m_mouseDeltaRotation = ZERO;

        transformation.SetRotation33(CCamera::CreateOrientationYPR(ypr));

        pcPc->GetEntity()->SetWorldTM(transformation);
    #pragma endregion
    }
};
 

} // namespace players
} // namespace ceblankfs