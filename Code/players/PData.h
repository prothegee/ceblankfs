#pragma once
#include "GameConfig.h"


namespace ceblankfs
{
namespace players
{

/**
 * @brief player data storage core class, e.g. health, power, armor & etc.
 * 
 */
class PData
    :   public IEntityComponent
{
private:
    bool m_isAlive = false;




public:
    // PData default value
    struct DVPData
    {
        static constexpr bool spawnOnCamera = false;
        static constexpr bool ableToDoubleJump = false;

        static constexpr float hitPoint = 100.f;
        static constexpr float manaPoint = 100.f;
        static constexpr float energyPoint = 100.f;
        static constexpr float weight = 60.f;
        static constexpr float movementSpeed = 60.f;
        static constexpr float jumpForce = 6.f;
        static constexpr float jumpCharge = 0.f;
        static constexpr float sprintMultiplier = 2.f;
        static constexpr float jumpChargeMultiplier = 2.f;
        static constexpr float jumpDurationOnHold = 0.f;
    };

    Schematyc::CSharedString m_username;
    Schematyc::CSharedString m_charactername;

    // spawn on camera after game started. Usage on editor & under development only
    bool m_spawnOnCamera;
    // player is able to do double jump
    bool m_ableToDoubleJump;

    // health
    float m_hitPoint;
    // mana
    float m_manaPoint;
    // energy/stamina
    float m_energyPoint;
    // mass/weight
    float m_weight;
    // movement speed
    float m_movementSpeed;
    // jump force/power
    float m_jumpForce;
    // jump charge force/power
    float m_jumpCharge;
    // sprint multiplier
    float m_sprintMultiplier;
    // jump charge multiplier
    float m_jumpChargeMultiplier;
    // charge jump on hold value
    float m_jumpDurationOnHold;

    // minimum time to allow charge jump
    const float m_minJumpCharge
        = []
        {
            float retval;

        #pragma region m_minJumpCharge patch
            switch (GameConfig::MAJOR, GameConfig::MINOR, GameConfig::PATCH)
            {
                case (0, 0, 0):
                {
                    retval = .3f;
                }
                break;


                default:
                {
                    retval = .3f;
                }
                break;
            }
        #pragma endregion

            return retval;
        }();

    // player data pointer from PData
    ceblankfs::players::PData* m_pData = nullptr;

public:
    PData(/* args */);
    virtual ~PData();


    virtual void Initialize() override;


    virtual Cry::Entity::EventFlags GetEventMask() const override;
    virtual void ProcessEvent(const SEntityEvent& e) override;


    static void ReflectType(Schematyc::CTypeDesc<PData>& desc)
    {
        desc.SetGUID("{c88a9699-3f33-4304-8365-3a2f1074f5f4}"_cry_guid);
        desc.SetLabel("PData");
        desc.SetEditorCategory("_players");
        desc.SetDescription("Players base data");
        desc.AddMember(
            &PData::m_username,
            'pdun',
            "PlayerDataUsername",
            "username",
            "username value",
            ""
        );
        desc.AddMember(
            &PData::m_charactername,
            'pdcn',
            "PlayerDataCharacterName",
            "character name",
            "character name value",
            ""
        );
        desc.AddMember(
            &PData::m_spawnOnCamera,
            'psoc',
            "PlayerDataSpawnOnCamera",
            "spawn on camera",
            "spawn on camera value",
            DVPData::spawnOnCamera
        );
        desc.AddMember(
            &PData::m_ableToDoubleJump,
            'padj',
            "PlayerDataAbleToDoubleJump",
            "double jump",
            "double jump value",
            DVPData::ableToDoubleJump
        );
        desc.AddMember(
            &PData::m_hitPoint,
            'pdhp',
            "PlayerDataHitPoint",
            "health",
            "health value",
            DVPData::hitPoint
        );
        desc.AddMember(
            &PData::m_manaPoint,
            'pdmp',
            "PlayerDataManaPoint",
            "mana",
            "mana value",
            DVPData::manaPoint
        );
        desc.AddMember(
            &PData::m_energyPoint,
            'pdep',
            "PlayerDataEnergyPoint",
            "energy",
            "energy value",
            DVPData::energyPoint
        );
        desc.AddMember(
            &PData::m_weight,
            'pdw',
            "PlayerDataWeight",
            "weight",
            "weight value",
            DVPData::weight
        );
        desc.AddMember(
            &PData::m_movementSpeed,
            'pdms',
            "PlayerDataMovementSpeed",
            "movement speed",
            "movement speed value",
            DVPData::movementSpeed
        );
        desc.AddMember(
            &PData::m_jumpForce,
            'pdjf',
            "PlayerDataJumpForce",
            "jump force",
            "jump force value",
            DVPData::jumpForce
        );
        desc.AddMember(
            &PData::m_jumpCharge,
            'pdjf',
            "PlayerDataJumpForce",
            "jump force",
            "jump force value",
            DVPData::jumpCharge
        );
        desc.AddMember(
            &PData::m_jumpChargeMultiplier,
            'pdjm',
            "PlayerDataJumpmultiplier",
            "jump multiplier",
            "jump multiplier value",
            DVPData::jumpChargeMultiplier
        );
    }




protected:
    /* data */

protected:
    /**
     * @brief rules for PData where some variable not allow to less than 0
     * 
     * @note insecure mode
     * 
     */
    void PDataValueRules();


    /**
     * @brief PDataPointer register
     * 
     */
    void RegisterPDataPointer();


    /**
     * @brief PDataP default value register
     * 
     */
    void RegisterPDataDefaultValue();
};
 

} // namespace players
} // namespace ceblankfs