#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    int count; // to store the number of occurrences of the word
};

// Trie structure
struct Trie {
    struct TrieNode* root;
};

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie* trie = (struct Trie*)malloc(sizeof(struct Trie));
    if (trie) {
        trie->root = (struct TrieNode*)malloc(sizeof(struct TrieNode));
        if (trie->root) {
            trie->root->count = 0;
            for (int i = 0; i < ALPHABET_SIZE; i++) {
                trie->root->children[i] = NULL;
            }
        } else {
            free(trie);
            trie = NULL;
        }
    }
    return trie;
}

// Inserts a word into the trie
void insert(struct Trie *pTrie, char *word)
{
    struct TrieNode* current = pTrie->root;
    int index;
    for (int i = 0; word[i] != '\0'; i++) {
        index = word[i] - 'a';
        if (!current->children[index]) {
            current->children[index] = (struct TrieNode*)malloc(sizeof(struct TrieNode));
            if (!current->children[index]) {
                printf("Memory allocation error.\n");
                return;
            }
            current->children[index]->count = 0;
            for (int j = 0; j < ALPHABET_SIZE; j++) {
                current->children[index]->children[j] = NULL;
            }
        }
        current = current->children[index];
    }
    // Mark the end of word and increment count
    current->count++;
}

// Computes the number of occurrences of a word in the trie
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    struct TrieNode* current = pTrie->root;
    int index;
    for (int i = 0; word[i] != '\0'; i++) {
        index = word[i] - 'a';
        if (!current->children[index]) {
            return 0; // Word not found
        }
        current = current->children[index];
    }
    return current->count;
}

// Deallocate memory for trie nodes
struct TrieNode* deallocateTrieNodes(struct TrieNode* node)
{
    if (node) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (node->children[i]) {
                deallocateTrieNodes(node->children[i]);
            }
        }
        free(node);
    }
    return NULL;
}

// Deallocate the trie structure
struct Trie* deallocateTrie(struct Trie* trie)
{
    if (trie) {
        trie->root = deallocateTrieNodes(trie->root);
        free(trie);
    }
    return NULL;
}

// Reads dictionary file and store words in array
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open dictionary file.\n");
        return 0;
    }

    char buffer[256];
    int numWords = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        char *newline = strchr(buffer, '\n');
        if (newline) {
            *newline = '\0';
        }
        pInWords[numWords] = strdup(buffer);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char *inWords[256];
    
    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }
    
    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }
    
    // Words to search for occurrences
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");
    return 0;
}
