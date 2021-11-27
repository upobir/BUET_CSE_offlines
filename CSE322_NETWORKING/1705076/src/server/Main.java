package src.server;

public class Main {
    public static void main(String[] args) {
        Server server = new Server();
        server.start();

        while(true){
            server.handlePacket(server.readFromBuffer());
        }

        // server.shutdown();
    }
}