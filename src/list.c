#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node buf[10];
struct Node* head = NULL;

struct Node* list_add() {
    struct Node* new_node = &buf[0];
    new_node->data = 3; // picked three as new element being added 
    new_node->next = &buf[1];

    return new_node;
}



void list_remove() { 
    head = head->next;
}

int main() {
    list_add();
    list_remove();
}
