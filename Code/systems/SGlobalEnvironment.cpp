#include "SGlobalEnvironment.h"


static void Register_SGlobalEnvironment_Component(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(SGlobalEnvironment));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&Register_SGlobalEnvironment_Component);
SGlobalEnvironment::SGlobalEnvironment(/* args */)
{
}
SGlobalEnvironment::~SGlobalEnvironment()
{
}
