#include "mcapi.h"
#include "mcobject.h"
#include <new>

#include "status.h"

#define CHECK(obj) if (!MCObject::Check(obj)) return int(mcStatus::InvalidObject);

int mcGetApiBufferSize()
{
    return sizeof(MCObject);
}

int mcInit(void* mcApi)
{
    (void)new(mcApi)MCObject();
    return int(mcStatus::Success);
}

int mcDestroy(void* mcApi)
{
    CHECK(mcApi);
    reinterpret_cast<MCObject*>(mcApi)->~MCObject();
    return int(mcStatus::Success);
}

int mcSetLocation(void* mcApi, double longitudeDeg, double latitudeDeg)
{
    CHECK(mcApi);
    return int(reinterpret_cast<MCObject*>(mcApi)->SetLocation(longitudeDeg, latitudeDeg));
}

int mcSetTime(void* mcApi, double timeInSecondsSinceJ2000, double localTimeZone)
{
    CHECK(mcApi);
    return int(reinterpret_cast<MCObject*>(mcApi)->SetTime(timeInSecondsSinceJ2000, localTimeZone));
}

int mcGetEclipticPosition(void* mcApi, double& lon, double& lat, double& distance)
{
    CHECK(mcApi);
    return int(reinterpret_cast<MCObject*>(mcApi)->GetEclipticPosition(lon, lat, distance));
}

int mcGetEquatorialPosition(void* mcApi, double& ra, double& dec, double& distance)
{
    CHECK(mcApi);
    return int(reinterpret_cast<MCObject*>(mcApi)->GetEquatorialPosition(ra, dec, distance));
}

int mcGetElongation(void* mcApi, double& elongation)
{
    CHECK(mcApi);
    return int(reinterpret_cast<MCObject*>(mcApi)->GetElongation(elongation));
}

int mcGetElongationPrecise(void* mcApi, double& elongation)
{
    CHECK(mcApi);
    return int(reinterpret_cast<MCObject*>(mcApi)->GetElongationPrecise(elongation));
}

int mcGetPhaseInformation(void* mcApi, double& phase, double& age, double& luminance)
{
    CHECK(mcApi);
    return int(reinterpret_cast<MCObject*>(mcApi)->GetPhaseInformation(phase, age, luminance));
}

int mcGetPhaseInformationPrecise(void* mcApi, double& phase, double& age, double& luminance)
{
    CHECK(mcApi);
    return int(reinterpret_cast<MCObject*>(mcApi)->GetPhaseInformationPrecise(phase, age, luminance));
}

int mcGetRelativePlanetshine(void* mcApi, double& planetshine)
{
    CHECK(mcApi);
    return int(reinterpret_cast<MCObject*>(mcApi)->GetRelativePlanetshine(planetshine));
}

int mcGetMoonHorizontalPosition(void* mcApi, double& altitude, double& azimuth, double& posAngle)
{
    CHECK(mcApi);
    return int(reinterpret_cast<MCObject*>(mcApi)->GetMoonHorizontalPosition(altitude, azimuth, posAngle));
}

int mcMoonGetRiseSet(void* mcApi, double& rise, double& set)
{
    CHECK(mcApi);
    return int(reinterpret_cast<MCObject*>(mcApi)->GetMoonRiseSet(rise, set));
}

int mcGetSunHorizontalPosition(void* mcApi, double& altitude, double& azimuthFromNorth, double& distance)
{
    CHECK(mcApi);
    return int(reinterpret_cast<MCObject*>(mcApi)->GetSunHorizontalPosition(altitude, azimuthFromNorth, distance));
}

int mcGetSunRiseSet(void* mcApi, double& rise, double& set)
{
    CHECK(mcApi);
    return int(reinterpret_cast<MCObject*>(mcApi)->GetSunRiseSet(rise, set));
}

int mcEnumeratePhases(void* mcApi, double dateStart, double dateEnd)
{
    CHECK(mcApi);
    return int(reinterpret_cast<MCObject*>(mcApi)->EnumeratePhases(dateStart, dateEnd));
}

int mcNextPhase(void* mcApi, double& when, int& phaseKind)
{
    CHECK(mcApi);
    return int(reinterpret_cast<MCObject*>(mcApi)->NextPhase(when, phaseKind));
}


int mcGetNewMoon(void* mcApi, double& when)
{
    CHECK(mcApi);
    return int(reinterpret_cast<MCObject*>(mcApi)->GetNewMoon(when));
}

int mcGetRiseCount(void* mcApi, int& count)
{
    CHECK(mcApi);
    return int(reinterpret_cast<MCObject*>(mcApi)->GetRiseCount(count));
}

int mcGetGlobalStellarTime(void* mcApi, double& globalStellarTime)
{
    CHECK(mcApi);
    return int(reinterpret_cast<MCObject*>(mcApi)->GetGlobalStellarTime(globalStellarTime));
}

int mcGetLocalStellarTime(void* mcApi, double& localStallarTime)
{
    CHECK(mcApi);
    return int(reinterpret_cast<MCObject*>(mcApi)->GetLocalStellarTime(localStallarTime));
}
