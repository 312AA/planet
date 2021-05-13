#include "body.h"

#include "constants.h"

#include <cmath>
#include <stdio.h>

double M = 5.9722e24;

Body::Body(double m, double x, double y, double vx, double vy, bool rk, double tetherlength, double k, double c)
    : rk(rk), x(x), y(y), vx(vx), vy(vy), ax(0.0), ay(0.0), angle(0.0), tetherlength(tetherlength), k(k), c(c), tether(nullptr), vends(0)
{
    (void) m;
    this->m = body_m;
}

void Body::EcalcAcceleration()
{
    ax = atractionLaw(this, x, 0, 1);
    ay = atractionLaw(this, y, 0, 1);
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

    calcAngle(x, y, newx, newy);

    x = newx;
    y = newy;
}

void Body::calcAngle(double oldx, double oldy, double newx, double newy)
{
    double diffy = newx - oldx, diffx = newy - oldy;

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
    if(tether == nullptr)
    {
        if (rk) {
            rk4(dt,atractionLaw);
        } else {
            EcalcAcceleration();
            EcalcSpeed(dt);
            EcalcPos(dt);
        }
    }
    else
    {
        calcTH(dt);
    }

}

void Body::calcTH(double dt)
{
    ax =  -G*M*x/(std::pow(x*x + y*y, 3.0/2));
    ay =  -G*M*y/(std::pow(x*x + y*y, 3.0/2));

    tether->ax = -G*M*tether->x/(std::pow(tether->x*tether->x + tether->y*tether->y, 3.0/2));
    tether->ay = -G*M*tether->y/(std::pow(tether->x*tether->x + tether->y*tether->y, 3.0/2));

    double dist, S=0;

    dist = sqrt((x-tether->x)*(x-tether->x) + (y-tether->y)*(y-tether->y));
    vends = ((tether->vx - vx)*(tether->x - x) + (tether->vy - vy)*(tether->y - y)) / dist;
    if(dist > tetherlength)
    {
        S = k * (dist - tetherlength) + c * vends;
        S = S / (body_m * dist);
    }
    ax += S * (tether->x - x);
    ay += S * (tether->y - y);

    tether->ax -= S * (tether->x - x);
    tether->ay -= S * (tether->y - y);

    x += vx * dt;
    y += vy * dt;
    tether->x += tether->vx * dt;
    tether->y += tether->vy * dt;


    vx += ax * dt;
    vy += ay * dt;
    tether->vx += tether->ax * dt;
    tether->vy += tether->ay * dt;

}


double atractionLaw(Body *b, double coord, double coordspeed, char c)
{
    (void) coordspeed;
    double x = b->x, y = b->y;

    if(x*x + y*y >= R*R)
    {
        return  -G*M*coord/(std::pow(x*x + y*y, 3.0/2));
    }
    else
    {
        return  -G*M*coord/(R*R*R);
    }

}

double getacc(Body *b, double coord, double coordspeed, char c)
{
    double acc = 0;
    double len = b->tetherlength;
    double xt = b->tether->x, yt = b->tether->y;
    double vtx = b->tether->vx, vty = b->tether->vy;
    double x,y, vx,vy;
    if( c == 'x')
    {
        x = coord; y = b->y;
        vx = coordspeed; vy = b->vy;
    }
    else
    {
        y = coord; x = b->x;
        vy = coordspeed; vx = b->vx;
    }
    if(x*x + y*y >= R*R)
    {
        acc += -G*M*coord/(std::pow(x*x + y*y, 1.5));
    }
    else
    {
        acc += -G*M*coord/(R*R*R);
    }

    double S;

    double dist = sqrt((x-xt)*(x-xt) + (y-yt)*(y-yt));

    if( dist <= len )
        return acc;
    else
    {
            b->vends = ((vtx - vx)*(xt - x) + (vty - vy)*(yt - y)) / dist;
            S = b->k * (dist - len) + b->c * b->vends;
            printf("Vrel = %f\n Distanta = %f\n S = %f\n",b->vends,dist,S);
        if( c == 'x')
            acc += (S/body_m) * (xt - coord) / dist;
        else
            acc += (S/body_m) * (yt - coord) / dist;

        return acc;
    }
}

void Body::rk4(double dt, double (*f)(Body *, double, double, char))
{
    double k1,k2,k3,k4;
    double xprec = x + vx*dt/2;
    double yprec = y + vy*dt/2;
    double oldx = x, oldy = y;

    k1 = dt * f(this, x, vx, 'x');
    k2 = dt * f(this, xprec, vx+0.5*k1, 'x');
    k3 = dt * f(this, xprec + 0.25 * k1 * dt, vx+0.5*k2, 'x');
    k4 = dt * f(this, x + vx * dt + dt * k2/2, vx+k3, 'x');

    x += (vx + (k1 + k2 +k3)/6)*dt;
    vx += (k1 +2*k2 + 2*k3 +k4)/6;

    k1 = dt * f(this, y, vy, 'y');
    k2 = dt * f(this, yprec, vy+0.5*k1, 'y');
    k3 = dt * f(this, yprec + 0.25 * k1 * dt, vy+0.5*k2, 'y');
    k4 = dt * f(this, y + vy * dt + dt * k2/2, vy+k3, 'y');

    y += (vy + (k1 + k2 +k3)/6)*dt;
    vy += (k1 +2*k2 + 2*k3 +k4)/6;

    calcAngle(oldx, oldy, x, y);
}
