// ONLY for player handler logicc
#include "PCore.h"


#pragma region player handler
void PCore::ValuePolicy(float dt)
{
    // health
    (m_health < 0.f)
        ? m_health = 0.f
        : m_health = this->m_health;
    
    // stamina
    (m_stamina < m_staminaMinLimit)
        ? m_stamina = m_staminaMinLimit
        : m_stamina = this->m_stamina;
    (m_stamina > m_staminaMaxLimit)
        ? m_stamina = m_staminaMaxLimit
        : m_stamina = this->m_stamina;
    
    // movement speed
    (m_movementSpeed < 0.f)
        ? m_movementSpeed = 0.f
        : m_movementSpeed = this->m_movementSpeed;
    
    // jumpforce
    (m_jumpForce < 0.f)
        ? m_jumpForce = 0.f
        : m_jumpForce = this->m_jumpForce;
    
    // jumpcharge
    (m_jumpCharge < 0.f)
        ? m_jumpCharge = 0.f
        : m_jumpCharge = this->m_jumpCharge;
    
    // jumpcharge multiplier
    (m_jumpChargeMultiplier < 0.f)
        ? m_jumpChargeMultiplier = 0.f
        : m_jumpChargeMultiplier = this->m_jumpChargeMultiplier;
    
    // jump on hold
    (m_jumpDurationOnHold < 0.f)
        ? m_jumpDurationOnHold = 0.f
        : m_jumpDurationOnHold = this->m_jumpDurationOnHold;

    // weight
    (m_weight < 0.f)
        ? m_weight = 0.f
        : m_weight = this->m_weight;
    
    // sprint multiplier
    (m_sprintMultiplier < 0.f)
        ? m_sprintMultiplier = 0.f
        : m_sprintMultiplier = this->m_sprintMultiplier;

    // sensitivity
    (m_sensitivity < 0.f)
        ? m_sensitivity = 0.f
        : m_sensitivity = this->m_sensitivity;

    // aim stance
    (m_aimStance)
        ? m_movementSpeed = DVPData::movementSpeed/2
        : m_movementSpeed = DVPData::movementSpeed;

    // stamina
    m_stamina += m_staminaRegenerationRate * dt;

    /**
     * disable some interaction on some condition
     * e.g. on main-menu and etc.
     * 
     * list:
     * - movement speed
     * - input interaction
     * - camera view
     * - animation fragment
     * 
     * return void
     * 
     */
    if (m_disableInteraction)
    {

    }
    else
    {

    }
}


void PCore::HandleInputFlagChange(const CEnumFlags<EInputFlag> flags, const CEnumFlags<EActionActivationMode> activationMode, const EInputFlagType type)
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
            if (activationMode == eAAM_OnRelease)
            {
                // Toggle the bit(s)
                m_inputFlags ^= flags;
            }
        }
        break;
	}
	
	if(IsLocalClient())
	{
		NetMarkAspectsDirty(InputAspect);
	}
}


void PCore::JumpHandleInputFlagChange(CEnumFlags<EInputFlag> flags, CEnumFlags<EActionActivationMode> activationMode, EInputFlagType type)
{
    switch (type)
    {
        case EInputFlagType::Hold:
        {
            if (activationMode == eAAM_OnRelease)
            {
                m_jumpInputFlags &= ~flags;
            }
            else
            {
                m_jumpInputFlags |= flags;
            }
        }
        break;

        case EInputFlagType::Toggle:
        {
            m_jumpInputFlags ^= flags;
        }
        break;
    }

    if(IsLocalClient())
	{
		NetMarkAspectsDirty(InputAspect);
	}
}


void PCore::GroundMovementHandler(float dt)
{
    if (!m_pCC->IsOnGround()) return;

    Vec3 velocity = ZERO;

    // Check input to calculate local space velocity
    if (m_inputFlags & EInputFlag::MoveLeft)
    {
        velocity.x -= m_movementSpeed * dt;
    }
    if (m_inputFlags & EInputFlag::MoveRight)
    {
        velocity.x += m_movementSpeed * dt;
    }
    if (m_inputFlags & EInputFlag::MoveForward)
    {
        velocity.y += m_movementSpeed * dt;
    }
    if (m_inputFlags & EInputFlag::MoveBack)
    {
        velocity.y -= m_movementSpeed * dt;
    }


    float tmpjumpcharge = 0.0f; // temporal jump charge

    if (m_jumpInputFlags & EInputFlag::DoJump)
    {
        m_jumpDurationOnHold += 1.f * dt;

        (m_jumpDurationOnHold >= m_minJumpCharge)
            ?   
                tmpjumpcharge = m_jumpDurationOnHold,
                m_canJumpNow = true,
                m_stamina -= m_staminaReductionRate * dt
            :   
                tmpjumpcharge = 1.f,
                m_canJumpNow = true,
                m_stamina -= m_staminaReductionRate * dt;

        (tmpjumpcharge >= m_maxJumpCharge)
            ? m_jumpCharge = m_maxJumpCharge
            : m_jumpCharge = tmpjumpcharge;
    }
    else
    {
        m_jumpDurationOnHold = 0.f;
    }

    if (m_canJumpNow && m_jumpDurationOnHold == 0.0f && m_stamina >= m_staminaMinLimit)
    {
        if (m_pCC->IsOnGround())
        {
            m_pCC->AddVelocity(Vec3(0, 0, ((m_jumpForce * m_jumpCharge) * m_jumpChargeMultiplier)));
        }

        m_canJumpNow = false;
        m_jumpDurationOnHold = 0.f;
        m_jumpCharge = DVPData::jumpCharge;
        m_jumpChargeMultiplier = DVPData::jumpChargeMultiplier;
        tmpjumpcharge = 0.0f;
    }


    if (m_inputFlags & EInputFlag::DoSprint)
    {
        m_aimStance = false; // break the aim stance
        

        if (m_stamina > m_staminaMinLimit)
            m_pCC->AddVelocity(GetEntity()->GetWorldRotation() * (velocity * m_sprintMultiplier));

            // reduce stamina if moving
            if (m_pCC->GetVelocity() != Vec3(0, 0, 0))
            {
                m_stamina -= m_staminaReductionRate * dt;
            }
    }
    else
    {
        m_pCC->AddVelocity(GetEntity()->GetWorldRotation() * velocity);
    }
}


void PCore::OrientHandler(float dt)
{
    #pragma region root or parent section
    // core/root entity world tranformation
    Matrix34 etf = m_pEntity->GetWorldTM();
    Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_entityLookOrientation));

    ypr.x += m_entityDeltaRotation.x * (m_rotationSpeed * m_sensitivity);
    ypr.y = 0;
    ypr.z = 0;

    m_entityLookOrientation = Quat(CCamera::CreateOrientationYPR(ypr));
    m_entityDeltaRotation = ZERO;

    etf.SetRotation33(CCamera::CreateOrientationYPR(ypr));

    m_pEntity->SetWorldTM(etf);
    #pragma endregion


    #pragma region camera section
    // camera entity world tranformation
    Matrix34 ctf = m_pCamera->GetTransformMatrix();
    Ang3 rypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));

    rypr.x = 0; // x axis already handle #pragma region root or parent section
    rypr.y = CLAMP(rypr.y + m_mouseDeltaRotation.y * (m_rotationSpeed * m_sensitivity), m_rotationLimitsMinPitch, m_rotationLimitsMaxPitch);
    rypr.z = 0;

    m_lookOrientation = Quat(CCamera::CreateOrientationYPR(rypr));
    m_mouseDeltaRotation = ZERO;

    ctf.SetRotation33(CCamera::CreateOrientationYPR(rypr));
    
    ctf.SetTranslation(m_defaultCameraPosition); // camera adjustment

    m_pCamera->SetTransformMatrix(ctf);
    m_pAudio->SetOffset(ctf.GetTranslation());
    #pragma endregion
}
#pragma endregion
