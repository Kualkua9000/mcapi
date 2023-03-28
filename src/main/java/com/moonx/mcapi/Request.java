package com.moonx.mcapi;

public class Request {
    public final double time;
    public final double timeZone;
    public final double lon;
    public final double lat;

    public Request(double time, double timeZone, double lon, double lat) {
        this.time = time;
        this.timeZone = timeZone;
        this.lon = lon;

        this.lat = lat;
    }
}
