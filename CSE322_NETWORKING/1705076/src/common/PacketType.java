package src.common;

public enum PacketType {
    ConnectionRequest,
    BooleanResponse,
    ListOfUserRequest,
    ListResponse, 
    ListOfOwnfilesRequest, 
    ListOfUserFilesRequest,
    FileRequest,
    UnreadMessageRequest, 
    FileTransferRequest, 
    FileTransferRequestPacket, 
    FileChunk, 
    FileChunkAcknowledgement,
    FileTrasferComplete, 
    TransferCompleteResponse, 
    TransferTimeout, 
    DownloadRequest
}
