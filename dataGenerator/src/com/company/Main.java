package com.company;


import java.util.Arrays;

public class Main {

    public static void main(String[] args) {

        UDPSender.send(4, 1);
        /*short[] samples = SignalGenerator.getSamplesOfSineSignal( 0.0);
        for (short sample : samples) {
            System.out.print(sample + "  ");
        }
        System.out.println();
        byte[] packet = SignalGenerator.putSamplesToPacket(samples);
        System.out.println(Arrays.toString(packet));*/
    }
}
