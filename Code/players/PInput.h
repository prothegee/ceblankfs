#pragma once
#include "StdAfx.h"
#include "PCH.h"


/**
 * @brief core player input component
 * 
 */
class PInput
{
protected:
    enum class EInputFlagType
	{
		Hold = 0,
		Toggle
	};

	enum class EInputFlag : uint8
	{
		MoveLeft    = 1 << 0,
		MoveRight   = 1 << 1,
		MoveForward = 1 << 2,
		MoveBack    = 1 << 3,
		DoSprint    = 1 << 4,
		DoJump      = 1 << 5,
	};

    // default value data for PInput class
    struct DVPInput
    {
        static constexpr float sensitivity = 1.f;
    };

    const float m_minJumpCharge = 0.3f;
    const float m_maxJumpCharge = 3.0f;

    const float m_staminaMinLimit = 0.0f;
    const float m_staminaMaxLimit = 100.0f;
    const float m_staminaReductionRate = 30.0f;
    const float m_staminaRegenerationRate = 15.0f;

    const float m_rotationSpeed = 0.002f;

    float m_sensitivity;
    float m_rotationLimitsMinPitch = -0.85f; // dfr : -.84f;
    float m_rotationLimitsMaxPitch = 1.6f; // dfr: 1.5f;

	Vec2 m_mouseDeltaRotation;
    Quat m_lookOrientation;
    Vec2 m_entityDeltaRotation;
    Quat m_entityLookOrientation;

    CEnumFlags<EInputFlag> m_inputFlags, m_jumpInputFlags;
    
    static constexpr EEntityAspects InputAspect = eEA_GameClientD;
};
