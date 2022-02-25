#ifndef _TLDLIST_H_INCLUDED_
#define _TLDLIST_H_INCLUDED_

#include "date.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tldnode TLDNode;
typedef struct tldlist TLDList;

struct tldnode {
    TLDNode *parent;
    TLDNode *left;
    TLDNode *right;
    Date date;
    char domain;
    TLDList *list;
};

typedef struct tldlist {
    Date begin;
    Date end;
    TLDNode *root;
    long added;
} TLDList;

typedef struct tlditerator {
    TLDNode *node;
    TLDList *tld;
} TLDIterator;

/*
 * tldlist_create generates a list structure for storing counts against
 * top level domains (TLDs)
 *
 * creates a TLDList that is constrained to the `begin' and `end' Date's
 * returns a pointer to the list if successful, NULL if not
 */
TLDList *tldlist_create(Date *begin, Date *end) {
    TLDList *list_ptr = (TLDList *) malloc (sizeof(struct tldlist));
    if (list_ptr == NULL) {return NULL;}

    list_ptr->begin = *begin;
    list_ptr->end = *end;
    list_ptr->root = NULL;
    list_ptr->added = 0;

    return list_ptr;
}

/*
 * tldlist_destroy destroys the list structure in `tld'
 *
 * all heap allocated storage associated with the list is returned to the heap
 */
void tldlist_destroy(TLDList *tld){
    free(tld);
}

/*
 * tldlist_add adds the TLD contained in `hostname' to the tldlist if
 * `d' falls in the begin and end dates associated with the list;
 * returns 1 if the entry was counted, 0 if not
 */
int tldlist_add(TLDList *tld, char *hostname, Date *d) {
    if (date_compare(d, &tld->begin) < 0) { return 0; }
    if (date_compare(d, &tld->end) > 0) { return 0; }
    char ch = '.';
    char val = *strrchr(hostname, ch);
    //if (val == NULL) { return 0; }

    TLDNode *node_ptr = (TLDNode *) malloc (sizeof(struct tldnode));
    if (node_ptr == NULL) {return 0;}

    if(tld->root == NULL) {
        tld->root = node_ptr;
        tld->root->date = *d;
        tld->root->domain = val;
        tld->root->list = tld;
    }

    TLDNode *current = tld->root;
    while(1 == 1){
        if (date_compare(d, &current->date) > 0){
            if (current->right == NULL){
                current->right = node_ptr;
                current->right->date = *d;
                current->right->parent = current;
                current->right->domain = val;
                current->right->list = tld;
                tld->added = tld->added + 1;
                return 1;
            }
            else{
                current = current->right;
            }
        }
        else{
            if (current->left == NULL){
                current->left = node_ptr;
                current->left->date = *d;
                current->left->parent = current;
                current->left->domain = val;
                current->left->list = tld;
                tld->added = tld->added + 1;
                return 1;
            }
            else{
                current = current->left;
            }
        }
    }
}

/*
 * tldlist_count returns the number of successful tldlist_add() calls since
 * the creation of the TLDList
 */
long tldlist_count(TLDList *tld){
    return tld->added;
}

/*
 * tldlist_iter_create creates an iterator over the TLDList; returns a pointer
 * to the iterator if successful, NULL if not
 */
TLDIterator *tldlist_iter_create(TLDList *tld){
    TLDIterator *iter_ptr = (TLDIterator *) malloc (sizeof(struct tlditerator));
    if (iter_ptr == NULL) {return 0;}

    iter_ptr->tld = tld;
    TLDNode *node = tld->root;
    while(node->left != NULL){
        node = node->left;
    }
    iter_ptr->node = node;

    return iter_ptr;
}

/*
 * tldlist_iter_next returns the next element in the list; returns a pointer
 * to the TLDNode if successful, NULL if no more elements to return
 */
TLDNode *tldlist_iter_next(TLDIterator *iter) {
    Date val = iter->node->date;
    if (iter->node->right == NULL){
        if (iter->node->parent == NULL){
            return NULL;
        }
        while (1 == 1){
            iter->node = iter->node->parent;
            if(date_compare(&val, &iter->node->date) == 1){
                return iter->node;
            }
            else{
                if (iter->node->parent == NULL){
                    return NULL;
                }
            }
        }
    }
    else{
        iter->node = iter->node->parent;
        while (iter->node->left != NULL)
        {
            iter->node = iter->node->left;
        }
        return iter->node;
    }
}

/*
 * tldlist_iter_destroy destroys the iterator specified by `iter'
 */
void tldlist_iter_destroy(TLDIterator *iter){
    free(iter);
}

/*
 * tldnode_tldname returns the tld associated with the TLDNode
 */
char *tldnode_tldname(TLDNode *node){
    return &node->domain;
}

/*
 * tldnode_count returns the number of times that a log entry for the
 * corresponding tld was added to the list
 */
long tldnode_count(TLDNode *node){
    return node->list->added;
}

#endif /* _TLDLIST_H_INCLUDED_ */
