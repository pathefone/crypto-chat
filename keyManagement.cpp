#include <iostream>
#include <cmath>
#include<bits/stdc++.h>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <gmp.h>

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
        randomNumber=rand() % 14;
        User.p = primeNumList[randomNumber];

        do {
            randomNumber=rand() % 14;
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



int encrypt_message(user &User, int plainTextMsg) {

  //  int cypher = 0;
    mpz_t number;
    mpz_t base;
    mpz_init(base);
    mpz_init(number);
    mpz_set_ui(base, plainTextMsg);
    mpz_pow_ui(number, base, User.e);

   // = pow(plainTextMsg, User.e); //toobig num, generate smaller e
  //  printf("BIG NUM: %d", number);
    mpz_t preCypher;
    mpz_init(preCypher);
    mpz_t mpzUserN;
    mpz_init(mpzUserN);
    mpz_set_ui(mpzUserN, User.N);

    mpz_mod(preCypher, number, mpzUserN);

    unsigned long longCypher = mpz_get_ui(preCypher);
    int cypher = longCypher;

  //  cypher = fmod(number,User.N);
  //  printf("Cypher: %d Message: %d", cypher, plainTextMsg);
    return cypher;

}

int decrypt_message(user &User, int cypher) {

    mpz_t number;
    mpz_init(number);
    mpz_t cypherMPZ;
    mpz_init(cypherMPZ);
    mpz_set_ui(cypherMPZ, cypher);

    mpz_pow_ui(number, cypherMPZ, User.d);

    //long double number = pow(cypher, User.d);
    mpz_t preDecrypt;
    mpz_init(preDecrypt);
    mpz_t mpzUserN;
    mpz_init(mpzUserN);
    mpz_set_ui(mpzUserN, User.N);

    mpz_mod(preDecrypt, number, mpzUserN);

    unsigned long longDecrypt = mpz_get_ui(preDecrypt);
    int decrypted = longDecrypt;

    return decrypted;

}


vector<int> convertToASCII(string s)
{
    vector <int> list;

    for (int i = 0; i < s.length(); i++)
    {
        list.push_back((int)s[i]);
      //  cout << (int)s[i]<< endl;
    }

    return list;
}


int main()
{

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

            string text_message = "Labas";
            vector <int> ascii_keys;
            ascii_keys = convertToASCII(text_message);

         //   cout << "ASCII KEYS: " << ascii_keys[0] << "" << ascii_keys[4];

            int message = 80;


            int encrypted = encrypt_message(RandomUser, message);
            int decrypted = decrypt_message(RandomUser, encrypted);

            printf("Encrypted message: %d\n", encrypted);

            printf("Decrypt key: %d\n", RandomUser.d);
            printf("E: %d, euler: %d, N: %d \n", RandomUser.e, RandomUser.euler, RandomUser.N);



            printf("Decrypted message: %d\n", decrypted);

        //    printf("Random user name: %s", RandomUser.username.c_str());


    }


	return 0;
}
