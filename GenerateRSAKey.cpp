#include <fstream>
#include "openssl/rsa.h"
#include <openssl/pkcs12.h>
#include <openssl/pem.h>

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
 
	BIO *pri = BIO_new(BIO_s_mem());
	BIO *pub = BIO_new(BIO_s_mem());
 
         // Generate private key
	
	PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
    	
			// Note------Generate the public key in the first format
    			//PEM_write_bio_RSAPublicKey(pub, keypair);
   			 // Note------Generate the public key in the second format (this is used in the code here)
	
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

int main() {

	std::string pub_key, pri_key;

	GenerateRSAKey(pub_key, pri_key);



return 0;
}
