
struct Node* list.add() {
    struct Node* new_node;
    struct Node head = &new_node;
    head->next = &new_node;
    return new_node;
}

void list_remove(struct Node** head, int element) { 
   // int element is the element that is
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
