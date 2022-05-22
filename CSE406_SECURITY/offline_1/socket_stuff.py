import socket

def send_bytes(sock : socket, b : bytes) -> None:
    sock.send(len(b).to_bytes(4, 'little'))
    sock.send(b)

def receive_bytes(sock : socket) -> bytes:
    l : int = int.from_bytes(sock.recv(4), 'little')
    return sock.recv(l)