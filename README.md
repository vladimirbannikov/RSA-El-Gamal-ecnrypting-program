# RSA and El-Gamal encrypting/decrypting program
The program can generate keys of a given length, encrypt and decrypt files, create a signature and check it.

By default, the cryptosystem uses RSA algorythm.

To switch the algorythm, use "crypt eg" or "crypt rsa"

To generate the keys, use "crypt genkey --size 256 --pubkey name_of_public_key --secret name_of_secret_key"

To encrypt the file, use "crypt encrypt --infile name_of_infile --pubkey name_of_public_key --outfile name_of_encrypted_file"

To decrypt the file, use "crypt decrypt --infile name_of_encrypted_file --secret name_of_secret_key --outfile name_of_decrypted_file"

To sign the file, use "crypt sign --infile name_of_infile --secret name_of_secret_key --sigfile name_of_signature_file"

To check the sign, use "crypt check --infile name_of_infile --pubkey name_of_public_key --sigfile name_of_signature_file"

Files must be created in advance.

To close the program, use "exit"

Also you can use "crypt --help"



