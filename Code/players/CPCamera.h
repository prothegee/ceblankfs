#pragma once
#include "GameConfig.h"
#include "CPController.h"


namespace ceblankfs
{
namespace players
{

/**
 * @brief class player camera
 * 
 */
class CPCamera
    :   public IEntityComponent
{
private:
    bool m_isAlive = false;
    bool m_orientParentX;




public:
    struct DVCPCamera
    {
        static constexpr bool orientParentX = true;

        static constexpr float fov = 80.f;
        static constexpr float rotationLimitsMinPitch = -0.9f; // df : -.84f
        static constexpr float rotationLimitsMaxPitch = 1.3f; // df: 1.5f
    };

    // field of view
    float m_fov;
    // min rotation pitch
    float m_rotationLimitsMinPitch;
    // max rotation pitch
    float m_rotationLimitsMaxPitch;

public:
    CPCamera(/* args */);
    virtual ~CPCamera();


    virtual void Initialize() override;


    virtual Cry::Entity::EventFlags GetEventMask() const override;
    virtual void ProcessEvent(const SEntityEvent& e) override;


    static void ReflectType(Schematyc::CTypeDesc<CPCamera>& desc)
    {
        desc.SetGUID("{60c6297d-5905-4a8f-aaef-41c79774a2c2}"_cry_guid);
        desc.SetLabel("CPCamera");
        desc.SetEditorCategory("_players");
        desc.SetDescription("Players camera");
        desc.AddMember(
            &CPCamera::m_fov,
            'pfov',
            "PlayerFieldOfView",
            "fov",
            "fov value",
            DVCPCamera::fov
        );
        desc.AddMember(
            &CPCamera::m_orientParentX,
            'popx',
            "PlayerFieldOfViewOrientParentX",
            "oritent parent on x",
            "oritent parent on x value",
            DVCPCamera::orientParentX
        );
        desc.AddMember(
            &CPCamera::m_rotationLimitsMinPitch,
            'prmn',
            "PlayerCameraRotationMinPitch",
            "rotation min pitch",
            "rotation min pitch value",
            DVCPCamera::rotationLimitsMinPitch
        );
        desc.AddMember(
            &CPCamera::m_rotationLimitsMaxPitch,
            'prmx',
            "PlayerCameraRotationMaxPitch",
            "rotation max pitch",
            "rotation max pitch value",
            DVCPCamera::rotationLimitsMaxPitch
        );
    }




protected:
    // player camera pointer from CPPov
    Cry::DefaultComponents::CCameraComponent* m_pCamera = nullptr;

    // player controller pointer from CPPov
    ceblankfs::players::CPController* m_pController = nullptr;

protected:
    /**
     * @brief CPPov register pointer
     * 
     */
    void RegisterCPPovPointer();


    /**
     * @brief set default value for CPPov class
     * 
     */
    void RegisterCPPovDefaultValue();


    /**
     * @brief main view player camera logic
     * 
     * @param dt float
     */
    void MainCameraLogic(float dt);


    /**
     * @brief orient parent rotation on x axis
     * 
     * @param dt 
     */
    void OrientParentRotationOnX(float dt);
};

} // namespace players
} // namespace ceblankfs
