package src.server;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Random;

import src.common.BooleanPacket;
import src.common.DownloadRequestPacket;
import src.common.FileChunkPacket;
import src.common.FileReceiver;
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

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;

public class Server extends Thread {
    private final int MAX_BUFFER_SIZE = 100_000_000;//5_000_000;
    private final int MIN_CHUNK_SIZE = 5000;
    private final int MAX_CHUNK_SIZE = 6000;

    private HashSet<String> allConnections = new HashSet<>();
    public Queue<Packet> messageBuffer = new LinkedList<>();
    private ServerSocket serverSocket;
    public HashMap<String, ClientListener> connections = new HashMap<>();
    private HashMap<String, Queue<Request>> requests = new HashMap<>();
    private int fileIdCount = 0;
    private Integer bufferSize = 0;
    private Random random = new Random();
    public HashMap<Integer, FileReceiver> receivers = new HashMap<>();
    private ArrayList<Request> allRequests = new ArrayList<>();


    public Server(){
        try{
            serverSocket = new ServerSocket(1971);
            Logger.getLogger().log("Started server");
            createFolder("data/");
        } catch (Exception e) {
            Logger.getLogger().log("could not start server");
            e.printStackTrace();
        }
    }


    public void shutdown() {
        for(ClientListener s: connections.values()){
            s.close();
        }

        try{
            serverSocket.close();
        } catch (Exception e) {
            Logger.getLogger().log("Something wrong with stopping server");
            e.printStackTrace();
        }
    }


    public void removeConnection(ClientListener listener) {
        if(connections.containsKey(listener.id)){
            connections.remove(listener.id);
            Logger.getLogger().log("Removing client listener of "+listener.id);
        }

        for(Integer id: receivers.keySet()){
            if(receivers.get(id).from.equals(listener.id)){
                receivers.get(id).finishReceiving(true);
            }
        }

        listener.close();
    }


    @Override
    public void run() {
        Logger.getLogger().log("running server");
        while(true){
            try {
                Socket socket = serverSocket.accept();
                ClientListener listener = new ClientListener(socket, this);
                handleConnectionRequest(listener);

            } catch (SocketException e) {
                Logger.getLogger().log("stopping socketserver thread");
                break;
            } catch (Exception e) {
                Logger.getLogger().log("Something wrong with socket accepting");
                e.printStackTrace();
            }
        }
    }


    private void handleConnectionRequest(ClientListener listener) {
        Logger.getLogger().log("new conenction request "+ listener.id);
        if(connections.containsKey(listener.id)){
            listener.sendPacket(new BooleanPacket("server", false));
            Logger.getLogger().log("Rejected connection from " + listener.id);
        }
        else{
            listener.sendPacket(new BooleanPacket("server", true));
            connections.put(listener.id, listener);
            listener.start();
            Logger.getLogger().log("accepted connection from " + listener.id);

            initiateUser(listener.id);
        }
    }


    private void initiateUser(String id) {
        if(allConnections.contains(id)){
            Logger.getLogger().log("User "+id+ " is a old user");
        }
        else{
            Logger.getLogger().log("User "+id+" is a new user");
            createFolder("data/"+id);
            createFolder("data/"+id+"/public");
            createFolder("data/"+id+"/private");
            allConnections.add(id);
            requests.put(id, new LinkedList<>());
            for(Request r : allRequests)
                requests.get(id).add(r);
        }
    }


    private void createFolder(String path) {
        File folder = new File(path);
        if(folder.exists()){
            Logger.getLogger().log("`"+path+"` already exists");
        }
        else{
            if(folder.mkdir()){
                Logger.getLogger().log("created `"+path+"`");
            }
            else{
                Logger.getLogger().log("`"+path+"` creation failed");
            }
        }
    }


    public void handlePacket(Packet packet) {
        switch(packet.packetType){
            case ListOfUserRequest:
                Logger.getLogger().log("received request for user list from "+packet.from);
                connections.get(packet.from).sendPacket(new PayloadPacket("server", PacketType.ListResponse ,connectionsList()));
                Logger.getLogger().log("sent list of user to "+packet.from);
            break;

            case ListOfOwnfilesRequest:
                Logger.getLogger().log("received request of own file list from "+packet.from);
                connections.get(packet.from).sendPacket(new PayloadPacket("server", PacketType.ListResponse, userFileslist(packet.from)));
                Logger.getLogger().log("sent list of all files to "+packet.from);
            break;

            case ListOfUserFilesRequest: {
                UserFilesRequestPacket received = (UserFilesRequestPacket)packet;
                Logger.getLogger().log("received request of files of "+received.user + " from "+received.from);
                connections.get(packet.from).sendPacket(new PayloadPacket("server", PacketType.ListResponse, userFileslist(received.user)[0]));
                Logger.getLogger().log("sent list of public files of " + received.user + " to " + received.from);
            } break;

            case FileRequest: {
                PayloadPacket received = (PayloadPacket) packet;
                Request request = (Request) received.data;
                Logger.getLogger().log("received file request for `"+request.description+"` from "+request.from);
                request.id = allRequests.size();
                allRequests.add(request);

                for(String id: allConnections){
                    if(!id.equals(request.from))
                        requests.get(id).add(request);
                }
                Logger.getLogger().log("added request "+request.id);

            } break;

            case UnreadMessageRequest: {
                Logger.getLogger().log("received request for unread messages of user " +packet.from);
                connections.get(packet.from).sendPacket(new PayloadPacket("server", PacketType.ListResponse, getUnreadMessages(packet.from)));
                Logger.getLogger().log("sent unread messages to "+packet.from);
            } break;

            case FileTransferRequest: {
                FileTransferRequestPacket receieved = (FileTransferRequestPacket) packet;
                Logger.getLogger().log("received file upload request, name: "+receieved.filename+" , size: "+receieved.filesize + " , public: "+receieved.isPublic+" , requestId: "+receieved.requestId);

                handleFileUpload(receieved);
            } break;


            case FileChunk: {
                FileChunkPacket chunk = (FileChunkPacket) packet;
                if(!receivers.containsKey(chunk.fileId)){
                    Logger.getLogger().log("received unknown chunk, fileId: "+chunk.fileId+" , chunkid: "+chunk.chunkId);
                }
                else{
                    receivers.get(chunk.fileId).putChunk(chunk);
                }
            } break;

            case FileTrasferComplete: {
                TransferCompletePacket received = (TransferCompletePacket) packet;
                if(!receivers.containsKey(received.fileId)){
                    Logger.getLogger().log("received unknown completion packet, fileId: "+received.fileId);
                }
                else{
                    Logger.getLogger().log("received completion packet for fileId "+received.fileId);
                    boolean success = receivers.get(received.fileId).finishReceiving(false);
                    Logger.getLogger().log("Sending transfer "+(success? "success" : "failure")+" packet");
                    connections.get(packet.from).sendPacket(new TransferCompleteResponsePacket("server", success));
                }
            } break;

            case TransferTimeout: {
                TransferTimeoutPacket received = (TransferTimeoutPacket) packet;
                if(!receivers.containsKey(received.fileId)){
                    Logger.getLogger().log("received unknown timeout packet, fileId: "+received.fileId);
                }
                else{
                    Logger.getLogger().log("received timeout packet for fileId "+received.fileId);
                    receivers.get(received.fileId).finishReceiving(true);
                }
            } break;

            case DownloadRequest: {
                DownloadRequestPacket received = (DownloadRequestPacket) packet;
                Logger.getLogger().log("Received download request for "+received.filename+" of "+received.user+" from "+received.from);
                sendFile(received.filename, received.user, received.from);
            } break;

            default:
                Logger.getLogger().log("unknown packet");
            break;
        }
    }


    private void sendFile(String filename, String from, String to) {
        File file = new File("data/"+from+"/public/"+filename);
        if(file.exists()){
            Logger.getLogger().log("Found file `"+file.getPath()+"`");
        }
        else if(from.equals(to)){
            file = new File("data/"+from+"/private/"+filename);
            if(file.exists()){
                Logger.getLogger().log("Found file `"+file.getPath()+"`");
            }
            else{
                Logger.getLogger().log("cannot send the file, sending negative response");
                connections.get(to).sendPacket(new FileTransferResponsePacket("server", -1, -1, -1));
                return;
            }
        }
        else{
            Logger.getLogger().log("cannot send the file, sending negative response");
            connections.get(to).sendPacket(new FileTransferResponsePacket("server", -1, -1, -1));
            return;
        }

        int filesize = (int) file.length();
        int fileId = fileIdCount++;
        connections.get(to).sendPacket(new FileTransferResponsePacket("server", MAX_CHUNK_SIZE, fileId, filesize));
        Logger.getLogger().log("sent permission to dwonload "+filename+" of " + from+" , fileid: "+fileId);

        try {
            int sent = 0;
            BufferedInputStream instream = new BufferedInputStream(new FileInputStream(file));
            int chunkid = 0;
            while(sent < filesize){
                int sizetosend = Math.min(filesize-sent, MAX_CHUNK_SIZE);
                byte[] data = new byte[sizetosend];
                instream.read(data, 0, sizetosend);
                connections.get(to).sendPacket(new FileChunkPacket("server", fileId, data, chunkid));
                Logger.getLogger().log("Sent file chunk "+ chunkid + " of fileId "+fileId);
                sent += sizetosend;
                chunkid++;
            }
            instream.close();

            connections.get(to).sendPacket(new TransferCompletePacket("server", fileId));
            Logger.getLogger().log("Sent transfer complete packet for "+fileId);


        } catch (Exception e) {
            Logger.getLogger().log("Somethng wrong with sending file");
            e.printStackTrace();
        }
        

    }


    private Request[] getUnreadMessages(String id) {
        ArrayList<Request> unread = new ArrayList<>();
        synchronized(this){
            while(!requests.get(id).isEmpty())
                unread.add(requests.get(id).poll());
        }

        Request[] data = new Request[unread.size()];
        data = unread.toArray(data);
        return data;
    }


    private String[][] userFileslist(String id) {
        if(!allConnections.contains(id)){
            return new String[2][0];
        }

        String[][] result = new String[2][];

        ArrayList<String> list = new ArrayList<>();
        File publicfolder = new File("data/"+id+"/public");
        for(File file: publicfolder.listFiles()){
            list.add(file.getName());
        }
        result[0] = new String[list.size()];
        result[0] = list.toArray(result[0]);

        list.clear();
        File privatefolder = new File("data/"+id+"/private");
        for(File file: privatefolder.listFiles()){
            list.add(file.getName());
        }
        result[1] = new String[list.size()];
        result[1] = list.toArray(result[1]);

        return result;
    }


    private String[][] connectionsList() {
        String[][] result = new String[2][];

        ArrayList<String> list = new ArrayList<>();
        for(String id: connections.keySet()){
            list.add(id);
        }
        result[0] = new String[list.size()];
        result[0] = list.toArray(result[0]);

        list.clear();
        for(String id: allConnections){
            if(!connections.containsKey(id))
                list.add(id);
        }
        result[1] = new String[list.size()];
        result[1] = list.toArray(result[1]);

        return result;
    }


    public Packet readFromBuffer() {
        try {
            synchronized(messageBuffer){
                if(messageBuffer.isEmpty())
                    messageBuffer.wait();
                return messageBuffer.poll();
            }
        } catch (Exception e) {
            Logger.getLogger().log("read from buffer failed");
            e.printStackTrace();
        }
        return null;
    }


    private void handleFileUpload(FileTransferRequestPacket packet) {
        synchronized(bufferSize){
            if(bufferSize + packet.filesize > MAX_BUFFER_SIZE){
                Logger.getLogger().log("cannot accept file `"+packet.filename+"`, current buffersize : "+bufferSize);
                connections.get(packet.from).sendPacket(new FileTransferResponsePacket("server", -1, -1, -1));
                Logger.getLogger().log("sent upload rejection to "+packet.from);
                return;
            }

            bufferSize += packet.filesize;
            Logger.getLogger().log("file upload possible, new buffersize will be : "+bufferSize);
        }

        int chunkSize = random.nextInt(MAX_CHUNK_SIZE-MIN_CHUNK_SIZE+1)+MIN_CHUNK_SIZE;
        int fileId = fileIdCount++;

        File file = new File("data/" + packet.from + "/" +(packet.isPublic? "public" : "private") + "/" + packet.filename);
        receivers.put(fileId, new FileReceiver(this, packet.from, fileId, packet.filesize, chunkSize, file, packet.requestId));

        connections.get(packet.from).sendPacket(new FileTransferResponsePacket("server", chunkSize, fileId, packet.filesize));
        Logger.getLogger().log("File upload permission sent, fileId: "+fileId+" , chunksize: "+chunkSize);

        return;
    }


    public void fulfillRequest(int requestId, String from, String filename) {
        String id = allRequests.get(requestId).from;
        requests.get(id).add(new Request(from, "request "+requestId+" fulfilled with "+filename));
    }
}
