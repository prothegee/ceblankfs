#pragma once
#include "StdAfx.h"


/**
 * @brief player data core class
 * 
 */
class PData
{
protected:
    bool m_isAlive = false;
    bool m_canJumpNow = false;
    bool m_aimStance = false;

    // default value data for PData class
    struct DVPData
    {
        static constexpr float health = 100.f;
        static constexpr float stamina = 100.0f;
        static constexpr float movementSpeed = 50.f;
        static constexpr float jumpForce = 6.f;
        static constexpr float jumpCharge = 0.f;
        static constexpr float jumpChargeMultiplier = 1.f;
        static constexpr float jumpDurationOnHold = 0.f;
        static constexpr float weight = 60.f;
        static constexpr float sprintMultiplier = 2.f;
    };

    #pragma region player data variables
    // username
    Schematyc::CSharedString m_username;
    // character name
    Schematyc::CSharedString m_character;

    // health
    float m_health;
    // stamina
    float m_stamina;
    // movement speed
    float m_movementSpeed;
    // jump force
    float m_jumpForce;
    // jump charge
    float m_jumpCharge;
    // jump charge multiplier
    float m_jumpChargeMultiplier;
    // jump on hold duration
    float m_jumpDurationOnHold;
    // weight
    float m_weight;
    // sprint multiplier
    float m_sprintMultiplier;
    #pragma endregion

    // default camera position
    Vec3 m_defaultCameraPosition = Vec3(0, 0, 1.75f);


public:
    // pub get current player character value
    string PubGetCharacter() const
    {
        return m_character.c_str();
    }
};
