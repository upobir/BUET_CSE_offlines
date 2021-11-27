package src.client;

import java.io.File;
import java.util.Scanner;

import src.common.Logger;
import src.common.Request;

public interface Main {
    public static void main(String[] args) {
        Logger.getLogger().flip();

        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter ID: ");
        String id = scanner.next();

        Client client = new Client(id);
        if(client.isOpen()){
            System.out.println("connected to server as " + client.id);
            client.start();
        }
        else{
            System.out.println("Server rejected connection");
            client.shutdown();
            scanner.close();
            return;
        }

        mainloop: while(true){
            System.out.println();
            System.out.println("Options:");
            System.out.println("(L) list of connected students");
            System.out.println("(M) see my files");
            System.out.println("(O) see others files");
            System.out.println("(R) request file");
            System.out.println("(S) show unread messages");
            System.out.println("(U) upload file");
            System.out.println("(D) download file");
            System.out.println("(E) exit");
            System.out.print("> ");

            String command = scanner.next();
            scanner.nextLine();
            switch(command.toUpperCase()){
                case "L":{
                    String[][] lists = client.requestConnections();
                    System.out.println("Online:");
                    for(String connectionId: lists[0]){
                        System.out.println(connectionId);
                    }
                    System.out.println("Offline:");
                    for(String connectionId: lists[1]){
                        System.out.println(connectionId);
                    }
                } break;

                case "M":{
                    String[][] lists = client.requestMyFileList();
                    System.out.println("Public files:");
                    for(String filename: lists[0]){
                        System.out.println(filename);
                    }
                    System.out.println("Private files:");
                    for(String filename: lists[1]){
                        System.out.println(filename);
                    }
                } break;

                case "O":{
                    System.out.print("Enter user id: ");
                    String userId = scanner.next();
                    scanner.nextLine();
                    String[] list = client.requestOthersFileList(userId);
                    System.out.println("Public files of "+userId+" :");
                    for(String filename: list){
                        System.out.println(filename);
                    }
                } break;

                case "R":{
                    System.out.print("Enter file description: ");
                    String description = scanner.nextLine();
                    client.sendRequestForFile(description);
                } break;

                case "S":{
                    System.out.println("Messages:");
                    Request[] list = client.getUnreadMesssages();
                    for(Request request : list){
                        if(request.id == -1){
                            System.out.println("from "+request.from + " : "+request.description);
                        }
                        else{
                            System.out.println("Request "+request.id + " : "+request.from+ " requested for "+request.description);
                        }
                    }
                } break;

                case "U":{
                    System.out.print("Enter path to file: ");
                    File file = new File(scanner.next());
                    System.out.print("Upload public (y/n)? ");
                    boolean uploadPublic = scanner.next().toUpperCase().equals("Y");
                    int requestId = -1;
                    if(uploadPublic){
                        System.out.print("Is the upload in response to some request (y/n)? ");
                        if(scanner.next().toUpperCase().equals("Y")){
                            System.out.print("Enter request id: ");
                            requestId = scanner.nextInt();
                        }
                    }
                    client.uploadFile(file, uploadPublic, requestId);
                    scanner.nextLine();
                } break;

                case "D": {
                    System.out.println("Enter userId whose file you want to download: ");
                    String targetId = scanner.next();
                    System.out.println("Enter filename: ");
                    String filename = scanner.next();
                    scanner.nextLine();
                    client.downloadFile(targetId, filename);
                } break;

                case "E":
                    break mainloop;

                default:
                    System.out.println("Invalid command:");
                    break;
            }
        }

        client.shutdown();
        Logger.getLogger().log("here");
        scanner.close();
    }
}
