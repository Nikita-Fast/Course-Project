package com.company;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.net.*;
import java.util.Arrays;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.CountDownLatch;

public class UDPSender {
    public final static int SERVICE_PORT = 50001;
    public static final int PACKET_SIZE = 256 * 2;
    public static final String HOST_NAME = "localhost";
    public static final int PACKETS_FOR_ONE_SEC_OF_SIGNAL = SignalGenerator.SAMPLING_RATE / (PACKET_SIZE / 2);
    public static final double TOLERANCE = 3.0;

    private static BufferedReader br;

    public static void send(int signalDuration, double speedOfDataTransmission) {
        double packetSizeInKB = PACKET_SIZE / 1024.0;
        double packetsSendPerSecond = speedOfDataTransmission / packetSizeInKB;
        double sendingFrequencyInMillis = 1000.0 / packetsSendPerSecond;

        int packetsPerOneSending = 1;
        while (doubleToIntDeviation(sendingFrequencyInMillis) > TOLERANCE) {
            packetsPerOneSending *= 2;
            sendingFrequencyInMillis *= 2;
            System.out.println("packetsPerOneSending: " + packetsPerOneSending);
            System.out.println("sendingFrequencyInMillis: " + sendingFrequencyInMillis);
        }

        double speed = (PACKET_SIZE * packetsPerOneSending / 1024.0) / (int)(sendingFrequencyInMillis) * 1000;
        System.out.println("speed: " + speed);
        send(signalDuration, (int)sendingFrequencyInMillis, packetsPerOneSending);

    }

    public static void sendFromFile(String fileName, int samplingFrequency) {
        int samplesPerPacket = PACKET_SIZE / 2;
        int packetsPerSecond = samplingFrequency / samplesPerPacket;
        int timerPeriodMs = 1000 / packetsPerSecond;

        final CountDownLatch remainingTimerIterations =
                new CountDownLatch(1);

        try (DatagramSocket socket = new DatagramSocket()) {
            InetAddress inetAddress = InetAddress.getByName(HOST_NAME);

//            br = new BufferedReader(new FileReader("/home/nikita/PycharmProjects/DSP/modulation/out.txt"));
            br = new BufferedReader(new FileReader(fileName));

            Timer timer = new Timer();
            timer.scheduleAtFixedRate(new TimerTask() {
                @Override
                public void run() {
                    try {
                        short[] samples = readSamplesFromFile(br);
                        if (samples == null) {
                            System.out.println("cancel timer");
                            remainingTimerIterations.countDown();
                            timer.cancel();
                        }
                        else {
                            byte[] packet = SignalGenerator.putSamplesToPacket(samples);
//                            System.out.println(Arrays.toString(samples));
                            DatagramPacket outgoing = new DatagramPacket(packet, PACKET_SIZE, inetAddress, SERVICE_PORT);
                            socket.send(outgoing);
                        }
                    }
                    catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }, 0, timerPeriodMs);
            try {
                remainingTimerIterations.await();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            timer.cancel();
        }
        catch (SocketException | SecurityException | UnknownHostException | FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    private static short[] readSamplesFromFile(BufferedReader br) {
        try {
            String line = null;
            int c = 0;
            int samplesPerPacket = PACKET_SIZE / 2;
            short[] samples = new short[samplesPerPacket];
            while (c < samplesPerPacket && (line = br.readLine()) != null) {
                // process the line.
                short sample = Short.parseShort(line);
                samples[c++] = sample;

            }
            if (line == null) {
                return null;
            }
            return samples;
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    private static void send(int signalDuration, int timerPeriod, int packetsPerOneSending) {

        final int numberOfPacketsToBeSent = PACKETS_FOR_ONE_SEC_OF_SIGNAL * signalDuration;
        final CountDownLatch remainingTimerIterations =
                new CountDownLatch(numberOfPacketsToBeSent);

        try (DatagramSocket socket = new DatagramSocket()) {
            InetAddress inetAddress = InetAddress.getByName(HOST_NAME);

            Timer timer = new Timer();
            timer.scheduleAtFixedRate(new TimerTask() {
                @Override
                public void run() {
                try {
                    if (remainingTimerIterations.getCount() > 0) {
                        for (int i = 0; i < packetsPerOneSending; i++) {
                            if (remainingTimerIterations.getCount() <= 0) {
                                break;
                            }

                            byte[] packet;
                            int packetNumber = numberOfPacketsToBeSent - (int) remainingTimerIterations.getCount();
                            double startTimeForPacket = (double) packetNumber / PACKETS_FOR_ONE_SEC_OF_SIGNAL;
                            short[] samples = SignalGenerator.getSamplesOfSineSignal(startTimeForPacket);

                            packet = SignalGenerator.putSamplesToPacket(samples);

                            DatagramPacket outgoing = new DatagramPacket(packet, PACKET_SIZE, inetAddress, SERVICE_PORT);
                            socket.send(outgoing);
                            //System.out.println(bytesToHex(packet));
                            remainingTimerIterations.countDown();

                        }
                    }
                }
                catch (IOException e) {
                    e.printStackTrace();
                }
                }
            }, 0, timerPeriod);
            try {
                remainingTimerIterations.await();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            timer.cancel();
        }
        catch (SocketException | SecurityException | UnknownHostException e) {
            e.printStackTrace();
        }
    }

    private static double doubleToIntDeviation(double doubleForm) {
        int roundedForm = (int)Math.round(doubleForm);
        double diff = Math.abs(roundedForm - doubleForm);
        return (diff / doubleForm) * 100;
    }

    private static byte[] generateMarkedAsLastPacket() {
        byte[] packet = new byte[PACKET_SIZE];
        byte[] values = {-128, 0, 127};
        for (int i = 0; i < PACKET_SIZE; i++) {
            packet[i ] = values[i % 3];
        }
        return packet;
    }

    private static final char[] HEX_ARRAY = "0123456789ABCDEF".toCharArray();
    public static String bytesToHex(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        for (int j = 0; j < bytes.length; j++) {
            int v = bytes[j] & 0xFF;
            hexChars[j * 2] = HEX_ARRAY[v >>> 4];
            hexChars[j * 2 + 1] = HEX_ARRAY[v & 0x0F];
        }
        return new String(hexChars);
    }
}
