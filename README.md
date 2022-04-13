# RSA and El-Gamal encrypting/decrypting program
The program can generate keys of a given length, encrypt and decrypt files, create a signature and check it.

By default, the cryptosystem uses RSA algorythm.
To switch the algorythm, use "crypt eg" or "crypt rsa"
To generate the keys, use "crypt genkey --size 256 --pubkey pk.txt --secret secret.txt"
To encrypt the file, use "crypt encrypt --infile file.txt --pubkey pk.txt --outfile enc.txt"
To decrypt the file, use "crypt decrypt --infile enc.txt --secret secret.txt --outfile file.txt"
To sign the file, use "crypt sign --infile file.txt --secret secret.txt --sigfile signature.txt"
To check the sign, use "crypt check --infile file.txt --pubkey pk.txt --sigfile signature.txt"
Files must be created in advance.
To close the program, use "exit"
Also you can use "crypt --help"



