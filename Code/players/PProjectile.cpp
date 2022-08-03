#include "PProjectile.h"


static void RegisterPProjectileComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(PProjectile));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPProjectileComponent);
PProjectile::PProjectile(/* args */)
{
}
PProjectile::~PProjectile()
{
}
