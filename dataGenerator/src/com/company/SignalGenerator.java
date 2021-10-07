package com.company;

import java.nio.ByteBuffer;

public class SignalGenerator {
    public static final int SAMPLING_RATE = 48000;

    public static void generateSquareSignal(byte[] buffer) {
        int length = buffer.length;
        for (int i = 0; i < length; i++) {
            buffer[i] = (byte) (i >= length / 2 ? 0 : 1);
        }
    }
    /*
    * fill a given packet with samples of sine signal
    * 48k samples for a second
    * 750 of packs filled with shorts
    * so we have 750 packets in a second
    * */
    public static short[] getSamplesOfSineSignal(double baseTime) {
        //numberOfSamples assumed to be half of UDPSender.PACKET_SIZE
        int numberOfSamples = UDPSender.PACKET_SIZE / 2;
        short[] samples = new short[numberOfSamples];
        double step = 1.0 / SAMPLING_RATE;
        for (int i = 0; i < numberOfSamples; i++) {
            samples[i] = sineSignal(baseTime + i * step);
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
}
