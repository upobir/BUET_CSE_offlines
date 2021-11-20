/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package chatbox;

import java.io.File;
import java.io.IOException;
import java.util.Scanner;

/**
 *
 * @author Abir
 */
public class LMessage extends Message{

    public LMessage(DedicatedProgram caller, int id, String[] parts) {
        super(caller, id, parts);
    }
    
    String process()
    {
        WorkerThread client = caller.getClient(id);
        if(client.isLoggedIn()) return "Already Logged In";
        String ret = "Login Failed";

        try{
            Scanner scn = new Scanner(new File("Credentials.txt"));
            while(scn.hasNext()){
                String username = scn.next();
                String password = scn.next();
                String type = scn.next();
                
                if(username.equals(parts[1]) && !caller.isUserLoggedIn(parts[1]))
                {
                    if(password.equals(parts[2]) && type.equals(parts[3]))
                    {
                        client.logIn(parts[1], parts[3]);
                        ret = "Login Successful";
                        break;
                    }
                }
            }
        }
        catch(IOException e){
            System.out.println("Couldn't open creds");
        }
        return ret;
    }
}
