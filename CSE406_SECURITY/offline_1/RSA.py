from BitVector import *

class RSA:
    def __init__(self, K: int):
        self.K = K

    def encrypt(self, plaintext : bytearray) -> bytearray:
        return plaintext

    def decrypt(self, plaintext : bytearray) -> bytearray:
        return plaintext