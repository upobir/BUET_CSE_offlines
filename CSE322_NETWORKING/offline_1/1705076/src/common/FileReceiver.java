package src.common;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;

import src.server.Server;

public class FileReceiver {
    private Server server;
    private int fileId;
    private int fileSize;
    private int received;
    public String from;
    private File file;
    private BufferedOutputStream out;
    private int requestId;


    public FileReceiver(Server server, String from, int fileId, int fileSize, int chunksize, File file, int requestId) {
        this.server = server;
        this.fileId = fileId;
        this.fileSize = fileSize;
        this.file = file;
        this.from = from;
        this.requestId = requestId;
        if(file.exists()){
            Logger.getLogger().log("WARNING:::overwriting file "+file.getPath());
        }
        this.received = 0;
        try {
            this.out = new BufferedOutputStream(new FileOutputStream(file));
        } catch (Exception e) {
            Logger.getLogger().log("Something wrong with setting outptustream");
            e.printStackTrace();
        }
        Logger.getLogger().log("Created file receiver for "+fileId);
    }


    public void putChunk(FileChunkPacket chunk) {
        try {
            synchronized(this){
                if(chunk.chunkId != received){
                    Logger.getLogger().log("ERROR:::chunk index does not match");
                    return;
                }

                out.write(chunk.data, 0, chunk.data.length);
                received++;

                // Logger.getLogger().log("received chunk "+chunk.chunkId+" and saving it");
                // if(received == 4){
                //     Logger.getLogger().log("Sabotage");
                //     return;
                // }

                server.connections.get(chunk.from).sendPacket(new FileChunkAcknowledgePacket("server", fileId, chunk.chunkId));
                Logger.getLogger().log("Sending ack for chunk "+chunk.chunkId);
            }
        } catch (Exception e) {
            Logger.getLogger().log("Something wrong with saving data");
            e.printStackTrace();
        }
    }


    public boolean finishReceiving(boolean forceDelete) {
        Logger.getLogger().log("Ending receiving of file "+fileId);
        try {
            out.close();
        } catch (Exception e) {
            Logger.getLogger().log("Something wrong happened with closing buffer");
            e.printStackTrace();
        }
        server.receivers.remove(fileId);
        if(forceDelete || file.length() != fileSize){
            Logger.getLogger().log("Something wrong happened, deleting "+file.getPath());
            file.delete();
            return false;
        }


        if(requestId != -1){
            server.fulfillRequest(requestId, from, file.getName());
        }

        return true;
    }
    
}
