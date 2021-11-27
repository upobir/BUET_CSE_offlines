package src.common;

import java.io.Serializable;

public class Packet implements Serializable {
    public PacketType packetType;
    public String from;
    public Packet(String from, PacketType packetType){
        this.from = from;
        this.packetType = packetType;
    }
}

