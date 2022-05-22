import socket

s = socket.socket()

port = 12345

s.connect(('127.0.0.1', port))

print(s.recv(1024).decode())

print(s.recv(1024).decode())

path = 'dont_open_this'

with open(path + '/secret.txt', 'r') as f:
    rsa_key = f.read()

print(rsa_key)

with open(path+'/dpt.txt', 'w') as f:
    f.write('plaintext')

s.send('ok'.encode())

print('fin')

s.close()