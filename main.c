#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_DIM 35
#define RED 0
#define BLACK 1

// STRUCTURES
typedef struct ent_tree t_ent;
typedef struct rel_list t_rel;
typedef struct dest_tree t_dest;

struct rel_list {
    char id_rel[STR_DIM];
    t_dest *dest_list;
    t_dest *last_dest;
    struct rel_list *next;
    struct rel_list *prev;
};

struct dest_tree {
    char id_dest[STR_DIM];
    int counter;
    t_ent *orig_root;
    struct dest_tree *next;
    struct dest_tree *prev;
};

struct ent_tree {
    char id_ent[STR_DIM];
    int color;
    struct ent_tree *p;
    struct ent_tree *left;
    struct ent_tree *right;
};


// GLOBAL VARIABLES

t_ent *ent_nil;
t_ent *ent_root;
t_rel *rel_root = NULL;
t_rel *last_rel = NULL;


void add_ent(char *id_ent);

void del_ent(char *id_ent);

void add_rel(char *id_orig, char *id_dest, char *id_rel);

void del_rel(char *id_orig, char *id_dest, char *id_rel);

void report();

// UTILS FUNCTIONS

t_ent *ent_minimum(t_ent *x) {
    while (x->left != ent_nil) {
        x = x->left;
    }
    return x;
}

t_ent *ent_successor(t_ent *x) {
    if (x->right != ent_nil) {
        return ent_minimum(x->right);
    }
    t_ent *y = x->p;
    while (y != ent_nil && x == y->right) {
        x = y;
        y = y->p;
    }
    return y;
}

t_ent *ent_left_rotate(t_ent *root, t_ent *x) {
    t_ent *y = x->right;
    x->right = y->left;

    if (y->left != ent_nil) {
        y->left->p = x;
    }
    y->p = x->p;
    if (x->p == ent_nil) {
        root = y;
    } else if (x == x->p->left) {
        x->p->left = y;
    } else {
        x->p->right = y;
    }
    y->left = x;
    x->p = y;
    return root;
}

t_ent *ent_right_rotate(t_ent *root, t_ent *y) {
    t_ent *x = y->left;
    y->left = x->right;
    if (x->right != ent_nil) {
        x->right->p = y;
    }
    x->p = y->p;
    if (y->p == ent_nil) {
        root = x;
    } else if (y == y->p->left) {
        y->p->left = x;
    } else {
        y->p->right = x;
    }
    x->right = y;
    y->p = x;
    return root;
}

t_ent *ent_insert_fixup(t_ent *root, t_ent *z) {
    if (z == root) {
        root->color = BLACK;
    } else {
        t_ent *x = z->p;
        if (x->color == RED) {
            if (x == x->p->left) {
                t_ent *y = x->p->right;
                if (y->color == RED) {
                    x->color = BLACK;
                    y->color = BLACK;
                    x->p->color = RED;
                    root = ent_insert_fixup(root, x->p);
                } else {
                    if (z == x->right) {
                        z = x;
                        root = ent_left_rotate(root, z);
                        x = z->p;
                    }
                    x->color = BLACK;
                    x->p->color = RED;
                    root = ent_right_rotate(root, x->p);
                }
            } else {
                t_ent *y = x->p->left;
                if (y->color == RED) {
                    x->color = BLACK;
                    y->color = BLACK;
                    x->p->color = RED;
                    root = ent_insert_fixup(root, x->p);
                } else {
                    if (z == x->left) {
                        z = x;
                        root = ent_right_rotate(root, z);
                        x = z->p;
                    }
                    x->color = BLACK;
                    x->p->color = RED;
                    root = ent_left_rotate(root, x->p);
                }
            }
        }
    }
    return root;
}

t_ent *insert_ent(t_ent *root, char *id_ent) {
    t_ent *y = ent_nil;
    t_ent *x = root;
    while (x != ent_nil) {
        y = x;
        if (strcmp(id_ent, x->id_ent) == 0) {
            return root;
        } else if (strcmp(id_ent, x->id_ent) < 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    t_ent *z = (t_ent *) malloc(sizeof(t_ent));
    strcpy(z->id_ent, id_ent);
    z->p = y;
    if (y == ent_nil) {
        root = z;
    } else if (strcmp(z->id_ent, y->id_ent) < 0) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = ent_nil;
    z->right = ent_nil;
    z->color = RED;
    return ent_insert_fixup(root, z);
}

t_ent *ent_delete_fixup(t_ent *root, t_ent *x) {
    t_ent *w;
    if (x->color == RED || x->p == ent_nil) {
        x->color = BLACK;
    } else if (x == x->p->left) {
        w = x->p->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->p->color = RED;
                root = ent_left_rotate(root, x->p);
                w = x->p->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                root = ent_delete_fixup(root, x->p);
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    root = ent_right_rotate(root, w);
                    w = w->p->right;
                }
                w->color = x->p->color;
                x->p->color = BLACK;
                w->right->color = BLACK;
                root = ent_left_rotate(root, x->p);
            }
    } else {
        w = x->p->left;
        if (w->color == RED) {
            w->color = BLACK;
            x->p->color = RED;
            root = ent_right_rotate(root, x->p);
            w = x->p->left;
        }
        if (w->left->color == BLACK && w->right->color == BLACK) {
            w->color = RED;
            root = ent_delete_fixup(root, x->p);
        } else {
            if (w->left->color == BLACK) {
                w->right->color = BLACK;
                w->color = RED;
                root = ent_left_rotate(root, w);
                w = w->p->left;
            }
            w->color = x->p->color;
            x->p->color = BLACK;
            w->left->color = BLACK;
            root = ent_right_rotate(root, x->p);
        }
    }
    return root;
}

t_ent *delete_ent(t_ent *root, t_ent *z) {
    t_ent *y, *x;
    if (z->left == ent_nil || z->right == ent_nil) {
        y = z;
    } else {
        y = ent_successor(z);
    }
    if (y->left != ent_nil) {
        x = y->left;
    } else {
        x = y->right;
    }
    x->p = y->p;
    if (y->p == ent_nil) {
        root = x;
    } else if (y == y->p->left) {
        y->p->left = x;
    } else {
        y->p->right = x;
    }
    if (y != z) {
        strcpy(z->id_ent, y->id_ent);
    }
    if ( y->color == BLACK) {
        root = ent_delete_fixup(root, x);
    }
    free(y);
    return root;
}

t_ent *is_in_ent(t_ent *x, char *id_ent) {
    if (x == ent_nil || strcmp(id_ent, x->id_ent) == 0) {
        return x;
    }
    if (strcmp(id_ent, x->id_ent) < 0) {
        return is_in_ent(x->left, id_ent);
    } else {
        return is_in_ent(x->right, id_ent);
    }
}

t_rel *insert_rel(char *id_rel, t_rel *prev, t_rel *next) {
    t_rel *new_rel = (t_rel *) malloc(sizeof(t_rel));
    strcpy(new_rel->id_rel, id_rel);
    new_rel->dest_list = NULL;
    new_rel->last_dest = NULL;
    if (prev == NULL && next == NULL) {
        new_rel->prev = NULL;
        new_rel->next = NULL;
        rel_root = new_rel;
        last_rel = new_rel;
    } else if (prev == NULL && next != NULL) {
        new_rel->prev = NULL;
        new_rel->next = rel_root;
        rel_root->prev = new_rel;
        rel_root = new_rel;
    } else if (prev != NULL && next == NULL) {
        new_rel->prev = last_rel;
        new_rel->next = NULL;
        last_rel->next = new_rel;
        last_rel = new_rel;
    } else if (prev != NULL && next != NULL) {
        new_rel->prev = prev;
        new_rel->next = next;
        prev->next = new_rel;
        next->prev = new_rel;
    }
    return new_rel;
}

t_rel *rel_get_next(char *id_rel) {
    t_rel *pnt = rel_root;
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

t_rel *is_in_rel(char *id_rel) {
    if (rel_root != NULL) {
        t_rel *pnt = rel_root;
        do {
            if (strcmp(pnt->id_rel, id_rel) == 0) {
                return pnt;
            }
            pnt = pnt->next;
        } while (pnt != NULL);
    }
    return NULL;
}

t_dest *is_in_dest(char *id_dest, t_dest *dest_list) {
    if (dest_list != NULL) {
        t_dest *pnt = dest_list;
        do {
            if (strcmp(pnt->id_dest, id_dest) == 0) {
                return pnt;
            }
            pnt = pnt->next;
        } while (pnt != NULL);
    }
    return NULL;
}

t_dest *insert_dest(t_rel *relation, char *id_dest) {
    t_dest *new_dest = (t_dest *) malloc(sizeof(t_dest));
    strcpy(new_dest->id_dest, id_dest);
    new_dest->counter = 0;
    new_dest->orig_root = ent_nil;
    if (relation->last_dest == NULL) {
        new_dest->prev = NULL;
        new_dest->next = NULL;
        relation->dest_list = new_dest;
        relation->last_dest = new_dest;
    } else {
        new_dest->prev = relation->last_dest;
        new_dest->next = NULL;
        relation->last_dest->next = new_dest;
        relation->last_dest = new_dest;
    }
    return new_dest;
}

void insert_orig(t_rel *relation, t_dest *dest, char *id_orig) {

    dest->orig_root = insert_ent(dest->orig_root, id_orig);
    dest->counter++;

    if (dest->prev != NULL && (dest->prev->counter != dest->counter || (dest->prev->counter == dest->counter &&
                                                                        strcmp(dest->prev->id_dest, dest->id_dest) >
                                                                        0))) {
        if (dest->counter == relation->dest_list->counter) {
            t_dest *dest_pnt = relation->dest_list;
            while (dest_pnt->counter == dest->counter && strcmp(dest_pnt->id_dest, dest->id_dest) < 0) {
                dest_pnt = dest_pnt->next;
            }
            if (dest_pnt->prev == NULL) {
                if (dest->next == NULL) {
                    dest->prev->next = NULL;
                    relation->last_dest = dest->prev;
                } else {
                    dest->prev->next = dest->next;
                    dest->next->prev = dest->prev;
                }
                dest->prev = NULL;
                dest->next = relation->dest_list;
                relation->dest_list->prev = dest;
                relation->dest_list = dest;
            } else {
                if (dest->next == NULL) {
                    dest->prev->next = NULL;
                    relation->last_dest = dest->prev;
                } else {
                    dest->prev->next = dest->next;
                    dest->next->prev = dest->prev;
                }
                dest->prev = dest_pnt->prev;
                dest->next = dest_pnt;
                dest_pnt->prev->next = dest;
                dest_pnt->prev = dest;
            }
        } else if (dest->counter > relation->dest_list->counter) {
            if (dest->next == NULL) {
                dest->prev->next = NULL;
                relation->last_dest = dest->prev;
            } else {
                dest->prev->next = dest->next;
                dest->next->prev = dest->prev;
            }
            dest->prev = NULL;
            dest->next = relation->dest_list;
            relation->dest_list->prev = dest;
            relation->dest_list = dest;
        }
    }
}

void dest_sort(t_rel *relation) {
    int max_counter = relation->dest_list->counter;
    t_dest *dest_pnt = relation->dest_list;
    do {
        if (dest_pnt->counter > max_counter) {
            t_dest *dest_to_move = dest_pnt;
            dest_pnt = dest_pnt->next;
            max_counter = dest_to_move->counter;
            if (dest_to_move->next == NULL) {
                dest_to_move->prev->next = NULL;
                relation->last_dest = dest_to_move->prev;
            } else {
                dest_to_move->prev->next = dest_to_move->next;
                dest_to_move->next->prev = dest_to_move->prev;
            }
            dest_to_move->prev = NULL;
            dest_to_move->next = relation->dest_list;
            relation->dest_list->prev = dest_to_move;
            relation->dest_list = dest_to_move;
        } else if (dest_pnt->counter == max_counter && dest_pnt->prev != NULL) {
            t_dest *dest_to_move = dest_pnt;
            dest_pnt = dest_pnt->next;
            if (dest_to_move->prev->counter != dest_to_move->counter ||
                (dest_to_move->prev->counter == dest_to_move->counter &&
                 strcmp(dest_to_move->prev->id_dest, dest_to_move->id_dest) > 0)) {
                t_dest *order_dest_pnt = relation->dest_list;
                while (order_dest_pnt->counter == dest_to_move->counter &&
                       strcmp(order_dest_pnt->id_dest, dest_to_move->id_dest) < 0) {
                    order_dest_pnt = order_dest_pnt->next;
                }

                if (order_dest_pnt->prev == NULL) {
                    if (dest_to_move->next == NULL) {
                        dest_to_move->prev->next = NULL;
                        relation->last_dest = dest_to_move->prev;
                    } else {
                        dest_to_move->prev->next = dest_to_move->next;
                        dest_to_move->next->prev = dest_to_move->prev;
                    }
                    dest_to_move->prev = NULL;
                    dest_to_move->next = relation->dest_list;
                    relation->dest_list->prev = dest_to_move;
                    relation->dest_list = dest_to_move;
                } else {
                    if (dest_to_move->next == NULL) {
                        dest_to_move->prev->next = NULL;
                        relation->last_dest = dest_to_move->prev;
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

void delete_orig(t_rel *relation, t_dest *dest, t_ent *orig_to_del) {

    dest->orig_root = delete_ent(dest->orig_root, orig_to_del);
    dest->counter = dest->counter - 1;

    if (dest->prev == NULL && dest->next != NULL) {
        if (dest->counter + 1 == dest->next->counter) {
            dest->next->prev = NULL;
            relation->dest_list = dest->next;
            dest->prev = relation->last_dest;
            dest->next = NULL;
            relation->last_dest->next = dest;
            relation->last_dest = dest;
        } else {
            dest_sort(relation);
        }
    } else if (dest->counter + 1 == relation->dest_list->counter) {
        if (dest->next != NULL && dest->next->counter == dest->counter + 1) {
            dest->prev->next = dest->next;
            dest->next->prev = dest->prev;
            dest->prev = relation->last_dest;
            dest->next = NULL;
            relation->last_dest->next = dest;
            relation->last_dest = dest;
        }
    }
}

/*int double_ent_search(char *id_dest, char *id_orig) {
    int dest_found = 0;
    int orig_found = 0;
    t_ent *pnt_to_dest = ent_root;
    t_ent *pnt_to_orig = ent_root;
    while () {
        if (pnt_to_dest == NULL && pnt_to_orig != NULL) {
            if (orig_found == 0) {
                if (strcmp(pnt_to_orig->id_ent, id_orig) == 0) {
                    orig_found = 1;
                } else if (strcmp(pnt_to_orig->id_ent, id_orig) < 0) {
                    pnt_to_orig = pnt_to_orig->right;
                } else if (strcmp(pnt_to_orig->id_ent, id_orig) > 0) {
                    pnt_to_orig = pnt_to_orig->left;
                }
            }
        } else if (pnt_to_dest != NULL && pnt_to_orig == NULL) {
            if (dest_found == 0) {
                if (strcmp(pnt_to_dest->id_ent, id_dest) == 0) {
                    dest_found = 1;
                } else if (strcmp(pnt_to_dest->id_ent, id_dest) < 0) {
                    pnt_to_dest = pnt_to_dest->right;
                } else if (strcmp(pnt_to_dest->id_ent, id_dest) > 0) {
                    pnt_to_dest = pnt_to_dest->left;
                }
            }
        } else if (pnt_to_dest == NULL && pnt_to_orig == NULL) {
            if (dest_found == 1 && orig_found == 0) {
                if (strcmp(pnt_to_orig->id_ent, id_orig) == 0) {
                    orig_found = 1;
                } else if (strcmp(pnt_to_orig->id_ent, id_orig) < 0) {
                    pnt_to_orig = pnt_to_orig->right;
                } else if (strcmp(pnt_to_orig->id_ent, id_orig) > 0) {
                    pnt_to_orig = pnt_to_orig->left;
                }
            } else if (dest_found == 0 && orig_found == 1) {
                if (strcmp(pnt_to_dest->id_ent, id_dest) == 0) {
                    dest_found = 1;
                } else if (strcmp(pnt_to_dest->id_ent, id_dest) < 0) {
                    pnt_to_dest = pnt_to_dest->right;
                } else if (strcmp(pnt_to_dest->id_ent, id_dest) > 0) {
                    pnt_to_dest = pnt_to_dest->left;
                }
            } else {
                if (strcmp(pnt_to_dest->id_ent, id_dest) == 0) {
                    dest_found = 1;
                } else if (strcmp(pnt_to_dest->id_ent, id_dest) < 0) {
                    pnt_to_dest = pnt_to_dest->right;
                } else if (strcmp(pnt_to_dest->id_ent, id_dest) > 0) {
                    pnt_to_dest = pnt_to_dest->left;
                }

                if (strcmp(pnt_to_orig->id_ent, id_orig) == 0) {
                    orig_found = 1;
                } else if (strcmp(pnt_to_orig->id_ent, id_orig) < 0) {
                    pnt_to_orig = pnt_to_orig->right;
                } else if (strcmp(pnt_to_orig->id_ent, id_orig) > 0) {
                    pnt_to_orig = pnt_to_orig->left;
                }
            }
        }
    }
    if (dest_found == 1 && orig_found == 1) {
        return 1;
    } else {
        return 0;
    }
}*/

// REQUIRED FUNCTIONS

void add_ent(char *id_ent) {
    ent_root = insert_ent(ent_root, id_ent);
}

void del_ent(char *id_ent) {
    t_ent *ent_to_del = is_in_ent(ent_root, id_ent);
    if (ent_to_del != ent_nil) {
        if (rel_root != NULL) {
            t_rel *rel_pnt = rel_root;
            do {
                t_dest *dest_pnt = rel_pnt->dest_list;
                if (dest_pnt != NULL) {
                    do {
                        if (strcmp(dest_pnt->id_dest, id_ent) == 0) {
                            if (dest_pnt->prev == NULL && dest_pnt->next == NULL) {
                                rel_pnt->dest_list = NULL;
                                rel_pnt->last_dest = NULL;
                            } else if (dest_pnt->prev == NULL && dest_pnt->next != NULL) {
                                dest_pnt->next->prev = NULL;
                                rel_pnt->dest_list = dest_pnt->next;
                            } else if (dest_pnt->prev != NULL && dest_pnt->next == NULL) {
                                dest_pnt->prev->next = NULL;
                                rel_pnt->last_dest = dest_pnt->prev;
                            } else if (dest_pnt->prev != NULL && dest_pnt->next != NULL) {
                                dest_pnt->prev->next = dest_pnt->next;
                                dest_pnt->next->prev = dest_pnt->prev;
                            }
                            free(dest_pnt);
                        } else {
                            t_ent *orig = is_in_ent(dest_pnt->orig_root, id_ent);
                            if (orig != ent_nil) {
                                dest_pnt->orig_root = delete_ent(dest_pnt->orig_root, orig);
                                dest_pnt->counter = dest_pnt->counter - 1;
                            }
                        }
                        dest_pnt = dest_pnt->next;
                    } while (dest_pnt != NULL);
                }
                if (rel_pnt->dest_list != NULL) {
                    dest_sort(rel_pnt);
                };
                rel_pnt = rel_pnt->next;
            } while (rel_pnt != NULL);
        }

        ent_root = delete_ent(ent_root, ent_to_del);
    }
}

void add_rel(char *id_orig, char *id_dest, char *id_rel) {
    if (is_in_ent(ent_root, id_dest) != ent_nil && is_in_ent(ent_root, id_orig) != ent_nil) {
        t_rel *next_rel = rel_get_next(id_rel);
        if (next_rel == NULL && last_rel == NULL) {
            //RELAZIONE: lista vuota
            t_rel *new_rel = insert_rel(id_rel, NULL, NULL);
            t_dest *new_dest = insert_dest(new_rel, id_dest);
            insert_orig(new_rel, new_dest, id_orig);
        } else if (next_rel == NULL && last_rel != NULL) {
            if (strcmp(last_rel->id_rel, id_rel) == 0) {
                //RELAZIONE: presente in coda -> controllare le destinazioni
                t_dest *dest = is_in_dest(id_dest, last_rel->dest_list);
                if (dest == NULL) {
                    //DESTINAZIONE: aggiungere in coda
                    t_dest *new_dest = insert_dest(last_rel, id_dest);
                    insert_orig(last_rel, new_dest, id_orig);
                } else {
                    //DESTINAZIONE: presente -> controllare origini
                    if (is_in_ent(dest->orig_root, id_orig) == ent_nil) {
                        insert_orig(last_rel, dest, id_orig);
                    }
                }
            } else {
                //RELAIONE: aggiungere in coda
                t_rel *new_rel = insert_rel(id_rel, last_rel, NULL);
                t_dest *new_dest = insert_dest(new_rel, id_dest);
                insert_orig(new_rel, new_dest, id_orig);
            }
        } else if (next_rel != NULL) {
            if (next_rel->prev == NULL) {
                //RELAZIONE: aggiungere in testa
                t_rel *new_rel = insert_rel(id_rel, NULL, rel_root);
                t_dest *new_dest = insert_dest(new_rel, id_dest);
                insert_orig(new_rel, new_dest, id_orig);
            } else {
                if (strcmp(next_rel->prev->id_rel, id_rel) == 0) {
                    t_dest *dest = is_in_dest(id_dest, next_rel->prev->dest_list);
                    if (dest == NULL) {
                        //DESTINAZIONE: aggiungere in coda
                        t_dest *new_dest = insert_dest(next_rel->prev, id_dest);
                        insert_orig(next_rel->prev, new_dest, id_orig);
                    } else {
                        //DESTINAZIONE: presente -> controllare origini
                        if (is_in_ent(dest->orig_root, id_orig) == ent_nil) {
                            insert_orig(next_rel->prev, dest, id_orig);
                        }
                    }
                } else {
                    //RELAZIONE: aggiugere tra next_rel->prev e next_rel
                    t_rel *new_rel = insert_rel(id_rel, next_rel->prev, next_rel);
                    t_dest *new_dest = insert_dest(new_rel, id_dest);
                    insert_orig(new_rel, new_dest, id_orig);
                }
            }
        }
    }
}

void del_rel(char *id_orig, char *id_dest, char *id_rel) {
    t_rel *relation = is_in_rel(id_rel);
    if (relation != NULL) {
        t_dest *dest = is_in_dest(id_dest, relation->dest_list);
        if (dest != NULL) {
            t_ent *orig = is_in_ent(dest->orig_root, id_orig);
            if (orig != ent_nil) {
                delete_orig(relation, dest, orig);
            }
        }
    }
}

void report() {
    int prev_max_counter = 0;
    if (rel_root != NULL) {
        t_rel *rel_pnt = rel_root;
        do {
            if (rel_pnt->dest_list != NULL) {
                if (rel_pnt->dest_list->counter != 0) {
                    if (rel_pnt->prev != NULL && prev_max_counter != 0) {
                        fputs(" ", stdout);
                    }
                    fputs(rel_pnt->id_rel, stdout);
                    fputs(" ", stdout);
                    t_dest *dest_pnt = rel_pnt->dest_list;
                    do {
                        fputs(dest_pnt->id_dest, stdout);
                        fputs(" ", stdout);
                        dest_pnt = dest_pnt->next;
                    } while (dest_pnt != NULL && dest_pnt->counter == rel_pnt->dest_list->counter);
                    printf("%d;", rel_pnt->dest_list->counter);
                    prev_max_counter = rel_pnt->dest_list->counter;
                }
            }
            rel_pnt = rel_pnt->next;
        } while (rel_pnt != NULL);
    } else {
        fputs("none", stdout);
    }
    fputs("\n", stdout);
}

int main() {

    ent_nil = (t_ent *)malloc(sizeof(t_ent));
    ent_nil->color = BLACK;
    ent_nil->right = ent_nil;
    ent_nil->left = ent_nil;
    ent_nil->p = ent_nil;
    ent_root = ent_nil;

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
