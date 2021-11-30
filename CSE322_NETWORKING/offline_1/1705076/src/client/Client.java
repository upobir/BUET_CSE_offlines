package src.client;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Queue;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import src.common.BooleanPacket;
import src.common.DownloadRequestPacket;
import src.common.FileChunkAcknowledgePacket;
import src.common.FileChunkPacket;
import src.common.TransferCompletePacket;
import src.common.TransferCompleteResponsePacket;
import src.common.TransferTimeoutPacket;
import src.common.FileTransferRequestPacket;
import src.common.FileTransferResponsePacket;
import src.common.PayloadPacket;
import src.common.Request;
import src.common.Logger;
import src.common.Packet;
import src.common.PacketType;
import src.common.UserFilesRequestPacket;

public class Client extends Thread {
    private final int TIME_TO_WAIT = 5;

    public String id;
    private Socket socket;
    private ObjectInputStream in;
    private ObjectOutputStream out;
    public ArrayList<String> Messages;
    private Queue<Packet> messageBuffer = new LinkedList<>();


    Client(String id){
        this.id = new String(id);
        try{
            socket = new Socket("localhost", 1971);

            out = new ObjectOutputStream(socket.getOutputStream());
            in = new ObjectInputStream(socket.getInputStream());
            sendPacket(new Packet(this.id, PacketType.ConnectionRequest));
            
            BooleanPacket response = (BooleanPacket) in.readObject();
            if(response.success){
                Logger.getLogger().log("Connection was accepeted");
            }
            else{
                socket = null;
                Logger.getLogger().log("Connection was rejected");
            }
            
        } catch (Exception e) {
            Logger.getLogger().log("could not start socket or initiate messaging");
            e.printStackTrace();
        }
    }


    public boolean isOpen() {
        return socket != null;
    }


    private void sendPacket(Packet packet){
        try {
            out.writeObject(packet);
        } catch (Exception e) {
            Logger.getLogger().log("something went wrong with writing");
        }
    }   


    @Override
    public void run() {
        while(true){
            try {
                Packet packet = (Packet) in.readObject();
                Logger.getLogger().log("received a packet");
                synchronized(messageBuffer){
                    messageBuffer.add(packet);
                    messageBuffer.notify();
                }
            } catch (Exception e) {
                Logger.getLogger().log("Stopping listening thread");
                break;
            }
        }
    }


    public void shutdown() {
        Logger.getLogger().log("Shutting down");
        try{
            if(socket != null){
                this.interrupt();
                socket.close();
            }

        } catch (Exception e) {
            System.err.println("Could not close server");
            e.printStackTrace();
        }
    }


    private Packet readFromBuffer() {
        try {
            synchronized(messageBuffer){
                if(messageBuffer.isEmpty()){
                    messageBuffer.wait();
                }

                return messageBuffer.poll();
            }    
        } catch(InterruptedException e){
            Logger.getLogger().log("Interrupted");
        } catch (Exception e) {
            Logger.getLogger().log("read from buffer failed");
            e.printStackTrace();
        }
        return null;
    }


    public String[][] requestConnections() {
        Logger.getLogger().log("Sending list of user request packet");
        sendPacket(new Packet(id, PacketType.ListOfUserRequest));

        PayloadPacket packet = (PayloadPacket) readFromBuffer();
        Logger.getLogger().log("received list of user packet");
        return (String[][]) packet.data;
    }


    String[][] requestMyFileList() {
        Logger.getLogger().log("Sending list of own files request packet");
        sendPacket(new Packet(id, PacketType.ListOfOwnfilesRequest));

        PayloadPacket packet = (PayloadPacket) readFromBuffer();
        Logger.getLogger().log("received list of own files packet");
        return (String[][]) packet.data;
    }


    String[] requestOthersFileList(String userId) {
        Logger.getLogger().log("Sending list of files of " + userId + " request packet");
        sendPacket(new UserFilesRequestPacket(id, userId));

        PayloadPacket packet = (PayloadPacket) readFromBuffer();
        Logger.getLogger().log("received list of files of " + userId + " packet");
        return (String[]) packet.data;
    }


    void sendRequestForFile(String description) {
        Logger.getLogger().log("Sending Request for `"+description+"`");
        sendPacket(new PayloadPacket(id, PacketType.FileRequest, new Request(id, description)));
        return;
    }


    Request[] getUnreadMesssages() {
        Logger.getLogger().log("Sending Request for unread messages");
        sendPacket(new Packet(id, PacketType.UnreadMessageRequest));

        PayloadPacket packet = (PayloadPacket) readFromBuffer();
        Logger.getLogger().log("receieved unread messages");
        return (Request[]) packet.data;
    }


    void uploadFile(File file, boolean uploadPublic, int requestId) {
        if(!file.exists()){
            Logger.getLogger().log("file does not exist");
            System.out.println("File doesn not exists");
            return;
        }
        String filename = file.getName();
        int filesize = (int)file.length();
        Logger.getLogger().log("file to be uploaded, name: "+filename+" , size: "+filesize + " , public: "+uploadPublic+" , requestId: "+requestId);

        sendPacket(new FileTransferRequestPacket(id, filename, filesize, uploadPublic, requestId));
        Logger.getLogger().log("Sent file upload request");

        FileTransferResponsePacket packet = (FileTransferResponsePacket)readFromBuffer();
        if(packet.fileId == -1){
            Logger.getLogger().log("File upload request was rejected");
            System.out.println("File upload request was rejected");
            return;
        }
        Logger.getLogger().log("File upload request was accepted, fileId : "+packet.fileId+" , chunksize : "+packet.chunkSize);
        System.out.println("File upload chunk size = "+packet.chunkSize);
        System.out.println("uploading");

        try {
            int sent = 0;
            BufferedInputStream instream = new BufferedInputStream(new FileInputStream(file));
            int chunkId = 0;
            while(sent < filesize){

                // if(chunkId == 5){
                //     Logger.getLogger().log("sabotage");
                //     System.out.println("Stopping");
                //     shutdown();
                //     System.exit(-1);
                // }

                int sizetosend = Math.min(filesize-sent, packet.chunkSize);
                byte[] data = new byte[sizetosend];
                instream.read(data, 0, sizetosend);
                sendPacket(new FileChunkPacket(id, packet.fileId, data, chunkId));
                Logger.getLogger().log("Sent file chunk "+ chunkId + " of fileId "+packet.fileId);

                if(waitForAcknowledgement(chunkId)){
                    chunkId++;
                    sent += sizetosend;
                }
                else{
                    sendPacket(new TransferTimeoutPacket(id, packet.fileId));
                    Logger.getLogger().log("Sent timeout packet");
                    instream.close();
                    System.out.println("Upload timedout");
                    return;
                }

            }
            instream.close();

            Logger.getLogger().log("sending compleition message");
            sendPacket(new TransferCompletePacket(id, packet.fileId));

            TransferCompleteResponsePacket response = (TransferCompleteResponsePacket) readFromBuffer();
            Logger.getLogger().log("File "+filename+" upload is "+(response.success?"success":"failure"));
            System.out.println("Upload successful");
        } catch (Exception e) {
            Logger.getLogger().log("Something wrong with sending file");
            System.out.println("Upload failed");
            e.printStackTrace();
        }
    }


    private boolean waitForAcknowledgement(int chunkId) {

        ExecutorService executor = Executors.newCachedThreadPool();
        Callable<FileChunkAcknowledgePacket> task = new Callable<FileChunkAcknowledgePacket>() {
            public FileChunkAcknowledgePacket call() {
                return (FileChunkAcknowledgePacket)readFromBuffer();
            }
        };
        Future<FileChunkAcknowledgePacket> future = executor.submit(task);
        try {
            FileChunkAcknowledgePacket result = future.get(TIME_TO_WAIT, TimeUnit.SECONDS); 
            if(result.chunkId != chunkId){
                Logger.getLogger().log("ERROR::: "+result.chunkId+", "+chunkId+" chunkid's dont match");
            }
            else{
                Logger.getLogger().log("received acknowledgement for "+chunkId);
                return true;
            }
        } catch (TimeoutException ex) {
            Logger.getLogger().log("Timeout occured");
        } catch (InterruptedException e) {
            Logger.getLogger().log("Interrupt in timeout");
        } catch (ExecutionException e) {
            Logger.getLogger().log("Executor problem in timeout");
        } finally {
            Logger.getLogger().log("Cancelling");
            future.cancel(true);
            executor.shutdown();
        }
        return false;
    }


    void downloadFile(String targetId, String filename) {
        Logger.getLogger().log("Sent download request for "+targetId+", file: "+filename);
        sendPacket(new DownloadRequestPacket(id, targetId, filename));
        FileTransferResponsePacket response = (FileTransferResponsePacket) readFromBuffer();
        if(response.fileId == -1){
            Logger.getLogger().log("File download request denied");
            System.out.println("File download request denied");
            return;
        }
        Logger.getLogger().log("received file download permission");

        int chunkCount = (response.fileSize + response.chunkSize - 1)/response.chunkSize;

        try {
            File file = new File(filename);
            BufferedOutputStream outstream = new BufferedOutputStream(new FileOutputStream(file));
            for(int i = 0; i<chunkCount; i++){
                FileChunkPacket chunk = (FileChunkPacket) readFromBuffer();
                outstream.write(chunk.data, 0, chunk.data.length);
                Logger.getLogger().log("received chunk "+chunk.chunkId+" and saving it");
            }

            TransferCompletePacket finalpacket = (TransferCompletePacket) readFromBuffer();
            Logger.getLogger().log("Received download complete packet and saving file");
            outstream.close();
            System.out.println("Download complete");

        } catch (Exception e) {
            Logger.getLogger().log("something wrong with downloading file");
            System.out.println("Download failed");
        }
        
    }

}
