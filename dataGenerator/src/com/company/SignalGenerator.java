package com.company;

public class SignalGenerator {

    public static void generateSquareSignal(byte[] buffer) {
        int length = buffer.length;
        for (int i = 0; i < length; i++) {
            buffer[i] = (byte) (i >= length / 2 ? 0 : 1);
        }
    }
}
