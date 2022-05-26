import socket
import os
import importlib
socket_stuff = importlib.import_module('1705076_socket_stuff')
RSA = importlib.import_module('1705076_RSA')
AES = importlib.import_module('1705076_AES')

if __name__ == "__main__" : 
    # take input
    plaintext : bytearray = bytearray(input("Plain Text:\n"), 'latin-1')
    print()

    rsa : RSA.RSA = RSA.RSA.generate_random(128)
    while True:
        # take key
        key : bytearray = bytearray(input("Key:\n"), 'latin-1')

        # create AES, key scheduling
        try:
            aes : AES.AES = AES.AES(key)
        except Exception as e:
            print(e)
            continue
        break
    print()

    path : str = 'dont_open_this'
    if not os.path.exists(path):
        os.makedirs(path)

    with open(path+'/secret.txt', 'w') as f:
        f.write(str(rsa.d))

    # create server socket
    s = socket.socket()
    port = 12345
    s.bind(('', port))
    s.listen()
    print("server started")

    # accept client connection
    c, addr = s.accept()
    print("conenction from client established")

    # send public key
    socket_stuff.send_bytes(c, str(rsa.n).encode())
    socket_stuff.send_bytes(c, str(rsa.e).encode())
    print("RSA public key sent")

    # send RSA encrypted key
    socket_stuff.send_bytes(c, bytes(rsa.encrypt(key)))
    print("RSA encrypted key sent")

    # send AES cipher text
    socket_stuff.send_bytes(c, bytes(aes.encrypt(plaintext)))
    print("AES cipher text sent")

    _ = socket_stuff.receive_bytes(c)
    print("client process completed")

    with open(path + "/dpt.txt", 'r') as f:
        deciphered_text : str = f.read()

    if deciphered_text != str(plaintext, 'latin-1') + (' ' * (len(deciphered_text) - len(plaintext))):
        raise Exception("> System failure")
    else:
        print("> System success")

    c.close()

