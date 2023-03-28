#include "mcobject.h"
#include <cmath>

const double pi = std::atan(1) * 4;

#define REQUIRE(what) { auto status = what; if (status != mcStatus::Success) return status; }

mcStatus MCObject::Data::GetEclipticInclination()
{
    if ((status & StatusEclipticInclination) == 0)
    {
        REQUIRE(RequireTime());

        epsilon = EclipticInclination(time);
        status |= StatusEclipticInclination;
    }

    return mcStatus::Success;
}

mcStatus MCObject::Data::RequireSunPosition()
{
    if ((status & StatusSunPosition) == 0)
    {
        REQUIRE(RequireTime());
        REQUIRE(GetEclipticInclination());

        SunPos(this->time, sun.lambda, sun.distance);
        e2q(sun.lambda, 0, epsilon, sun.alpha, sun.delta);
        status |= StatusSunPosition;
    }

    return mcStatus::Success;
}

mcStatus MCObject::Data::RequireMoonPosition()
{
    if ((status & StatusMoonPosition) == 0)
    {
        REQUIRE(RequireTime());
        REQUIRE(GetEclipticInclination());

        MoonPos(time, moon.lambda, moon.beta, moon.distance);
        e2q(moon.lambda, moon.beta, epsilon, moon.alpha, moon.delta);
        status |= StatusMoonPosition;
    }

    return mcStatus::Success;
}

mcStatus MCObject::Data::RequireTime() const
{
    if ((status & StatusTime) == 0)
    {
        return mcStatus::InvalidDate;
    }

    return mcStatus::Success;
}

mcStatus MCObject::Data::RequireLocation() const
{
    if ((status & StatusLocation) == 0)
    {
        return mcStatus::LocationRequired;
    }

    return mcStatus::Success;
}


mcStatus MCObject::Data::RequireStellarTimeGlobal()
{
    if ((status & GlobalStellarTime) == 0)
    {
        REQUIRE(RequireTime());
        this->stellarGlobal = ::GlobalStellarTime(this->time);
        status |= GlobalStellarTime;
    }
    return mcStatus::Success;
}


mcStatus MCObject::Data::RequireStellarTimeLocal()
{
    if ((status & LocalStellarTime) == 0)
    {
        REQUIRE(RequireLocation());
        REQUIRE(RequireTime());
        this->stellarLocal = ::LocalStellarTime(this->time, this->longitude);
        status |= LocalStellarTime;
    }
    return mcStatus::Success;
}

mcStatus MCObject::Data::SetLocation(double longitudeDeg, double latitudeDeg)
{
    if (longitudeDeg < -180 || longitudeDeg >= 360) return mcStatus::ArgumentRange;
    if (latitudeDeg < -90 || latitudeDeg >= 90) return mcStatus::ArgumentRange;
    if (longitudeDeg > 180) longitudeDeg -= 360;

    this->longitude = longitudeDeg / 180 * pi;
    this->latitude = latitudeDeg / 180 * pi;
    this->status |= StatusLocation;
    this->status &= ~StatusResetLocation;

    return mcStatus::Success;
}

mcStatus MCObject::Data::SetTime(double timeInSecondsSinceJ2000, double localTimeZone)
{
    const double time = timeInSecondsSinceJ2000 / (86400. * 36525);
    if (time < -1 || time > 1) return mcStatus::ArgumentRange;
    this->time = time;
    this->timeZone = localTimeZone;
    this->status |= StatusTime;
    this->status &= ~StatusResetTime;
    return mcStatus::Success;
}

void MCObject::Data::AdjustTime(double seconds)
{
    if (status & StatusTime)
    {
        time += seconds / 86400. / 36525.;
        status &= ~StatusResetTime;
    }
}

double MCObject::Data::GetLocalMidnight() const
{
    const double tz = timeZone * 3600;
    const double localTime = time * 36525. * 86400. + tz;
    const double days = floor(localTime / 86400);
    return days * 86400 - tz - 12 * 3600;
}


mcStatus MCObject::Data::GetEclipticPosition(double& lon, double& lat, double& distance)
{
    REQUIRE(RequireMoonPosition());
    lon = moon.lambda;
    lat = moon.beta;
    distance = moon.distance;
    return mcStatus::Success;
}

mcStatus MCObject::Data::GetEquatorialPosition(double& ra, double& dec, double& distance)
{
    REQUIRE(RequireMoonPosition());
    ra = moon.alpha;
    dec = moon.delta;
    distance = moon.distance;
    return mcStatus::Success;
}

mcStatus MCObject::Data::GetElongation(double& elongation)
{
    REQUIRE(RequireSunPosition());
    REQUIRE(RequireMoonPosition());
    elongation = norm(moon.lambda - sun.lambda + 360 * Deg(), 360 * Deg());
    return mcStatus::Success;
}

mcStatus MCObject::Data::GetElongationPrecise(double& elongation)
{
    double ex;
    REQUIRE(GetElongation(ex));
    const double e = acos(cos(moon.beta) * cos(moon.lambda - sun.lambda));

    elongation = ex > 180 * Deg() ? 360 * Deg() - e : e;
    return mcStatus::Success;
}

mcStatus MCObject::Data::GetPhaseInformation(double& phase, double& age, double& luminance)
{

    double e;
    REQUIRE(GetElongation(e));

    phase = e / (360 * Deg());
    age = phase * 29.53;

    const auto psi = sin(e / 2);
    luminance = psi * psi;

    return mcStatus::Success;
}

mcStatus MCObject::Data::GetPhaseInformationPrecise(double& phase, double& age, double& luminance)
{
    double ep, e;
    REQUIRE(GetElongation(e));
    REQUIRE(GetElongationPrecise(ep));

    phase = e / (360 * Deg());

    age = phase * 29.53;

    const auto psi = sin(ep / 2);
    luminance = psi * psi;

    return mcStatus::Success;
}

mcStatus MCObject::Data::GetRelativePlanetshine(double& planetshine)
{
    if ((status & StatusMoonPlanetshine) == 0)
    {
        double e;
        REQUIRE(GetElongation(e));
        moon.planetshine = pow(abs(sin(e / 2)), 2.5);

        status |= StatusMoonPlanetshine;
    }

    planetshine = moon.planetshine;

    return mcStatus::Success;
}

mcStatus MCObject::Data::GetMoonHorizontalPosition(double& altitude, double& azimuth, double& posAngle)
{
    if ((status & StatusMoonHorizontal) == 0)
    {
        REQUIRE(RequireLocation());
        REQUIRE(RequireMoonPosition());
        REQUIRE(RequireStellarTimeLocal());

        double sunAlt, sunAz, distance;
        REQUIRE(GetSunHorizontalPosition(sunAlt, sunAz, distance));

        const double tau = norm(stellarLocal / 86400 * 360 * Deg() - moon.alpha, 360 * Deg());

        q2h(tau, moon.delta, latitude, longitude, moon.azimuth, moon.altitude);


        moon.posAngle = acos(cos(sunAlt) * cos(moon.altitude) * sin(sun.azimuth - moon.azimuth));


        const double parallax = asin(6378.0 / (384400.0 * moon.distance)) * cos(moon.altitude);
        moon.altitude -= parallax;

        status |= StatusMoonHorizontal;
    }

    altitude = moon.altitude;
    azimuth = moon.azimuth;
    posAngle = moon.posAngle;

    return mcStatus::Success;
}

//static double CosT(double alt, double lat, double dec)
//{
//    return (sin(alt) - sin(lat) * sin(dec)) / (cos(lat) * cos(dec));
//}

mcStatus MCObject::Data::GetMoonRiseSet(double& _rise, double& _set)
{
    if ((status & StatusMoonRiseSet) == 0)
    {
        double alt, az;

        REQUIRE(RequireLocation());
        //const double hemisphere = latitude < 0 ? -1 : 1;

        double step;
        if (abs(this->latitude) > DMS(62))
        {
            step = 900; // 15 minutes
        }
        else if (abs(this->latitude) > DMS(58))
        {
            step = 3600; // 1 hour
        }
        else
        {
            step = 3 * 3600; // 3 hours
        }


        double pa;
        const double midnight = GetLocalMidnight();
        moon.rise = std::nan("");
        moon.set = std::nan("");

        double prevAlt = std::nan("");
        for (double t = 0; t < 86400 + step/2; t += step)
        {
            Data newData = *this;
            newData.SetTime(midnight + t, timeZone);
            newData.GetMoonHorizontalPosition(alt, az, pa);

            if (!std::isnan(prevAlt) && (prevAlt * alt < 0))
            {
                double& target = prevAlt < 0 ? moon.rise : moon.set;
                double prevAlt2 = prevAlt;

                double a = t - step;
                double b = t;

                while (b - a > 15)
                {
                    newData.SetTime(midnight + (a + b) / 2, timeZone);
                    double alt2;
                    newData.GetMoonHorizontalPosition(alt2, az, pa);
                    if (prevAlt2 * alt2 < 0)
                    {
                        b = (a + b) / 2;
                    }
                    else
                    {
                        a = (a + b) / 2;
                        prevAlt2 = alt2;
                    }
                }

                target = midnight + (a + b) / 2;
                if (!std::isnan(moon.rise) && !std::isnan(moon.set))
                {
                    break;
                }
            }

            prevAlt = alt;

        }

        status |= StatusMoonRiseSet;
    }

    _rise = moon.rise;
    _set = moon.set;

    return mcStatus::Success;
}


mcStatus MCObject::Data::GetSunHorizontalPosition(double& altitude, double& azimuth, double& distance)
{
    if ((status & StatusSunHorizontal) == 0)
    {
        REQUIRE(RequireLocation());
        REQUIRE(RequireSunPosition());
        REQUIRE(RequireStellarTimeLocal());

        const double tau = norm(stellarLocal / 86400 * 360 * Deg() - sun.alpha, 360 * Deg());

        q2h(tau, sun.delta, latitude, longitude, sun.azimuth, sun.altitude);

        status |= StatusSunPosition;
    }

    altitude = sun.altitude;
    azimuth = sun.azimuth;
    distance = sun.distance;

    return mcStatus::Success;
}

mcStatus MCObject::Data::GetSunRiseSet(double& rise, double& set)
{
    if ((status & StatusSunRiseSet) == 0)
    {
        double alt, az, distance;

        REQUIRE(RequireLocation());
        //const double hemisphere = latitude < 0 ? -1 : 1;

        double step;
        if (abs(this->latitude) > DMS(66))
        {
            step = 900; // 15 minutes
        }
        else if (abs(this->latitude)> DMS(63))
        {
            step = 3600; // 1 hour
        }
        else
        {
            step = 3 * 3600; // 3 hours
        }


        const double midnight = GetLocalMidnight();
        sun.rise = std::nan("");
        sun.set = std::nan("");

        double prevAlt = std::nan("");
        for (double t = 0; t < 86400 + step/2; t += step)
        {
            Data newData = *this;
            newData.SetTime(midnight + t, timeZone);
            newData.GetSunHorizontalPosition(alt, az, distance);

            if (!std::isnan(prevAlt) && (prevAlt * alt < 0))
            {
                double& target = prevAlt < 0 ? sun.rise : sun.set;
                double prevAlt2 = prevAlt;

                double a = t - step;
                double b = t;

                while (b - a > 15)
                {
                    newData.SetTime(midnight + (a + b) / 2, timeZone);
                    double alt2;
                    newData.GetSunHorizontalPosition(alt2, az, distance);
                    if (prevAlt2 * alt2 < 0)
                    {
                        b = (a + b) / 2;
                    }
                    else
                    {
                        a = (a + b) / 2;
                        prevAlt2 = alt2;
                    }
                }

                target = midnight + (a + b) / 2;
                if (!std::isnan(sun.rise) && !std::isnan(sun.set))
                {
                    break;
                }
            }

            prevAlt = alt;

        }

        status |= StatusSunRiseSet;
    }

    rise = sun.rise;
    set = sun.set;

    return mcStatus::Success;
}

mcStatus MCObject::Data::EnumeratePhases(double dateStart, double dateEnd)
{
    if (dateEnd <= dateStart)
    {
        return mcStatus::InvalidDate;
    }

    phase.start = dateStart;
    phase.end = dateEnd;
    phase.currentPhase = -1;
    phase.current = dateStart;

    return mcStatus::Success;
}

mcStatus MCObject::Data::NextPhase(double& when, int& phaseKind)
{
    MCObject::Data testData;
    double e;

    if (phase.currentPhase == -1) {
        const double m = (phase.start - FirstNewMoon) / SynodicMonth;
        int n = lround(4 * m) % 8;
        if (n > 4)
        {
            n = n % 4;
        }
        phase.current = (floor(m) + (n - 1) * 0.25) * SynodicMonth + FirstNewMoon;
        phase.currentPhase = (n + 3) % 4;
    } else {
        phase.current += SynodicMonth / 4;
        phase.currentPhase = (phase.currentPhase + 1) % 4;
    }

    if (phase.current > phase.end)
    {
        return mcStatus::Complete;
    }

    for (int i = 0; i < 10; ++i)
    {
        testData.SetTime(phase.current, this->timeZone);
        testData.GetElongation(e);
        double correction = (phase.currentPhase * 90 * Deg() - e) / (360 * Deg()) * SynodicMonth;
        if (correction < -SynodicMonth / 2)
        {
            correction += SynodicMonth;
        }
        phase.current += correction;
        if (abs(correction) < 60) break;
    }

    if (phase.current < phase.start)
    {
        return NextPhase(when, phaseKind);
    }

    /*testData.SetTime(phase.current);
    testData.GetElongation(e);*/
    when = phase.current;
    phaseKind = phase.currentPhase;
    if (phase.current > phase.end)
    {
        return mcStatus::Complete;
    }
    return mcStatus::Success;
}

mcStatus MCObject::Data::GetNewMoon(double& when)
{
    if ((status & StatusNewMoon) == 0)
    {
        REQUIRE(RequireTime());

        const double m = floor((this->time * 86400. * 36525. - FirstNewMoon) / SynodicMonth);
        MCObject::Data testData;

        double t = FirstNewMoon + m * SynodicMonth;

        for (int i = 0; i < 10; ++i)
        {
            double e;
            testData.SetTime(t, this->timeZone);
            testData.GetElongation(e);
            double correction = -e / (360 * Deg()) * SynodicMonth;
            if (correction < -SynodicMonth / 2)
            {
                correction += SynodicMonth;
            }
            t += correction;
            if (abs(correction) < 60) break;
        }
        moon.newMoon = t;
        status |= StatusNewMoon;
    }
    when = moon.newMoon;

    return mcStatus::Success;
}

mcStatus MCObject::Data::GetRiseCount(int& count)
{
    REQUIRE(RequireTime());
    REQUIRE(RequireLocation());

    double start;
    REQUIRE(GetNewMoon(start));

    const double now = time * 86400. * 36525.;

    Data test = *this;
    bool prevNeg = 0;

    double rise, set;

    REQUIRE(GetMoonRiseSet(rise, set));

    count = 1;

    double alt, az, pos;
    test.SetTime(start, this->timeZone);
    REQUIRE(test.GetMoonHorizontalPosition(alt, az, pos));
    prevNeg = (alt < 0);

    for (double t = start + 3600; t < now; t += 3600.)
    {
        test.SetTime(t, this->timeZone);
        test.GetMoonHorizontalPosition(alt, az, pos);
        if (alt > 0 && prevNeg)
        {
            ++count;
        }
        prevNeg = alt < 0;
    }
    if (prevNeg)
    {
        test.SetTime(now, this->timeZone);
        test.GetMoonHorizontalPosition(alt, az, pos);
        if (alt > 0)
        {
            ++count;
        }
    }

    return mcStatus::Success;
}

mcStatus MCObject::Data::GetGlobalStellarTime(double& globalStellarTime)
{
    REQUIRE(RequireStellarTimeGlobal());
    globalStellarTime = this->stellarGlobal;
    return mcStatus::Success;
}

mcStatus MCObject::Data::GetLocalStellarTime(double& localStellarTime)
{
    REQUIRE(RequireStellarTimeLocal());
    localStellarTime = this->stellarLocal;
    return mcStatus::Success;
}


MCObject::MCObject() : signature(Signature)
{
}
