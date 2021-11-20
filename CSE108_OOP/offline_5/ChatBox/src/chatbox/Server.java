/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package chatbox;

import java.io.*;
import java.net.*;
import java.util.*;


/**
 *
 * @author Abir
 */
public class Server {
    static DedicatedProgram program;
    public static void main(String args[]) throws Exception{
        program = new DedicatedProgram();
        int ThreadCount = 0;
        ServerSocket welcomeSocket = new ServerSocket(420);
        int id = 1;
        while(true)
        {
            Socket connectionSocket = welcomeSocket.accept();
            WorkerThread client = new WorkerThread(connectionSocket, id);
            Thread thread = new Thread(client);
            thread.start();
            ThreadCount++;
            System.out.println("Client "+id+" is connected");
            id++;
        }
    }
}

class WorkerThread implements Runnable{
    private Socket connectionSocket;
    private int id;
    private boolean loggedIn = false;
    private String username;
    private String Type;
    private BufferedReader clientIS;
    
    public String getUsername() {
        return username;
    }
    
    Socket getSocket(){
        return connectionSocket;
    }
    
    public boolean isAdmin(){
        return Type.equals("admin");
    }
    
    public WorkerThread(Socket socket, int id) throws Exception{
        this.connectionSocket = socket;
        this.clientIS = new BufferedReader(new InputStreamReader(connectionSocket.getInputStream()));
        this.id = id;
        Server.program.addClient(this);
    }
    
    boolean isLoggedIn(){
        return loggedIn;
    }
    
    int getID(){
        return id;
    }
    
    void logIn(String name, String Type){
        this.loggedIn = true;
        this.username = name;
        this.Type = Type;
    }
    
    
    void sendMessage(String m){
        try{
            PrintWriter clientOS = new PrintWriter(connectionSocket.getOutputStream());
            clientOS.println(m);
            clientOS.flush();
        }
        catch(Exception e){
            
        }
    }
    
    String getMessage() throws Exception{
        return clientIS.readLine();
    }
    
    public void run(){
        
        while(true){
            try{
                PrintWriter clientOS = new PrintWriter(connectionSocket.getOutputStream());
                String input = getMessage();
                //if(input != null) System.out.println("Client "+id+" says: "+input);
                String output = Server.program.process(input, id);
                clientOS.println(output);
                clientOS.flush();
            }
            catch(Exception e){
                try{
                    connectionSocket.close();
                    System.out.println("Client "+id+" is diconnected");
                    Server.program.removeClient(this);
                    Thread.currentThread().interrupt();
                }
                catch (IOException e1){
                }
                
                return;
            }
        }
    }
}
