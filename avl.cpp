#include <iostream>
#include <vector>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    int height;
};

// Función para obtener la altura de un nodo
int height(Node* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

// Función para obtener el máximo de dos enteros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Función para crear un nuevo nodo
Node* newNode(int key) {
    Node* node = new Node();
    node->key = key;
    node->left = nullptr;
    node->right = nullptr;
    node->height = 1; // Nuevo nodo es inicialmente hoja
    return node;
}


// Función para encontrar el nodo con el valor mínimo (sucesor inmediato) en un subárbol
Node* minValueNode(Node* node) {
    Node* current = node;

    // Itera hacia la izquierda hasta encontrar el nodo hoja más a la izquierda
    while (current->left != nullptr)
        current = current->left;

    return current;
}


// Rotación a la derecha
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Realiza la rotación
    x->right = y;
    y->left = T2;

    // Actualiza las alturas
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Rotación a la izquierda
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Realiza la rotación
    y->left = x;
    x->right = T2;

    // Actualiza las alturas
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Obtiene el balance del nodo
int getBalance(Node* node) {
    if (node == nullptr)
        return 0;
    return height(node->left) - height(node->right);
}

// Función principal para insertar un nodo
Node* insert(Node* node, int key) {
// Inserta el nodo de manera normal
if (node == nullptr)
    return newNode(key);

if (key < node->key)
    node->left = insert(node->left, key);
else if (key > node->key)
    node->right = insert(node->right, key);
else // Claves iguales no están permitidas
    return node;

// Actualiza la altura del nodo padre
node->height = 1 + max(height(node->left), height(node->right));

// Obtiene el balance del nodo para verificar si se necesita reequilibrar
int balance = getBalance(node);

// Casos de rotación

// Caso izquierda-izquierda
if (balance > 1 && key < node->left->key)
    return rightRotate(node);

// Caso derecha-derecha
if (balance < -1 && key > node->right->key)
    return leftRotate(node);

// Caso izquierda-derecha
if (balance > 1 && key > node->left->key) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
}

// Caso derecha-izquierda
if (balance < -1 && key < node->right->key) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
}

// Retorna el nodo sin cambios
return node;
}

// Función para imprimir el árbol en orden
void inorder(Node* root) {
    if (root != nullptr) {
        inorder(root->left);
        cout << root->key << " ";
        inorder(root->right);
    }
}

// Función para eliminar un nodo
Node* deleteNode(Node* root, int key) {
    // Paso base: si el árbol está vacío
    if (root == nullptr)
        return root;

    // Si la clave a eliminar es menor que la clave del nodo actual, entonces está en el subárbol izquierdo
    if (key < root->key)
        root->left = deleteNode(root->left, key);

    // Si la clave a eliminar es mayor que la clave del nodo actual, entonces está en el subárbol derecho
    else if (key > root->key)
        root->right = deleteNode(root->right, key);

    // Si la clave a eliminar es igual a la clave del nodo actual, entonces este es el nodo que queremos eliminar
    else {
        // Nodo con solo un hijo o sin hijo
        if (root->left == nullptr || root->right == nullptr) {
            Node* temp = root->left ? root->left : root->right;

            // Sin hijos
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            }
            // Un hijo
            else
                *root = *temp; // Copia el contenido del hijo no vacío

            delete temp; // Libera la memoria del nodo eliminado
        } else {
            // Nodo con dos hijos: obtiene el sucesor inmediato (menor en el subárbol derecho)
            Node* temp = minValueNode(root->right);

            // Copia el sucesor inmediato al nodo actual
            root->key = temp->key;

            // Elimina el sucesor inmediato
            root->right = deleteNode(root->right, temp->key);
        }
    }

    // Si el árbol tenía solo un nodo, entonces retorna
    if (root == nullptr)
        return root;

    // Actualiza la altura del nodo actual
    root->height = 1 + max(height(root->left), height(root->right));

    // Obtiene el balance del nodo actual para verificar si se necesita reequilibrar
    int balance = getBalance(root);

    // Casos de rotación

    // Caso izquierda-izquierda
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Caso izquierda-derecha
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Caso derecha-derecha
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Caso derecha-izquierda
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}


// Función para construir un árbol AVL a partir de un vector de enteros
Node* buildAVLTree(vector<int>& nums) {
    Node* root = nullptr;

    // Inserta cada elemento del vector en el árbol AVL
    for (int num : nums) {
        root = insert(root, num);
    }

    return root;
}


int main() {
    Node* root = nullptr;

    // Inserta nodos de ejemplo
    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 30);
    root = insert(root, 40);
    root = insert(root, 50);
    root = insert(root, 25);
    root = insert(root, 35);

    // Imprime el árbol en orden
    cout << "Inorder traversal of the constructed AVL tree is:" << endl;
    inorder(root);

    vector<int> nums = {50, 30, 70, 20, 40, 60, 80};

    // Construye un árbol AVL a partir del vector de enteros
    Node* root2 = buildAVLTree(nums);

    // Imprime el árbol en orden
    cout << "Inorder traversal of the constructed AVL tree is:" << endl;
    inorder(root2);

    return 0;
}
