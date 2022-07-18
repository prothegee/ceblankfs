#include "PlayerCamera.h"
#include "PlayerData.h"
#include "PlayerMovement.h"


using namespace ceblankfs::players;


#pragma region players constructor desturctor


PlayerCamera::PlayerCamera(/* args */)
{
}
PlayerCamera::~PlayerCamera()
{
}


PlayerData::PlayerData(/* args */)
{
}
PlayerData::~PlayerData()
{
}


PlayerMovement::PlayerMovement(/* args */)
{
}
PlayerMovement::~PlayerMovement()
{
}
#pragma endregion




#pragma region player components registrar
// PlayerData registrar
static void RegisterPlayerCameraEntity(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(
            SCHEMATYC_MAKE_ENV_COMPONENT(PlayerCamera)); // EnvComponent.h
    } // IEnvRegistrar.h
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayerCameraEntity); // StaticInstanceList.h


// PlayerData registrar
static void RegisterPlayerDataEntity(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(
            SCHEMATYC_MAKE_ENV_COMPONENT(PlayerData)); // EnvComponent.h
    } // IEnvRegistrar.h
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayerDataEntity); // StaticInstanceList.h


// PlayerMovement registrar
static void RegisterPlayerMovementEntity(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(
            SCHEMATYC_MAKE_ENV_COMPONENT(PlayerMovement)); // EnvComponent.h
    } // IEnvRegistrar.h
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayerMovementEntity); // StaticInstanceList.h
#pragma endregion