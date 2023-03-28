package com.moonx;

import com.moonx.mcapi.EclipticPosition;
import com.moonx.mcapi.PhaseInformation;
import com.moonx.mcapi.Request;
import com.moonx.model.Phase;

import java.time.LocalDateTime;

public class MCAPI {
    private final Request request;
    private long pointer;

    private EclipticPosition eclipticPosition;
    private double newMoon;

    private Phase nextPhase;
    private PhaseInformation phaseInformation;

    private int riseCount;

    MCAPI(Request request) {
        this.request = request;
        if (init() != 0)
            throw new RuntimeException();
    }

    MCAPI(double time, double timeZone, double longitude, double latitude) {
        this.request = new Request(time, timeZone, longitude, latitude);
        if (init() != 0)
            throw new RuntimeException();
    }

    private native int init();

    public native int destroy();

    public native int eclipticPosition();

    public native int phaseInformation();

    public native int newMoon();

    public native int enumeratePhases(double start, double end);

    public native int nextPhase();

    public native int riseCount();

    public static void main(String[] args) {
        //System.loadLibrary("/Users/akoira/_prj/_hoollyby/MoonXBack/mcapi/build/lib/main/debug/libmcapi.dylib");
        MCAPI mcapi = new MCAPI(LocalDateTime.now().getSecond(), 10800, 0, 0);
        mcapi.riseCount();
        mcapi.eclipticPosition();
        System.out.println(mcapi.riseCount);

//        if (mcapi.eclipticPosition() != 0)
//            throw new RuntimeException();
//        System.out.println(mcapi.eclipticPosition);
//
//        if (mcapi.newMoon() != 0)
//            throw new RuntimeException();
//        System.out.println(mcapi.newMoon);
//
//        if (mcapi.enumeratePhases(mcapi.request.time, mcapi.request.time + 24 * 60 * 60 * 50) != 0)
//            throw new RuntimeException();
//
//        int code = mcapi.nextPhase();
//        while (code == 0) {
//            System.out.println(mcapi.nextPhase);
//            code = mcapi.nextPhase();
//        }
        mcapi.destroy();
    }

    public Phase getNextPhase() {
        return nextPhase;
    }

    public int getRiseCount() {
        return riseCount;
    }

    public PhaseInformation getPhaseInformation() {
        return phaseInformation;
    }

    public EclipticPosition getEclipticPosition() {
        return eclipticPosition;
    }
}
