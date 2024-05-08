#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) 
{
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {

}

TreeNode * minimum(TreeNode * x){

    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) 
{
    TreeNode* temp = tree->root;
    while(temp != NULL)
    {
        if(is_equal(tree,temp->pair->key,key))
        {
            tree->current = temp;
            return temp->pair;
        }
        else
        {
            if(tree->lower_than(key,temp->pair->key) == 1)
            {
                temp = temp->left;
            }
            else
            {
                temp = temp->right;
            }
        }
    }
    return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) 
{
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

TreeNode* getMinimunNode(TreeNode* node)
{
    if (node == NULL) return NULL;
    
    TreeNode* temp = node;
    while(temp->left != NULL)
    {
        temp = temp->left;
    }
    return temp;
}

Pair * nextTreeMap(TreeMap * tree) 
{
    if (tree == NULL || tree->root == NULL) return NULL;

    TreeNode* temp = tree->current;
    TreeNode* successor = NULL;

    if (temp->right != NULL)
    {
        successor = getMinimunNode(temp->right);
    }
    else
    {
        TreeNode* current = tree->root;
        while (current != NULL)
        {
            if (tree->lower_than(temp->pair->key, current->pair->key))
            {
                successor = current;
                current = current->left;
            }
            else if (tree->lower_than(current->pair->key, temp->pair->key))
            {
                current = current->right;
            }
            else
            {
               break; 
            }
        }
    }
    tree->current = successor;

    if (successor != NULL){
        return successor->pair;
    }
    return NULL;
}
