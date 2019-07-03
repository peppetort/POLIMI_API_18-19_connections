#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_DIM 50
#define NULL ((void *)0)

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
    struct rel_node*next;
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

t_ent *orig_get_next(char *id_ent, t_ent *orig_list){
    t_ent *pnt = orig_list;
    if(pnt != NULL) {
        do {
            if (strcmp(pnt->id_ent, id_ent) > 0) {
                return pnt;
            }
            pnt = pnt->next;
        } while (pnt != NULL);
    }
    return  NULL;
}

t_ent *is_in_ent(char *id_ent){
    if(entity_list != NULL){
        t_ent *pnt;
        pnt = entity_list;
        do {
            if (strcmp(pnt->id_ent, id_ent) == 0) {
                return pnt;
            }
            pnt = pnt->next;
        }while (pnt != NULL);
    }
    return NULL;
}

t_rel_list *is_in_rel(char *id_rel){
    if(rel_list != NULL){
        t_rel_list *pnt = rel_list;
        do{
            if(strcmp(pnt->id_rel, id_rel)==0){
                return pnt;
            }
            pnt = pnt->next;
        }while (pnt != NULL);
    }
    return  NULL;
}

t_rel_list *rel_get_next(char *id_rel){
    t_rel_list *pnt = rel_list;
    if(pnt != NULL){
        do {
            if (strcmp(pnt->id_rel, id_rel) > 0) {
                return pnt;
            }
            pnt = pnt->next;
        } while (pnt != NULL);
    }
    return NULL;
}

t_rel_node *is_in_rel_node(char *id_dest, t_rel_node *dest_list){
    if(dest_list != NULL){
        t_rel_node *pnt = dest_list;
        do{
            if(strcmp(pnt->id_dest, id_dest)==0){
                return pnt;
            }
            pnt = pnt->next;
        }while (pnt != NULL);
    }
    return  NULL;
}

t_rel_node *rel_node_get_next(char *id_dest, t_rel_node *relation){
    t_rel_node *pnt = relation;
    if(pnt != NULL){
        do {
            if (strcmp(pnt->id_dest, id_dest) > 0) {
                return pnt;
            }
            pnt = pnt->next;
        } while (pnt != NULL);
    }
    return NULL;
}

t_ent *is_in_orig(char *id_orig, t_ent *origin_list){
    if(origin_list != NULL){
        t_ent *pnt = origin_list;
        do {
            if (strcmp(pnt->id_ent, id_orig) == 0) {
                return pnt;
            }
            pnt = pnt->next;
        }while (pnt != NULL);
    }
    return NULL;
}

// REQUIRED FUNCTIONS

void add_ent(char *id_ent){
    t_ent *result = is_in_ent(id_ent);
    if(result == NULL && last_ent_node == NULL) {
        t_ent *new_ent = (t_ent *) malloc(sizeof(t_ent));
        strcpy(new_ent->id_ent, id_ent);
        new_ent->next = NULL;
        new_ent->prev = NULL;
        entity_list = new_ent;
        last_ent_node = entity_list;
    } else if(result == NULL && last_ent_node != NULL){
        t_ent *new_ent = (t_ent *) malloc(sizeof(t_ent));
        strcpy(new_ent->id_ent, id_ent);
        new_ent->next = NULL;
        new_ent->prev = last_ent_node;
        last_ent_node->next = new_ent;
        last_ent_node = last_ent_node->next;
    }
}

void del_ent(char *id_ent){
    t_ent *ent_to_del = is_in_ent(id_ent);
    if(ent_to_del != NULL){
        if(rel_list != NULL){
            t_rel_list *rel_pnt = rel_list;
            do{
                t_rel_node *dest_pnt = rel_pnt->node_list;
                do{
                    if(strcmp(dest_pnt->id_dest, id_ent)==0){
                        if(dest_pnt->prev == NULL && dest_pnt->next == NULL){
                            rel_pnt->node_list = NULL;
                            rel_pnt->last_node = NULL;
                            free(dest_pnt);
                        }else if(dest_pnt->prev == NULL && dest_pnt->next != NULL){
                            dest_pnt->next->prev = NULL;
                            rel_pnt->node_list = dest_pnt->next;
                            free(dest_pnt);
                        } else if(dest_pnt->prev != NULL && dest_pnt->next == NULL){
                            dest_pnt->prev->next = NULL;
                            rel_pnt->last_node = dest_pnt->prev;
                            free(dest_pnt);
                        } else if(dest_pnt->prev != NULL && dest_pnt->next != NULL){
                            dest_pnt->prev->next = dest_pnt->next;
                            dest_pnt->next->prev = dest_pnt->prev;
                            free(dest_pnt);
                        }
                    } else{
                        t_ent *orig = is_in_orig(id_ent, dest_pnt->orig_list);
                        if(orig != NULL){
                            dest_pnt->counter = dest_pnt->counter - 1;
                            if(orig->prev == NULL && orig->next == NULL){
                                dest_pnt->orig_list = NULL;
                                dest_pnt->last_orig = NULL;
                                free(orig);
                            } else if(orig->prev == NULL && orig->next != NULL){
                                orig->next->prev = NULL;
                                dest_pnt->orig_list = orig->next;
                                free(orig);
                            } else if(orig->prev != NULL && orig->next == NULL){
                                orig->prev->next = NULL;
                                dest_pnt->last_orig = orig->prev;
                                free(orig);
                            } else if(orig->prev != NULL && orig->next != NULL){
                                orig->prev->next = orig->next;
                                orig->next->prev = orig->prev;
                                free(orig);
                            }
                        }
                    }
                    dest_pnt = dest_pnt->next;
                }while (dest_pnt != NULL);
                rel_pnt = rel_pnt->next;
            }while (rel_pnt != NULL);
        }

        if(ent_to_del->prev == NULL && ent_to_del->next == NULL){
            entity_list = NULL;
            last_ent_node = NULL;
            free(ent_to_del);
        } else if(ent_to_del->prev == NULL && ent_to_del->next != NULL){
            ent_to_del->next->prev = NULL;
            entity_list = ent_to_del->next;
            free(ent_to_del);
        } else if(ent_to_del->prev != NULL && ent_to_del->next == NULL){
            ent_to_del->prev->next = NULL;
            last_ent_node = ent_to_del->prev;
            free(ent_to_del);
        } else if(ent_to_del->prev != NULL && ent_to_del->next != NULL){
            ent_to_del->prev->next = ent_to_del->next;
            ent_to_del->next->prev = ent_to_del->prev;
            free(ent_to_del);
        }
    }
}

void add_rel(char *id_orig, char *id_dest, char *id_rel){
    if(is_in_ent(id_orig) != NULL && is_in_ent(id_dest) != NULL){
        t_rel_list *relation = rel_get_next(id_rel);
        if(relation == NULL && last_rel_node == NULL){
            //lista vuota
            t_rel_list *new_rel = (t_rel_list *)malloc(sizeof(t_rel_list));
            strcpy(new_rel->id_rel, id_rel);
            new_rel->node_list = (t_rel_node *)malloc(sizeof(t_rel_node));
            strcpy(new_rel->node_list->id_dest, id_dest);
            new_rel->node_list->counter = 1;
            new_rel->node_list->orig_list = (t_ent *)malloc(sizeof(t_rel_node));
            strcpy(new_rel->node_list->orig_list->id_ent, id_orig);
            new_rel->node_list->orig_list->next = NULL;
            new_rel->node_list->orig_list->prev = NULL;
            new_rel->node_list->last_orig = new_rel->node_list->orig_list;
            new_rel->node_list->next = NULL;
            new_rel->node_list->prev = NULL;
            new_rel->last_node = new_rel->node_list;
            new_rel->prev = NULL;
            new_rel->next = NULL;
            rel_list = new_rel;
            last_rel_node = new_rel;
        } else if(relation == NULL && last_rel_node != NULL){
            if(strcmp(last_rel_node->id_rel, id_rel)==0){
                t_rel_node *node = rel_node_get_next(id_dest, last_rel_node->node_list);
                if(node == NULL && last_rel_node->last_node == NULL){
                    //lista destinazioni vuota
                    t_rel_node *new_dest = (t_rel_node *)malloc(sizeof(t_rel_node));
                    strcpy(new_dest->id_dest, id_dest);
                    new_dest->counter = 1;
                    new_dest->orig_list = (t_ent *)malloc(sizeof(t_rel_node));
                    strcpy(new_dest->orig_list->id_ent, id_orig);
                    new_dest->orig_list->next = NULL;
                    new_dest->orig_list->prev = NULL;
                    new_dest->last_orig = new_dest->orig_list;
                    new_dest->next = NULL;
                    new_dest->prev = NULL;
                    last_rel_node->node_list = new_dest;
                    last_rel_node->last_node = new_dest;
                }else if(node == NULL && last_rel_node->last_node != NULL){
                    if(strcmp(last_rel_node->last_node->id_dest, id_dest)==0){
                        t_ent *origin = orig_get_next(id_orig, last_rel_node->last_node->orig_list);
                        if(origin == NULL && last_rel_node->last_node->last_orig == NULL){
                            //lista origini vuota
                            t_ent *new_orig = (t_ent *)malloc(sizeof(t_ent));
                            strcpy(new_orig->id_ent, id_orig);
                            new_orig->next = NULL;
                            new_orig->prev = NULL;
                            last_rel_node->last_node->orig_list = new_orig;
                            last_rel_node->last_node->last_orig = new_orig;
                            last_rel_node->last_node->counter = last_rel_node->last_node->counter + 1;
                        }else if(origin == NULL && last_rel_node->last_node->last_orig != NULL){
                            if(strcmp(last_rel_node->last_node->last_orig->id_ent, id_orig) != 0){
                                //aggiungere origine in coda
                                t_ent *new_orig = (t_ent *)malloc(sizeof(t_ent));
                                strcpy(new_orig->id_ent, id_orig);
                                new_orig->next = NULL;
                                new_orig->prev = last_rel_node->last_node->last_orig;
                                last_rel_node->last_node->last_orig->next = new_orig;
                                last_rel_node->last_node->last_orig = last_rel_node->last_node->last_orig->next;
                                last_rel_node->last_node->counter = last_rel_node->last_node->counter + 1;
                            }
                        } else if(origin != NULL && last_rel_node->last_node->last_orig != NULL){
                            if(origin->prev != NULL){
                                if(strcmp(origin->prev->id_ent, id_orig)!=0){
                                    //aggiungere origine in mezzo
                                    t_ent *new_orig = (t_ent *)malloc(sizeof(t_ent));
                                    strcpy(new_orig->id_ent, id_orig);
                                    new_orig->next = origin;
                                    new_orig->prev = origin->prev;
                                    origin->prev->next = new_orig;
                                    origin->prev = new_orig;
                                    last_rel_node->last_node->counter = last_rel_node->last_node->counter + 1;
                                }
                            } else{
                                // aggiungere origine in testa
                                t_ent *new_orig = (t_ent *)malloc(sizeof(t_ent));
                                strcpy(new_orig->id_ent, id_orig);
                                new_orig->next = origin;
                                new_orig->prev = NULL;
                                origin->prev = new_orig;
                                last_rel_node->last_node->orig_list = new_orig;
                                last_rel_node->last_node->counter = last_rel_node->last_node->counter + 1;
                            }
                        }
                    } else{
                        //aggiungere destinazione in coda
                        t_rel_node *new_dest = (t_rel_node *)malloc(sizeof(t_rel_node));
                        strcpy(new_dest->id_dest, id_dest);
                        new_dest->counter = 1;
                        new_dest->orig_list = (t_ent *)malloc(sizeof(t_rel_node));
                        strcpy(new_dest->orig_list->id_ent, id_orig);
                        new_dest->orig_list->next = NULL;
                        new_dest->orig_list->prev = NULL;
                        new_dest->last_orig = new_dest->orig_list;
                        new_dest->next = NULL;
                        new_dest->prev = last_rel_node->last_node;
                        last_rel_node->last_node->next = new_dest;
                        last_rel_node->last_node = last_rel_node->last_node->next;
                    }
                }else if(node != NULL && last_rel_node->last_node != NULL){
                    if(node->prev != NULL){
                        if(strcmp(node->prev->id_dest, id_dest)==0){
                            t_ent *origin = orig_get_next(id_orig, node->prev->orig_list);
                            if(origin == NULL && node->prev->last_orig == NULL){
                                // lista origini vuota
                                t_ent *new_orig = (t_ent *)malloc(sizeof(t_ent));
                                strcpy(new_orig->id_ent, id_orig);
                                new_orig->next = NULL;
                                new_orig->prev = NULL;
                                node->prev->orig_list = new_orig;
                                node->prev->last_orig = new_orig;
                                node->prev->counter = node->prev->counter + 1;
                            } else if(origin == NULL && node->prev->last_orig != NULL){
                                if(strcmp(node->prev->last_orig->id_ent, id_orig)!=0){
                                    // aggiungere origine in coda
                                    t_ent *new_orig = (t_ent *)malloc(sizeof(t_ent));
                                    strcpy(new_orig->id_ent, id_orig);
                                    new_orig->next = NULL;
                                    new_orig->prev = node->prev->last_orig;
                                    node->prev->last_orig->next = new_orig;
                                    node->prev->last_orig = node->prev->last_orig->next;
                                    node->prev->counter = node->prev->counter + 1;
                                }
                            } else if(origin != NULL && node->prev->last_orig != NULL){
                                if(origin->prev != NULL){
                                    if(strcmp(origin->prev->id_ent, id_orig)!=0){
                                        //aggiunegere origine in mezzo
                                        t_ent *new_orig = (t_ent *)malloc(sizeof(t_ent));
                                        strcpy(new_orig->id_ent, id_orig);
                                        new_orig->next = origin;
                                        new_orig->prev = origin->prev;
                                        origin->prev->next = new_orig;
                                        origin->prev = new_orig;
                                        node->prev->counter = node->prev->counter + 1;
                                    }
                                } else{
                                    //aggiungere origine in testa
                                    t_ent *new_orig = (t_ent *)malloc(sizeof(t_ent));
                                    strcpy(new_orig->id_ent, id_orig);
                                    new_orig->next = origin;
                                    new_orig->prev = NULL;
                                    origin->prev = new_orig;
                                    node->prev->orig_list = new_orig;
                                    node->prev->counter = node->prev->counter + 1;
                                }
                            }
                        } else{
                            //aggiungere  destinazione in mezzo
                            t_rel_node *new_dest = (t_rel_node *)malloc(sizeof(t_rel_node));
                            strcpy(new_dest->id_dest, id_dest);
                            new_dest->counter = 1;
                            new_dest->orig_list = (t_ent *)malloc(sizeof(t_rel_node));
                            strcpy(new_dest->orig_list->id_ent, id_orig);
                            new_dest->orig_list->next = NULL;
                            new_dest->orig_list->prev = NULL;
                            new_dest->last_orig = new_dest->orig_list;
                            new_dest->next = node;
                            new_dest->prev = node->prev;
                            node->prev->next = new_dest;
                            node->prev = new_dest;
                        }
                    } else{
                        //aggiungere destinazione in testa
                        t_rel_node *new_dest = (t_rel_node *)malloc(sizeof(t_rel_node));
                        strcpy(new_dest->id_dest, id_dest);
                        new_dest->counter = 1;
                        new_dest->orig_list = (t_ent *)malloc(sizeof(t_rel_node));
                        strcpy(new_dest->orig_list->id_ent, id_orig);
                        new_dest->orig_list->next = NULL;
                        new_dest->orig_list->prev = NULL;
                        new_dest->last_orig = new_dest->orig_list;
                        new_dest->next = last_rel_node->node_list;
                        new_dest->prev = NULL;
                        last_rel_node->node_list->prev = new_dest;
                        last_rel_node->node_list = new_dest;
                    }
                }
            } else{
                //aggiungere in coda
                t_rel_list *new_rel = (t_rel_list *)malloc(sizeof(t_rel_list));
                strcpy(new_rel->id_rel, id_rel);
                new_rel->node_list = (t_rel_node *)malloc(sizeof(t_rel_node));
                strcpy(new_rel->node_list->id_dest, id_dest);
                new_rel->node_list->counter = 1;
                new_rel->node_list->orig_list = (t_ent *)malloc(sizeof(t_rel_node));
                strcpy(new_rel->node_list->orig_list->id_ent, id_orig);
                new_rel->node_list->orig_list->next = NULL;
                new_rel->node_list->orig_list->prev = NULL;
                new_rel->node_list->last_orig = new_rel->node_list->orig_list;
                new_rel->node_list->next = NULL;
                new_rel->node_list->prev = NULL;
                new_rel->last_node = new_rel->node_list;
                new_rel->prev = last_rel_node;
                new_rel->next = NULL;
                last_rel_node->next = new_rel;
                last_rel_node = last_rel_node->next;
            }
        }else if(relation != NULL && last_rel_node != NULL){
            if(relation->prev != NULL){
                if(strcmp(relation->prev->id_rel, id_rel)==0){
                    t_rel_node *destination = rel_node_get_next(id_dest, relation->prev->node_list);
                    if(destination == NULL && relation->prev->last_node == NULL){
                        //lista destinazioni vuota
                        t_rel_node *new_dest = (t_rel_node *)malloc(sizeof(t_rel_node));
                        strcpy(new_dest->id_dest, id_dest);
                        new_dest->counter = 1;
                        new_dest->orig_list = (t_ent *)malloc(sizeof(t_rel_node));
                        strcpy(new_dest->orig_list->id_ent, id_orig);
                        new_dest->orig_list->next = NULL;
                        new_dest->orig_list->prev = NULL;
                        new_dest->last_orig = new_dest->orig_list;
                        new_dest->next = NULL;
                        new_dest->prev = NULL;
                        relation->prev->node_list = new_dest;
                        relation->prev->last_node = new_dest;
                    }else if(destination == NULL && relation->prev->last_node != NULL){
                        if(strcmp(relation->prev->last_node->id_dest, id_dest)==0){
                            t_ent *origin = orig_get_next(id_orig, relation->prev->last_node->orig_list);
                            if(origin == NULL && relation->prev->last_node->last_orig == NULL){
                                // origini vuote
                                t_ent *new_orig = (t_ent *)malloc(sizeof(t_ent));
                                strcpy(new_orig->id_ent, id_orig);
                                new_orig->next = NULL;
                                new_orig->prev = NULL;
                                relation->prev->last_node->orig_list = new_orig;
                                relation->prev->last_node->last_orig = new_orig;
                                relation->prev->last_node->counter = relation->prev->last_node->counter + 1;
                            } else if(origin == NULL && relation->prev->last_node->last_orig != NULL){
                                if(strcmp(relation->prev->last_node->last_orig->id_ent, id_orig)!=0){
                                    // aggiunegere in coda
                                    t_ent *new_orig = (t_ent *)malloc(sizeof(t_ent));
                                    strcpy(new_orig->id_ent, id_orig);
                                    new_orig->next = NULL;
                                    new_orig->prev = relation->prev->last_node->last_orig;
                                    relation->prev->last_node->last_orig->next = new_orig;
                                    relation->prev->last_node->last_orig = relation->prev->last_node->last_orig->next;
                                    relation->prev->last_node->counter = relation->prev->last_node->counter + 1;
                                }
                            } else if(origin != NULL && relation->prev->last_node->last_orig != NULL){
                                if(origin->prev != NULL){
                                    if(strcmp(origin->prev->id_ent, id_orig)!=0){
                                        //aggiunegere in mezzo
                                        t_ent *new_orig = (t_ent *)malloc(sizeof(t_ent));
                                        strcpy(new_orig->id_ent, id_orig);
                                        new_orig->next = origin;
                                        new_orig->prev = origin->prev;
                                        origin->prev->next = new_orig;
                                        origin->prev = new_orig;
                                        relation->prev->last_node->counter = relation->prev->last_node->counter + 1;
                                    }
                                } else{
                                    //aggiuneger in testa
                                    t_ent *new_orig = (t_ent *)malloc(sizeof(t_ent));
                                    strcpy(new_orig->id_ent, id_orig);
                                    new_orig->next = origin;
                                    new_orig->prev = NULL;
                                    origin->prev = new_orig;
                                    relation->prev->last_node->orig_list = new_orig;
                                    relation->prev->last_node->counter = relation->prev->last_node->counter + 1;
                                }
                            }
                        } else{
                            //aggiungere destinazione in coda
                            t_rel_node *new_dest = (t_rel_node *)malloc(sizeof(t_rel_node));
                            strcpy(new_dest->id_dest, id_dest);
                            new_dest->counter = 1;
                            new_dest->orig_list = (t_ent *)malloc(sizeof(t_rel_node));
                            strcpy(new_dest->orig_list->id_ent, id_orig);
                            new_dest->orig_list->next = NULL;
                            new_dest->orig_list->prev = NULL;
                            new_dest->last_orig = new_dest->orig_list;
                            new_dest->next = NULL;
                            new_dest->prev = relation->prev->last_node;
                            relation->prev->last_node->next = new_dest;
                            relation->prev->last_node = relation->prev->last_node->next;
                        }
                    } else if(destination != NULL && relation->prev->last_node != NULL){
                        if(destination->prev != NULL){
                            if(strcmp(destination->prev->id_dest, id_dest)==0){
                                t_ent *origin = orig_get_next(id_orig, destination->prev->orig_list);
                                if(origin == NULL && destination->prev->last_orig == NULL){
                                    //origini vuote
                                    t_ent *new_orig = (t_ent *)malloc(sizeof(t_ent));
                                    strcpy(new_orig->id_ent, id_orig);
                                    new_orig->next = NULL;
                                    new_orig->prev = NULL;
                                    destination->prev->orig_list = new_orig;
                                    destination->prev->last_orig = new_orig;
                                    destination->prev->counter = destination->prev->counter + 1;
                                } else if(origin == NULL && destination->prev->last_orig != NULL){
                                    if(strcmp(destination->prev->last_orig->id_ent, id_orig)!=0){
                                        //aggiunegere in coda
                                        t_ent *new_orig = (t_ent *)malloc(sizeof(t_ent));
                                        strcpy(new_orig->id_ent, id_orig);
                                        new_orig->next = NULL;
                                        new_orig->prev = destination->prev->last_orig;
                                        destination->prev->last_orig->next = new_orig;
                                        destination->prev->last_orig = destination->prev->last_orig->next;
                                        destination->prev->counter = destination->prev->counter + 1;
                                    }
                                } else if(origin != NULL && destination->prev->last_orig != NULL){
                                    if(origin->prev != NULL){
                                        if(strcmp(origin->prev->id_ent, id_orig)!=0){
                                            // aggiunegere in mezzo
                                            t_ent *new_orig = (t_ent *)malloc(sizeof(t_ent));
                                            strcpy(new_orig->id_ent, id_orig);
                                            new_orig->next = origin;
                                            new_orig->prev = origin->prev;
                                            origin->prev->next = new_orig;
                                            origin->prev = new_orig;
                                            destination->prev->counter = destination->prev->counter + 1;
                                        }
                                    } else{
                                        // agiguneger ein testa
                                        t_ent *new_orig = (t_ent *)malloc(sizeof(t_ent));
                                        strcpy(new_orig->id_ent, id_orig);
                                        new_orig->next = origin;
                                        new_orig->prev = NULL;
                                        origin->prev = new_orig;
                                        destination->prev->orig_list = new_orig;
                                        destination->prev->counter = destination->prev->counter + 1;
                                    }
                                }
                            } else{
                                //aggiungere destinazione in mezzo
                                t_rel_node *new_dest = (t_rel_node *)malloc(sizeof(t_rel_node));
                                strcpy(new_dest->id_dest, id_dest);
                                new_dest->counter = 1;
                                new_dest->orig_list = (t_ent *)malloc(sizeof(t_rel_node));
                                strcpy(new_dest->orig_list->id_ent, id_orig);
                                new_dest->orig_list->next = NULL;
                                new_dest->orig_list->prev = NULL;
                                new_dest->last_orig = new_dest->orig_list;
                                new_dest->next = destination;
                                new_dest->prev = destination->prev;
                                destination->prev->next = new_dest;
                                destination->prev = new_dest;
                            }
                        } else{
                            // aggiungere destinazione in testa
                            t_rel_node *new_dest = (t_rel_node *)malloc(sizeof(t_rel_node));
                            strcpy(new_dest->id_dest, id_dest);
                            new_dest->counter = 1;
                            new_dest->orig_list = (t_ent *)malloc(sizeof(t_rel_node));
                            strcpy(new_dest->orig_list->id_ent, id_orig);
                            new_dest->orig_list->next = NULL;
                            new_dest->orig_list->prev = NULL;
                            new_dest->last_orig = new_dest->orig_list;
                            new_dest->next = relation->prev->node_list;
                            new_dest->prev = NULL;
                            relation->prev->node_list->prev = new_dest;
                            relation->prev->node_list = new_dest;
                        }
                    }

                } else{
                    //aggiungere in mezzo
                    t_rel_list *new_rel = (t_rel_list *)malloc(sizeof(t_rel_list));
                    strcpy(new_rel->id_rel, id_rel);
                    new_rel->node_list = (t_rel_node *)malloc(sizeof(t_rel_node));
                    strcpy(new_rel->node_list->id_dest, id_dest);
                    new_rel->node_list->counter = 1;
                    new_rel->node_list->orig_list = (t_ent *)malloc(sizeof(t_rel_node));
                    strcpy(new_rel->node_list->orig_list->id_ent, id_orig);
                    new_rel->node_list->orig_list->next = NULL;
                    new_rel->node_list->orig_list->prev = NULL;
                    new_rel->node_list->last_orig = new_rel->node_list->orig_list;
                    new_rel->node_list->next = NULL;
                    new_rel->node_list->prev = NULL;
                    new_rel->last_node = new_rel->node_list;
                    new_rel->prev = relation->prev;
                    new_rel->next = relation;
                    relation->prev->next = new_rel;
                    relation->prev = new_rel;
                }
            } else{
                //aggiungere in testa
                t_rel_list *new_rel = (t_rel_list *)malloc(sizeof(t_rel_list));
                strcpy(new_rel->id_rel, id_rel);
                new_rel->node_list = (t_rel_node *)malloc(sizeof(t_rel_node));
                strcpy(new_rel->node_list->id_dest, id_dest);
                new_rel->node_list->counter = 1;
                new_rel->node_list->orig_list = (t_ent *)malloc(sizeof(t_rel_node));
                strcpy(new_rel->node_list->orig_list->id_ent, id_orig);
                new_rel->node_list->orig_list->next = NULL;
                new_rel->node_list->orig_list->prev = NULL;
                new_rel->node_list->last_orig = new_rel->node_list->orig_list;
                new_rel->node_list->next = NULL;
                new_rel->node_list->prev = NULL;
                new_rel->last_node = new_rel->node_list;
                new_rel->prev = NULL;
                new_rel->next = rel_list;
                rel_list->prev = new_rel;
                rel_list = new_rel;
            }
        }
    }
}

void del_rel(char *id_orig, char *id_dest, char *id_rel){
    if(is_in_ent(id_orig) != NULL && is_in_ent(id_dest) != NULL){
        t_rel_list *relation = is_in_rel(id_rel);
        if(relation != NULL){
            t_rel_node *dest = is_in_rel_node(id_dest, relation->node_list);
            if(dest != NULL){
                t_ent *orig = is_in_orig(id_orig, dest->orig_list);
                if(orig != NULL){
                    dest->counter = dest->counter - 1;
                    if(orig->prev == NULL && orig->next == NULL){
                        dest->orig_list = NULL;
                        dest->last_orig = NULL;
                        free(orig);
                    } else if(orig->prev == NULL && orig->next != NULL){
                        dest->orig_list = orig->next;
                        orig->next->prev = NULL;
                        free(orig);
                    } else if(orig->prev != NULL && orig->next == NULL){
                        orig->prev->next = NULL;
                        dest->last_orig = orig->prev;
                        free(orig);
                    } else if(orig->prev != NULL && orig->next != NULL){
                        orig->prev->next = orig->next;
                        orig->next->prev = orig->prev;
                        free(orig);
                    }
                }
            }
        }
    }
}

void report() {
    int max_cnt = 0;
    int no_rel = 0;
    if(rel_list != NULL) {
        t_rel_list *rel_pnt = rel_list;
        do{
            if(rel_pnt->node_list != NULL) {
                t_rel_node *node_pnt = rel_pnt->node_list;
                do {
                    if (node_pnt->counter > max_cnt) {
                        max_cnt = node_pnt->counter;
                    }
                    node_pnt = node_pnt->next;
                } while (node_pnt != NULL);
                if (max_cnt != 0) {
                    no_rel = 1;
                    printf("%s ", rel_pnt->id_rel);
                    node_pnt = rel_pnt->node_list;
                    do {
                        if (node_pnt->counter == max_cnt) {
                            printf("%s ", node_pnt->id_dest);
                        }
                        node_pnt = node_pnt->next;
                    } while (node_pnt != NULL);
                    if (rel_pnt->next != NULL) {
                        printf("%d; ", max_cnt);
                    } else {
                        printf("%d;", max_cnt);
                    }
                }
            }
            max_cnt = 0;
            rel_pnt = rel_pnt->next;
        }while (rel_pnt != NULL);
    }
    if(no_rel == 0){
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

    while(strcmp(input, "end")!=0){
        if(strcmp(input, "addent")==0){
            scanf("%s", ent_name);
            //printf("add_ent(%s);\n", ent_name);
            add_ent(ent_name);
        } else if(strcmp(input, "delent")==0){
            scanf("%s", ent_name);
            //printf("del_ent(%s);\n", ent_name);
            del_ent(ent_name);
        } else if(strcmp(input, "addrel")==0){
            scanf("%s%s%s", orig_name, dest_name, rel_id);
            //printf("add_rel(%s, %s, %s);\n", orig_name, dest_name, rel_id);
            add_rel(orig_name, dest_name, rel_id);
        }else if(strcmp(input, "delrel")==0){
            scanf("%s%s%s", orig_name, dest_name, rel_id);
            //printf("del_rel(%s, %s, %s);\n", orig_name, dest_name, rel_id);
            del_rel(orig_name, dest_name, rel_id);
        }else if(strcmp(input, "report")==0){
            //printf("report();\n");
            report();
        }
        scanf("%s", input);
    }

    return 0;
}
