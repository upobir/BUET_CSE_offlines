from AES import AES
import time


if __name__ == "__main__":
    # time stuff
    start_time : float
    end_time : float

    # take input
    plaintext : bytearray = bytearray(input("Plain Text:\n"), 'latin-1')
    print(plaintext.hex(), '[HEX]')
    print()

    while True:
        # take key
        key : bytearray = bytearray(input("Key:\n"), 'latin-1')
        print(key.hex(), '[HEX]')

        # create AES, key scheduling
        start_time = time.time()
        try:
            aes : AES = AES(key)
        except Exception as e:
            print(e)
            continue
        end_time   = time.time()
        key_scheduling_time : float = end_time - start_time
        break
    print()

    # encrypt
    start_time = time.time()
    ciphertext : bytearray = aes.encrypt(plaintext)
    end_time   = time.time()
    encryption_time : float = end_time - start_time

    # decrypt
    start_time = time.time()
    deciphered_text : bytearray = aes.decrypt(ciphertext)
    end_time   = time.time()
    decryption_time : float = end_time - start_time

    # show cipher text
    print("Cipher Text:")
    print(ciphertext.hex(), '[HEX]')
    # print(str(ciphertext.replace(b'\x99', b' '), 'latin-1')) # TODO fix
    print()

    # show decipher text
    print("Deciphererd Text:")
    print(deciphered_text.hex(), '[HEX]')
    print(str(deciphered_text, 'latin-1'))
    print()

    # show execution time
    print("Execution Time:")
    print("Key Scheduling: ", key_scheduling_time, "seconds")
    print("Encryption Time: ", encryption_time, "seconds")
    print("Decryption Time: ", decryption_time, "seconds")
