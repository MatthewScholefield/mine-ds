#include <stdio.h>
#include "../debugflag.h"
#include "baseMob.h"

baseMob::baseMob()
{
    x=0;
    y=0;
    vy=0;
    vx=0;
}
baseMob::baseMob(int a,int b)
{
    x=a;
    y=b;
    vy=0;
    vx=0;
}
void baseMob::resetVelocity()
{
    vy=0;
    vx=0;
}
void baseMob::updateMob()
{
}
void baseMob::setXYPos(int a,int b)
{
    x=a;
    y=b;
}
void baseMob::sendWifiUpdate()
{
}
void baseMob::saveToFile(FILE* pFile)
{
}
void baseMob::loadFromFile(FILE* pFile)
{
}
