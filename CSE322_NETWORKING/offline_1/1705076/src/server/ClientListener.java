package src.server;

import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

import src.common.Logger;
import src.common.Packet;

public class ClientListener extends Thread {
    private Socket socket;
    public String id;
    private ObjectInputStream in;
    private ObjectOutputStream out;
    private Server server;

    public ClientListener(Socket socket, Server server){
        this.socket = socket;
        this.id = "";
        this.server = server;
        try{
            out = new ObjectOutputStream(socket.getOutputStream());
            in = new ObjectInputStream(socket.getInputStream());

            id = ((Packet) in.readObject()).from;
        } catch (Exception e) {
            Logger.getLogger().log("Could not get input/output streams in client listener");
            e.printStackTrace();
        }
    }


    public void sendPacket(Packet packet) {
        try {
            out.writeObject(packet);
        } catch (Exception e) {
            Logger.getLogger().log("Packet sending failed for "+id);
            e.printStackTrace();
        }
    }


    public void close() {
        try{
            this.socket.close();
            this.interrupt();
        } catch (Exception e) {
            Logger.getLogger().log("Could not close client connection or could not interrupt listener thread for "+id);
            e.printStackTrace();
        }
    }
    

    @Override
    public void run() {
        while(true){
            try {
                Packet packet = (Packet)in.readObject();
                Logger.getLogger().log("received packet from "+id);
                
                synchronized(server.messageBuffer){
                    server.messageBuffer.add(packet);
                    server.messageBuffer.notify();
                    Logger.getLogger().log("Sent packet to server from "+id);
                }
            } catch (Exception e) {
                Logger.getLogger().log("stopping listening thread of "+id);
                server.removeConnection(this);
                break;
            }
        }
    }
}
