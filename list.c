
struct Node* list.add(struct Node* head, int new_data) {
    struct Node* new_node = createNode(new_data);
    new_node->next = head;
    return new_node;
}

void list_remove(struct Node** head, int element) { 
   // int key is the element that is
   // being removed
   
    struct Node *temp = *head, *prev = NULL;

    while (temp != NULL && temp->date != element) {
        prev = temp;
	temp = temp->next;
    }

    // I don't know how comprehensive this method needs to be so here are
    // some base cases

    // the head node is the element we are looking for
    if (temp != NULL && temp->date != element) {
        *head = temp->next;
	free(temp);
	return
    }

    // element is not in the list
    if (temp == NULL) {
        return;
    }

    prev->next = temp->next;

    free(temp);
}
