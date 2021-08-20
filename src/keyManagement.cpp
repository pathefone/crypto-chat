#include <iostream>
#include <cmath>
#include<bits/stdc++.h>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <gmp.h>
#include <string>
#include "cpp-base64/base64.h"
#include "cpp-base64/base64.cpp"

using namespace std;

struct user {
        string username;
        int p, q;
        int N, euler, e;
        int d;

};

bool coprime(int a, int b) {

                if ( __gcd(a, b) == 1)
                    return true; //coprime
                else
                    return false; //not coprime

}

int bruteforce(int e, int mod) {

    //e*d=1(modφ(N))
    int d = 0;

    while(fmod((e*d),mod) != 1) {
            d++;
    }

    return d;
}

void generate_RSA(user &User) {

    int primeNumList[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293};



        int randomNumber = 0; //private keys and random number

        //generate random seed
        srand(time(NULL));
        randomNumber=rand() % 61;
        User.p = primeNumList[randomNumber];

        do {
            randomNumber=rand() % 61;
        }while(primeNumList[randomNumber] == User.p);

            User.q = primeNumList[randomNumber];


    //    long N, e, eulerTotient; //public keys
        printf("P: %d Q: %d \n", User.p, User.q);
        User.N = User.p * User.q;
        User.euler = (User.p-1)*(User.q-1);

        //e calculation
        do{
            randomNumber=rand() % User.euler + 2;
            if(coprime(randomNumber,User.euler)) {
                    User.e = randomNumber;
                    break;
            }
        }while(1);

        //Set d key
        User.d = bruteforce(User.e,User.euler);

     //   printf("p: %d, q: %d, N: %d, eulerTotient: %d, e: %d",p,q,N,eulerTotient,e);




}

vector<int> encrypt_message(user &User, vector<int> ascii_plain_list) {

    vector<int> ascii_cyphered_list;
    int asciiPlainListSize = ascii_plain_list.size();


    mpz_t number;
    mpz_t base;
    mpz_init(base);
    mpz_init(number);

    mpz_t preCypher;
    mpz_init(preCypher);
    mpz_t mpzUserN;
    mpz_init(mpzUserN);

    mpz_set_ui(mpzUserN, User.N);

    for(int i=0;i<=asciiPlainListSize;i++) {

    mpz_set_ui(base, ascii_plain_list[i]);
    mpz_pow_ui(number, base, User.e);
    mpz_mod(preCypher, number, mpzUserN);

    //check this
    unsigned long longCypher = mpz_get_ui(preCypher);
    int cypher = longCypher;

    ascii_cyphered_list.push_back(cypher);
}

    return ascii_cyphered_list;

}

vector <int> decrypt_message(user &User, vector <int> ascii_cypher_list) {

    vector <int> ascii_plain_list;
    int asciiCypheredListSize = ascii_cypher_list.size();

    mpz_t number;
    mpz_init(number);
    mpz_t cypherMPZ;
    mpz_init(cypherMPZ);

    mpz_t preDecrypt;
    mpz_init(preDecrypt);
    mpz_t mpzUserN;
    mpz_init(mpzUserN);
    mpz_set_ui(mpzUserN, User.N);

    for(int i=0;i<=asciiCypheredListSize;i++) {

    mpz_set_ui(cypherMPZ, ascii_cypher_list[i]);
    mpz_pow_ui(number, cypherMPZ, User.d);
    mpz_mod(preDecrypt, number, mpzUserN);

    unsigned long longDecrypt = mpz_get_ui(preDecrypt);
    int decrypted = longDecrypt;

    ascii_plain_list.push_back(decrypted);

    }

    return ascii_plain_list;

}


vector<int> convertToASCII(string s)
{
    vector <int> list;

    for (int i = 0; i < s.length(); i++)
    {
        list.push_back((int)s[i]);
    }

    return list;
}

string convertFromASCII(vector<int> ascii_list) {

    string converted;
    int ascii_list_size = ascii_list.size();

    for(int i=0;i<ascii_list_size;i++) {
        char temp = (char)ascii_list[i];
        converted += temp;
    }

    return converted;
}

const size_t split_num = 13;

string convertToBinary(unsigned int n)
{   //(in the future could check the size of biggest encrypted ascii and choose width of bitset accordingly)
    string binary = std::bitset<split_num>(n).to_string(); //to binary
        return binary;
}

vector <string>
listToBinary(vector <int> encrypted_ascii_list) 
{
    int listSize = encrypted_ascii_list.size();
    vector <string> binary_list;

    for(int i=0;i<listSize;i++) {
            binary_list.push_back(convertToBinary(encrypted_ascii_list[i]));
    }

    return binary_list;

}

vector <string>
base64List(vector <string> binary_list) 
{
        int listSize = binary_list.size();

        for(int i=0;i<listSize;i++) {
            //double base64 encoding for pretty text
             binary_list[i] = base64_encode(base64_encode(binary_list[i]));
        }
        return binary_list;
}

string 
formatToString(vector <string> binary_base64_list, string split_num)
{
        string formatted_list;
	string digit_count = to_string(split_num.size());
	int listSize = binary_base64_list.size();

        for(int i=0;i<listSize;i++) {
            string temp = binary_base64_list[i];
            formatted_list +=temp;
        }
	// last number represents count of digits to read from the end not including the last
	formatted_list +=split_num;
	formatted_list +=digit_count;

    return formatted_list;
}

vector <string>
b64_to_list(string formattedMessage) 
{

	int size = formattedMessage.size();
	vector <string> b64listed;

	string temp_string;

	for(int i = 0; i < size; i++) {
		
		if(i==28) {
		     b64listed.push_back(temp_string);
		     temp_string = "";
		}
		else if(i == size-1) {
			temp_string += formattedMessage[i];
			b64listed.push_back(temp_string);
			break;
		}
		temp_string += formattedMessage[i];
		
	}
	return b64listed;

}


string 
format_remote_message(string formattedMessage, int &split_num_out) 
{

	string decrypted_message;
	int formatted_size = formattedMessage.size()-1;
	char split_num_count_char = char(formattedMessage[formatted_size]);
	int split_num_count = split_num_count_char - '0';
	formattedMessage.erase(formattedMessage.begin()+formatted_size);
	string split_num;

	for (int i = formatted_size-split_num_count; i<=formatted_size; i++) {
		split_num+=formattedMessage[i];
	}

	int split_num_int = stoi(split_num);
	
	while(0!=split_num_count) {
		formattedMessage.erase(formattedMessage.begin()+formattedMessage.size()-1);
		split_num_count--;
	}
	
	split_num_out = split_num_int;
	decrypted_message = formattedMessage;

	return decrypted_message;
}

vector <string> 
b64_decode_list (vector <string> list_to_decode) 
{
		vector <string> decoded_list;
		int size = list_to_decode.size();

		for(int i = 0; i < size; i++) {
			decoded_list.push_back(base64_decode(base64_decode(list_to_decode[i])));
		}
	return decoded_list;
}

vector <int> 
binary_list_to_ascii(vector <string> binary_list, const size_t split_number_F) 
{
		// can save some space and rewrite binary_list
		vector <int> ascii_list;
       		int size = binary_list.size();
		
		for (int i = 0; i < size; i++ ) {
			
			string temp_string = binary_list[i];
			bitset<13> bits(temp_string);
			ascii_list.push_back(bits.to_ulong());
		}

		return ascii_list;

}


//improve codebase ASAP

void dashboard() {

        printf("Hello and welcome to RSA key management and communication software in CLI!\n");
        printf("This is a super mega ultra demo version.\n");

        int option;

        cout << "To generate a new RSA key pair press 1\n";
        cout << "To exit press 0\n";
        cout << "Your option: ";
        cin >> option;

        if(option==1) {
                printf("You have successfully selected new RSA key pair generation!\n");

                user RandomUser;
                generate_RSA(RandomUser);
                string plainTextMessage;

                cout << "Enter a message you want to encrypt: ";
                cin.ignore();
                getline(cin, plainTextMessage);

                vector <int> ascii_keys = convertToASCII(plainTextMessage); //convert plaintext msg to ascii list
                vector <int> encrypted_ascii_list = encrypt_message(RandomUser, ascii_keys); //encrypt individual ascii
                vector <string> binary_ascii_list = listToBinary(encrypted_ascii_list);
                vector <string> binary_base64_list = base64List(binary_ascii_list);
                string splitNumString = to_string(split_num);

		//Formatted message to send
                string formattedMessage = formatToString(binary_base64_list, splitNumString);
		

                cout << "\nFormatted message: " << formattedMessage;
		
		//Getting remote message to decrypt
		int split_number;
		string dcrpt = format_remote_message(formattedMessage, split_number);
		vector <string> b64list = b64_to_list(dcrpt);
		vector <string> b64decoded = b64_decode_list(b64list);
		
		vector <int> ascii_list = binary_list_to_ascii(b64decoded, split_number);
		vector <int> plain_list = decrypt_message(RandomUser, ascii_list);
		string receivedMessage = convertFromASCII(plain_list);

		// decrypt every individual element in ascii_list and put it into string.

                cout << "\nDEcrypted message: " << receivedMessage;
        }

}


int main()
{
    dashboard();

    //g++ -lgmp keyManagement.cpp


	return 0;
}

