package src.common;

public class UserFilesRequestPacket extends Packet {
    public String user;
    public UserFilesRequestPacket(String from, String user){
        super(from, PacketType.ListOfUserFilesRequest);
        this.user = user;
    }
}
