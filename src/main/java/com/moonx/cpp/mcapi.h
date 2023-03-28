#pragma once
#ifndef _MCAPI_H_
#define _MCAPI_H_

extern "C" {

    int mcGetApiBufferSize();

    int mcInit(void* mcApi);

    int mcDestroy(void*);

    int mcSetLocation(void*, double longitudeDeg, double latitudeDeg);

    int mcSetTime(void*, double timeInSecondsSinceJ2000, double localTimeZone);

    int mcGetGlobalStellarTime(void*, double& globalStellarTime);

    int mcGetLocalStellarTime(void*, double& localStellarTime);

    int mcGetEclipticPosition(void*, double& lon, double& lat, double& distance);

    int mcGetEquatorialPosition(void*, double& ra, double& dec, double& distance);

    int mcGetElongation(void*, double& elongation);

    int mcGetElongationPrecise(void*, double& elongation);

    int mcGetPhaseInformation(void*, double& phase, double& age, double& luminance);

    int mcGetPhaseInformationPrecise(void*, double& phase, double& age, double& luminance);

    int mcGetRelativePlanetshine(void*, double& planetshine);

    int mcGetMoonHorizontalPosition(void*, double& altitude, double& azimuth, double& posAngleDeg);

    // NaN - Moon does not rise or set this day
    int mcMoonGetRiseSet(void*, double& rise, double& set);

    int mcGetSunHorizontalPosition(void*, double& altitude, double& azimuthFromNorth, double& distance);

    // NaN - Moon does not rise or set this day
    int mcGetSunRiseSet(void*, double& rise, double& set);

    int mcEnumeratePhases(void* mcApi, double dateStart, double dateEnd);

    int mcNextPhase(void* mcApi, double& when, int& phaseKind);

    int mcGetNewMoon(void* mcApi, double& when);

    int mcGetRiseCount(void* mcApi, int& count);
}

#endif
