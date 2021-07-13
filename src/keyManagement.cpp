#include <iostream>
#include <cmath>
#include<bits/stdc++.h>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <gmp.h>
#include <string>

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

//g++ -lgmp keyManagement.cpp


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

    for(int i=0;i<=ascii_list_size-1;i++) {
        char temp = (char)ascii_list[i];
        converted += temp;
    }

    return converted;
}

void dashboard() {

        printf("Hello and welcome to RSA key management and communication software in CLI!\n");
        printf("This is a super mega ultra demo version -0.9999 v.\n");
        printf("Have fun! \n\n");

        int option;

        cout << "To generate a new5 RSA key pair press 1\n";
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




                cout << "YOU ENTERED : " << plainTextMessage;


                vector <int> ascii_keys = convertToASCII(plainTextMessage);

                vector <int> encrypted_ascii_list = encrypt_message(RandomUser, ascii_keys);
                vector <int> decrypted_ascii_list = decrypt_message(RandomUser, encrypted_ascii_list);

                string decryptedMessage = convertFromASCII(decrypted_ascii_list);

            //    printf("Encrypted message first letter: %d\n", encrypted_ascii_list[0]);

            //    printf("Decrypt key: %d\n", RandomUser.d);
          //      printf("E: %d, euler: %d, N: %d \n", RandomUser.e, RandomUser.euler, RandomUser.N);


                cout << "Your decrypted message: " << decryptedMessage << "\n" ;


            //    printf("Random user name: %s", RandomUser.username.c_str());


        }

}


int main()
{
    dashboard();



	return 0;
}

//when the keys are very big, for example 269 251 yet again the numbers are too high for calculations, find solution!