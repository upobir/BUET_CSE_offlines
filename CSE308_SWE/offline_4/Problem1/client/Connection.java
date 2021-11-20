package client;

import java.net.Socket;
import java.io.*;
import java.util.Scanner;

public class Connection extends Thread {
    private Socket socket;
    private Scanner in;
    private PrintWriter out;
    private boolean running;
    private String name;

    @Override
    public void run() {
        running = true;

        println(this.name);

        while(running && in.hasNext()){
            System.out.println(in.nextLine());
        }

        System.out.println("disconnected from server");

        running = false;
    }

    public Connection(String name){
        try {
            this.socket = new Socket("localhost", 1971);
            this.in = new Scanner(new InputStreamReader(socket.getInputStream()));
            this.out = new PrintWriter(socket.getOutputStream());
            this.name = name;

            System.out.println("connected to server");

        } catch (Exception e) {
            System.err.println("Socket could not be created");
            System.exit(-1);
        }
    }

    public void println(String string) {
        if(running){
            out.println(string);
            out.flush();
        }
    }

    public void stopListening() {
        if(running){
            try{
                System.err.println("closing");
                out.flush();
                socket.close();
                running = false;
                this.interrupt();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
