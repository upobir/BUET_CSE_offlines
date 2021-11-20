package server.networking;

import java.net.Socket;
import java.io.*;
import java.util.Scanner;
import server.stock.User;

public class UserConnection extends Thread {
    private Socket socket;
    private boolean running;
    private User user;

    private Scanner in;
    private PrintWriter out;

    @Override
    public void run() {
        running = true;

        while(running && in.hasNext()){
            String command = in.next();
            String stockName = in.next();
            user.update(command, stockName);
        }
        running = false;
        user.stop();
    }

    public UserConnection(User user, Socket socket) {
        this.user = user;
        this.socket = socket;

        try {
            this.in = new Scanner(new InputStreamReader(socket.getInputStream()));
            this.out = new PrintWriter(socket.getOutputStream());

            String name = in.next();
            this.user.setName(name);
            
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void println(String string){
        if(running){
            out.println(string);
            out.flush();
        }
    }

    public void stopListening() {
        if(running){
            try{
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
