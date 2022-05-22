import socket
import os
from socket_stuff import send_bytes, receive_bytes
from RSA import RSA
from AES import AES

if __name__ == "__main__" : 
    # take input
    plaintext : bytearray = bytearray(input("Plain Text:\n"), 'latin-1')
    print()

    rsa : RSA = RSA.generate_random(128)
    while True:
        # take key
        key : bytearray = bytearray(input("Key:\n"), 'latin-1')

        # create AES, key scheduling
        try:
            aes : AES = AES(key)
        except Exception as e:
            print(e)
            continue
        break
    print()

    path : str = 'dont_open_this'
    if not os.path.exists(path):
        os.makedirs(path)

    with open(path+'/secret.txt', 'w') as f:
        f.write(str(rsa.d.int_val()))

    # create server socket
    s = socket.socket()
    port = 12345
    s.bind(('', port))
    s.listen()

    # accept client connection
    c, addr = s.accept()

    send_bytes(c, str(rsa.n).encode())
    send_bytes(c, str(rsa.e.int_val()).encode())

    send_bytes(c, bytes(rsa.encrypt(key)))

    send_bytes(c, bytes(aes.encrypt(plaintext)))

    _ = receive_bytes(c)

    with open(path + "/dpt.txt", 'r') as f:
        deciphered_text : str = f.read()

    if deciphered_text != str(plaintext, 'latin-1') + (' ' * (len(deciphered_text) - len(plaintext))):
        raise Exception("System failure")
    else:
        print("System success")

    c.close()

