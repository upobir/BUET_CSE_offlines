package src.common;

public class FileChunkPacket extends Packet {
    public int fileId;
    public byte[] data;
    public int chunkId;

    public FileChunkPacket(String from, int fileId, byte[] data, int chunkId){
        super(from, PacketType.FileChunk);
        this.fileId = fileId;
        this.data = data;
        this.chunkId = chunkId;
    }
}
