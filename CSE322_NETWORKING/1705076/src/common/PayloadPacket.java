package src.common;

public class PayloadPacket extends Packet {
    public Object data;
    public PayloadPacket(String from, PacketType packetType, Object data){
        super(from, packetType);
        this.data = data;
    }
}
