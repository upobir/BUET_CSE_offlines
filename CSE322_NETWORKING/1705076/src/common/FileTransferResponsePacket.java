package src.common;

public class FileTransferResponsePacket extends Packet {
    public int chunkSize;
    public int fileId;
    public int fileSize;
    public FileTransferResponsePacket(String from, int chunkSize, int fileId, int fileSize){
        super(from, PacketType.FileTransferRequestPacket);
        this.chunkSize = chunkSize;
        this.fileId = fileId;
        this.fileSize = fileSize;
    }
}
