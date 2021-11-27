package src.common;

public class FileChunkAcknowledgePacket extends Packet {
    public int fileId;
    public int chunkId;
    public FileChunkAcknowledgePacket(String from, int fileId, int chunkId){
        super(from, PacketType.FileChunkAcknowledgement);
        this.fileId = fileId;
        this.chunkId = chunkId;
    }
}
