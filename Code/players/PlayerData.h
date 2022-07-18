#pragma once
#include "GameConfig.h"


namespace ceblankfs
{
namespace players
{

/**
 * @brief core player data storage & data member handler
 * 
 */
class PlayerData
    :   public IEntityComponent
{
public:
    struct DVPlayerData
    {
        static constexpr float massWeight = 60.f; // default value for mass weight
        static constexpr float movementSpeed = 60.f; // default value for movement speed
        static constexpr float jumpForce = 6.f; // default value for jump force
        static constexpr float jumpCharge = 0.f; // default value for jump charge
        static constexpr float sprintMultiplier = 2.f; // default value for sprint multiplier
    };

    float m_massWeight; // player mass weight value
    float m_movementSpeed; // player movement speed value
    float m_jumpForce; // player jump force value
    float m_jumpCharge; // player jump charge value
    float m_sprintMultiplier; // player sprint multiplier value


    // player jump charge policy value, required projectVersion
    const float pv_minJumpCharge 
        = []
        {
            GameConfig GC;
            float retval;

        #pragma region pv_minJumpCharge data patch adjustment
            if (GC.projectVersion == "0.0.0")
            {
                retval = .3f;
            }
            else
            {
                retval = .0f;

                #ifndef NDEBUG
                CryLog("### WARNING: PlayerData patch data variable doesn't recognized");
                #endif
            }
        #pragma endregion

            return retval;
        }();

public:
    PlayerData(/* args */);
    virtual ~PlayerData();


    virtual void Initialize() override
    {
        initializePlayerData();
    }


    // initialize PlayerData
    void initializePlayerData()
    {
        m_massWeight = DVPlayerData::massWeight;
        m_movementSpeed = DVPlayerData::movementSpeed;
        m_jumpForce = DVPlayerData::jumpForce;
        m_jumpCharge = DVPlayerData::jumpCharge;
        m_sprintMultiplier = DVPlayerData::sprintMultiplier;

        #ifndef NDEBUG
        CryLog("### PlayerMovement::initializePlayerMovement");
        #endif
    }


    static void ReflectType(Schematyc::CTypeDesc<PlayerData>& desc)
    {
        desc.SetGUID("{2630c2d0-3aa3-4ee0-8859-085135e980eb}"_cry_guid);
        desc.SetLabel("PlayerData");
        desc.SetEditorCategory("_players");
        desc.SetDescription("Core player data entity");
        desc.AddMember(
            &PlayerData::m_massWeight,
            'pdmw',
            "playerDataMassWeight",
            "mass weight",
            "mass weight value",
            DVPlayerData::massWeight
        );
        desc.AddMember(
            &PlayerData::m_movementSpeed,
            'pdms',
            "playerDataMovementSpeed",
            "movement speed",
            "movement speed value",
            DVPlayerData::movementSpeed
        );
        desc.AddMember(
            &PlayerData::m_jumpForce,
            'pdjf',
            "playerDataJumpForce",
            "jump force",
            "jump force value",
            DVPlayerData::jumpForce
        );
        desc.AddMember(
            &PlayerData::m_sprintMultiplier,
            'pdsm',
            "playerDataSprintMultiplier",
            "sprint multiplier",
            "sprint multiplier value",
            DVPlayerData::sprintMultiplier
        );
    }
};

} // namespace players
} // namespace ceblankfs