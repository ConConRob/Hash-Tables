#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the 
  next `LinkedPair` in the list of `LinkedPair` nodes. 
 */
typedef struct LinkedPair
{
    char *key;
    char *value;
    struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable
{
    int capacity;
    LinkedPair **storage;
} HashTable;

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
    LinkedPair *pair = malloc(sizeof(LinkedPair));
    pair->key = strdup(key);
    pair->value = strdup(value);
    pair->next = NULL;

    return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
    if (pair != NULL)
    {
        free(pair->key);
        free(pair->value);
        free(pair);
    }
}

/*
  djb2 hash function

  Do not modify this!
 */
unsigned int hash(char *str, int max)
{
    unsigned long hash = 5381;
    int c;
    unsigned char *u_str = (unsigned char *)str;

    while ((c = *u_str++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % max;
}

/*
  Fill this in.

  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
    HashTable *ht = malloc(sizeof(HashTable));
    ht->capacity = capacity;
    ht->storage = calloc(capacity, sizeof(LinkedPair *));
    return ht;
}

/*
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 */
void hash_table_insert(HashTable *ht, char *key, char *value)
{
    
    int index = hash(key, ht->capacity);
    LinkedPair *pair = create_pair(key, value);
    if (ht->storage[index] != NULL)
    {
        // look at each item till at end or matching index
        LinkedPair *currentPair = ht->storage[index];
        if (strcmp(currentPair->key, key) == 0)
        {
            // if match replace this item value and free old value
            destroy_pair(currentPair->value);
            currentPair->value = strdup(value);
            return;
        }
        while (currentPair->next != NULL)
        {
            // check if key matches
            if (strcmp(currentPair->key, key) == 0)
            {
                // if match replace this item value and free old value
                destroy_pair(currentPair->value);
                currentPair->value = strdup(value);
                return;
            }
            // go next pair
            currentPair = currentPair->next;
        }
        // didn't add any and at last in linked list
        // set the next of current
        currentPair->next = pair;
    }
    else
    {
        // no value at this index yet
        ht->storage[index] = pair;
    }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key)
{
    // check if index exists
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
    // check if index exists
    int index = hash(key, ht->capacity);
    if (ht->storage[index] != NULL)
    {
        // check key then go next till found or no more next
        LinkedPair *currentPair = ht->storage[index];
        if (strcmp(currentPair->key, key) == 0)
        {
            return currentPair->value;
        }
        while (currentPair->next != NULL)
        {
            // go next pair
            currentPair = currentPair->next;
            // check if key matches
            if (strcmp(currentPair->key, key) == 0)
            {
                return currentPair->value;
            }
        }
    }
    return NULL;
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{
    // destroy all pairs
    for (int i = 0; i < ht->capacity; i++)
    {
        LinkedPair *c_pair = ht->storage[i];
        while (c_pair != NULL)
        {
            // set pair to destroy
            LinkedPair *d_pair = c_pair;
            // set c pair
            c_pair = c_pair->next;
            // destroy set to destroy
            destroy_pair(d_pair);
        }
    }
    // destory storage
    free(ht->storage);
    // free hashtable
    free(ht);
}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
    HashTable *new_ht = create_hash_table(ht->capacity * 2);
    // loop all index of old table
     
    for (int i = 0; i < ht->capacity; i++)
    {
        LinkedPair *current_pair = ht->storage[i];
        // each index loops till null
         
        while (current_pair != NULL)
        {
            // adding pairs to new has table
            hash_table_insert(new_ht, current_pair->key, current_pair->value);
            //  next pair
            current_pair = current_pair->next;
        }
    }
    //  call destroy hash table on old table
    // destroy_hash_table(ht);
    return new_ht;
}

#ifndef TESTING
int main(void)
{
    struct HashTable *ht = create_hash_table(2);

    hash_table_insert(ht, "line_1", "Tiny hash table\n");
    hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
    hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

    printf("%s", hash_table_retrieve(ht, "line_1"));
    printf("%s", hash_table_retrieve(ht, "line_2"));
    printf("%s", hash_table_retrieve(ht, "line_3"));

    int old_capacity = ht->capacity;
    ht = hash_table_resize(ht);
    int new_capacity = ht->capacity;

    printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

    destroy_hash_table(ht);

    return 0;
}
#endif
