from typing import Tuple
from BitVector import *
from math import gcd

class RSA:
    def __init__(self, key_length: int, n : int, e : int, d : int):
        if key_length % 8 != 0:
            raise Exception("Invalid Key Length!")

        self.key_length : int = key_length
        self.n : int = n
        self.e : BitVector = BitVector(intVal=e)
        self.d : BitVector = BitVector(intVal=d)

        if self.n <= 255 : 
            raise Exception("n too small")
    
    @classmethod
    def generate_random(cls, key_length : int) -> "RSA":
        if key_length % 8 != 0:
            raise Exception("Invalid Key Length!")

        p : int
        q : int
        p, q = cls.generate_prime_pair(key_length)

        n : int = p * q
        if n <= 255 : 
            raise Exception("n too small")

        e : int
        d : int
        e, d = cls.generate_keys(p, q)

        return RSA(key_length, n, e, d)

    @classmethod
    def generate_prime(cls, length: int) -> int:
        while True:
            candidate : BitVector = BitVector(intVal=0).gen_random_bits(length)
            if candidate.test_for_primality():   # TODO change
                return candidate.int_val()

    @classmethod
    def generate_prime_pair(cls, double_length: int) -> Tuple[int, int]:
        p : int = cls.generate_prime(double_length // 2)
        while True:
            q : int = cls.generate_prime(double_length // 2)
            if p != q:
                return p, q

    @classmethod
    def generate_keys(cls, p : int, q : int) -> Tuple[int, int]:
        e : int = 100
        phi_n : int = (p-1) * (q-1)

        while True:
            if gcd(e, phi_n) == 1:
                break
            e += 1

        d : int = BitVector(intVal=e).multiplicative_inverse(BitVector(intVal=phi_n)).int_val()

        if e * d % phi_n != 1:
            raise Exception("Error in generating keys!")

        return e, d

    def pow(self, base : int, exp : BitVector) -> BitVector:
        ret : int = 1
        # x : int = base.int_val()
        for i in range(len(exp)-1, -1, -1):
            if exp[i] == 1:
                ret = ret * base % self.n
            base = base * base % self.n
        
        return BitVector(intVal=ret)

    def encrypt(self, plaintext : bytearray) -> bytearray:
        ciphertext : bytearray = bytearray()
        for c in plaintext:
            e : BitVector = self.pow(c, self.e)
            if len(e) < self.key_length:
                e.pad_from_left(self.key_length - len(e))
            ciphertext += bytearray.fromhex(e.get_bitvector_in_hex())
        return ciphertext


    def decrypt(self, ciphertext : bytearray) -> bytearray:
        plaintext : bytearray = bytearray()
        stride : int = self.key_length // 8
        for i in range(0, len(ciphertext), stride):
            c : BitVector = self.pow(BitVector(rawbytes = ciphertext[i : i+stride]).int_val(), self.d)
            plaintext.append(c.int_val())
        return plaintext