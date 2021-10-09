package com.company;

import java.io.IOException;
import java.net.*;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.CountDownLatch;

public class UDPSender {
    public final static int SERVICE_PORT = 50001;
    public static final int PACKET_SIZE = 128;

    public static void send(int signalDuration, int timerPeriod) {

        final int numberOfPacketsToBeSent = SignalGenerator.SAMPLING_RATE / (PACKET_SIZE / 2) * signalDuration;
        final CountDownLatch latch =
                new CountDownLatch(SignalGenerator.SAMPLING_RATE / (PACKET_SIZE / 2) * signalDuration + 1);

        try (DatagramSocket socket = new DatagramSocket()) {
            InetAddress inetAddress = InetAddress.getByName("localhost");

            Timer timer = new Timer();
            timer.scheduleAtFixedRate(new TimerTask() {
                @Override
                public void run() {
                    try {
                        if (latch.getCount() > 0) {
                            int numberOfPacket = numberOfPacketsToBeSent - (int)latch.getCount();
                            byte[] packet = SignalGenerator.putSamplesToPacket(
                                    SignalGenerator.getSamplesOfSineSignal(
                                            numberOfPacket /
                                                    (double)(SignalGenerator.SAMPLING_RATE / (PACKET_SIZE / 2))));

                            if (latch.getCount() == 1) {
                                byte[] values = {-128, 0, 127};
                                for (int i = 0; i < PACKET_SIZE; i++) {
                                    packet[i ] = values[i % 3];
                                }
                            }
                            DatagramPacket outgoing = new DatagramPacket(packet, PACKET_SIZE, inetAddress, SERVICE_PORT);
                            socket.send(outgoing);
                            //System.out.println("packet sent!");
                            System.out.println(bytesToHex(packet));
                            latch.countDown();
                        }
                    }
                    catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }, 0, timerPeriod);

            try {
                latch.await();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            timer.cancel();
        }
        catch (SocketException | SecurityException | UnknownHostException e) {
            e.printStackTrace();
        }
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
