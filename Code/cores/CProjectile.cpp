#include "CProjectile.h"


static void RegisterCProjectileComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CProjectile));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterCProjectileComponent);
CProjectile::CProjectile(/* args */)
{
}
CProjectile::~CProjectile()
{
}
