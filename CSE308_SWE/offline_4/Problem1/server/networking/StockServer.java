package server.networking;

import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.util.ArrayList;

import server.stock.User;

public class StockServer extends Thread {
    ArrayList<UserConnection> connections;

    @Override
    public void run() {
        while(true){
            try {
                Socket socket = serverSocket.accept();
                
                User user = new User(socket);

                connections.add(user.getConnection());

            } catch (SocketException e) {
                break;
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        
    }

    public void stopServer() {
        for (UserConnection connection : connections){
            connection.stopListening();
        }
        
        try{
            serverSocket.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private ServerSocket serverSocket;

    public StockServer(){
        this.connections = new ArrayList<>();

        try{
            serverSocket = new ServerSocket(1971);
        } catch (Exception e) {
            System.err.println("could not start server socket");
            e.printStackTrace();
        }
    }
    
}
