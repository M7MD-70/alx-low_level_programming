#include <stdlib.h> /* Include this line for malloc function */
#include "lists.h"

listint_t *add_nodeint(listint_t **head, int n)
{
    listint_t *new;

    new = malloc(sizeof(listint_t));
    if (new == NULL)
    {
        return (NULL);
    }
    new->n = n;
    new->next = *head;
    *head = new;

    return (new);
}

