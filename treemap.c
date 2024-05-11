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


void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
    TreeNode* newNode = createTreeNode(key, value);
    
    // Si el arbol esta vacío.
    if (tree->root == NULL)
    {
        tree->root = newNode;
        tree->current = newNode;
        return;
    }

    // Recorremos para encontrar la posición de inserción.
    TreeNode* temp = tree->root;
    TreeNode* parent = NULL;
    while (temp != NULL)
    {
        parent = temp;
        if (tree->lower_than(key, temp->pair->key))
        {
            temp = temp->left;
        }
        else if (tree->lower_than(key, temp->pair->key))
        {
            temp = temp->right;
        }
        else
        {
            // Silas claves son iguales, actualizamos el valor.
            temp->pair->value = value;
            free(newNode->pair); // Liberamos la memoria del par del nuevo nodo.
            free(newNode); // Liberamos la memoria del nuevo nodo.
            return;
        }
    }
    // Insertamos el nuevo nodo en la posición.
    newNode->parent = parent;
    if (tree->lower_than(key, parent->pair->key))
    {
        parent->left = newNode;
    }
    else
    {
        parent->right = newNode;
    }
    tree->current = newNode;
}

TreeNode * minimum(TreeNode * x)
{
    while(x->left != NULL)
    {
        x = x->left;
    }
    
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) 
{
    if (node->left == NULL && node->right == NULL)
    {
        if (node->parent->left == node)
        {
            node->parent->left = NULL;
        }

        if (node->parent->right == node)
        {
            node->parent->right = NULL;
        }
    }
    else if (node->left == NULL || node->right == NULL)
    {
        if (node->left != NULL)
        {
            if (node->parent->left == node)
            {
                node->parent->left = node->left;
                node->left->parent = node->parent;
            }

            if (node->parent->right == node)
            {
                node->parent->right = node->left;
                node->left->parent = node->parent;
            }
        }
        else if (node->right != NULL)
        {
            if (node->parent->left == node)
            {
                node->parent->left = node->right;
                node->right->parent = node->parent;
            }

            if (node->parent->right == node)
            {
                node->parent->right = node->right;
                node->right->parent = node->parent;
            }
        }
    }
    else if (node->left != NULL && node->right != NULL)
    {
        TreeNode* min = minimum(node->right);

        node->pair->key = min->pair->key;
        node->pair->value = min->pair->value;
        removeNode(tree, min);
    }
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
    TreeNode* temp = tree->current;
    TreeNode* successor = NULL;

    // Si el nodo actual tiene un hijo derecho, el sucesor será el menor de su subárbol derecho.
    if (temp->right != NULL)
    {
        successor = getMinimunNode(temp->right);
    }
    else
    {
        // Si no tiene hijo derecho, buscamos el sucesor en el subárbol izquierdo.
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
               break;  // Si encontramos un nodo igual, detenemos la busqueda.
            }
        }
    }
    // Actualizamos el nodo actual en el arbol.
    tree->current = successor;

    if (successor != NULL){
        return successor->pair;
    }
    return NULL;
}
