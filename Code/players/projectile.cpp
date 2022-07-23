#include "projectile.h"


static void RegisterProjectileComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(
            SCHEMATYC_MAKE_ENV_COMPONENT(ceblankfs::players::projectile));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterProjectileComponent);
ceblankfs::players::projectile::projectile(/* args */)
{
}
ceblankfs::players::projectile::projectile::~projectile()
{
}