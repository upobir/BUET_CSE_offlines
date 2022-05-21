from io import RawIOBase
from pydoc import plain
from turtle import bye
from typing import Tuple
from BitVector import *
from math import gcd

class RSA:
    def __init__(self, key_length: int):
        if key_length % 8 != 0:
            raise Exception("Invalid Key Length!")

        self.key_length = key_length

        p : int
        q : int
        p, q = self.generate_prime_pair()

        self.n = p * q
        if self.n <= 255 : 
            raise Exception("n too small")

        self.e : BitVector
        self.d : BitVector
        self.e, self.d = self.generate_keys(p, q)

    def generate_prime(self, lenght: int) -> int:
        while True:
            candidate : BitVector = BitVector(intVal=0).gen_random_bits(lenght)
            if candidate.test_for_primality():   # TODO change
                return candidate.int_val()

    def generate_prime_pair(self) -> Tuple[int, int]:
        p : int = self.generate_prime(self.key_length // 2)
        while True:
            q : int = self.generate_prime(self.key_length // 2)
            if p != q:
                return p, q

    def generate_keys(self, p : int, q : int) -> Tuple[BitVector, BitVector]:
        cand : int = 100
        phi_n : int = (p-1) * (q-1)

        while True:
            if gcd(cand, phi_n) == 1:
                break
            cand += 1

        e : BitVector = BitVector(intVal=cand)
        d : BitVector = e.multiplicative_inverse(BitVector(intVal=phi_n))

        if e.int_val() * d.int_val() % phi_n != 1:
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