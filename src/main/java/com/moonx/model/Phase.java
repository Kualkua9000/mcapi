package com.moonx.model;

import org.apache.commons.lang3.builder.ToStringBuilder;

import java.util.regex.Pattern;

public class Phase {
    private String message;
    private Integer phase;
    private String locale;
    private Long lastUsage;

    public String getMessage() {
        return message;
    }

    public Phase setMessage(String message) {
        this.message = message;
        return this;
    }

    public Integer getPhase() {
        return phase;
    }

    public Phase setPhase(Integer phase) {
        this.phase = phase;
        return this;
    }

    public String getLocale() {
        return locale;
    }

    public Phase setLocale(String locale) {
        this.locale = locale;
        return this;
    }

    public Long getLastUsage() {
        return lastUsage;
    }

    public Phase setLastUsage(Long lastUsage) {
        this.lastUsage = lastUsage;
        return this;
    }

    @Override
    public String toString() {
        return ToStringBuilder.reflectionToString(this);
    }

    public static String locale(String message) {
        if (Pattern.matches(".*\\p{InCyrillic}.*", message))
            return "RU";
        else
            return "EN";

    }

    public static Phase Phase(String message, Integer phase, String locale, Long lastUsage) {
        return new Phase().setMessage(message).setPhase(phase).setLocale(locale).setLastUsage(lastUsage);
    }

    public static Phase Phase(String message, Integer phase, String locale) {
        return new Phase().setMessage(message).setPhase(phase).setLocale(locale).setLastUsage(0L);
    }

    public static Phase New(String message) {
        return Phase(message, 0);
    }

    public static Phase First(String message) {
        return Phase(message, 1);
    }

    public static Phase Third(String message) {
        return Phase(message, 3);
    }

    public static Phase Full(String message) {
        return Phase(message, 2);
    }

    public static Phase Phase(String message, int phase) {
        return new Phase().setMessage(message).setPhase(phase).setLocale(locale(message)).setLastUsage(0L);
    }
}
