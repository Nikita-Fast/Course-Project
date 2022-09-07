package com.company;

public class Main {

    public static void main(String[] args) {

//        UDPSender.send(1500, ((SignalGenerator.SAMPLING_RATE * 2) / 1024.0));
        String path = "/home/nikita/PycharmProjects/DSP/modulation/out.txt";
        UDPSender.sendFromFile(path, 64_000);
        System.out.println("end");
    }
}
