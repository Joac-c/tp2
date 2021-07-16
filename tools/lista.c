#include "lista.h"

#include <stdio.h>
#include <stdlib.h>
/* *****************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// ...
typedef struct nodo{
    struct nodo* next;
    void* dato;
} nodo_t;

struct lista { 
    nodo_t *first;
    nodo_t *last;
    size_t len;
};

nodo_t *crear_nodo(void *valor){
    nodo_t *nodo=malloc(sizeof(nodo_t));
    if(!nodo){
        return NULL;
    }
    nodo->dato=valor;
    nodo->next=NULL;
    return nodo;
}

lista_t *lista_crear(void){
    lista_t *lista=malloc(sizeof(lista_t));
    if(!lista){
        return NULL;
    }
    lista->first=NULL;
    lista->last=NULL;
    lista->len=0;
    return lista;
}

bool lista_esta_vacia(const lista_t *lista){
    return lista->len==0;
}

void *lista_ver_primero(const lista_t *lista){
    if(lista_esta_vacia(lista)==true){
        return NULL;
    }
    return lista->first->dato;
}

void *lista_ver_ultimo(const lista_t *lista){
    if(lista_esta_vacia(lista)==true){
        return NULL;
    }
    return lista->last->dato;
}

bool lista_insertar_primero(lista_t *lista, void *dato){
    nodo_t *nuevo_nodo=crear_nodo(dato);
    if(!nuevo_nodo){
        return false;
    }
    if (lista_esta_vacia(lista)==true){
        lista->first=nuevo_nodo;
        lista->last=nuevo_nodo;
        lista->len++;
        return true;
    }
    nuevo_nodo->next=lista->first;
    lista->first=nuevo_nodo;
    lista->len++;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
    nodo_t *nuevo_nodo=crear_nodo(dato);
    if(!nuevo_nodo){
        return false;
    }
    if (lista_esta_vacia(lista)==true){
        lista->first=nuevo_nodo;
        lista->last=nuevo_nodo;
        lista->len++;
        return true;
    }
    lista->last->next=nuevo_nodo;
    lista->last=nuevo_nodo;
    lista->len++;
    return true;
}

size_t lista_largo(const lista_t *lista){
    return lista->len;
}

void* lista_borrar_primero(lista_t *lista){
    if(lista_esta_vacia(lista)==true){
        return NULL;
    }
    void *dato_a_borrar=lista_ver_primero(lista);
    nodo_t* nodo_a_eliminar=lista->first;
    lista->first=lista->first->next;
    free(nodo_a_eliminar);
    lista->len--;
    return dato_a_borrar;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)){
    if(lista_esta_vacia(lista)==true){
        free(lista);
        return;
    }
    void *dato=lista_borrar_primero(lista);
    if(destruir_dato){
        (*destruir_dato)(dato);
    }
    lista_destruir(lista, destruir_dato);
}

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
    if(!visitar){
        return;
    }
    if(lista_esta_vacia(lista)==true){
        return;
    }
    nodo_t* nodo=lista->first;
    while(nodo){
        if((*visitar)(nodo->dato, extra)==false){
            break;
        }      
        nodo=nodo->next;
    }
}
/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR
 * *****************************************************************/

// ...
struct lista_iter{ 
    lista_t *lista;
    nodo_t *pre;
    nodo_t *act;
};

lista_iter_t *lista_iter_crear(lista_t *lista){
    lista_iter_t *iterador=malloc(sizeof(lista_iter_t));
    if(!iterador){
        return NULL;
    }
    iterador->lista=lista;
    iterador->act=lista->first;
    iterador->pre=NULL;
    return iterador;
}

void* lista_iter_ver_actual(const lista_iter_t *iter){
    if(!iter->act){
        return NULL;    
    }
    return iter->act->dato;
}

bool lista_iter_avanzar(lista_iter_t *iter){
    if(!iter->act){
        return false;    
    }
    iter->pre=iter->act;
    iter->act=iter->act->next;
    return true;
}

bool lista_iter_al_final(const lista_iter_t *iter){
    if(!iter->act){
        return true;
    }
    return false;
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
    nodo_t* nodo=crear_nodo(dato);
    if(!nodo){
        return false;
    }
    nodo->next=iter->act;
    iter->act=nodo;
    if(!nodo->next){
        iter->lista->last=nodo;
    }   
    if(!iter->pre){
        iter->lista->first=nodo;
    }
    else{
        iter->pre->next=nodo;
    }     
    iter->lista->len++;
    return true;
}

void* lista_iter_borrar(lista_iter_t *iter){
    if(!iter->act){
        return NULL;
    }
    nodo_t* nodo=iter->act;
    void* dato=nodo->dato;
    iter->act=nodo->next;
    if(!iter->act){
        iter->lista->last=iter->pre;
    } 
    if(!iter->pre){
        iter->lista->first=iter->act;
    }
    else{
        iter->pre->next=iter->act;
    }
    iter->lista->len--;
    free(nodo);   
    return dato;
}

void lista_iter_destruir(lista_iter_t *iter){
    free(iter);
}