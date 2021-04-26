#include "body.h"

#include "constants.h"

#include <cmath>

Body::Body(double m, double x, double y, double vx, double vy)
    : m(m), x(x), y(y), vx(vx), vy(vy), ax(0.0), ay(0.0), tether(nullptr)
{

}

void Body::calcAcceleration()
{
    if(x*x + y*y >= R*R)
    {
        ax = -G*M*m*x/(std::pow(x*x + y*y, 3.0/2));
        ay = -G*M*m*y/(std::pow(x*x + y*y, 3.0/2));
    }
    else
    {
        ax = -G*M*m*x/(R*R*R);
        ay = -G*M*m*y/(R*R*R);
    }
}

void Body::calcSpeed(double dt)
{
    vx += ax * dt;
    vy += ay * dt;
}

void Body::calcPos(double dt)
{
    x += vx * dt;
    y += vy * dt;
}

void Body::calc(double dt)
{
    calcAcceleration();
    calcSpeed(dt);
    calcPos(dt);
}
