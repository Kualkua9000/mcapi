package com.moonx.model;

import org.apache.commons.lang3.tuple.Pair;

import java.util.List;

public interface MoonApi {
    List<Phase> phases(double start, double end, Request request);

    int day(Request request);

    int riseCount(Request request);

    Zodiac zodiac(ZodiacType type, Request request);

    List<Pair<Long, Zodiac>> zodiacs(ZodiacType type, Long start, Integer days);
}
