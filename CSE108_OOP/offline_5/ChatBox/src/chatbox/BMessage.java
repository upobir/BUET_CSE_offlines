/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package chatbox;

import java.util.ArrayList;

/**
 *
 * @author Abir
 */
public class BMessage extends Message{
    public BMessage(DedicatedProgram caller, int id, String[] parts) {
        super(caller, id, parts);
    }
    
    String process()
    {
        WorkerThread client = caller.getClient(id);
        String ret = "Cannot execute command";
        if(!client.isLoggedIn() || !client.isAdmin()) return ret;
        
        String message = client.getUsername()+" : ";
        if(parts.length > 1) message = message+parts[1];
        
        ArrayList<WorkerThread> L = caller.getUserList();
        for(WorkerThread w: L){
            if(w == null || !w.isLoggedIn() || w.getUsername().equals(client.getUsername()))
                continue;
            w.sendMessage("#M(Broadcast) " + message);
        }
        ret = "Successfully Brodcasted";
        return ret;
    }
            
}
