package src.common;

public class TransferTimeoutPacket extends Packet {
    public int fileId;
    public TransferTimeoutPacket(String from, int fileId) {
        super(from, PacketType.TransferTimeout);
        this.fileId = fileId;
    }
}
