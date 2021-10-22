package com.company;

public class Main {

    public static void main(String[] args) {

        UDPSender.send(15, (SignalGenerator.SAMPLING_RATE * 1.75) / 1024.0);
    }
}
