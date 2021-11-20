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
public class DedicatedProgram {
    private WorkerThread[] clients;
    
    DedicatedProgram(){
        clients = new WorkerThread[50];
    }
    
    void addClient(WorkerThread clientThread){
        clients[clientThread.getID()] = clientThread;
    }
    
    WorkerThread getClient(int id){
        return clients[id];
    }
    
    void removeClient(WorkerThread clientThread){
        clients[clientThread.getID()] = null;
    }
    
    boolean isUserLoggedIn(String name){
        for(WorkerThread w: clients){
            if(w == null) continue;
            if(name.equals(w.getUsername())) return true;
        }
        return false;
    }
    
    ArrayList<WorkerThread> getUserList()
    {
        ArrayList<WorkerThread> L = new ArrayList<WorkerThread>();
        for(WorkerThread w: clients){
            if(w == null || !w.isLoggedIn()) continue;
            L.add(w);
        }
        return L;
    }
    
    String process(String message, int id){
        String[] parts = message.split("#");
        for(int i = 0; i<parts.length; i++) parts[i] = parts[i].trim();
        
        //System.out.println("Type "+ parts[0]);
        
        String ret = "Invalid Command";
        if(parts.length == 0) return ret;
        Message m = null;
        switch(parts[0]){
            case "L":
                m = new LMessage(this, id, parts);
                break;
            case "S":
                m = new SMessage(this, id, parts);
                break;
            case "B":
                m = new BMessage(this, id, parts);
                break;
            case "C":
                m = new CMessage(this, id, parts);
                break;
            default:
                return ret;            
        }
        ret = m.process();
        return ret;
    }    
    
    
}
