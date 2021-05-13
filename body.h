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
    double vends;

    double tetherlength;
    double k;
    Body *tether;
    double tether_angle;
    double c;

    Body(double m, double x, double y, double vx, double vy, bool rk, double tetherlength = 0, double k = 0, double c = 0);


    void EcalcAcceleration();
    void EcalcSpeed(double dt);
    void EcalcPos(double dt);

    void calcTH(double dt);

    void calcAngle(double oldx, double oldy, double newx, double newy);

    void calc(double dt);

    void rk4(double dt, double (*f)(Body *, double, double, char));

    double getangle(char c);

    void calcvends(double dt);
};

double atractionLaw(Body *b, double coord, double coordspeed, char c);
double getacc(Body *b, double coord, double coordspeed, char c);


#endif // BODY_H
