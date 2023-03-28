#include "transform.h"
#include "utils.h"
#include <cmath>

static double GetUTC(double time)
{
    const double days = time * 36525;
    const double partOfDay = days - floor(days);
    return partOfDay * 86400;
}

double GlobalStellarTime(double time)
{
    const double t = time * 36525;
    const double T = time;
    const double T0 = floor(t) / 36525;
    const double utc = GetUTC(time);
    const double A = 86400.0 / 2 + 24110.54841 + 8640184.812866*T0 + 1.0027379093 * utc + 0.093104 * T * T - 6.2e-6 * T * T * T;
    return norm(A, 86400);
}

double LocalStellarTime(double time, double longitudeRad)
{
    return GlobalStellarTime(time) + longitudeRad / (360 * Deg()) * 86400;
}

double TimeEquationRad(double sunLongitudeRad)
{
    return HMS(0, 7.7) * sin(sunLongitudeRad + DMS(78)) - HMS(0, 9.5) *sin(2 * sunLongitudeRad);
}

double EclipticInclination(double time)
{
    const double T = time;
    return DMS(23.43929111) + sec(-46.8150 * T - 0.00059 * T * T - 0.001813 * T * T * T);
}

void q2h(double t, double delta, double phi, double lambda, double& azimuth, double& altitude)
{
    const double cosZ = sin(phi) * sin(delta) + cos(phi) * cos(delta) * cos(t);
    const double sinZsinA = cos(delta) * sin(t);
    const double sinZcosA = -cos(phi) * sin(delta) + sin(phi) * cos(delta) * cos(t);

    double tmp = cosZ * cosZ + sinZsinA * sinZsinA + sinZcosA * sinZcosA;
    (void)tmp;

    azimuth = atan2(sinZsinA, sinZcosA);
    altitude = asin(cosZ);
    azimuth -= 180 * Deg();
    if (azimuth < 0) azimuth += 360 * Deg();
}

void h2q(double azimuth, double altitude, double phi, double lambda, double& t, double& delta)
{
    const double sinD = sin(phi) * sin(altitude) - cos(phi)*cos(altitude) * cos(azimuth);
    const double cosDsinT = cos(altitude) * sin(azimuth);
    const double cosDcosT = sin(altitude) * cos(phi) + cos(altitude) * sin(phi) * cos(azimuth);
    delta = asin(sinD);
    t = atan2(cosDsinT, cosDcosT);
    if (t < 0) t += 360 * Deg();
}

void e2q(double lambda, double beta, double epsilon, double& ra, double& dec)
{
    const double sinDelta = cos(epsilon) * sin(beta) + sin(epsilon) * cos(beta) * sin(lambda);
    const double cosDeltaCosAlpha = cos(beta) * cos(lambda);
    const double cosDeltaSinAlpha = -sin(beta) * sin(epsilon) + cos(beta) * cos(epsilon) * sin(lambda);

    double tmp = sinDelta * sinDelta + cosDeltaCosAlpha * cosDeltaCosAlpha + cosDeltaSinAlpha * cosDeltaSinAlpha;
    (void)tmp;

    dec = asin(sinDelta);
    ra = atan2(cosDeltaSinAlpha, cosDeltaCosAlpha);
    if (ra < 0) ra += 360 * Deg();
}

void q2e(double ra, double dec, double epsilon, double& lambda, double& beta)
{
    const double sinBeta = cos(epsilon) * sin(dec) - sin(epsilon) * cos(dec) * sin(ra);
    const double cosBetaCosLambda = cos(dec) * cos(ra);
    const double cosBetaSinLambda = sin(dec) * sin(epsilon) + cos(dec) * cos(epsilon) * sin(ra);

    beta = asin(sinBeta);
    lambda = atan2(cosBetaSinLambda, cosBetaCosLambda);
    if (lambda < 0) lambda += 360 * Deg();
}

