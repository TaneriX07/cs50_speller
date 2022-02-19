// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 45000;

// Hash table
node *table[N];

// Counter for words in hash table
int word_count;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Get the hash value for the word
    int index = hash(word);

    // Access the linked list at that index in the hash table
    node *trav = table[index];

    // Traverse the linked list, checking if the word exist
    while (trav != NULL)
    {
        // The check should be case insensitive (strcasecmp)
        if (strcasecmp(word, trav->word) == 0)
        {
            // Word found
            return true;
        }
        else
        {
            // Move to the next node
            trav = trav->next;
        }
    }

    // Word not in the list
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // The hash function must also be case insensitive
    unsigned int hash = 0;
    for (int i = 0 ; word[i] != '\0' ; i++)
    {
        hash = 31*hash + tolower(word[i]);
    }
    return hash % N;

    // return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open Dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Couldn't open the file!\n");
        return false;
    }

    // Loop through every word in the dictionary
    char word[LENGTH];
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create a new node to store the word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Malloc failed!\n");
            return false;
        }
        // Fill in the node
        strcpy(n->word, word);

        // Hash word
        int index = hash(word);

        // Insert node into hash table
        // If there is a linked list inside the table
        // We have to make the current node point to that list and only then can we have the table point to the current node
        n->next = table[index];
        table[index] = n;

        // Update the global variable: word_count
        word_count++;
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Loop through the whole table
    for (int i = 0; i < N; i++)
    {
        // Loop through the linked list and free each node
        node *trav = table[i];
        while (trav != NULL)
        {
            node *tmp = trav;
            trav = trav->next;
            free(tmp);
        }
    }

    return true;
}

