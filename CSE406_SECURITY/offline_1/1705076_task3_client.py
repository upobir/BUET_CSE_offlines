import socket
import importlib
socket_stuff = importlib.import_module('1705076_socket_stuff')
RSA = importlib.import_module('1705076_RSA')
AES = importlib.import_module('1705076_AES')


if __name__ == "__main__":
    s = socket.socket()
    port = 12345
    s.connect(('127.0.0.1', port))
    print("connected to server")

    # get public key
    n : int = int(socket_stuff.receive_bytes(s).decode())
    e : int = int(socket_stuff.receive_bytes(s).decode())
    print("RSA public key received")

    # get RSA encrypted key
    encrypted_key : bytearray = bytearray(socket_stuff.receive_bytes(s))
    print("RSA encrypted key received")

    # get AES cipher text
    ciphertext : bytearray = bytearray(socket_stuff.receive_bytes(s))
    print("AES cipher text received")

    path = 'dont_open_this'

    with open(path + '/secret.txt', 'r') as f:
        d : int = int(f.read())

    rsa : RSA.RSA = RSA.RSA(128, n, e, d)
    decrypted_key : bytearray = rsa.decrypt(encrypted_key)

    aes : AES.AES = AES.AES(decrypted_key)
    plaintext : bytearray = aes.decrypt(ciphertext)

    print()
    print("decrypted plaintext:")
    print(str(plaintext, 'latin-1'))
    print()

    with open(path+'/dpt.txt', 'w') as f:
        f.write(str(plaintext, 'latin-1'))

    print("decrypting finished")
    socket_stuff.send_bytes(s, 'ok'.encode())

    s.close()