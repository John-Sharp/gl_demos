#include "actor.h"
#include <stdlib.h>

actor_list *actor_list_add(actor_list *al, actor *a)
{
    actor_list *new_node = malloc(sizeof(*new_node));
    new_node->a = a;
    new_node->next = al;
    return new_node;
}

actor *actor_init(
        actor *a,
        actor_render_handler render_handler,
        actor_logic_handler logic_handler)
{
    a->render_handler = render_handler;
    a->logic_handler = logic_handler;
    return a;
}
