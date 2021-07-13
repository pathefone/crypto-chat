#include <fstream>
#include "openssl/rsa.h"
#include <openssl/pem.h>
#include <cstring>

 #define KEY_LENGTH 2048 // Key length
 #define PUB_KEY_FILE "pubkey.pem" // public key path
 #define PRI_KEY_FILE "prikey.pem" // private key path
 
/*
 Manufacturing key pair: private key and public key
**/
void GenerateRSAKey(std::string & out_pub_key, std::string & out_pri_key)
{
	 size_t pri_len = 0; // Private key length
     size_t pub_len = 0; // public key length
	 char *pri_key = nullptr; // private key
	 char *pub_key = nullptr; // public key
 
	 // Generate key pair
	RSA *keypair = RSA_generate_key(KEY_LENGTH, RSA_3, NULL, NULL);
 

    //we need bio to safely transfer pub/pri keys?
	BIO *pri = BIO_new(BIO_s_mem());
	BIO *pub = BIO_new(BIO_s_mem());
 
         // Generate private key
	
	PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
	PEM_write_bio_RSA_PUBKEY(pub, keypair);
 
	 // Get the length  
	pri_len = BIO_pending(pri);
	pub_len = BIO_pending(pub);
 
	 // The key pair reads the string  
	pri_key = (char *)malloc(pri_len + 1);
	pub_key = (char *)malloc(pub_len + 1);
 
	BIO_read(pri, pri_key, pri_len);
	BIO_read(pub, pub_key, pub_len);
 
	pri_key[pri_len] = '\0';
	pub_key[pub_len] = '\0';
 
	out_pub_key = pub_key;
	out_pri_key = pri_key;
 
	 // Write the public key to the file
	std::ofstream pub_file(PUB_KEY_FILE, std::ios::out);
	if (!pub_file.is_open())
	{
		perror("pub key file open fail:");
		return;
	}
	pub_file << pub_key;
	pub_file.close();
 
	 // write private key to file
	std::ofstream pri_file(PRI_KEY_FILE, std::ios::out);
	if (!pri_file.is_open())
	{
		perror("pri key file open fail:");
		return;
	}
	pri_file << pri_key;
	pri_file.close();
 
	 // release memory
	RSA_free(keypair);
	BIO_free_all(pub);
	BIO_free_all(pri);
 
	free(pri_key);
	free(pub_key);
}

std::string RsaPriEncrypt(const std::string &clear_text, std::string &pri_key)
{
	std::string encrypt_text;
	BIO *keybio = BIO_new_mem_buf((unsigned char *)pri_key.c_str(), -1);
	RSA* rsa = RSA_new();
	rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
	if (!rsa)
	{
		BIO_free_all(keybio);
		return std::string("");
	}

	 // Get the maximum length of the data block that RSA can process at a time
	int key_len = RSA_size(rsa);
	 int block_len = key_len-11; // Because the filling method is RSA_PKCS1_PADDING, so you need to subtract 11 from the key_len

	 // Apply for memory: store encrypted ciphertext data
	char *sub_text = new char[key_len + 1];
	memset(sub_text, 0, key_len + 1);
	int ret = 0;
	int pos = 0;
	std::string sub_str;
	 // Encrypt the data in segments (the return value is the length of the encrypted data)
	while (pos < clear_text.length()) {
		sub_str = clear_text.substr(pos, block_len);
		memset(sub_text, 0, key_len + 1);
		ret = RSA_private_encrypt(sub_str.length(), (const unsigned char*)sub_str.c_str(), (unsigned char*)sub_text, rsa, RSA_PKCS1_PADDING);
		if (ret >= 0) {
			encrypt_text.append(std::string(sub_text, ret));
		}
		pos += block_len;
	}

	 // release memory
	delete sub_text;
	BIO_free_all(keybio);
	RSA_free(rsa);

	return encrypt_text;
}

std::string RsaPubDecrypt(const std::string & cipher_text, const std::string & pub_key)
{
	std::string decrypt_text;
	BIO *keybio = BIO_new_mem_buf((unsigned char *)pub_key.c_str(), -1);
	RSA* rsa = RSA_new();

	 // Note-------Use the public key in the first format for decryption
	//rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);
	 // Note-------Use the public key in the second format for decryption (we use this format as an example)
	rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
	if (!rsa)
	{
		BIO_free_all(keybio);

        return decrypt_text;
	}

	 // Get the maximum length of RSA single processing
	int len = RSA_size(rsa);
	char *sub_text = new char[len + 1];
	memset(sub_text, 0, len + 1);
	int ret = 0;
	std::string sub_str;
	int pos = 0;
	 // Decrypt the ciphertext in segments
	while (pos < cipher_text.length()) {
		sub_str = cipher_text.substr(pos, len);
		memset(sub_text, 0, len + 1);
		ret = RSA_public_decrypt(sub_str.length(), (const unsigned char*)sub_str.c_str(), (unsigned char*)sub_text, rsa, RSA_PKCS1_PADDING);
		if (ret >= 0) {
			decrypt_text.append(std::string(sub_text, ret));
			printf("pos:%d, sub: %s\n", pos, sub_text);
			pos += len;
		}
	}

	 // release memory
	delete sub_text;
	BIO_free_all(keybio);
	RSA_free(rsa);

	return decrypt_text;
}



int main() {

	std::string pub_key, pri_key, plain_text;
    plain_text = "1";

	GenerateRSAKey(pub_key, pri_key);
    std::string encrypted = RsaPriEncrypt(plain_text, pri_key);
    printf("\nENCRYPTED: %s", encrypted);
    std::string decrypted = RsaPubDecrypt(encrypted, pub_key);
    printf("\nDECRYPTED: %s", decrypted);



return 0;
}
