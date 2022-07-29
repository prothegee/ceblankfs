#pragma once
#include "StdAfx.h"


/**
 * @brief player spawn point
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

