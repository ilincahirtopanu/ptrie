#ifndef PTRIE_H
#define PTRIE_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <error.h>

/***
 * The prefix trie enables you to add strings that are tracked by the
 * data-structure, and to autocomplete to get the most-frequently
 * added string that has the query string as a prefix.
 */

/**
 * The ptrie is the main structure that the client uses/sees. Think of
 * this like the class in Java. We might need additional structures
 * that we'll use to implement that ptrie, but the client likely won't
 * use those as part of the API.
 *
 * To understand how object oriented programming works in C, see
 * "Object Orientation: Initialization, Destruction, and Methods" in
 * the class lectures.
 */

struct ptrie;
struct ptrie_node;
struct ptrie_entry;
static void recursive(struct ptrie_node * array);

//box
struct ptrie_entry {
    char* word;
    struct ptrie_node *next;
    int counter;
};
//array
struct ptrie_node {
    struct ptrie_entry entries[256];
};
//tree
struct ptrie {
    struct ptrie_node *root;
};

/**
 * Allocate a new `ptrie` for the client to use.
 *
 * Note that this might return `NULL` in the case that we cannot
 * successfully allocate memory with `malloc`.
 */
struct ptrie *ptrie_allocate(void) {
    struct ptrie *new_ptrie = (struct ptrie*)calloc(1 ,sizeof(struct ptrie));
    if (new_ptrie == NULL) {
        return NULL;
    }
    new_ptrie -> root = (struct ptrie_node*)calloc(1, sizeof(struct ptrie_node));
    if (new_ptrie->root == NULL) {
        free(new_ptrie);
        return NULL;
    }
    return new_ptrie;
};

/**
 * Free an existing `ptrie`. This *must* free not just the `struct
 * ptrie`, but also all of the internal data-structure.
 *
 * Arguments:
 *
 * - `@pt` - The ptrie to free.
 */
void recursive(struct ptrie_node* array) {
    if (array == NULL) {
        return;
    }
    size_t i = 0;
    while(i < 256) {
        if (array -> entries[i].next != NULL) {
            recursive(array -> entries[i].next);
        }
        free(array -> entries[i].word);
        i++;
    }
    free(array -> entries);
}

void ptrie_free(struct ptrie *pt) {
    if (pt == NULL) {
        return;
    }
    recursive(pt -> root);
    free(pt);
}

/**
 * `ptrie_add` adds a string to the ptrie. If the string has
 * previously been added, increase the count that tracks how many
 * times it was added, so that we can track frequency.
 *
 * Arguments:
 *
 * - `@pt` - The ptrie to add the string into.
 * - `@str` - The string to add into the `pt`. The `str` is *owned* by
 *     the caller, and is only *borrowed* by this function. Thus, if
 *     you want to store the `str` as part of the data-structure,
 *     you'll have to copy it into the data-structure (recall:
 *     `strdup`). See the section on "Memory Ownership" in the
 *     lectures.
 * - `@return` - Return `0` upon successful addition. Return `-1` if
 *     the `str` could not be added due to `malloc` failure, or if the
 *     string has invalid characters (ascii values < 32, see
 *     https://upload.wikimedia.org/wikipedia/commons/1/1b/ASCII-Table-wide.svg).
 */
int ptrie_add(struct ptrie *pt, const char *str) {
    if (pt == NULL || str == NULL) 
    {
        return -1;
    }

    struct ptrie_node *curr = pt -> root;

    for (size_t i = 0; i < strlen(str); i++) {
        //if the next value is the null character, then the string is done looping
        if(str[i + 1] == '\0') 
        {
            curr -> entries[(int)str[i]].word = strdup(str);
            if (!curr -> entries[(int)str[i]].word) {
                return -1;
            }
            curr -> entries[(int)str[i]].counter++;
            //printf("COUNTER = %d", curr -> entries[(int)str[i]].counter++);
        }
        
        
        //checks that there's been space for the next node allocated, if not allocate
        if (curr -> entries[(int)str[i]].next == NULL)
        {
            curr -> entries[(int)str[i]].next = (struct ptrie_node*)calloc(1, sizeof(struct ptrie_node));
            
            //if something failed with memory allocation
            if (curr -> entries[(int)str[i]].next == NULL) 
            {
                return -1;
            }
        }
        curr = curr -> entries[(int)str[i]].next;
        
     }
    return 0;
}

/**
 * `ptrie_autocomplete` provides an autocompletion for a given string,
 * driven by the frequency of the addition of various strings. It
 * returns the string that has been added the most for which `str` is
 * its prefix. Return a copy of `str` if no such strings have `str` as
 * a prefix. If two strings with an *equal* frequency of addition have
 * prefixes that match the `str`, the one with a lower
 * `ptrie_char2off` value is the one returned (see the helper in
 * `ptrie.c`).
 *
 * An example:
 *
 * ```c
 * struct ptrie *pt = ptrie_allocate();
 * ptrie_add(pt, "he");
 * ptrie_add(pt, "hey");
 * ptrie_add(pt, "hello");
 * ptrie_add(pt, "hello");
 * ptrie_add(pt, "helloworld");
 * assert(strcmp(ptrie_autocomplete(pt, "h"), "hello") == 0);
 * ptrie_add(pt, "hey");
 * ptrie_add(pt, "hey");
 * assert(strcmp(ptrie_autocomplete(pt, "h"), "hey") == 0);
 * ptrie_free(pt);
 * ```
 */
char *ptrie_autocomplete(struct ptrie *pt, const char *str) {
    //struct ptrie_entry *curr = pt -> root -> entries[(int) str[i]];

    //go until the end of the string is null? or until the end of the ptrie
    //make a node, do the depth=depth-> node[int str[i]].next, then strdup the str into that node
    //for loop going thru length of the string, directly access letter you want to go to, if that doesnt have next
    
    //if all of them don't have a count but they have a next level
    if (pt == NULL || str == NULL || pt -> root == NULL) {
        return NULL;
    }
    struct ptrie_node *curr = pt -> root;
    size_t i = 0;

    //goes to the end of the inputted string, moving current down nodes
    while(str[i] != '\0') {

        //if the current node is the last on
        if (curr -> entries[(int)str[i]].word != NULL && str[i + 1] == '\0') {
            return strdup(curr -> entries[(int)str[i]].word);
        }
        //go to the index of the next letter
        curr = curr -> entries[(int)str[i]].next;

        if (!curr) {
            return strdup(str);
        }
        i++;
    }

    //while the next entry of current is not null
    int j = 0;
    //iterate through the 256 buckets and see if any of them have a count, 
    //meaning that it has been used before and follow the path 
    while(j < 256) {
        if (curr -> entries[j].counter > 0) {
            //set max count to be the index of where the max entry bucket is
            return strdup(curr -> entries[j].word);
        } 
        if (curr -> entries[j].next != NULL) {
            //printf("hiiii\n");
            curr = curr -> entries[j].next;
            j=0;
        }
        j++;
    }
    return strdup(str);
}

    
    //go through the 256 until you find the maximum count... 
    //if the string is not attached to that entry, 
    //then go down to thenext and find the max count again and see if the string is attached to there

/**
 * `ptrie_print` is a utility function that you are *not* required to
 * implement, but that is quite useful for debugging. It is easiest to
 * implement using a pre-order traversal with recursion.
 *
 * Arguments:
 *
 * - `@pt` - The prefix trie to print.
 */
void ptrie_print(struct ptrie *pt);
#endif /* PTRIE_H */
