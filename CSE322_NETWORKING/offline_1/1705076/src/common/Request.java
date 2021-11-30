package src.common;

import java.io.Serializable;

public class Request implements Serializable{
    public String from;
    public String description;
    public int id = -1;

    public Request(String from, String description){
        this.from = from;
        this.description = description;
    }
}
