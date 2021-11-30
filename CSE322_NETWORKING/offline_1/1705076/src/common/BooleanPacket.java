package src.common;

public class BooleanPacket extends Packet {
    public boolean success;
    public BooleanPacket(String from, boolean success){
        super(from, PacketType.BooleanResponse);
        this.success = success;
    }
}
