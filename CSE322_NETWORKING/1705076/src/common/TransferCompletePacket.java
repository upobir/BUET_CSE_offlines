package src.common;

public class TransferCompletePacket extends Packet{
    public int fileId;
    public TransferCompletePacket(String from, int fileId){
        super(from, PacketType.FileTrasferComplete);
        this.fileId = fileId;
    }
}
