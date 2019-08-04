#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_DIM 35

// STRUCTURES
typedef struct entity t_ent;
typedef struct rel_list t_rel_list;
typedef struct rel_node t_rel_node;

struct entity {
    char id_ent[STR_DIM];
    struct entity *next;
    struct entity *prev;
};

struct rel_list {
    char id_rel[STR_DIM];
    t_rel_node *node_list;
    t_rel_node *last_node;
    struct rel_list *next;
    struct rel_list *prev;
};

struct rel_node {
    char id_dest[STR_DIM];
    int counter;
    t_ent *orig_list;
    t_ent *last_orig;
    struct rel_node *next;
    struct rel_node *prev;
};


// GLOBAL VARIABLES

t_ent *entity_list = NULL;
t_ent *last_ent_node = NULL;
t_rel_list *rel_list = NULL;
t_rel_list *last_rel_node = NULL;

void add_ent(char *id_ent);
void del_ent(char *id_ent);
void add_rel(char *id_orig, char *id_dest, char *id_rel);
void del_rel(char *id_orig, char *id_dest, char *id_rel);
void report();

// UTILS FUNCTIONS

t_ent *ent_get_next(char *id_ent, t_ent *ent_list) {
    t_ent *pnt = ent_list;
    if (pnt != NULL) {
        do {
            if (strcmp(pnt->id_ent, id_ent) > 0) {
                return pnt;
            }
            pnt = pnt->next;
        } while (pnt != NULL);
    }
    return NULL;
}

t_ent *is_in_ent(char *id_ent) {
    if (entity_list != NULL) {
        t_ent *pnt;
        pnt = entity_list;
        do {
            if (strcmp(pnt->id_ent, id_ent) == 0) {
                return pnt;
            }
            pnt = pnt->next;
        } while (pnt != NULL);
    }
    return NULL;
}

t_rel_list *is_in_rel(char *id_rel) {
    if (rel_list != NULL) {
        t_rel_list *pnt = rel_list;
        do {
            if (strcmp(pnt->id_rel, id_rel) == 0) {
                return pnt;
            }
            pnt = pnt->next;
        } while (pnt != NULL);
    }
    return NULL;
}

t_rel_list *rel_get_next(char *id_rel) {
    t_rel_list *pnt = rel_list;
    if (pnt != NULL) {
        do {
            if (strcmp(pnt->id_rel, id_rel) > 0) {
                return pnt;
            }
            pnt = pnt->next;
        } while (pnt != NULL);
    }
    return NULL;
}

t_rel_node *is_in_dest(char *id_dest, t_rel_node *dest_list) {
    if (dest_list != NULL) {
        t_rel_node *pnt = dest_list;
        do {
            if (strcmp(pnt->id_dest, id_dest) == 0) {
                return pnt;
            }
            pnt = pnt->next;
        } while (pnt != NULL);
    }
    return NULL;
}

t_ent *is_in_orig(char *id_orig, t_ent *origin_list) {
    if (origin_list != NULL) {
        t_ent *pnt = origin_list;
        do {
            if (strcmp(pnt->id_ent, id_orig) == 0) {
                return pnt;
            }
            pnt = pnt->next;
        } while (pnt != NULL);
    }
    return NULL;
}

void insert_ent(char *id_ent, t_ent *prev, t_ent *next) {
    t_ent *new_ent = (t_ent *) malloc(sizeof(t_ent));
    strcpy(new_ent->id_ent, id_ent);
    if (prev == NULL && next == NULL) {
        new_ent->prev = NULL;
        new_ent->next = NULL;
        entity_list = new_ent;
        last_ent_node = new_ent;
    } else if (prev == NULL && next != NULL) {
        new_ent->prev = NULL;
        new_ent->next = entity_list;
        entity_list->prev = new_ent;
        entity_list = new_ent;
    } else if (prev != NULL && next == NULL) {
        new_ent->prev = last_ent_node;
        new_ent->next = NULL;
        last_ent_node->next = new_ent;
        last_ent_node = new_ent;
    } else if (prev != NULL && next != NULL) {
        new_ent->prev = prev;
        new_ent->next = next;
        prev->next = new_ent;
        next->prev = new_ent;
    }
}

t_rel_list *insert_rel(char *id_rel, t_rel_list *prev, t_rel_list *next) {
    t_rel_list *new_rel = (t_rel_list *) malloc(sizeof(t_rel_list));
    strcpy(new_rel->id_rel, id_rel);
    new_rel->node_list = NULL;
    new_rel->last_node = NULL;
    if (prev == NULL && next == NULL) {
        new_rel->prev = NULL;
        new_rel->next = NULL;
        rel_list = new_rel;
        last_rel_node = new_rel;
    } else if (prev == NULL && next != NULL) {
        new_rel->prev = NULL;
        new_rel->next = rel_list;
        rel_list->prev = new_rel;
        rel_list = new_rel;
    } else if (prev != NULL && next == NULL) {
        new_rel->prev = last_rel_node;
        new_rel->next = NULL;
        last_rel_node->next = new_rel;
        last_rel_node = new_rel;
    } else if (prev != NULL && next != NULL) {
        new_rel->prev = prev;
        new_rel->next = next;
        prev->next = new_rel;
        next->prev = new_rel;
    }
    return new_rel;
}

t_rel_node *insert_dest(t_rel_list *relation, char *id_dest) {
    t_rel_node *new_dest = (t_rel_node *) malloc(sizeof(t_rel_node));
    strcpy(new_dest->id_dest, id_dest);
    new_dest->counter = 0;
    new_dest->orig_list = NULL;
    new_dest->last_orig = NULL;
    if (relation->last_node == NULL) {
        new_dest->prev = NULL;
        new_dest->next = NULL;
        relation->node_list = new_dest;
        relation->last_node = new_dest;
    } else {
        new_dest->prev = relation->last_node;
        new_dest->next = NULL;
        relation->last_node->next = new_dest;
        relation->last_node = new_dest;
    }
    return new_dest;
}

t_ent *insert_orig(t_rel_list *relation, t_rel_node *dest, char *id_orig, t_ent *prev, t_ent *next) {
    t_ent *new_orig = (t_ent *) malloc(sizeof(t_ent));
    strcpy(new_orig->id_ent, id_orig);
    dest->counter = dest->counter + 1;
    if (prev == NULL && next == NULL) {
        new_orig->prev = NULL;
        new_orig->next = NULL;
        dest->orig_list = new_orig;
        dest->last_orig = new_orig;
    } else if (prev == NULL && next != NULL) {
        new_orig->prev = NULL;
        new_orig->next = dest->orig_list;
        dest->orig_list->prev = new_orig;
        dest->orig_list = new_orig;
    } else if (prev != NULL && next == NULL) {
        new_orig->prev = dest->last_orig;
        new_orig->next = NULL;
        dest->last_orig->next = new_orig;
        dest->last_orig = new_orig;
    } else if (prev != NULL && next != NULL) {
        new_orig->prev = prev;
        new_orig->next = next;
        prev->next = new_orig;
        next->prev = new_orig;
    }

    if (dest->prev != NULL && (dest->prev->counter != dest->counter || (dest->prev->counter == dest->counter &&
                                                                        strcmp(dest->prev->id_dest, dest->id_dest) >
                                                                        0))) {
        if (dest->counter == relation->node_list->counter) {
            t_rel_node *dest_pnt = relation->node_list;
            while (dest_pnt->counter == dest->counter && strcmp(dest_pnt->id_dest, dest->id_dest) < 0) {
                dest_pnt = dest_pnt->next;
            }
            if (dest_pnt->prev == NULL) {
                if (dest->next == NULL) {
                    dest->prev->next = NULL;
                    relation->last_node = dest->prev;
                } else {
                    dest->prev->next = dest->next;
                    dest->next->prev = dest->prev;
                }
                dest->prev = NULL;
                dest->next = relation->node_list;
                relation->node_list->prev = dest;
                relation->node_list = dest;
            } else {
                if (dest->next == NULL) {
                    dest->prev->next = NULL;
                    relation->last_node = dest->prev;
                } else {
                    dest->prev->next = dest->next;
                    dest->next->prev = dest->prev;
                }
                dest->prev = dest_pnt->prev;
                dest->next = dest_pnt;
                dest_pnt->prev->next = dest;
                dest_pnt->prev = dest;
            }
        } else if (dest->counter > relation->node_list->counter) {
            if (dest->next == NULL) {
                dest->prev->next = NULL;
                relation->last_node = dest->prev;
            } else {
                dest->prev->next = dest->next;
                dest->next->prev = dest->prev;
            }
            dest->prev = NULL;
            dest->next = relation->node_list;
            relation->node_list->prev = dest;
            relation->node_list = dest;
        }
    }

    return new_orig;
}

void dest_sort(t_rel_list *relation){
    int max_counter = relation->node_list->counter;
    t_rel_node *dest_pnt = relation->node_list;
    do {
        if (dest_pnt->counter > max_counter) {
            t_rel_node *dest_to_move = dest_pnt;
            dest_pnt = dest_pnt->next;
            max_counter = dest_pnt->counter;
            if (dest_to_move->next == NULL) {
                dest_to_move->prev->next = NULL;
                relation->last_node = dest_to_move->prev;
            } else {
                dest_to_move->prev->next = dest_to_move->next;
                dest_to_move->next->prev = dest_to_move->prev;
            }
            dest_to_move->prev = NULL;
            dest_to_move->next = relation->node_list;
            relation->node_list->prev = dest_to_move;
            relation->node_list = dest_to_move;
        } else if (dest_pnt->counter == max_counter && dest_pnt->prev != NULL) {
            t_rel_node *dest_to_move = dest_pnt;
            dest_pnt = dest_pnt->next;
            if (dest_to_move->prev->counter != dest_to_move->counter ||
                (dest_to_move->prev->counter == dest_to_move->counter && strcmp(dest_to_move->prev->id_dest, dest_to_move->id_dest) > 0)) {
                t_rel_node *order_dest_pnt = relation->node_list;
                while (order_dest_pnt->counter == dest_to_move->counter &&
                       strcmp(order_dest_pnt->id_dest, dest_to_move->id_dest) < 0) {
                    order_dest_pnt = order_dest_pnt->next;
                }

                if (order_dest_pnt->prev == NULL) {
                    if (dest_to_move->next == NULL) {
                        dest_to_move->prev->next = NULL;
                        relation->last_node = dest_to_move->prev;
                    } else {
                        dest_to_move->prev->next = dest_to_move->next;
                        dest_to_move->next->prev = dest_to_move->prev;
                    }
                    dest_to_move->prev = NULL;
                    dest_to_move->next = relation->node_list;
                    relation->node_list->prev = dest_to_move;
                    relation->node_list = dest_to_move;
                } else {
                    if (dest_to_move->next == NULL) {
                        dest_to_move->prev->next = NULL;
                        relation->last_node = dest_to_move->prev;
                    } else {
                        dest_to_move->prev->next = dest_to_move->next;
                        dest_to_move->next->prev = dest_to_move->prev;
                    }
                    dest_to_move->prev = order_dest_pnt->prev;
                    dest_to_move->next = order_dest_pnt;
                    order_dest_pnt->prev->next = dest_to_move;
                    order_dest_pnt->prev = dest_to_move;
                }
            }
        } else {
            dest_pnt = dest_pnt->next;
        }
    } while (dest_pnt != NULL);
}

void delete_orig(t_rel_list *relation, t_rel_node *dest, t_ent *orig_to_del) {
    if (orig_to_del->prev != NULL && orig_to_del->next != NULL) {
        orig_to_del->prev->next = orig_to_del->next;
        orig_to_del->next->prev = orig_to_del->prev;
    } else if (orig_to_del->prev == NULL && orig_to_del->next != NULL) {
        orig_to_del->next->prev = NULL;
        dest->orig_list = orig_to_del->next;
    } else if (orig_to_del->prev != NULL && orig_to_del->next == NULL) {
        orig_to_del->prev->next = NULL;
        dest->last_orig = orig_to_del->prev;
    } else if (orig_to_del->prev == NULL && orig_to_del->next == NULL) {
        dest->orig_list = NULL;
        dest->last_orig = NULL;
    }
    free(orig_to_del);
    dest->counter = dest->counter - 1;

    if (dest->prev == NULL && dest->next != NULL) {
            if (dest->counter + 1 == dest->next->counter) {
                dest->next->prev = NULL;
                relation->node_list = dest->next;
                dest->prev = relation->last_node;
                dest->next = NULL;
                relation->last_node = dest;
            } else {
                dest_sort(relation);
            }
    } else if (dest->counter + 1 == relation->node_list->counter) {
        if (dest->next != NULL) {
            dest->prev->next = dest->next;
            dest->next->prev = dest->prev;
            dest->prev = relation->last_node;
            dest->next = NULL;
            relation->last_node = dest;
        }
    }
}

// REQUIRED FUNCTIONS

void add_ent(char *id_ent) {
    t_ent *next_ent = ent_get_next(id_ent, entity_list);
    if (next_ent == NULL && last_ent_node == NULL) {
        insert_ent(id_ent, NULL, NULL);
    } else if (next_ent == NULL && last_ent_node != NULL) {
        if (strcmp(last_ent_node->id_ent, id_ent) != 0) {
            insert_ent(id_ent, last_ent_node, NULL);
        }
    } else if (next_ent != NULL) {
        if (next_ent->prev != NULL) {
            if (strcmp(next_ent->prev->id_ent, id_ent) != 0) {
                insert_ent(id_ent, next_ent->prev, next_ent);
            }
        } else {
            insert_ent(id_ent, NULL, entity_list);
        }
    }
}

void del_ent(char *id_ent) {
    t_ent *ent_to_del = is_in_ent(id_ent);
    if (ent_to_del != NULL) {
        if (rel_list != NULL) {
            t_rel_list *rel_pnt = rel_list;
            do {
                t_rel_node *dest_pnt = rel_pnt->node_list;
                if (dest_pnt != NULL) {
                    do {
                        if (strcmp(dest_pnt->id_dest, id_ent) == 0) {
                            if (dest_pnt->prev == NULL && dest_pnt->next == NULL) {
                                rel_pnt->node_list = NULL;
                                rel_pnt->last_node = NULL;
                            } else if (dest_pnt->prev == NULL && dest_pnt->next != NULL) {
                                if(dest_pnt->counter == dest_pnt->next->counter){
                                    dest_pnt->next->prev = NULL;
                                    rel_pnt->node_list = dest_pnt->next;
                                } else{
                                    dest_pnt->next->prev = NULL;
                                    rel_pnt->node_list = dest_pnt->next;
                                    dest_sort(rel_pnt);
                                }
                            } else if (dest_pnt->prev != NULL && dest_pnt->next == NULL) {
                                dest_pnt->prev->next = NULL;
                                rel_pnt->last_node = dest_pnt->prev;
                            } else if (dest_pnt->prev != NULL && dest_pnt->next != NULL) {
                                dest_pnt->prev->next = dest_pnt->next;
                                dest_pnt->next->prev = dest_pnt->prev;
                            }
                            free(dest_pnt);
                        } else {
                            t_ent *orig = is_in_orig(id_ent, dest_pnt->orig_list);
                            if (orig != NULL) {
                                delete_orig(rel_pnt, dest_pnt, orig);
                            }
                        }
                        dest_pnt = dest_pnt->next;
                    } while (dest_pnt != NULL);
                }
                rel_pnt = rel_pnt->next;
            } while (rel_pnt != NULL);
        }

        if (ent_to_del->prev == NULL && ent_to_del->next == NULL) {
            entity_list = NULL;
            last_ent_node = NULL;
            free(ent_to_del);
        } else if (ent_to_del->prev == NULL && ent_to_del->next != NULL) {
            ent_to_del->next->prev = NULL;
            entity_list = ent_to_del->next;
            free(ent_to_del);
        } else if (ent_to_del->prev != NULL && ent_to_del->next == NULL) {
            ent_to_del->prev->next = NULL;
            last_ent_node = ent_to_del->prev;
            free(ent_to_del);
        } else if (ent_to_del->prev != NULL && ent_to_del->next != NULL) {
            ent_to_del->prev->next = ent_to_del->next;
            ent_to_del->next->prev = ent_to_del->prev;
            free(ent_to_del);
        }
    }
}

void add_rel(char *id_orig, char *id_dest, char *id_rel) {
    if (is_in_ent(id_orig) != NULL && is_in_ent(id_dest) != NULL) {
        t_rel_list *next_rel = rel_get_next(id_rel);
        if (next_rel == NULL && last_rel_node == NULL) {
            //RELAZIONE: lista vuota
            t_rel_list *new_rel = insert_rel(id_rel, NULL, NULL);
            t_rel_node *new_dest = insert_dest(new_rel, id_dest);
            insert_orig(new_rel, new_dest, id_orig, NULL, NULL);
        } else if (next_rel == NULL && last_rel_node != NULL) {
            if (strcmp(last_rel_node->id_rel, id_rel) == 0) {
                //RELAZIONE: presente in coda -> controllare le destinazioni
                t_rel_node *dest = is_in_dest(id_dest, last_rel_node->node_list);
                if (dest == NULL) {
                    //DESTINAZIONE: aggiungere in coda
                    t_rel_node *new_dest = insert_dest(last_rel_node, id_dest);
                    insert_orig(last_rel_node, new_dest, id_orig, NULL, NULL);
                } else {
                    //DESTINAZIONE: presente -> controllare origini
                    t_ent *next_orig = ent_get_next(id_orig, dest->orig_list);
                    if (next_orig == NULL && dest->last_orig == NULL) {
                        //ORIGINI: lista vuota
                        insert_orig(last_rel_node, dest, id_orig, NULL, NULL);
                    } else if (next_orig == NULL && dest->last_orig != NULL) {
                        if (strcmp(dest->last_orig->id_ent, id_orig) != 0) {
                            //ORIGINI: aggiungere in coda
                            insert_orig(last_rel_node, dest, id_orig, dest->last_orig, NULL);
                        }
                    } else if (next_orig != NULL) {
                        if (next_orig->prev != NULL) {
                            if (strcmp(next_orig->prev->id_ent, id_orig) != 0) {
                                //ORIGINI: aggiungere in mezzo
                                insert_orig(last_rel_node, dest, id_orig, next_orig->prev, next_orig);
                            }
                        } else {
                            //ORIGINI: aggiungere in testa
                            insert_orig(last_rel_node, dest, id_orig, NULL, dest->orig_list);
                        }
                    }
                }
            } else {
                //RELAIONE: aggiungere in coda
                t_rel_list *new_rel = insert_rel(id_rel, last_rel_node, NULL);
                t_rel_node *new_dest = insert_dest(new_rel, id_dest);
                insert_orig(new_rel, new_dest, id_orig, NULL, NULL);
            }
        } else if (next_rel != NULL) {
            if (next_rel->prev == NULL) {
                //RELAZIONE: aggiungere in testa
                t_rel_list *new_rel = insert_rel(id_rel, NULL, rel_list);
                t_rel_node *new_dest = insert_dest(new_rel, id_dest);
                insert_orig(new_rel, new_dest, id_orig, NULL, NULL);
            } else {
                if (strcmp(next_rel->prev->id_rel, id_rel) == 0) {
                    t_rel_node *dest = is_in_dest(id_dest, next_rel->prev->node_list);
                    if (dest == NULL) {
                        //DESTINAZIONE: aggiungere in coda
                        t_rel_node *new_dest = insert_dest(next_rel->prev, id_dest);
                        insert_orig(next_rel->prev, new_dest, id_orig, NULL, NULL);
                    } else {
                        //DESTINAZIONE: presente -> controllare origini
                        t_ent *next_orig = ent_get_next(id_orig, dest->orig_list);
                        if (next_orig == NULL && dest->last_orig == NULL) {
                            //ORIGINI: lista vuota
                            insert_orig(next_rel->prev, dest, id_orig, NULL, NULL);
                        } else if (next_orig == NULL && dest->last_orig != NULL) {
                            if (strcmp(dest->last_orig->id_ent, id_orig) != 0) {
                                //ORIGINI: aggiungere in coda
                                insert_orig(next_rel->prev, dest, id_orig, dest->last_orig, NULL);
                            }
                        } else if (next_orig != NULL) {
                            if (next_orig->prev != NULL) {
                                if (strcmp(next_orig->prev->id_ent, id_orig) != 0) {
                                    //ORIGINI: aggiungere in mezzo
                                    insert_orig(next_rel->prev, dest, id_orig, next_orig->prev,
                                                next_orig);
                                }
                            } else {
                                //ORIGINI: aggiungere in testa
                                insert_orig(next_rel->prev, dest, id_orig, NULL, dest->orig_list);
                            }
                        }
                    }
                } else {
                    //RELAZIONE: aggiugere tra next_rel->prev e next_rel
                    t_rel_list *new_rel = insert_rel(id_rel, next_rel->prev, next_rel);
                    t_rel_node *new_dest = insert_dest(new_rel, id_dest);
                    insert_orig(new_rel, new_dest, id_orig, NULL, NULL);
                }
            }
        }
    }
}

void del_rel(char *id_orig, char *id_dest, char *id_rel) {
    if (is_in_ent(id_orig) != NULL && is_in_ent(id_dest) != NULL) {
        t_rel_list *relation = is_in_rel(id_rel);
        if (relation != NULL) {
            t_rel_node *dest = is_in_dest(id_dest, relation->node_list);
            if (dest != NULL) {
                t_ent *orig = is_in_orig(id_orig, dest->orig_list);
                if (orig != NULL) {
                    delete_orig(relation, dest, orig);
                }
            }
        }
    }
}

void report() {
    int prev_max_counter = 0;
    if (rel_list != NULL) {
        t_rel_list *rel_pnt = rel_list;
        do {
            if (rel_pnt->node_list != NULL) {
                if (rel_pnt->node_list->counter != 0) {
                    if (rel_pnt->prev != NULL && prev_max_counter != 0) {
                        printf(" ");
                    }
                    printf("%s ", rel_pnt->id_rel);
                    t_rel_node *dest_pnt = rel_pnt->node_list;
                    do {
                        printf("%s ", dest_pnt->id_dest);
                        dest_pnt = dest_pnt->next;
                    } while (dest_pnt != NULL && dest_pnt->counter == rel_pnt->node_list->counter);
                    printf("%d;", rel_pnt->node_list->counter);
                    prev_max_counter = rel_pnt->node_list->counter;
                }
            }
            rel_pnt = rel_pnt->next;
        } while (rel_pnt != NULL);
    } else {
        printf("none");
    }
    printf("\n");
}

int main() {

    char input[STR_DIM];
    char ent_name[STR_DIM];
    char orig_name[STR_DIM];
    char dest_name[STR_DIM];
    char rel_id[STR_DIM];

    while (strcmp(input, "end") != 0) {
        if (strcmp(input, "addent") == 0) {
            scanf("%s", ent_name);
            //printf("add_ent(%s);\n", ent_name);
            add_ent(ent_name);
        } else if (strcmp(input, "delent") == 0) {
            scanf("%s", ent_name);
            //printf("del_ent(%s);\n", ent_name);
            del_ent(ent_name);
        } else if (strcmp(input, "addrel") == 0) {
            scanf("%s%s%s", orig_name, dest_name, rel_id);
            //printf("add_rel(%s, %s, %s);\n", orig_name, dest_name, rel_id);
            add_rel(orig_name, dest_name, rel_id);
        } else if (strcmp(input, "delrel") == 0) {
            scanf("%s%s%s", orig_name, dest_name, rel_id);
            //printf("del_rel(%s, %s, %s);\n", orig_name, dest_name, rel_id);
            del_rel(orig_name, dest_name, rel_id);
        } else if (strcmp(input, "report") == 0) {
            //printf("report();\n");
            report();
        }
        scanf("%s", input);
    }

    return 0;
}
