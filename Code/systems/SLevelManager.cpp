#include "SLevelManager.h"


static void Register_SLevelManager_Component(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(SLevelManager));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&Register_SLevelManager_Component);
SLevelManager::SLevelManager(/* args */)
{
}
SLevelManager::~SLevelManager()
{
}
