#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

// STRUCTURES

typedef struct orig_rel t_orig;
typedef struct dest_rel t_dest;
typedef struct rel t_rel;
typedef struct entity t_ent;

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
    char *id_rel;
    int counter;
    t_orig *orig_list;
    t_dest *dest_list;
    t_dest *last_dest_node;
    t_orig *last_orig_node;
    struct rel *next;
    struct rel *prev;
};

struct entity {
    char *id_ent;
    t_rel *rel_list;
    t_rel *last_rel_node;
    struct entity *next;
    struct entity *prev;
};

// GLOBAL VARIABLES

t_ent **last_ent_node = NULL;

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

void print_ent(t_ent *entity_list){
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
        printf("      Riferimenti: %d\n", relation_list->counter);
        printf("      Origini:\n");
        t_orig *origin_list = relation_list->orig_list;
        t_rel *orig_rel = is_in_rel(relation_list->id_rel, origin_list->org_pnt->rel_list);
        t_dest *orig_dest = is_in_dest(result->id_ent, orig_rel->dest_list);
        do{
            printf("         - %s -> %s\n", origin_list->org_pnt->id_ent, orig_dest->dest_pnt->id_ent);
            origin_list = origin_list->next;
        }while (origin_list != NULL);
        relation_list = relation_list->next;
    }while (relation_list != NULL);
}

// REQUIRED FUNCTIONS

void add_ent(char *id_ent, t_ent **entity_list){
    t_ent *result = is_in_ent(id_ent, *entity_list);
    if(result == NULL && last_ent_node == NULL) {
        t_ent *new_node = (t_ent *) malloc(sizeof(t_ent));
        new_node->id_ent = id_ent;
        new_node->rel_list = NULL;
        new_node->last_rel_node = NULL;
        new_node->next = NULL;
        new_node->prev = NULL;
        *entity_list = new_node;
        last_ent_node = entity_list;
    } else if(result == NULL && last_ent_node != NULL){
        t_ent *new_node = (t_ent *) malloc(sizeof(t_ent));
        new_node->id_ent = id_ent;
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
        t_ent *prev_node = node_to_del->prev;
        t_ent *next_node = node_to_del->next;
        if(prev_node == NULL){
            *entity_list = next_node;
            next_node->prev = NULL;
            free(node_to_del);
        } else if(next_node == NULL){
            last_ent_node =  &((*last_ent_node)->prev);
            prev_node->next = NULL;
            free(node_to_del);
        }else{
            last_ent_node = &(prev_node->next);
            prev_node->next = next_node;
            next_node->prev = prev_node;
            free(node_to_del);
        }
    }
}

void add_rel(char *id_orig, char *id_dest, char *id_rel, t_ent **entity_list ){
    t_ent *dest_ent = is_in_ent(id_dest, *entity_list);
    t_ent *orig_ent = is_in_ent(id_orig, *entity_list);
    if(dest_ent != NULL && orig_ent != NULL) {
        t_rel *dest_rel_list = dest_ent->rel_list;
        t_rel *orig_rel_list = orig_ent->rel_list;
        t_rel *orig_relation = is_in_rel(id_rel, orig_rel_list);
        t_rel *dest_relation = is_in_rel(id_rel, dest_rel_list);
        if(dest_relation == NULL && dest_ent->last_rel_node == NULL){
            t_rel *new_rel = (t_rel *) malloc(sizeof(t_rel));
            new_rel->id_rel = id_rel;
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
        }else if(dest_relation == NULL && dest_ent->last_rel_node != NULL){
            t_rel *new_rel = (t_rel *) malloc(sizeof(t_rel));
            new_rel->id_rel = id_rel;
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
        }
        if(orig_relation == NULL && orig_ent->last_rel_node == NULL){
            t_rel *new_rel = (t_rel *) malloc(sizeof(t_rel));
            new_rel->id_rel = id_rel;
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
            new_rel->id_rel = id_rel;
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
            }
        }
    }
}

void report(){
    //TODO
}

int main() {
    t_ent *entity_list = NULL;

    add_ent("Giovanni", &entity_list);
    add_ent("Luca", &entity_list);
    add_ent("Sofia", &entity_list);
    add_ent("Nicola", &entity_list);
    print_ent(entity_list);

    add_rel("Giovanni", "Luca", "amico_di", &entity_list);
    add_rel("Sofia", "Luca", "amico_di", &entity_list);
    add_rel("Nicola", "Luca", "amico_di", &entity_list);

    del_rel("Nicola", "Luca", "amico_di", &entity_list);
    add_rel("Nicola", "Luca", "amico_di", &entity_list);

    print_all_rel("Luca", entity_list);

    return 0;
}