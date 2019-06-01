#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct rel_ent{
    char *id_org;
    struct rel_ent *next;
}t_rel_ent;

typedef struct rel{
    char *id_rel;
    int pointed_count; //numero di relazioni del tipo id_rel ricevute
    t_rel_ent *ent; //lista di persone di origine della relazione id_rel
    struct rel *next;
}t_rel;

typedef struct entity{
    char *id_ent;
    t_rel *relation_type;
    struct entity *next;
}t_ent;

// GLOBAL VARIABLES
t_ent **last_ent_node = NULL;
int is_ent_start = 1;
int is_rel_end = 1;
int is_rel_ent_end = 1;

// UTILS FUNCTIONS

void print_ent(t_ent *entity_list){
    t_ent *pnt = entity_list;
    printf("O- ");
    if(pnt != NULL){
        do{
            if(pnt->next != NULL) {
                printf("%s -> ", pnt->id_ent);
            } else{
                printf("%s ", pnt->id_ent);
            }
            pnt = pnt->next;
        }while (pnt != NULL);
    }
    printf("-|");
}


// controlla se un elemento è nella lista delle entità: se c'è restituisce il puntatore al nodo precedente altrimenti NULL
t_ent *is_in_ent(char *id_ent, t_ent *entity_list){
    t_ent *pnt, *prev_pnt;
    if(entity_list != NULL){
        pnt = entity_list;
        prev_pnt = pnt;
        do {
            if (strcmp(pnt->id_ent, id_ent) == 0) {
                if(pnt == prev_pnt){
                   is_ent_start = 1;
                } else{
                    is_ent_start = 0;
                }
                return prev_pnt;
            }
            prev_pnt = pnt;
            pnt = pnt->next;
        }while (pnt != NULL);
    }
    return NULL;
}

t_rel *is_in_rel(char *id_rel, t_rel *rel_list){
    t_rel *pnt = rel_list;
    t_rel *prev_pnt = NULL;
    do{
        if(strcmp(pnt->id_rel, id_rel) == 0){
            is_rel_end = 1;
            return pnt;
        }
        prev_pnt = pnt;
        pnt = pnt->next;
    }while (pnt != NULL);
    is_rel_end = 0;
    return prev_pnt;
}

t_rel_ent *is_in_rel_ent(char *id_orig, t_rel_ent *rel_ent_list){
    t_rel_ent *pnt = rel_ent_list;
    t_rel_ent *prev_pnt = rel_ent_list;
        do{
            if(strcmp(pnt->id_org, id_orig) == 0){
                is_rel_ent_end = 1;
                return pnt;
            }
            prev_pnt = pnt;
            pnt = pnt->next;
        }while (pnt != NULL);
        is_rel_ent_end = 0;
        return prev_pnt;
}

void print_all_rel(char *ent, t_ent *entuty_list){
    printf("RELAZIONI PUNTANTI %s\n", ent);
    t_ent *pnt = is_in_ent(ent, entuty_list)->next;
    t_rel *relations_list = pnt->relation_type;
    printf("Tipo di relazione:\n");
    do{
        printf("  @%s\n", relations_list->id_rel);
        printf("    Riferiment: %d\n", relations_list->pointed_count);
        printf("    Origini:\n");
        t_rel_ent *origin_list = relations_list->ent;
        do{
            printf("      - %s\n", origin_list->id_org);
            origin_list = origin_list->next;
        }while (origin_list != NULL);
        relations_list = relations_list->next;
    }while (relations_list != NULL);
}


// REQUIRED FUNCTIONS

void add_ent(char *id_ent, t_ent **entity_list){
    if(is_in_ent(id_ent, *entity_list) == NULL){
        t_ent *new_node = (t_ent *)malloc(sizeof(t_ent));
        new_node->id_ent = id_ent;
        new_node->relation_type = NULL;
        new_node->next = NULL;

        if(*last_ent_node != NULL){
            (*last_ent_node)->next = new_node;
            last_ent_node = &((*last_ent_node)->next);
        } else{
            *entity_list = new_node;
            last_ent_node = entity_list;
        }
    }
}

void del_ent(char *id_ent, t_ent **entity_list){
    t_ent *prev_pnt = is_in_ent(id_ent, *entity_list);

    if(prev_pnt != NULL){
        t_ent *node_to_del;
        t_ent *node_to_link;

        if (is_ent_start == 1) {
            node_to_del = prev_pnt;
            node_to_link = prev_pnt->next;
            *entity_list = node_to_link;
            free(node_to_del);
        } else {
            node_to_del = prev_pnt->next;
            node_to_link = node_to_del->next;
            prev_pnt->next = node_to_link;
            free(node_to_del);
        }

        //TODO: eliminare i link puntanti al nodo dagli altri nodi
    }

}

int add_rel(char *id_orig, char *id_dest, char *id_rel, t_ent **entity_list ){
    t_ent *dest_ent = is_in_ent(id_dest, *entity_list);

    if(dest_ent != NULL){
        if(is_ent_start != 1){
            dest_ent = dest_ent->next;
        }
        t_rel *ent_rel_list = dest_ent->relation_type;

        if(ent_rel_list == NULL){
            t_rel *new_rel = (t_rel *)malloc(sizeof(t_rel));
            new_rel->id_rel = id_rel;
            new_rel->pointed_count = 1;
            new_rel->ent = (t_rel_ent *)malloc(sizeof(t_rel_ent));
            new_rel->ent->id_org = id_orig;
            new_rel->next = NULL;
            new_rel->next = NULL;
            dest_ent->relation_type = new_rel;
        }else{
            t_rel *relation = is_in_rel(id_rel, ent_rel_list);
            if(is_rel_end == 0){
                t_rel *new_rel = (t_rel *)malloc(sizeof(t_rel));
                new_rel->id_rel = id_rel;
                new_rel->pointed_count = 1;
                new_rel->ent = (t_rel_ent *)malloc(sizeof(t_rel_ent));
                new_rel->ent->id_org = id_orig;
                new_rel->next = NULL;
                new_rel->next = NULL;
                relation->next = new_rel;
            } else{
                if(relation->ent == NULL){
                    t_rel_ent *new_orig_ent = (t_rel_ent *) malloc(sizeof(t_rel_ent));
                    new_orig_ent->id_org = id_orig;
                    new_orig_ent->next = NULL;
                    relation->ent = new_orig_ent;
                } else {
                    t_rel_ent *origin_ent = is_in_rel_ent(id_orig, relation->ent);
                    if (is_rel_ent_end == 0) {
                        t_rel_ent *new_orig_ent = (t_rel_ent *) malloc(sizeof(t_rel_ent));
                        new_orig_ent->id_org = id_orig;
                        new_orig_ent->next = NULL;
                        origin_ent->next = new_orig_ent;
                        relation->pointed_count = relation->pointed_count +1;
                    }
                }
            }
        }
    }
}

int del_rel(char *id_orig, char *id_dest, char *id_rel, t_ent **entity_list ){
    t_ent *dest_ent = is_in_ent(id_dest, *entity_list);

    if(dest_ent != NULL){
        if(is_ent_start != 1){
            dest_ent = dest_ent->next;
        }
        t_rel *ent_rel_list = dest_ent->relation_type;

        if(ent_rel_list != NULL){
            t_rel *relation = is_in_rel(id_rel, ent_rel_list);

            if(relation != 0){
                t_rel_ent * rel_ent_list = relation->ent;
                if(rel_ent_list != NULL){
                    t_rel_ent *orig_ent = is_in_rel_ent(id_dest, rel_ent_list);
                    if(orig_ent != 0){
                        relation->pointed_count = relation->pointed_count - 1;
                        
                    }
                }
            }
        }
    }
}

void report(){
    //TODO
}

int main() {
    t_ent *entity_list = NULL;
    last_ent_node = &entity_list;

    add_ent("Marco", &entity_list);
    add_ent("Giovanni", &entity_list);
    add_ent("Luca", &entity_list);
    del_ent("Luca", &entity_list);
    print_ent(entity_list);
    printf("\n");
    add_rel("Marco", "Giovanni", "amico_di", &entity_list);
    add_rel("Luca", "Giovanni", "amico_di", &entity_list);
    add_rel("Marco", "Giovanni", "collega_di", &entity_list);
    print_all_rel("Giovanni", entity_list);

}