struct Node {
        int data;
        struct Node* next;
    };

struct Node* createNode(int new_data) {
    struct Node* new_node =
            (struct Node*)malloc(sizeof(struct Node))
    new_node->data = new_data;
    new_node->next = NULL;
    return new_node
    }

