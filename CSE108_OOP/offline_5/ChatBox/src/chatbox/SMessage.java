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
public class SMessage extends Message{
    public SMessage(DedicatedProgram caller, int id, String[] parts) {
        super(caller, id, parts);
    }
    
    String process()
    {
        WorkerThread client = caller.getClient(id);
        String ret = "Cannot execute Command";
        if(!client.isLoggedIn() || parts.length < 2) return ret;
        
        switch(parts[1])
        {
            case "show":
                ret = activeUsers();
                break;
            case "logout":
                ret = "CLOSE";
                System.out.print("Client "+id+" logging out");
                if(parts.length >= 3) System.out.println(": "+parts[2]);
                else System.out.println("");
                break;
        }
        
        return ret;
    }
    
    private String activeUsers()
    {
        String ret = "Active Users:";
        ArrayList<WorkerThread> L = caller.getUserList();
        
        for(WorkerThread w: L){
            ret = ret + " " + w.getUsername();
        }
        return ret;
    }
}
