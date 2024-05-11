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
    // Mientras haya un nodo izquierdo, continua moviendose a la izquierda.
    while(x->left != NULL)
    {
        x = x->left;
    }
    
    return x; // Devuelve el nodo más a la izquierda (mínimo).
}


void removeNode(TreeMap * tree, TreeNode* node) 
{
    // Si el nodo no tiene hijos.
    if (node->left == NULL && node->right == NULL)
    {
        // Si el nodo es el hijo izquierdo del padre, elimina la referencia del padre al nodo
        if (node->parent->left == node)
        {
            node->parent->left = NULL;
        }
        // Si el nodo es hijo derecho del padre, elimina la referencia del padre al nodo
        if (node->parent->right == node)
        {
            node->parent->right = NULL;
        }
    }
    // Si el nodo tiene solo un hijo.
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
    // Si el nodo tiene dos hijos.
    else if (node->left != NULL && node->right != NULL)
    {
        // Encuentra el nodo mínimo en el subárbol derecho.
        TreeNode* min = minimum(node->right);

        // Copia el valor del nodo mínimo al nodo actual.
        node->pair->key = min->pair->key;
        node->pair->value = min->pair->value;
        removeNode(tree, min); // Elimina el nodo mínimo (que esta duplicado) del subárbol derecho.
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
        // Si las claves son iguales, hemos encontrado el nodo.
        if(is_equal(tree,temp->pair->key,key))
        {
            tree->current = temp;
            return temp->pair;
        }
        else
        {
            // Si la clave en menor, se mueve a la izquierda.
            if(tree->lower_than(key,temp->pair->key) == 1)
            {
                temp = temp->left;
            }
            // Si la clave es mayor, se mueve a la derecha.
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
    if (tree == NULL || tree->root == NULL) return NULL; // Si el árbol está vacío, retorna NULL.
    
    TreeNode* temp = tree->root;
    TreeNode* upbd_node = NULL;

    while (temp != NULL) 
    {
        // Si encontramos una clave igual a la clave buscada.
        if (is_equal(tree, temp->pair->key, key))
        {
            tree->current = temp;
            return temp->pair;
        }
        // Si la clave del nodo actual es menor que la clave buscada.
        else if (tree->lower_than(temp->pair->key, key))
        {
            temp = temp->right; // Buscamos en el subárbol derecho.
        }
        // Si la clave del nodo actual es mayor que la clave buscada.
        else
        {
            upbd_node = temp;
            temp = temp->left; // Buscamos en el subárbol izquierdo.
        }
    }

    // Si se encontró un nodo superior
    if (upbd_node != NULL)
    {
        tree->current = upbd_node;
        return upbd_node->pair;
    }
    else
    {
        return NULL;
    }
}

Pair * firstTreeMap(TreeMap * tree) 
{
    if (tree == NULL || tree->root == NULL) return NULL;
    
    TreeNode* temp = tree->root;
    
    // Mientras haya un nodo izquierdo, avanza hacia la izquierda.
    while(temp->left != NULL)
    {
        temp = temp->left;
    }
    
    tree->current = temp; // Actualiza el nodo actual del arbol al nodo mas a la izquierda.
    return temp->pair;
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

    // Si el nodo actual tiene un hijo derecho.
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
