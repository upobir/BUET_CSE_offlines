from BitVector import *

class AES:
    def __init__(self, key : bytearray):
        self.key = key

    def encrypt(self, plaintext : bytearray) -> bytearray:
        return plaintext

    def decrypt(self, plaintext : bytearray) -> bytearray:
        return plaintext