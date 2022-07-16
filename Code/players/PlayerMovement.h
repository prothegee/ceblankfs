#pragma once
#include "GameConfig.h"
#include "PlayerData.h"


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
    };

public:
    PlayerMovement(/* args */);
    virtual ~PlayerMovement();


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


    static void ReflectType(Schematyc::CTypeDesc<PlayerMovement>& desc)
    {
        desc.SetGUID("{c7b725e9-8885-4852-bb5c-930a4d2a7924}"_cry_guid);
        desc.SetLabel("PlayerMovement");
        desc.SetEditorCategory("_players");
        desc.SetDescription("Core player movement entity");
    }


public:
    CEnumFlags<EInputFlag> m_inputFlags;

    // player input pointer
    Cry::DefaultComponents::CInputComponent*
        m_pInput = nullptr;
    // player character controller pointer
    Cry::DefaultComponents::CCharacterControllerComponent*
        m_pCC = nullptr;

public:
    // update player movement on request
    void HanldePlayerMovementRequest(float dt)
    {
        if (!m_pCC->IsOnGround()) return;
        
        Vec3 velocity = ZERO;

        if (m_inputFlags & EInputFlag::forward) // move forward
        {
            velocity.y += pmPd->m_movementSpeed * dt;

            #ifndef NDEBUG
            CryLog("### Player moving forward %f", velocity.y);
            #endif
        }

        if (m_inputFlags & EInputFlag::backward) // move backward
        {
            velocity.y -= pmPd->m_movementSpeed * dt;

            #ifndef NDEBUG
            CryLog("### Player moving backward %f", velocity.y);
            #endif
        }

        if (m_inputFlags & EInputFlag::left) // move left
        {
            velocity.x -= pmPd->m_movementSpeed * dt;

            #ifndef NDEBUG
            CryLog("### Player moving left %f", velocity.x);
            #endif
        }

        if (m_inputFlags & EInputFlag::right) // move right
        {
            velocity.x += pmPd->m_movementSpeed * dt;

            #ifndef NDEBUG
            CryLog("### Player moving right %f", velocity.x);
            #endif
        }

        // update character controller to move
        m_pCC->AddVelocity(GetEntity()->GetWorldRotation() * velocity);
    }


    // update player jump response on request
    void HandlePlayerJumpRequest(float dt)
    {
        if (m_inputFlags & EInputFlag::jump) // do jump
        {
            jumpDurationOnHold += 1.f * dt;

            // POLICY PlayerData
            (jumpDurationOnHold > pmPd->pv_minJumpCharge)
                ? pmPd->m_jumpCharge = jumpDurationOnHold
                : pmPd->m_jumpCharge = 0.f;

            #ifndef NDEBUG
            CryLog(
                "### Player jump | hold duration: %f | mode: %s | charge: %f",
                    jumpDurationOnHold,
                    (jumpDurationOnHold < pmPd->pv_minJumpCharge)
                        ? "normal"
                        : "charging",
                    pmPd->m_jumpCharge
            );
            #endif
        }
        else
        {
            jumpDurationOnHold = 0.f; // kinda insecure, so set to 0 again
        }

        // update character controller to jump
        if (m_pCC->IsOnGround())
        {
            m_pCC->AddVelocity(Vec3(0, 0, pmPd->m_jumpForce));
        }
    }


protected:
    PlayerData* pmPd = nullptr; // player data pointer from PlayerMovement
};

} // namespace players
} // namespace ceblankfs