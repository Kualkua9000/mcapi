package com.moonx.mcapi;

import org.apache.commons.lang3.builder.ToStringBuilder;

public class EclipticPosition {
    public final double lon;
    public final double lat;
    public final double distance;

    public EclipticPosition(double lon, double lat, double distance) {
        this.lon = lon;
        this.lat = lat;
        this.distance = distance;
    }

    @Override
    public String toString() {
        return ToStringBuilder.reflectionToString(this);
    }
}
