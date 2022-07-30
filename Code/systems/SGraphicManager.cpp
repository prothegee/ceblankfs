#include "SGraphicManager.h"


static void Register_SGraphicManager_Component(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(SGraphicManager));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&Register_SGraphicManager_Component);
SGraphicManager::SGraphicManager(/* args */)
{
}
SGraphicManager::~SGraphicManager()
{
}