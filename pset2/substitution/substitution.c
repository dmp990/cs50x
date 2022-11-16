#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

bool isKeyInvalid(string key);

int main(int argc, string argv[])
{
    if (argc == 1)
    {
        printf("no command-line arguments provided!\n");
        return 1;
    }
    if (argc == 3)
    {
        printf("too many arguments\n");
        return 1;
    }

    string key = argv[1];
    if (isKeyInvalid(key))
    {
        printf("key invalid\n");
        return 1;
    }

    string plaintext = get_string("Plaintext: ");
    printf("ciphertext: ");

    int n = strlen(plaintext);
    for (int i = 0; i < n; i++)
    {
        if (isalpha(plaintext[i])) // if is alphabet
        {
            if (islower(plaintext[i]))
            {
                printf("%c", tolower(key[(((int)plaintext[i]) % 97)]));
            }
            else
            {
                printf("%c", toupper(key[(((int)plaintext[i]) % 65)]));
            }
        }
        else // is is not alphabet
        {
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");
    return 0;
}

bool isKeyInvalid(string key)
{
    int n = strlen(key);

    // check if the key is of length > || < 26
    if (n != 26)
    {
        return true;
    }

    // check if key contains non alphabetical characters or duplicates
    for (int i = 0; i < n; i++)
    {
        if (!isalpha(key[i])) // making sure it is alphabet
        {
            return true;
        }
        for (int j = 0; j < i; j++) // making sure it is unique
        {
            if (key[j] == key[i])
            {
                return true;
            }
        }
    }
    return false;
}