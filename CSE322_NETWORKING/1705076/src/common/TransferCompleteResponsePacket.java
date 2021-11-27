package src.common;

public class TransferCompleteResponsePacket extends Packet {
    public boolean success;
    public TransferCompleteResponsePacket(String from, boolean success) {
        super(from, PacketType.TransferCompleteResponse);
        this.success = success;
    }
}
