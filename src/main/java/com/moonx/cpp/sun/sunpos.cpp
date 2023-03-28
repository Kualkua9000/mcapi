#include "sunpos.h"
#include "../math/utils.h"
#include <cmath>

void SunPos(double time, double& longitude, double& distance)
{
    const double M = norm(0.993133 + 99.997361 * time, 1) * 360 * Deg();

    longitude = norm(M + 0.7859453 * 360 * Deg() + sec(6893 * sin(M) + 72 * sin(2 * M) + 6191.2 * time), 360 * Deg());

    distance = 1.00014 - 0.01673*cos(M) - 0.00014*cos(2 * M);
}
