#ifndef BODY_H
#define BODY_H

struct Body
{
    double m;

    double x;
    double y;

    double vx;
    double vy;

    double ax;
    double ay;

    Body *tether;

    Body(double m, double x, double y, double vx, double vy);

    void calcAcceleration();
    void calcSpeed(double dt);
    void calcPos(double dt);

    void calc(double dt);
};

#endif // BODY_H
