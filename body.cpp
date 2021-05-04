#include "body.h"

#include "constants.h"

#include <cmath>

Body::Body(double m, double x, double y, double vx, double vy, bool rk)
    : rk(rk), m(m), x(x), y(y), vx(vx), vy(vy), ax(0.0), ay(0.0), angle(0.0), tether(nullptr)
{

}

void Body::EcalcAcceleration()
{
    ax = atractionLaw(x);
    ay = atractionLaw(y);
}

void Body::EcalcSpeed(double dt)
{
    vx += ax * dt;
    vy += ay * dt;
}

void Body::EcalcPos(double dt)
{
    double newx = x + vx * dt;
    double newy = y + vy * dt;

    calcAngle(newx, newy);

    x = newx;
    y = newy;
}

void Body::calcAngle(double newx, double newy)
{
    double diffy = newx - x, diffx = newy - y;

    angle = std::atan(std::fabs(diffy) / std::fabs(diffx));

    if (diffx < 0 && diffy < 0)
        angle = PI + angle;

    if (diffx < 0 && diffy > 0)
        angle = PI - angle;

    if (diffx > 0 && diffy < 0)
        angle = 2 * PI - angle;
}

void Body::calc(double dt)
{
    if (rk) {
        rk4(dt);
    } else {
        EcalcAcceleration();
        EcalcSpeed(dt);
        EcalcPos(dt);
    }
}

double Body::atractionLaw(double coord)
{
    if(x*x + y*y >= R*R)
    {
        return  -G*M*coord/(std::pow(x*x + y*y, 3.0/2));
    }
    else
    {
        return  -G*M*m*coord/(R*R*R);
    }

}

void Body::rk4(double dt)
{
    double k1,k2,k3,k4;
    double xprec = x + vx*dt/2;
    double yprec = y + vy*dt/2;

    k1 = dt * atractionLaw(x);
    k2 = dt * atractionLaw(xprec);
    k3 = dt * atractionLaw(xprec + 0.25 * k1 * dt);
    k4 = dt * atractionLaw(x + vx * dt + dt * k2/2);

    x += (vx + (k1 + k2 +k3)/6)*dt;
    vx += (k1 +2*k2 + 2*k3 +k4)/6;

    k1 = dt * atractionLaw(y);
    k2 = dt * atractionLaw(yprec);
    k3 = dt * atractionLaw(yprec + 0.25 * k1 * dt);
    k4 = dt * atractionLaw(y + vy * dt + dt * k2/2);

    y += (vy + (k1 + k2 +k3)/6)*dt;
    vy += (k1 +2*k2 + 2*k3 +k4)/6;
}
