package com.company;

import java.nio.ByteBuffer;

public class SignalGenerator {
    public static final int SAMPLING_RATE = 2048 * 4;

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
        double v = Math.sin(time * 4 * 2 * Math.PI);
        v *= 32767;
        return (short)v;
    }

    public static short sineSignal2(double time) {
        double p = 2 * Math.PI;
        double v = Math.sin(1*time* p) + Math.sin(3*time* p) + Math.sin(8*time* p);
        v *= 32767;
        v /= 3;
        return (short)v;
    }

    public static short sawtoothWave(double time) {
        //int freqHz = 20;
        double freqHz = 1;
        double v = 2 * 32767 * (time * freqHz - (Math.floor(1.0 / 2.0 + time * freqHz)));
        return (short)v;
    }
}
