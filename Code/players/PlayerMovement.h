#pragma once
#include "GameConfig.h"
#include "PlayerData.h"
#include "PlayerCamera.h"


namespace ceblankfs
{
namespace players
{

/**
 * @brief core player movement handler
 * 
 */
class PlayerMovement final
    :   public IEntityComponent
{
private:
    float jumpDurationOnHold = 0.f;
    float jumpCharge = 0.f;
    bool youCanJump = false;
    bool cc_isAlive = false;


public:
    enum class EInputFlagType
    {
        Hold = 0,
        Toggle
    };

    enum class EInputFlag : uint8
    {
        forward = 1 << 0,
        backward = 1 << 1,
        left = 1 << 2,
        right = 1 << 3,
        jump = 1 << 4,
        sprint = 1 << 5,
    };

public:
    PlayerMovement(/* args */);
    virtual ~PlayerMovement();


    virtual void Initialize() override
    {
        initializePlayerMovement();
    }


    // initialize PlayerMovement
    void initializePlayerMovement()
    {
        m_pInput = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();

    #pragma region action register
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
    #pragma endregion

        m_pCC = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
        m_pCC->SetTransformMatrix(
            Matrix34::Create(Vec3(1.f),
            IDENTITY,
            Vec3(0, 0, 1.f)));

        pmPd = m_pEntity->GetOrCreateComponent<PlayerData>();
        pmPd->initializePlayerData();

        #ifndef NDEBUG
        CryLog("### PlayerMovement::initializePlayerMovement");
        #endif
    }


    // player movement event flag change
    void HandleInputFlagChange(
        CEnumFlags<EInputFlag> flags,
        CEnumFlags<EActionActivationMode> activationMode,
        EInputFlagType type
    )
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
                cc_isAlive = e.nParam[0] != 0;
                #ifndef NDEBUG
                CryLog("### PlayerMovement cc_isAlive: %s", cc_isAlive ? "true" : "false");
                #endif
            }
            break;


            case Cry::Entity::EEvent::Update:
            {
                if (!cc_isAlive) return;

                const float dt = e.fParam[0]; // frametime or deltatime

                PM_JumpLogic(dt);
                PM_MovementLogic(dt);
            }
            break;
        }
    }
#pragma endregion


    static void ReflectType(Schematyc::CTypeDesc<PlayerMovement>& desc)
    {
        desc.SetGUID("{c7b725e9-8885-4852-bb5c-930a4d2a7924}"_cry_guid);
        desc.SetLabel("PlayerMovement");
        desc.SetEditorCategory("_players");
        desc.SetDescription("Core player movement entity");
    }


protected:
    CEnumFlags<EInputFlag> m_inputFlags;

    // player data pointer from PlayerMovement
    PlayerData* pmPd = nullptr;

    // player input pointer
    Cry::DefaultComponents::CInputComponent*
        m_pInput = nullptr;
    // player character controller pointer
    Cry::DefaultComponents::CCharacterControllerComponent*
        m_pCC = nullptr;

protected:
    // update player movement 
    void PM_MovementLogic(float dt)
    {
        if (!m_pCC->IsOnGround()) return;
        
        Vec3 velocity = ZERO;

        if (m_inputFlags & EInputFlag::forward) // move forward
        {
            velocity.y += pmPd->m_movementSpeed * dt;

            #ifndef NDEBUG
            // CryLog("### Player moving %f forward", velocity.y);
            #endif
        }

        if (m_inputFlags & EInputFlag::backward) // move backward
        {
            velocity.y -= pmPd->m_movementSpeed * dt;

            #ifndef NDEBUG
            // CryLog("### Player moving %f backward", velocity.y);
            #endif
        }

        if (m_inputFlags & EInputFlag::left) // move left
        {
            velocity.x -= pmPd->m_movementSpeed * dt;

            #ifndef NDEBUG
            // CryLog("### Player moving %f left", velocity.x);
            #endif
        }

        if (m_inputFlags & EInputFlag::right) // move right
        {
            velocity.x += pmPd->m_movementSpeed * dt;

            #ifndef NDEBUG
            // CryLog("### Player moving %f right", velocity.x);
            #endif
        }

        // update character controller to move when sprint or not
        if (m_inputFlags & EInputFlag::sprint)
        {
            m_pCC->AddVelocity(GetEntity()->GetWorldRotation() * (velocity * pmPd->m_sprintMultiplier));
        }
        else
        {
            m_pCC->AddVelocity(GetEntity()->GetWorldRotation() * velocity);
        }
        
    }


    // player movement jump logic
    void PM_JumpLogic(float dt)
    {
        if (!m_pCC->IsOnGround()) return;

        if (m_inputFlags & EInputFlag::jump) // do jump
        {
            jumpDurationOnHold += 1.f * dt;

            // POLICY PlayerData
            (jumpDurationOnHold > pmPd->pv_minJumpCharge)
                ? pmPd->m_jumpCharge = jumpDurationOnHold
                : pmPd->m_jumpCharge = 0.f;

            jumpCharge = pmPd->m_jumpCharge;
            youCanJump = true;
        }
        else
        {
            jumpDurationOnHold = 0.f;
        }

        // update character controller to jump
        if (youCanJump && jumpDurationOnHold == 0.f)
        {
            if (jumpCharge == 0.f)
            {
                m_pCC->AddVelocity(Vec3(0, 0, pmPd->m_jumpForce));

                jumpCharge = 0.f;
                youCanJump = false;
            }
            else
            {
                m_pCC->AddVelocity(Vec3(0, 0, (pmPd->m_jumpForce * jumpCharge)));

                jumpCharge = 0.f;
                youCanJump = false;
            }
        }
    }
};

} // namespace players
} // namespace ceblankfs