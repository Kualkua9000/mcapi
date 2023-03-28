#pragma once
#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

double GlobalStellarTime(double time);

double LocalStellarTime(double time, double longitudeRad);

double TimeEquationRad(double sunLongitudeRad);

double EclipticInclination(double time);

void q2h(double t, double delta, double phi, double lambda, double& azimuth, double& altitude);

void h2q(double azimuth, double altitude, double phi, double lambda, double& t, double& delta);

void e2q(double lambda, double beta, double epsilon, double& ra, double& dec);

void q2e(double ra, double dec, double epsilon, double& lambda, double& beta);



#endif
