#pragma once
#ifndef _MC_OBJECT_H_
#define _MC_OBJECT_H_
#include "mcapi.h"
#include "status.h"
#include "sun/sunpos.h"
#include "moon/moonpos.h"
#include "math/transform.h"
#include "math/utils.h"

class MCObject
{
    static constexpr unsigned Signature = 'MCAO';
    static constexpr unsigned StatusTime = 1;
    static constexpr unsigned StatusLocation = 2;

    static constexpr unsigned StatusSunPosition = 0x100;
    static constexpr unsigned StatusSunHorizontal = 0x200;
    static constexpr unsigned StatusSunRiseSet = 0x400;

    static constexpr unsigned StatusMoonPosition = 0x1000;
    static constexpr unsigned StatusMoonHorizontal = 0x2000;
    static constexpr unsigned StatusMoonRiseSet = 0x4000;
    static constexpr unsigned StatusMoonPlanetshine = 0x8000;
    static constexpr unsigned StatusNewMoon = 0x10000;

    static constexpr unsigned GlobalStellarTime = 0x100000;
    static constexpr unsigned LocalStellarTime = 0x200000;

    static constexpr unsigned StatusEclipticInclination = 0x10000000;

    static constexpr unsigned StatusResetTime = 0
        | StatusSunPosition | StatusSunHorizontal | StatusSunRiseSet
        | StatusMoonPosition | StatusMoonHorizontal | StatusMoonRiseSet | StatusNewMoon
        | GlobalStellarTime | LocalStellarTime
        | StatusEclipticInclination;

    static constexpr unsigned StatusResetLocation = 0
        | StatusMoonHorizontal | StatusMoonRiseSet
        | StatusSunHorizontal | StatusSunRiseSet
        | LocalStellarTime
        ;

    static constexpr double FirstNewMoon = 454416.;
    static constexpr double SynodicMonth = 29.5305882 * 86400.;

    unsigned signature{};

    struct Data
    {
        unsigned status{};

        double longitude{};
        double latitude{};

        double time{};
        double timeZone;

        double stellarGlobal{};
        double stellarLocal{};

        double epsilon{};

        struct SunData
        {
            double lambda;
            double alpha;
            double delta;
            double distance;

            double azimuth;
            double altitude;

            double rise;
            double set;
        } sun{};

        struct MoonData
        {
            double lambda;
            double beta;
            double alpha;
            double delta;
            double distance;

            double azimuth;
            double altitude;
            double posAngle;

            double planetshine;

            double rise;
            double set;

            double newMoon;
        } moon{};



        mcStatus RequireTime() const;

        mcStatus RequireLocation() const;

        mcStatus GetEclipticInclination();

        mcStatus RequireSunPosition();

        mcStatus RequireMoonPosition();

        mcStatus RequireStellarTimeGlobal();

        mcStatus RequireStellarTimeLocal();

        mcStatus SetLocation(double longitudeDeg, double latitudeDeg);

        mcStatus SetTime(double timeInSecondsSinceJ2000, double localTimeZone);

        mcStatus GetEclipticPosition(double& lon, double& lat, double& distance);

        mcStatus GetEquatorialPosition(double& ra, double& dec, double& distance);

        mcStatus GetElongation(double& elongation);

        mcStatus GetElongationPrecise(double& elongation);

        mcStatus GetPhaseInformation(double& phase, double& age, double& luminance);

        mcStatus GetPhaseInformationPrecise(double& phase, double& age, double& luminance);

        mcStatus GetRelativePlanetshine(double& planetshine);

        mcStatus GetMoonHorizontalPosition(double& altitude, double& azimuth, double& posAngle);

        mcStatus GetMoonRiseSet(double& rise, double& set);

        mcStatus GetSunHorizontalPosition(double& altitude, double& azimuth, double& distance);


        mcStatus GetSunRiseSet(double& rise, double& set);

        mcStatus EnumeratePhases(double dateStart, double dateEnd);

        mcStatus NextPhase(double& when, int& phaseKind);

        mcStatus GetNewMoon(double &when);

        mcStatus GetRiseCount(int& count);

        mcStatus GetGlobalStellarTime(double &globalStellarTime);

        mcStatus GetLocalStellarTime(double &localStellarTime);


    private:
        void AdjustTime(double seconds);

        double GetLocalMidnight() const;

        double ApproximateSunCross(double prevAlt, double alt, double midnight, double timeShift);

        struct
        {
            double start;
            double end;
            double current;
            int currentPhase;
        } phase {};

    } data {};

public:

    MCObject();

    inline static bool Check(const void* mcApi)
    {
        return (mcApi != nullptr) && (reinterpret_cast<const MCObject*>(mcApi)->signature == Signature);
    }

    inline mcStatus SetLocation(double longitudeDeg, double latitudeDeg)
    {
        return data.SetLocation(longitudeDeg, latitudeDeg);
    }

    inline mcStatus SetTime(double timeInSecondsSinceJ2000, double localTimeZone)
    {
        return data.SetTime(timeInSecondsSinceJ2000, localTimeZone);
    }

    mcStatus GetEclipticPosition(double& lon, double& lat, double& distance)
    {
        return data.GetEclipticPosition(lon, lat, distance);
    }

    mcStatus GetEquatorialPosition(double& ra, double& dec, double& distance)
    {
        return data.GetEquatorialPosition(ra, dec, distance);
    }

    mcStatus GetElongation(double& elongation)
    {
        return data.GetElongation(elongation);
    }

    mcStatus GetElongationPrecise(double& elongation)
    {
        return data.GetElongationPrecise(elongation);
    }

    mcStatus GetPhaseInformation(double& phase, double& age, double& luminance)
    {
        return data.GetPhaseInformation(phase, age, luminance);
    }

    mcStatus GetPhaseInformationPrecise(double& phase, double& age, double& luminance)
    {
        return data.GetPhaseInformationPrecise(phase, age, luminance);
    }

    mcStatus GetRelativePlanetshine(double& planetshine)
    {
        return data.GetRelativePlanetshine(planetshine);
    }

    mcStatus GetMoonHorizontalPosition(double& altitude, double& azimuth, double& posAngle)
    {
        return data.GetMoonHorizontalPosition(altitude, azimuth, posAngle);
    }

    mcStatus GetMoonRiseSet(double& rise, double& set)
    {
        return data.GetMoonRiseSet(rise, set);
    }

    mcStatus GetSunHorizontalPosition(double& altitude, double& azimuth, double& distance)
    {
        return data.GetSunHorizontalPosition(altitude, azimuth, distance);
    }

    mcStatus GetSunRiseSet(double& rise, double& set)
    {
        return data.GetSunRiseSet(rise, set);
    }

    mcStatus EnumeratePhases(double dateStart, double dateEnd)
    {
        return data.EnumeratePhases(dateStart, dateEnd);
    }

    mcStatus NextPhase(double& when, int& phaseKind)
    {
        return data.NextPhase(when, phaseKind);
    }

    mcStatus GetNewMoon(double& when)
    {
        return data.GetNewMoon(when);
    }

    mcStatus GetRiseCount(int& count)
    {
        return data.GetRiseCount(count);
    }

    mcStatus GetGlobalStellarTime(double &globalStellarTime)
    {
        return data.GetGlobalStellarTime(globalStellarTime);
    }

    mcStatus GetLocalStellarTime(double &localStellarTime)
    {
        return data.GetLocalStellarTime(localStellarTime);
    }

};

#endif
