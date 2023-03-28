package com.moonx.mcapi;

import org.apache.commons.lang3.builder.ToStringBuilder;

public class PhaseInformation {
    public final double age;
    public final double xz;
    public final double count;

    public PhaseInformation(double age, double count, double xz) {
        this.age = age;
        this.count = count;
        this.xz = xz;
    }

    @Override
    public String toString() {
        return ToStringBuilder.reflectionToString(this);
    }
}
