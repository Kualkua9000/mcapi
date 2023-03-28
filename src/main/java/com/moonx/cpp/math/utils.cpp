#include "utils.h"
#include <cmath>

double Deg()
{
    static const double _rad = std::atan(1) / 45;
    return _rad;
}

double norm(double value, double range)
{
    return value - floor(value / range) * range;
}

double DMS(double deg, double min, double sec)
{
    const double sign = deg < 0 ? -1 : 1;
    deg += sign * min / 60;
    deg += sign * sec / 3600;
    return Deg() * deg;
}

double HMS(double hours, double min, double sec)
{
    return DMS(hours * 15, min, sec);
}

double sec(double s)
{
    return s / 3600 * Deg();
}

