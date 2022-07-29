// ONLY for IEnvRegistrar
#include "PCore.h"
#include "PData.h"


#pragma region register PCore
static void RegisterPCoreComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(PCore));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPCoreComponent);
PCore::PCore(/* args */)
{
}
PCore::~PCore()
{
}
#pragma endregion