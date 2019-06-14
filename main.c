#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define STR_DIM 50

// STRUCTURES

typedef struct orig_rel t_orig;
typedef struct dest_rel t_dest;
typedef struct rel t_rel;
typedef struct entity t_ent;
typedef struct global_rel t_global_rel;
typedef struct rel_node t_rel_node;

struct orig_rel{
    t_ent *org_pnt;
    struct orig_rel *next;
    struct orig_rel *prev;
};

struct dest_rel{
    t_ent *dest_pnt;
    struct dest_rel *next;
    struct dest_rel *prev;
};

struct rel{
    char id_rel[STR_DIM];
    int counter;
    t_orig *orig_list;
    t_dest *dest_list;
    t_dest *last_dest_node;
    t_orig *last_orig_node;
    struct rel *next;
    struct rel *prev;
};

struct entity {
    char id_ent[STR_DIM];
    t_rel *rel_list;
    t_rel *last_rel_node;
    struct entity *next;
    struct entity *prev;
};

struct global_rel{
    char id_rel[STR_DIM];
    t_rel_node *rel_list;
    t_rel_node *last_node;
    struct global_rel *next;
    struct global_rel *prev;
};

struct rel_node{
    int counter;
    char id_ent[STR_DIM];
    struct rel_node *next;
    struct rel_node *prev;
};

// GLOBAL VARIABLES

t_ent **last_ent_node = NULL;
t_global_rel *global_rel_list = NULL;
t_global_rel *last_global_rel = NULL;

void add_ent(char *id_ent, t_ent **entity_list);
void del_ent(char *id_ent, t_ent **entity_list);
void add_rel(char *id_orig, char *id_dest, char *id_rel, t_ent **entity_list );
void del_rel(char *id_orig, char *id_dest, char *id_rel, t_ent **entity_list );
void report();

// UTILS FUNCTIONS

t_ent *is_in_ent(char *id_ent, t_ent *ent_list){
    if(ent_list != NULL){
        t_ent *pnt;
        pnt = ent_list;
        do {
            if (strcmp(pnt->id_ent, id_ent) == 0) {
                return pnt;
            }
            pnt = pnt->next;
        }while (pnt != NULL);
    }
    return NULL;
}

t_rel *is_in_rel(char *id_rel, t_rel *rel_list){
    if(rel_list != NULL){
        t_rel *pnt = rel_list;
        do{
            if(strcmp(pnt->id_rel, id_rel) == 0){
                return pnt;
            }
            pnt = pnt->next;
        }while (pnt != NULL);
    }
    return NULL;
}

 t_orig *is_in_origin(char *id_orig, t_orig *origin_list){
    if(origin_list != NULL){
      t_orig *pnt = origin_list;
      do{
          if(strcmp(pnt->org_pnt->id_ent, id_orig) == 0){
              return pnt;
          }
          pnt = pnt->next;
      }while (pnt != NULL);
    }
    return NULL;
}

t_dest *is_in_dest(char *id_dest, t_dest *destination_list){
    if(destination_list != NULL){
        t_dest *pnt = destination_list;
        do{
            if(strcmp(pnt->dest_pnt->id_ent, id_dest) == 0){
                return pnt;
            }
            pnt = pnt->next;
        }while (pnt != NULL);
    }
    return NULL;
}

t_global_rel *is_in_global_rel(char *id_rel){
    t_global_rel *pnt = global_rel_list;
    if(pnt != NULL) {
        do {
            if (strcmp(pnt->id_rel, id_rel) > 0) {
                return pnt;
            }
            pnt = pnt->next;
        } while (pnt != NULL);
    }
    return  NULL;
}

t_rel_node *is_in_global_rel_node(t_rel_node *relation, char *id_ent){
    t_rel_node *pnt = relation;
    if(pnt != NULL) {
        do {
            if (strcmp(pnt->id_ent, id_ent)>0) {
                return pnt;
            }
            pnt = pnt->next;
        } while (pnt != NULL);
    }
    return  NULL;
}

void set_rel_in_global(t_rel *relation, char *id_ent){
    t_global_rel *rel = is_in_global_rel(relation->id_rel);
    if(rel == NULL && last_global_rel == NULL){
        //lista relazioni vuota
        t_global_rel *new_rel = (t_global_rel *)malloc(sizeof(t_global_rel));
        strcpy(new_rel->id_rel, relation->id_rel);
        new_rel->rel_list = (t_rel_node *)malloc(sizeof(t_rel_node));
        new_rel->rel_list->counter = relation->counter;
        strcpy(new_rel->rel_list->id_ent, id_ent);
        new_rel->rel_list->next = NULL;
        new_rel->rel_list->prev = NULL;
        new_rel->last_node = new_rel->rel_list;
        new_rel->next = NULL;
        new_rel->prev = NULL;
        global_rel_list = new_rel;
        last_global_rel = new_rel;
    }else if(rel == NULL && last_global_rel != NULL){
        //lista non vuota ma elemento non presente, controllare se è luntimo se no aggiungerlo in coda
        if(strcmp(last_global_rel->id_rel, relation->id_rel)==0){
            //è l'ultimo elemento
            t_rel_node *node = is_in_global_rel_node(last_global_rel->rel_list, id_ent);
            if(node == NULL && last_global_rel->last_node == NULL){
                //lista vuota
                t_rel_node *new_node = (t_rel_node *) malloc(sizeof(t_rel_node));
                new_node->counter = relation->counter;
                strcpy(new_node->id_ent, id_ent);
                new_node->next = NULL;
                new_node->prev = NULL;
                last_global_rel->rel_list = new_node;
                last_global_rel->last_node = new_node;
            }else if(node == NULL && last_global_rel->last_node != NULL){
                //elemento non presente controllare se è il penultimo
                if (strcmp(last_global_rel->last_node->id_ent, id_ent) == 0) {
                    //nodo trovato
                    last_global_rel->last_node->counter = relation->counter;
                } else {
                    //aggiungere in coda
                    t_rel_node *new_node = (t_rel_node *) malloc(sizeof(t_rel_node));
                    new_node->counter = relation->counter;
                    strcpy(new_node->id_ent, id_ent);
                    new_node->next = NULL;
                    new_node->prev = last_global_rel->last_node;
                    last_global_rel->last_node->next = new_node;
                    last_global_rel->last_node = last_global_rel->last_node->next;
                }
            } else if(node != NULL && last_global_rel->last_node != NULL){
                //controllare se è il primo
                if(node->prev == NULL){
                    // aggiungere elemento in testa
                    t_rel_node *new_node = (t_rel_node *) malloc(sizeof(t_rel_node));
                    new_node->counter = relation->counter;
                    strcpy(new_node->id_ent, id_ent);
                    new_node->next = node;
                    node->prev = new_node;
                    last_global_rel->rel_list = new_node;
                } else{
                    if(strcmp(node->prev->id_ent, id_ent)==0){
                        //elemento trovato
                        node->prev->counter = relation->counter;
                    } else{
                        //aggiungere prima
                        t_rel_node *new_node = (t_rel_node *) malloc(sizeof(t_rel_node));
                        new_node->counter = relation->counter;
                        strcpy(new_node->id_ent, id_ent);
                        new_node->next = node;
                        new_node->prev = node->prev;
                        node->prev->next = new_node;
                        node->prev = new_node;
                    }
                }
            }
        } else{
            //aggiungere in coda
            t_global_rel *new_rel = (t_global_rel *)malloc(sizeof(t_global_rel));
            strcpy(new_rel->id_rel, relation->id_rel);
            new_rel->rel_list = (t_rel_node *)malloc(sizeof(t_rel_node));
            new_rel->rel_list->counter = relation->counter;
            strcpy(new_rel->rel_list->id_ent, id_ent);
            new_rel->rel_list->next = NULL;
            new_rel->rel_list->prev = NULL;
            new_rel->last_node = new_rel->rel_list;
            new_rel->prev = last_global_rel;
            new_rel->next = NULL;
            last_global_rel->next = new_rel;
            last_global_rel = last_global_rel->next;
        }
    }else if(rel != NULL && last_global_rel != NULL){
        //controllare se quello precendente è quello cercato se no aggiunerlo
        if(rel->prev != NULL){
            if(strcmp(rel->prev->id_rel, relation->id_rel)==0){
                //elemento esiste già
                t_rel_node *node = is_in_global_rel_node(rel->prev->rel_list, id_ent);
                if(node == NULL && rel->prev->last_node == NULL){
                    //lista vuota
                    t_rel_node *new_node = (t_rel_node *) malloc(sizeof(t_rel_node));
                    new_node->counter = relation->counter;
                    strcpy(new_node->id_ent, id_ent);
                    new_node->next = NULL;
                    new_node->prev = NULL;
                    rel->prev->rel_list = new_node;
                    rel->prev->last_node = new_node;
                } else if(node == NULL && rel->prev->last_node != NULL){
                    //elemento non trovato controllare se è il precedente se no aggiungere in coda
                    if(rel->prev->last_node != NULL){
                        if(strcmp(rel->prev->last_node->id_ent, id_ent)==0){
                            //elemento trovato
                            rel->prev->last_node->counter = relation->counter;
                        } else{
                            //aggiugere in coda
                            t_rel_node *new_node = (t_rel_node *) malloc(sizeof(t_rel_node));
                            new_node->counter = relation->counter;
                            strcpy(new_node->id_ent, id_ent);
                            new_node->next = NULL;
                            new_node->prev = rel->prev->last_node;
                            rel->prev->last_node->next = new_node;
                            rel->prev->last_node = rel->prev->last_node->next;
                        }
                    } else{
                        // aggiungere in coda
                        t_rel_node *new_node = (t_rel_node *) malloc(sizeof(t_rel_node));
                        new_node->counter = relation->counter;
                        strcpy(new_node->id_ent, id_ent);
                        new_node->next = NULL;
                        new_node->prev = rel->prev->last_node;
                        rel->prev->last_node->next = new_node;
                        rel->prev->last_node = rel->prev->last_node->next;
                    }
                } else if(node != NULL && rel->prev->last_node != NULL){
                    //elemento trovato controllare se è il precendete se no aggiungerlo
                    if(node->prev != NULL){
                        if(strcmp(node->prev->id_ent, id_ent)==0){
                            //elemento trovato
                            node->prev->counter = relation->counter;
                        } else{
                            //aggiungere prima
                            t_rel_node *new_node = (t_rel_node *) malloc(sizeof(t_rel_node));
                            new_node->counter = relation->counter;
                            strcpy(new_node->id_ent, id_ent);
                            new_node->next = node;
                            new_node->prev = node->prev;
                            node->prev->next = new_node;
                            node->prev = new_node;
                        }
                    } else{
                        //aggiungere in testa
                        t_rel_node *new_node = (t_rel_node *) malloc(sizeof(t_rel_node));
                        new_node->counter = relation->counter;
                        strcpy(new_node->id_ent, id_ent);
                        new_node->next = node;
                        node->prev = new_node;
                        rel->prev->rel_list = new_node;
                    }
                }
            }else{
                //aggiungerlo prima di rel
                t_global_rel *new_rel = (t_global_rel *)malloc(sizeof(t_global_rel));
                strcpy(new_rel->id_rel, relation->id_rel);
                new_rel->rel_list = (t_rel_node *)malloc(sizeof(t_rel_node));
                new_rel->rel_list->counter = relation->counter;
                strcpy(new_rel->rel_list->id_ent, id_ent);
                new_rel->rel_list->next = NULL;
                new_rel->rel_list->prev = NULL;
                new_rel->last_node = new_rel->rel_list;
                new_rel->prev = rel->prev;
                new_rel->next = rel;
                rel->prev->next = new_rel;
                rel->prev = new_rel;
            }
        } else{
            //rel è il primo elemento aggiungere in testa
            t_global_rel *new_rel = (t_global_rel *)malloc(sizeof(t_global_rel));
            strcpy(new_rel->id_rel, relation->id_rel);
            new_rel->rel_list = (t_rel_node *)malloc(sizeof(t_rel_node));
            new_rel->rel_list->counter = relation->counter;
            strcpy(new_rel->rel_list->id_ent, id_ent);
            new_rel->rel_list->next = NULL;
            new_rel->rel_list->prev = NULL;
            new_rel->last_node = new_rel->rel_list;
            new_rel->next = rel;
            rel->prev = new_rel;
            global_rel_list = new_rel;
        }
    }
}

void set_global_rel_counter(t_rel *relation, char *id_ent){
    t_global_rel *rel = is_in_global_rel(relation->id_rel);
    if(rel != NULL){
        t_rel_node *node = is_in_global_rel_node(rel->rel_list, id_ent);
        if(node != NULL){
            node->counter = relation->counter;
        }
    }
}

void del_rel_global_node(char *id_ent){
    if(global_rel_list != NULL) {
        t_global_rel *pnt = global_rel_list;
        do {
            t_rel_node *rel_list = pnt->rel_list;
            if(rel_list != NULL){
                t_rel_node *rel = is_in_global_rel_node(rel_list, id_ent);
                if(rel != NULL){
                    if(rel->prev == NULL && rel->next == NULL){
                        pnt->rel_list = NULL;
                        pnt->last_node = NULL;
                        free(rel);
                    }else if(rel->prev == NULL && rel->next != NULL){
                        pnt->rel_list = rel->next;
                        rel->next->prev = NULL;
                        free(rel);
                    } else if(rel->prev != NULL && rel->next == NULL){
                        pnt->last_node = pnt->last_node->prev;
                        rel->prev->next = NULL;
                        free(rel);
                    } else if(rel->prev != NULL && rel->next != NULL){
                        rel->prev->next = rel->next;
                        rel->next->prev = rel->prev;
                        free(rel);
                    }
                }
            }
            pnt = pnt->next;
        } while (pnt != NULL);
    }
}

/*void print_ent(t_ent *entity_list){
    t_ent *pnt = entity_list;
    printf("O<- ");
    if(pnt != NULL){
        do{
            if(pnt->next != NULL) {
                printf("%s <-> ", pnt->id_ent);
            } else{
                printf("%s ", pnt->id_ent);
            }
            pnt = pnt->next;
        }while (pnt != NULL);
    }
    printf("->|\n");
}

void print_all_rel(char *ent, t_ent *entity_list){
    printf("\nRELAZIONI PUNTANTI %s\n", ent);
    t_ent *result = is_in_ent(ent, entity_list);
    t_ent *pnt = result;
    t_rel *relation_list = pnt->rel_list;
    printf("Tipo di relazione\n");
    do{
        printf("   @%s\n", relation_list->id_rel);
        printf("      Origini:\n");
        printf("      Riferimenti:\n");
        t_orig *origin_list = relation_list->orig_list;
        t_rel *orig_rel = is_in_rel(relation_list->id_rel, origin_list->org_pnt->rel_list);
        t_dest *orig_dest = is_in_dest(result->id_ent, orig_rel->dest_list);
        do{
            printf("         - %s -> %s\n", origin_list->org_pnt->id_ent, orig_dest->dest_pnt->id_ent);
            origin_list = origin_list->next;
        }while (origin_list != NULL);
        relation_list = relation_list->next;
    }while (relation_list != NULL);
}*/

// REQUIRED FUNCTIONS


void add_ent(char *id_ent, t_ent **entity_list){
    t_ent *result = is_in_ent(id_ent, *entity_list);
    if(result == NULL && last_ent_node == NULL) {
        t_ent *new_node = (t_ent *) malloc(sizeof(t_ent));
        strcpy(new_node->id_ent, id_ent);
        new_node->rel_list = NULL;
        new_node->last_rel_node = NULL;
        new_node->next = NULL;
        new_node->prev = NULL;
        *entity_list = new_node;
        last_ent_node = entity_list;
    } else if(result == NULL && last_ent_node != NULL){
        t_ent *new_node = (t_ent *) malloc(sizeof(t_ent));
        strcpy(new_node->id_ent, id_ent);
        new_node->rel_list = NULL;
        new_node->last_rel_node = NULL;
        new_node->next = NULL;
        new_node->prev = *last_ent_node;
        (*last_ent_node)->next = new_node;
        last_ent_node = &(*last_ent_node)->next;
    }
}

void del_ent(char *id_ent, t_ent **entity_list){
    t_ent *result = is_in_ent(id_ent, *entity_list);
    if(result != NULL){
        t_ent *node_to_del = result;

        t_rel *rel_pnt = result->rel_list;
        if(rel_pnt != NULL) {
            do {
                t_dest *dest_pnt = rel_pnt->dest_list;
                if (dest_pnt != NULL) {
                    do {
                        t_rel *pointed_rel = is_in_rel(rel_pnt->id_rel, dest_pnt->dest_pnt->rel_list);
                        if (pointed_rel != NULL) {
                            t_orig *pointed_org = is_in_origin(id_ent, pointed_rel->orig_list);
                            if (pointed_org != NULL) {
                                if (pointed_org->prev == NULL && pointed_org->next == NULL) {
                                    pointed_rel->orig_list = NULL;
                                    pointed_rel->last_orig_node = NULL;
                                    free(pointed_org);
                                } else if (pointed_org->prev == NULL && pointed_org->next != NULL) {
                                    pointed_org->next->prev = NULL;
                                    pointed_rel->orig_list = pointed_org->next;
                                    free(pointed_org);
                                } else if (pointed_org->prev != NULL && pointed_org->next == NULL) {
                                    pointed_rel->last_orig_node = pointed_rel->last_orig_node->prev;
                                    pointed_org->prev->next = NULL;
                                    free(pointed_org);
                                } else if (pointed_org->prev != NULL && pointed_org->next != NULL) {
                                    pointed_org->prev->next = pointed_org->next;
                                    pointed_org->next->prev = pointed_org->prev;
                                    free(pointed_org);
                                }
                                pointed_rel->counter = pointed_rel->counter - 1;
                                set_global_rel_counter(pointed_rel, dest_pnt->dest_pnt->id_ent);
                            }
                        }
                        dest_pnt = dest_pnt->next;
                    } while (dest_pnt != NULL);
                }
                rel_pnt = rel_pnt->next;
            } while (rel_pnt != NULL);
        }

        del_rel_global_node(id_ent);

        if(node_to_del->prev == NULL && node_to_del->next == NULL){
            *entity_list = node_to_del->next;
            last_ent_node = NULL;
            free(node_to_del);
        } else if(node_to_del->prev == NULL && node_to_del->next != NULL){
            *entity_list = node_to_del->next;
            node_to_del->next->prev = NULL;
            free(node_to_del);
        }else if(node_to_del->prev != NULL && node_to_del->next == NULL){
            last_ent_node =  &((*last_ent_node)->prev);
            node_to_del->prev->next = NULL;
            free(node_to_del);

        } else if(node_to_del->prev != NULL && node_to_del->next != NULL){
            node_to_del->prev->next = node_to_del->next;
            node_to_del->next->prev = node_to_del->prev;
            free(node_to_del);
        }
    }
}

void add_rel(char *id_orig, char *id_dest, char *id_rel, t_ent **entity_list ){
    t_ent *dest_ent = is_in_ent(id_dest, *entity_list);
    t_ent *orig_ent = is_in_ent(id_orig, *entity_list);
    if(dest_ent != NULL && orig_ent != NULL) {
        t_rel *dest_rel_list = dest_ent->rel_list;
        t_rel *dest_relation = is_in_rel(id_rel, dest_rel_list);
        t_rel *orig_rel_list = orig_ent->rel_list;
        t_rel *orig_relation = is_in_rel(id_rel, orig_rel_list);
        if(dest_relation == NULL && dest_ent->last_rel_node == NULL){
            t_rel *new_rel = (t_rel *) malloc(sizeof(t_rel));
            strcpy(new_rel->id_rel, id_rel);
            new_rel->counter = new_rel->counter + 1;
            new_rel->next = NULL;
            new_rel->prev = NULL;
            new_rel->orig_list = (t_orig *) malloc(sizeof(t_orig));
            new_rel->orig_list->org_pnt = orig_ent;
            new_rel->orig_list->next = NULL;
            new_rel->orig_list->prev = NULL;
            new_rel->last_orig_node = new_rel->orig_list;
            dest_ent->rel_list = new_rel;
            dest_ent->last_rel_node = dest_ent->rel_list;

            set_rel_in_global(new_rel, id_dest);

        }else if(dest_relation == NULL && dest_ent->last_rel_node != NULL){
            t_rel *new_rel = (t_rel *) malloc(sizeof(t_rel));
            strcpy(new_rel->id_rel, id_rel);
            new_rel->counter = new_rel->counter + 1;
            new_rel->next = NULL;
            new_rel->prev = dest_ent->last_rel_node;
            new_rel->orig_list = (t_orig *) malloc(sizeof(t_orig));
            new_rel->orig_list->org_pnt = orig_ent;
            new_rel->orig_list->next = NULL;
            new_rel->orig_list->prev = NULL;
            new_rel->last_orig_node = new_rel->orig_list;
            dest_ent->last_rel_node->next = new_rel;
            dest_ent->last_rel_node = dest_ent->last_rel_node->next;

            set_rel_in_global(new_rel, id_dest);


        } else if(dest_relation != NULL){
            t_orig *org_list = dest_relation->orig_list;
            t_orig *origin = is_in_origin(id_orig, org_list);
            if (origin == NULL && dest_relation->last_orig_node == NULL) {
                t_orig *new_orig = (t_orig *) malloc(sizeof(t_orig));
                new_orig->org_pnt = orig_ent;
                new_orig->next = NULL;
                new_orig->prev = NULL;
                dest_relation->orig_list = new_orig;
                dest_relation->last_orig_node = dest_relation->orig_list;
                dest_relation->counter = dest_relation->counter + 1;
            } else if (origin == NULL && dest_relation->last_orig_node != NULL) {
                t_orig *new_orig = (t_orig *) malloc(sizeof(t_orig));
                new_orig->org_pnt = orig_ent;
                new_orig->next = NULL;
                new_orig->prev = dest_relation->last_orig_node;
                dest_relation->last_orig_node->next = new_orig;
                dest_relation->last_orig_node = dest_relation->last_orig_node->next;
                dest_relation->counter = dest_relation->counter + 1;
            }

            set_rel_in_global(dest_relation, id_dest);

        }
        if(orig_relation == NULL && orig_ent->last_rel_node == NULL){
            t_rel *new_rel = (t_rel *) malloc(sizeof(t_rel));
            strcpy(new_rel->id_rel, id_rel);
            new_rel->counter = 0;
            new_rel->next = NULL;
            new_rel->prev = NULL;
            new_rel->dest_list = (t_dest *)malloc(sizeof(t_dest));
            new_rel->dest_list->dest_pnt = dest_ent;
            new_rel->dest_list->next = NULL;
            new_rel->dest_list->prev = NULL;
            new_rel->last_dest_node = new_rel->dest_list;
            orig_ent->rel_list = new_rel;
            orig_ent->last_rel_node = orig_ent->rel_list;
        }else if(orig_relation == NULL && orig_ent->last_rel_node != NULL){
            t_rel *new_rel = (t_rel *) malloc(sizeof(t_rel));
            strcpy(new_rel->id_rel, id_rel);
            new_rel->counter = 0;
            new_rel->next = NULL;
            new_rel->prev = orig_ent->last_rel_node;
            new_rel->dest_list = (t_dest *) malloc(sizeof(t_dest));
            new_rel->dest_list->dest_pnt = dest_ent;
            new_rel->dest_list->next = NULL;
            new_rel->dest_list->prev = NULL;
            new_rel->last_dest_node = new_rel->dest_list;
            orig_ent->last_rel_node->next = new_rel;
            orig_ent->last_rel_node = orig_ent->last_rel_node->next;
        } else if(orig_relation != NULL){
            t_dest *dest_list = orig_relation->dest_list;
            t_dest *destination = is_in_dest(id_dest, dest_list);
            if (destination == NULL && orig_relation->last_dest_node == NULL) {
                t_dest *new_dest = (t_dest *) malloc(sizeof(t_dest));
                new_dest->dest_pnt = dest_ent;
                new_dest->next = NULL;
                new_dest->prev = NULL;
                orig_relation->dest_list = new_dest;
                orig_relation->last_dest_node = orig_relation->dest_list;
            } else if (destination == NULL && orig_relation->last_dest_node != NULL) {
                t_dest *new_dest = (t_dest *) malloc(sizeof(t_dest));
                new_dest->dest_pnt = dest_ent;
                new_dest->next = NULL;
                new_dest->prev = orig_relation->last_dest_node;
                orig_relation->last_dest_node->next = new_dest;
                orig_relation->last_dest_node = orig_relation->last_dest_node->next;
            }
        }
    }
}

void del_rel(char *id_orig, char *id_dest, char *id_rel, t_ent **entity_list ){
    t_ent *dest_ent = is_in_ent(id_dest, *entity_list);
    t_ent *orig_ent = is_in_ent(id_orig, *entity_list);
    if(dest_ent != NULL && orig_ent != NULL){
        t_rel *dest_rel_list = dest_ent->rel_list;
        t_rel *dest_rel = is_in_rel(id_rel, dest_rel_list);
        if(dest_rel != NULL){
            t_orig *org_list = dest_rel->orig_list;
            t_orig *origin = is_in_origin(id_orig, org_list);
            if(origin != NULL){
                t_ent *org_e = origin->org_pnt;
                if(org_e != NULL){
                    t_rel *orig_rel_list = org_e->rel_list;
                    t_rel *orig_rel = is_in_rel(id_rel, orig_rel_list);
                    if(orig_rel != NULL){
                        t_dest *orig_dest_list = orig_rel->dest_list;
                        t_dest *orig_dest = is_in_dest(id_dest, orig_dest_list);
                        if(orig_dest != NULL){
                            if(orig_dest->prev == NULL && orig_dest->next != NULL){
                                orig_dest->next->prev = NULL;
                                orig_rel->dest_list = orig_dest->next;
                                free(orig_dest);
                            }else if(orig_dest->prev != NULL && orig_dest->next == NULL){
                                orig_dest->prev->next = NULL;
                                orig_rel->last_dest_node = orig_dest->prev;
                                free(orig_dest);
                            }else if(orig_dest->prev != NULL && orig_dest->next != NULL){
                                orig_dest->prev->next = orig_dest->next;
                                orig_dest->next->prev = orig_dest->prev;
                                free(orig_dest);
                            }
                        }
                    }
                }
                if(origin->prev == NULL && origin->next != NULL){
                    origin->next->prev = NULL;
                    dest_rel->orig_list = origin->next;
                    free(origin);
                } else if(origin->prev != NULL && origin->next == NULL){
                    origin->prev->next = NULL;
                    dest_rel->last_orig_node = origin->prev;
                    free(origin);
                } else if(origin->prev != NULL && origin->next != NULL){
                    origin->prev->next = origin->next;
                    origin->next->prev = origin->prev;
                    free(origin);
                }
                dest_rel->counter = dest_rel->counter - 1;
                set_rel_in_global(dest_rel, id_dest);
            }
        }
    }
}

void report(){
    int max_cnt = 0;
    char rel_name[STR_DIM];
    if(global_rel_list != NULL) {
        t_global_rel *rel_pnt = global_rel_list;
        do {
            strcpy(rel_name, rel_pnt->id_rel);
            t_rel_node *node_pnt = rel_pnt->rel_list;
            if(node_pnt != NULL) {
                do {
                    if (node_pnt->counter > max_cnt) {
                        max_cnt = node_pnt->counter;
                    }
                    node_pnt = node_pnt->next;
                } while (node_pnt != NULL);
                node_pnt = rel_pnt->rel_list;
                if (max_cnt != 0){
                    printf("%s ", rel_name);
                    do {
                        if (node_pnt->counter == max_cnt) {
                            printf("%s ", node_pnt->id_ent);
                        }
                        node_pnt = node_pnt->next;
                    } while (node_pnt != NULL);
                    printf("%d; ", max_cnt);
                }
            }
            max_cnt = 0;
            rel_pnt = rel_pnt->next;
        } while (rel_pnt != NULL);
    } else{
        printf("none");
    }
    printf("\n");
}

int main() {
    t_ent *entity_list = NULL;

    char input[STR_DIM];
    char ent_name[STR_DIM];
    char orig_name[STR_DIM];
    char dest_name[STR_DIM];
    char rel_id[STR_DIM];

    while(strcmp(input, "end")!=0){
        if(strcmp(input, "addent")==0){
            scanf("%s", ent_name);
            add_ent(ent_name, &entity_list);
        } else if(strcmp(input, "delent")==0){
            scanf("%s", ent_name);
            del_ent(ent_name, &entity_list);
        } else if(strcmp(input, "addrel")==0){
            scanf("%s%s%s", orig_name, dest_name, rel_id);
            add_rel(orig_name, dest_name, rel_id, &entity_list);
        }else if(strcmp(input, "delrel")==0){
            scanf("%s%s%s", orig_name, dest_name, rel_id);
            del_rel(orig_name, dest_name, rel_id, &entity_list);
        }else if(strcmp(input, "report")==0){
            report();
        }
        scanf("%s", input);
    }

    return 0;
}
