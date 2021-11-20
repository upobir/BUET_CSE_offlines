package chatclient;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.io.*; 
import java.net.*; 

/**
 *
 * @author Abir
 */
public class Client {
    
    private static boolean Working;
    private static PrintWriter serverOS;
    
    public static void stop()
    {
        Working = false;
    }
    
    public static void main(String args[]) throws Exception
    {
        Working = true;
        try{
            Socket clientSocket = new Socket("localhost", 420);
            ReceiverThread t = new ReceiverThread(clientSocket);
            Thread thread = new Thread(t);
            thread.start();
            BufferedReader userIS = new BufferedReader(new InputStreamReader(System.in));
            serverOS = new PrintWriter(clientSocket.getOutputStream(), true);

            while(Working){
                String input = userIS.readLine();
                sendLine(input);
            }
        }
        catch(Exception e){
            System.out.println("Cannot connect to server");
            return;
        }
    }
    
    public static void sendLine(String input)
    {
        serverOS.println(input);
        serverOS.flush();
    }
}

class ReceiverThread implements Runnable{
    private Socket connectionSocket;
        
    ReceiverThread(Socket connectionSocket){
        this.connectionSocket = connectionSocket;
    }
        
    public void run(){
        while(true){
            try{
                BufferedReader serverIS = new BufferedReader(new InputStreamReader(connectionSocket.getInputStream()));
                String result = serverIS.readLine();
                if(result.equals("CLOSE")) 
                {
                    System.out.println("Closing connection. Press enter to exit.");
                    connectionSocket.close();
                    Thread.currentThread().interrupt();
                    Client.stop();
                    return;
                }
                if(!result.isEmpty() && result.charAt(0) == '#'){
                    switch(result.charAt(1)){
                        case 'M':
                            System.out.println(result.substring(2));
                            break;
                        case 'L':
                            sendFile(result.substring(2));
                            break;
                        case 'F':
                            if(!getFile(result.substring(2))) System.out.println("File Transfer failed.");
                            else System.out.println("Received file.");
                            break;
                        default:
                            break;
                    }
                }
                else System.out.println("SERVER: "+ result);
            }
            catch(Exception e){
                try{
                    System.out.println("Disconnected from server. Press enter to exit.");
                    connectionSocket.close();
                    Thread.currentThread().interrupt();
                    Client.stop();
                    return;
                }   
                catch (IOException e1){
                }
            }
        }
    }
    
    void sendFile(String fileName) throws Exception{
        try{
            File file = new File(fileName);
            
            long fileLength;
            if(!file.isFile()) fileLength = -1;
            else fileLength = file.length();
            
            Client.sendLine(String.valueOf(fileLength));
            if(fileLength == -1) return;
            
            FileInputStream fis = new FileInputStream(file);
            BufferedInputStream bis = new BufferedInputStream(fis);
            OutputStream os = connectionSocket.getOutputStream();
            byte[] contents;

            long current = 0;

            while(current!=fileLength){ 
                    int size = 10000;
                    if(fileLength - current >= size)
                        current += size;    
                    else{ 
                        size = (int)(fileLength - current); 
                        current = fileLength;
                    } 
                    contents = new byte[size]; 
                    bis.read(contents, 0, size); 
                    os.write(contents);
                    //System.out.println("Sending file ... "+(current*100)/fileLength+"% complete!");
                }   
            os.flush();
        }
        catch (Exception e){
            Client.sendLine("-1");
        }
    }
    
    boolean getFile(String fileSpec){
        String[] spec = fileSpec.split("#");
        try
        {
            long filesize = Long.parseLong(spec[1]);
            if(filesize == -1){
                return false;
            }
            byte[] contents = new byte[10000];

            FileOutputStream fos = new FileOutputStream("received_"+spec[0]);
            BufferedOutputStream bos = new BufferedOutputStream(fos);
            InputStream is = connectionSocket.getInputStream();

            int bytesRead = 0; 
            int total=0;
            while(total!=filesize)
            {
                bytesRead=is.read(contents);
                total+=bytesRead;
                bos.write(contents, 0, bytesRead); 
            }
            bos.flush(); 
            return true;
        }
        catch(Exception e)
        {
            return false;
        }
    }
}