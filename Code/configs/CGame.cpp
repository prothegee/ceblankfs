#include "configs/CGame.h"

static void RegisterGConfigComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CGame));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterGConfigComponent);
