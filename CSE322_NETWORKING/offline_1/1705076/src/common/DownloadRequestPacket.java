package src.common;

public class DownloadRequestPacket extends Packet {
    public String user;
    public String filename;
    public DownloadRequestPacket(String from, String user, String filename){
        super(from, PacketType.DownloadRequest);
        this.user = user;
        this.filename = filename;
    }
}
