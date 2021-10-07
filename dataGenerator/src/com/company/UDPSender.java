package com.company;

import java.io.IOException;
import java.net.*;
import java.util.Random;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.CountDownLatch;

public class UDPSender {
    public final static int SERVICE_PORT = 50001;
    public static final int BUFFER_SIZE = 64;

    public static void send(int workingTimeInMillis, double speedInKBS) {
        double period = ((BUFFER_SIZE / 1024.0) / speedInKBS) * 1000;
        System.out.println(period);
        final CountDownLatch latch = new CountDownLatch((int)(workingTimeInMillis / period));
        try (DatagramSocket socket = new DatagramSocket()) {
            byte[] data = new byte[BUFFER_SIZE];
            InetAddress inetAddress = InetAddress.getByName("localhost");

            Timer timer = new Timer();
            timer.scheduleAtFixedRate(new TimerTask() {
                @Override
                public void run() {
                    try {
                        if (latch.getCount() > 0) {
                            generateData(data);
                            if (latch.getCount() == 1) {
                                for (int i = 0; i < 25; i++) {
                                    data[i] = 127;
                                }
                            }
                            DatagramPacket outgoing = new DatagramPacket(data, BUFFER_SIZE, inetAddress, SERVICE_PORT);
                            socket.send(outgoing);
                            System.out.println("packet sent!");
                            System.out.println(bytesToHex(data));
                            latch.countDown();
                        }
                    }
                    catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }, 0, (int)period);

            try {
                latch.await();
                System.out.println("Speed = " + 1000.0 / (int)period * BUFFER_SIZE / 1024.0 + " KB/s");
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            timer.cancel();
        }
        catch (SocketException | SecurityException | UnknownHostException e) {
            e.printStackTrace();
        }
    }

    static void generateData(byte[] array) {
        Random random = new Random();
        for (int i = 0; i < array.length; i++) {
            array[i] = (byte) random.nextInt(128);
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
