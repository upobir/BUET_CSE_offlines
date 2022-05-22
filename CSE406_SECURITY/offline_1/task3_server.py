import socket
import os

print("Enter plaintext:")
plaintext : str = input()
print()

print("Enter AES key:")
key : str = input()
print()

# AES RSA

path : str = 'dont_open_this'
if not os.path.exists(path):
    os.makedirs(path)

with open(path+'/secret.txt', 'w') as f:
    f.write('secretttt')

s = socket.socket()
port = 12345

s.bind(('', port))

s.listen()

c, addr = s.accept()
c.send('test'.encode())
c.send('no'.encode())

print(c.recv(1024).decode())

# do checking

c.close()

