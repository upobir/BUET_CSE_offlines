from AES import AES
import time


if __name__ == "__main__":
    # take input
    plaintext : bytearray = bytes(input("Plain Text:\n"), 'utf-8')
    print(plaintext.hex(), '[HEX]')
    print()

    # take key
    key : bytearray = bytes(input("Key:\n"), 'utf-8')
    print(key.hex(), '[HEX]')
    print()

    # time stuff
    start_time : float
    end_time : float

    # create AES, key scheduling
    start_time = time.time()
    aes : AES = AES(key)
    end_time   = time.time()
    key_scheduling_time : float = end_time - start_time

    # encrypt
    start_time = time.time()
    ciphertext : str = aes.encrypt(plaintext)
    end_time   = time.time()
    encryption_time : float = end_time - start_time

    # decrypt
    start_time = time.time()
    deciphered_text : str = aes.decrypt(ciphertext)
    end_time   = time.time()
    decryption_time : float = end_time - start_time

    # show cipher text
    print("Cipher Text:")
    print(ciphertext.hex(), '[HEX]')
    print(str(ciphertext, 'utf-8'))
    print()

    # show decipher text
    
    print("Deciphererd Text:")
    print(deciphered_text.hex(), '[HEX]')
    print(str(deciphered_text, 'utf-8'))
    print()

    # show execution time
    print("Execution Time:")
    print("Key Scheduling: ", key_scheduling_time, "seconds")
    print("Encryption Time: ", encryption_time, "seconds")
    print("Decryption Time: ", decryption_time, "seconds")
