package com.company;

import java.nio.ByteBuffer;

public class SignalGenerator {
    public static final int SAMPLING_RATE = 48000;

    public static short[] getSamplesOfSineSignal(double baseTime) {
        int numberOfSamples = UDPSender.PACKET_SIZE / 2;
        short[] samples = new short[numberOfSamples];
        double step = 1.0 / SAMPLING_RATE;
        for (int i = 0; i < numberOfSamples; i++) {
            //samples[i] = sineSignal(baseTime + i * step);
            //samples[i] = sineSignal2(baseTime + i * step);
            samples[i] = sawtoothWave(baseTime + i * step);
        }
        return samples;
    }

    /*
    build a packet from array of samples
    */
    public static byte[] putSamplesToPacket(short[] samples) {
        if (2 * samples.length > UDPSender.PACKET_SIZE) {
            throw new IllegalArgumentException("Packet is too small to contain all samples!");
        }
        ByteBuffer buffer = ByteBuffer.allocate(samples.length * 2);
        for (short sample : samples) {
            //System.out.print(sample + " ");
            buffer.putShort(sample);
        }
        //System.out.println();
        return buffer.array();
    }

    public static short sineSignal(double time) {
        double v = Math.sin(time);
        v *= 32767;
        return (short)v;
    }

    public static short sineSignal2(double time) {
        double v = Math.sin(5*time) + Math.sin(66*time) + Math.sin(37*time);
        v *= 32767;
        v /= 3;
        return (short)v;
    }

    public static short sawtoothWave(double time) {
        int freqHz = 3;
        double v = 2 * 32767 * (time * freqHz - (Math.floor(1.0 / 2.0 + time * freqHz)));
        return (short)v;
    }
}
