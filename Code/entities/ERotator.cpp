#include "ERotator.h"


static void Register_ERotator_Component(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(ERotator));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&Register_ERotator_Component);
ERotator::ERotator(/* args */)
{
}
ERotator::~ERotator()
{
}
