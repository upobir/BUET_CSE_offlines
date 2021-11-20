/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package chatbox;

import java.io.BufferedInputStream;
import java.io.OutputStream;
import java.util.ArrayList;

/**
 *
 * @author Abir
 */
public class CMessage extends Message{
    public CMessage(DedicatedProgram caller, int id, String[] parts) {
        super(caller, id, parts);
    }
    
    String process()
    {
        WorkerThread client = caller.getClient(id);
        String ret = "Cannot execute Command";
        if(!client.isLoggedIn() || parts.length < 3) return ret;
        
        if(!caller.isUserLoggedIn(parts[1])) return ret = parts[1] + " not logged in";
        
        ArrayList<WorkerThread> L = caller.getUserList();
        for(WorkerThread w: L){
            if(w != null && w.isLoggedIn() && w.getUsername().equals(parts[1])){
                w.sendMessage("#M(Message) " + client.getUsername() + ": " + parts[2]);
                ret = "message sent.";
                
                if(parts.length >= 4){
                    if(sendFile(client, w, parts[3])) ret += "\nfile sent.";
                    else ret += "\nfile could not be sent.";
                }
                
                break;
            }
        }
        
        return ret;
    }
    
    private boolean sendFile(WorkerThread sender, WorkerThread receiver, String fileName){
        try
        {
            sender.sendMessage("#L" + fileName);
            long fileLength = Long.parseLong(sender.getMessage());
            if(fileLength == -1){
                receiver.sendMessage("#F"+fileName+"#"+String.valueOf(fileLength));
                return false;
            }
            
            BufferedInputStream bis = new BufferedInputStream(sender.getSocket().getInputStream());
            OutputStream os = receiver.getSocket().getOutputStream();
            byte[] contents;
            
            receiver.sendMessage("#F"+fileName+"#"+String.valueOf(fileLength));

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
            return true;
        }
        catch(Exception e)
        {
            return false;
        }
    }
}
