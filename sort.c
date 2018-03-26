//
// Created by algotrader on 7/11/17.
//
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <memory.h>

/* This program will only handle distances (squared) up to 2**32 -- I won't worry about precision */
/* only integers for now */

/* This program sorts by distances by origin. */

/* each bucket is a block, which is dynamically allocated */
#define BLOCKSIZE 65536
#define NUMBLOCKS 65536
#define COORDINATEPATTERN "<[ \\t]*(-?[0-9]+)[ \\t]*,[ \\t]*(-?[0-9]+)[ \\t]*>"
/* The C library I am using does not seem to support POSIX regular expressions. */

#define TWOSUBEX 3 /* The pattern matcher should find two parenthesized subexpressions. */
#define MSWTOINDEX(w) (((w) >> 16) & 0xFFFF)
#define LSWTOINDEX(w) ((w) & 0xFFFF)

/* Define the coordinate structure */

typedef struct node {
    unsigned int distance; /* actually squared */
    int x; /* read from input in ascii */
    int y; /* read from input in ascii */
    struct node *next; /* I think at a given distance there are at most 12 coordinate pairs. */
    /* if we have a Pythagorean triple x, y, z, then bucket can hold <x,y>, <y,x>, <-x, -y>, <-y, -x>, */
    /* <-x, y>, <-y, x>, <x, -y>, <y, -x>, <z, 0>, <0, z>, <-z,0>, and <0, -z> */
} Node;

/* Define the Block and Bucket structures */
/* A Bucket is just a singly linked list */
/* A block is a number of buckets */


typedef Node* Block[BLOCKSIZE];
Block *blockarray[NUMBLOCKS];

/* Note that I don't like to declare local variables without initialization. */

/* sort by distance to origin */
void sortLineInput(char *xstring, char *ystring) {
    int blockindex = -1; /* initialized to illegal value */
    int bucketindex = -1; /* initialized to illegal value */
    int x = atoi(xstring);
    int y = atoi(ystring);
    unsigned int distance = (x*x) + (y*y); /* can't be negative */
    Node *pnode = (Node *)calloc(1, sizeof(Node)); /* the block allocated is zeroed */
    Node **ppnode = NULL;

    /* allocate a node */
    if(pnode == NULL){
        fprintf(stderr, "Unable to allocate a node.\n");
        exit(EXIT_FAILURE);
    }
    /* fill in the new node */
    pnode->distance = distance;
    pnode->x = x;
    pnode->y = y;
    pnode->next = NULL;

    /* get ready to install it in a bucket */
    blockindex = MSWTOINDEX(distance);
    bucketindex = LSWTOINDEX(distance);

    if(blockarray[blockindex] == NULL) {
        blockarray[blockindex] = (Block *) calloc(1, sizeof(Block));
        if(blockarray[blockindex] == NULL) {
            fprintf(stderr, "Unable to allocate a block.\n");
            exit(EXIT_FAILURE);
        }
    }

    /* now let's install it */
    for(ppnode = &(*blockarray[blockindex])[bucketindex];
        *ppnode != NULL;
        ppnode = &(*ppnode)->next) {
        /* walk through the bucket to see where to install the new node */
        /* install so that smaller x's and y's come earlier in the list */
        if((*ppnode)->x > pnode->x) {
            pnode->next = *ppnode;
            break;
        } else if((*ppnode)->x == pnode->x) { /* x's are equal let's check y's */
            if ((*ppnode)->y > pnode->y) {
                pnode->next = *ppnode;
                break;
            }
        }
    }
    *ppnode = pnode;
}

/* print everything in the bucket */

void printBucket(Node *p) {
    while(p != NULL) {
        printf("<%d,%d>\n", p->x, p->y);
        p = p->next;
    }
}

/* walk through all the buckets which may be null */
void printBucketsInBlock(Block block) {
    int index = -1;
    for(index = 0; index < BLOCKSIZE; ++index) {
        if(block[index] == NULL) /* walk through block */
            continue;
        printBucket(block[index]);
    }
}

/* walk through the block pointers, which may be null */
void printSortedNodes() {
    /* buckets are set to NULL initially */
    int index = -1;

    for(index = 0; index < NUMBLOCKS; ++index) {
        if(blockarray[index] == NULL)
            continue;
        printBucketsInBlock(*blockarray[index]);
        //* index walks through blocks *//
    }

}

void main(int argc, char **argv) {
    FILE *f = NULL; /* illegal file descriptor */
    char *line = NULL;
    char *xstring = NULL;
    char *ystring = NULL;
    size_t n = 0;
    ssize_t retval = 0;
    regex_t    preg;
    int        rc = 0;
    size_t     nmatch = TWOSUBEX;
    regmatch_t pmatch[TWOSUBEX];
    const char *pattern = COORDINATEPATTERN;
    char *saved = NULL;

    if(argc != 2) {
        exit(EXIT_FAILURE);
    }

    memset(blockarray, 0, sizeof(blockarray));

    if (0 != (rc = regcomp(&preg, pattern, REG_EXTENDED))) {
        printf("regcomp() failed, returning nonzero (%d)\n", rc);
        exit(EXIT_FAILURE);
    }
    f = fopen(argv[1], "r");

    if(f == NULL){
        fprintf(stderr, "Bad file argument: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    /* clib regular expressions are slightly different from those in Java */
    /* check https://www.gnu.org/software/libc/manual/html_node/Regular-Expressions.html#Regular-Expressions */
    /* for more info */

    while(n = 0, line = NULL, retval = getline(&line, &n, f), retval > 0) {
        saved = line;
        while(rc = regexec(&preg, line, nmatch, pmatch, 0), rc == 0) {
            xstring = &line[pmatch[1].rm_so];
            ystring = &line[pmatch[2].rm_so];
            line[pmatch[1].rm_eo] = '\0';
            line[pmatch[2].rm_eo] = '\0';
            sortLineInput(xstring, ystring);
            line = &line[1 + pmatch[2].rm_eo];
        }
        free(saved); /* many versions of malloc&free only require the address argument to free be an address within original malloc'd block */
    }
    regfree(&preg);
    printSortedNodes();
    exit(EXIT_SUCCESS);
}

