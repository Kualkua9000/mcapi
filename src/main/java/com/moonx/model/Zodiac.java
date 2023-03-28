package com.moonx.model;

import java.util.Arrays;

public enum Zodiac {
    aries(0),
    taurus(1),
    gemini(2),
    cancer(3),
    leo(4),
    virgo(5),
    libra(6),
    scorpio(7),
    sagittarius(8),
    capricornus(9),
    aquarius(10),
    pisces(11);

    public final int index;

    Zodiac(int index) {
        this.index = index;
    }

    public static Zodiac valueOf(int index) {
        return Arrays.stream(values())
                .filter(v -> v.index == index).findFirst()
                .orElseThrow(() -> new IllegalArgumentException("index: " + index));
    }
}
