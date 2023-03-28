#include "moonpos.h"
#include "../math/utils.h"
#include <cmath>

void MoonPos(double time, double& lambda, double& beta, double& distance)
{
    const double T = time;

    const double L0 = norm(DMS(218.31617) + DMS(481267.88088) * T - sec(4.06) * T * T, 360 * Deg());
    const double l = norm(DMS(134.96292) + DMS(477198.86753) * T + sec(33.25) * T *T, 360 * Deg());
    const double l1 = norm(DMS(357.32543) + DMS(35999.04944) * T - sec(0.58) * T * T, 360 * Deg());
    const double F = norm(DMS(93.27283) + DMS(483202.01873) * T - sec(11.53) * T * T, 360 * Deg());
    const double D = norm(DMS(297.85027) + DMS(445267.11135) * T - sec(5.15) * T * T, 360 * Deg());
    

    const double dLSec = 22640 * sin(l) - 4586 * sin(l - 2 * D) + 2370 * sin(2 * D)
        + 769 * sin(2 * l) - 668 * sin(l1) - 412 * sin(2 * F)
        - 212 * sin(2 * l - 2 * D) - 206 * sin(l + l1 - 2 * D) + 192 * sin(l + 2 * D)
        - 165 * sin(l1 - 2 * D) + 148 * sin(l - l1) - 125 * sin(D)
        - 110 * sin(l + l1) - 55 * sin(2 * F - 2 * D);


    lambda = norm(L0 + sec(dLSec), 360 * Deg());

    const double bSec = 18461 * sin(F) + 1010 * sin(l + F) + 1000 * sin(l - F)
        - 624 * sin(F - 2 * D) - 199 * sin(l - F - 2 * D) - 167 * sin(l + F - 2 * D);

    beta = sec(bSec);

    distance = 1 / (1 + 0.0545 * cos(l) + 0.0100 * cos(l - 2 * D)
        + 0.0082 * cos(2 * D) + 0.0030 * cos(2 * l)
        + 0.0009 * cos(l + 2 * D) + 0.0006 * cos(l1 - 2 * D)
        + 0.0004 * cos(l + l1 - 2 * D) + 0.0003 * cos(l - l1));
}
