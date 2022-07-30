#include "PSpawnPoint.h"


static void Register_PSpawnPoint_Component(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(PSpawnPoint));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&Register_PSpawnPoint_Component);
PSpawnPoint::PSpawnPoint(/* args */)
{
}
PSpawnPoint::~PSpawnPoint()
{
}
