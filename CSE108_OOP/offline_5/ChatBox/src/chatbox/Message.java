/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package chatbox;

/**
 *
 * @author Abir
 */
public abstract class Message {
    String[] parts;
    int id;
    DedicatedProgram caller;

    public Message(DedicatedProgram caller, int id, String[] parts) {
        this.caller = caller;
        this.id = id;
        this.parts = parts;
    }
    
    abstract String process();
    
}
