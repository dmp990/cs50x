// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];

// Total number of words added to hash table

long total_words_added = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // obtain hash value
    int hash_value = hash(word);

    // create a temperory node
    node *temp_node = table[hash_value];

    // traverse linked list at that index to find the word
    while (temp_node != NULL)
    {
        if (strcasecmp(temp_node->word, word) == 0)
        {
            return true; // found it, return true
        }
        temp_node = temp_node->next;
    }
    return false; // did not find it, return false
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // ORIGINAL
    // return toupper(word[0]) - 'A';

    // return (first_pos * 25) + second_pos;

    // IMPROVED, HOPEFULLY
    /*int first_pos = toupper(word[0]) - 'A'; // 0 if a
    int second_pos = 0;
    word[1] ? second_pos = toupper(word[1]) - 'A' : 0; // if word's length is more than one, then do the thing, else assign 0
    return ((first_pos * 25) + second_pos) + first_pos;*/

    long sum = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        sum += toupper(word[i]);
    }
    return sum % N;
}

int max = -1;

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // open the dictionary file for reading
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    // node *str = malloc(sizeof(node));
    char word[LENGTH + 1];
    while (fscanf(dict, "%s", word) != EOF)
    {
        // create a new node
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            return false;
        }
        // copy word into new node
        total_words_added++;
        strcpy(new_node->word, word);

        // get the hash_index
        int hash_index = hash(new_node->word);

        // insert node into the linked list
        new_node->next = table[hash_index]; // new_node->next points at the HEAD
        table[hash_index] = new_node;       // HEAD now points at the new node
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return total_words_added;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // iterate through hash table
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }

        // execute at the last iteration of loop, we must have unloaded every bucket if we reached this point
        if (cursor == NULL && i == N - 1)
        {
            return true;
        }
    }
    return false; // did not unload successfully
}