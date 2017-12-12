package com.example.vikrant.battle_bot;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;

/**
 * Created by Varun on 12/05/2017.
 */

public class udp_client_class {

    static void send_msg(int type ,float val)  {
        int UDP_SERVER_PORT=1808;
        String udpMsg = String.valueOf(type)+":"+String.valueOf(val);

        DatagramSocket ds = null;
        try {
            ds = new DatagramSocket();
            String ipadd="192.168.1.64";
            InetAddress serverAddr = InetAddress.getByName(ipadd);
            DatagramPacket dp;
            dp = new DatagramPacket(udpMsg.getBytes(), udpMsg.length(), serverAddr, UDP_SERVER_PORT);
            ds.send(dp);
        } catch (SocketException e) {
            e.printStackTrace();
        }catch (UnknownHostException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (ds != null) {
                ds.close();
            }
        }
    }


    static void send_msgtoslave(int type ,float val)  {
        int UDP_SERVER_PORT=1809;
        String udpMsg = String.valueOf(type)+":"+String.valueOf(val);

        DatagramSocket ds = null;
        try {
            ds = new DatagramSocket();
            String ipadd="192.168.1.65";
            InetAddress serverAddr = InetAddress.getByName(ipadd);
            DatagramPacket dp;
            dp = new DatagramPacket(udpMsg.getBytes(), udpMsg.length(), serverAddr, UDP_SERVER_PORT);
            ds.send(dp);
        } catch (SocketException e) {
            e.printStackTrace();
        }catch (UnknownHostException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (ds != null) {
                ds.close();
            }
        }
    }

    static String recieve_health() throws SocketException {
        String text;
        int server_port = 2390;
        byte[] message = new byte[1500];
        DatagramPacket p = new DatagramPacket(message, message.length);
        DatagramSocket s = null;
        try {
            s = new DatagramSocket(server_port);
        } catch (SocketException e) {
            e.printStackTrace();
        }

        s.setSoTimeout(1000);   // set the timeout in millisecounds.
        while(true) {

            try {
                s.receive(p);
                if(p.getLength()>0)
                {
                    s.close();
                    return text = new String(message, 0, p.getLength());
                    //break;
                }
            } catch (SocketTimeoutException e) {
                // resend
                //s.send(p);
                s.close();
                break;
                //continue;
            } catch (IOException e) {
                e.printStackTrace();
            }
            // check received data...
        }

        /*while(true) {        // recieve data until timeout
            try {
                System.out.println("Receiving message...");
                s.receive(p); // receive the packet
                System.out.println("Message received");
                return text = new String(message, 0, p.getLength());
            }
            catch (SocketTimeoutException e) {
                // timeout exception.
                System.out.println("Timeout reached!!! " + e);
                s.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }*/


           // s.receive(p);

        //s.close();
        return text = "";
    }
}
