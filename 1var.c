#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура узла бинарного дерева
typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

// Функция для создания нового узла
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Структура для очереди (нужна для поиска места для вставки)
typedef struct Queue {
    Node* node;
    struct Queue* next;
} Queue;

// Добавление элемента в очередь
Queue* enqueue(Queue* rear, Node* node) {
    Queue* newElement = (Queue*)malloc(sizeof(Queue));
    newElement->node = node;
    newElement->next = NULL;
    if (rear != NULL) {
        rear->next = newElement;
    }
    return newElement;
}

// Удаление элемента из очереди
Queue* dequeue(Queue* front) {
    Queue* temp = front;
    front = front->next;
    free(temp);
    return front;
}

// Функция для добавления элемента в полное бинарное дерево
void insert(Node** root, int data) {
    Node* newNode = createNode(data);

    // Если дерево пусто
    if (*root == NULL) {
        *root = newNode;
        return;
    }

    // Используем очередь для поиска свободного места
    Queue* front = NULL;
    Queue* rear = NULL;

    // Добавляем корневой узел в очередь
    rear = enqueue(rear, *root);
    front = rear;

    // Поиск свободного места для вставки
    while (front != NULL) {
        Node* temp = front->node;
        front = dequeue(front);

        // Вставляем новый элемент в первое свободное место
        if (temp->left == NULL) {
            temp->left = newNode;
            break;
        } else {
            rear = enqueue(rear, temp->left);
        }

        if (temp->right == NULL) {
            temp->right = newNode;
            break;
        } else {
            rear = enqueue(rear, temp->right);
        }
    }
}

// Функция для поиска элемента в дереве
Node* search(Node* root, int data) {
    if (root == NULL || root->data == data)
        return root;
    
    Node* leftSearch = search(root->left, data);
    if (leftSearch != NULL) return leftSearch;

    return search(root->right, data);
}

// Функция для обхода дерева (In-order)
void inOrder(Node* root) {
    if (root == NULL)
        return;
    inOrder(root->left);
    printf("%d ", root->data);
    inOrder(root->right);
}

// Проверка на полное бинарное дерево (Full Binary Tree)
int isFullBinaryTree(Node* root) {
    if (root == NULL)
        return 1; // Пустое дерево считается полным
// Если у узла нет потомков
    if (root->left == NULL && root->right == NULL)
        return 1;

    // Если у узла есть оба потомка, рекурсивно проверяем их
    if (root->left != NULL && root->right != NULL)
        return isFullBinaryTree(root->left) && isFullBinaryTree(root->right);

    // Если у узла только один потомок, дерево не полное
    return 0;
}

// Основная программа с поддержкой командной строки
int main(int argc, char* argv[]) {
    Node* root = NULL;

    // Проверка количества аргументов
    if (argc < 4) {
        printf("Использование: ./full_binary_tree --query 'COMMAND'\n");
        return 1;
    }

    // Обработка команд
    if (strcmp(argv[2], "--query") == 0) {
        if (strncmp(argv[3], "TINSERT", 7) == 0) {
            int value;
            sscanf(argv[3] + 8, "%d", &value); // Получаем значение из команды
            insert(&root, value);
            printf("Добавлено %d в дерево.\n", value);
        }
        else if (strncmp(argv[3], "TGET", 4) == 0) {
            printf("Элементы дерева: ");
            inOrder(root);
            printf("\n");
        }
        else if (strncmp(argv[3], "TCHECK", 6) == 0) {
            if (isFullBinaryTree(root)) {
                printf("Дерево является полным.\n");
            } else {
                printf("Дерево не является полным.\n");
            }
        } else {
            printf("Неизвестная команда.\n");
        }
    }

    return 0;
}
