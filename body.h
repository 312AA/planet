#ifndef BODY_H
#define BODY_H

struct Body
{
    bool rk;

    double m;

    double x;
    double y;

    double vx;
    double vy;

    double ax;
    double ay;

    double angle;

    Body *tether;

    Body(double m, double x, double y, double vx, double vy, bool rk);

    void EcalcAcceleration();
    void EcalcSpeed(double dt);
    void EcalcPos(double dt);

    void calcAngle(double x, double y);

    void calc(double dt);

    double atractionLaw(double coord);


    void rk4(double dt);
};

#endif // BODY_H
