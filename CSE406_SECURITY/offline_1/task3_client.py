import socket
from socket_stuff import send_bytes, receive_bytes
from RSA import RSA
from AES import AES


if __name__ == "__main__":
    s = socket.socket()
    port = 12345
    s.connect(('127.0.0.1', port))

    n : int = int(receive_bytes(s).decode())
    e : int = int(receive_bytes(s).decode())

    encrypted_key : bytearray = bytearray(receive_bytes(s))

    ciphertext : bytearray = bytearray(receive_bytes(s))

    path = 'dont_open_this'

    with open(path + '/secret.txt', 'r') as f:
        d : int = int(f.read())

    rsa : RSA = RSA(128, n, e, d)
    decrypted_key : bytearray = rsa.decrypt(encrypted_key)

    aes : AES = AES(decrypted_key)
    plaintext : bytearray = aes.decrypt(ciphertext)


    with open(path+'/dpt.txt', 'w') as f:
        f.write(str(plaintext, 'latin-1'))

    send_bytes(s, 'ok'.encode())

    s.close()