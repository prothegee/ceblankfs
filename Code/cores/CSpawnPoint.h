#pragma once
#include "StdAfx.h"


/**
 * @brief player spawn point on game launcher
 * 
 */
class CSpawnPoint
    :   public IEntityComponent
{
public:
    CSpawnPoint(/* args */);
    virtual ~CSpawnPoint();
};


CSpawnPoint::CSpawnPoint(/* args */)
{
}
CSpawnPoint::~CSpawnPoint()
{
}

