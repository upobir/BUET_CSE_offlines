package src.common;

public class FileTransferRequestPacket extends Packet {
    public String filename;
    public int filesize;
    public boolean isPublic;
    public int requestId;
    public FileTransferRequestPacket(String from, String filename, int filesize, boolean isPublic, int requestId) {
        super(from, PacketType.FileTransferRequest);
        this.filename = filename;
        this.filesize = filesize;
        this.isPublic = isPublic;
        this.requestId = requestId;
    }
}
