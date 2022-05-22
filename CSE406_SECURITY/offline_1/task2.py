from RSA import RSA
import time


if __name__ == "__main__":
    # time stuff
    start_time : float
    end_time : float

    # take input
    plaintext : bytearray = bytearray(input("Plain Text:\n"), 'utf-8')
    print(plaintext.hex(), '[HEX]')
    print()

    # take key length
    while True:
        # take key
        key_length : int = int(input("key length in bits :\n"))

        # create RSA, key pair generation
        start_time = time.time()
        try:
            rsa : RSA = RSA.generate_random(key_length)
        except Exception as e:
            print(e)
            continue
        end_time   = time.time()
        keypair_generation_time : float = end_time - start_time
        break
    print()

    # encrypt
    start_time = time.time()
    ciphertext : bytearray = rsa.encrypt(plaintext)
    end_time   = time.time()
    encryption_time : float = end_time - start_time

    # decrypt
    start_time = time.time()
    deciphered_text : bytearray = rsa.decrypt(ciphertext)
    end_time   = time.time()
    decryption_time : float = end_time - start_time

    # show cipher text
    print("Cipher Text:")
    print(ciphertext.hex(), '[HEX]')
    # print(str(ciphertext, 'utf-8'))
    print()

    # show decipher text
    
    print("Deciphererd Text:")
    print(deciphered_text.hex(), '[HEX]')
    print(str(deciphered_text, 'utf-8'))
    print()

    # show execution time
    print("Execution Time:")
    print("Keypair generation time: ", keypair_generation_time, "seconds")
    print("Encryption Time: ", encryption_time, "seconds")
    print("Decryption Time: ", decryption_time, "seconds")
