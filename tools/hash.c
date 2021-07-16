#include "lista.h"
#include "hash.h"
#include "string.h"
#include "stdlib.h"
#define TAM_INIC 20
#define FACTOR_REDIM 2
/// ESTRUCTURAS ///

// Los structs deben llamarse "hash" y "hash_iter".

typedef struct objeto
{
    void* elemento;
    char* clave;
}objeto_t;


typedef struct hash{
    lista_t** tabla;
    size_t tam;
    size_t cant;
    hash_destruir_dato_t destructor;
}hash_t;

typedef struct hash_iter{
    size_t indice;
    lista_iter_t* actual;
    const hash_t* hash;
} hash_iter_t;

// tipo de función para destruir dato
typedef void (*hash_destruir_dato_t)(void *);

size_t generar_indice(const char *clave, size_t tam){
    size_t hash = 5381;
    size_t c;

    while ((c = (size_t)(*clave++))) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash % tam;
}

char* _copiar_clave(char* clave, size_t letras){
    if(clave[letras] == '\0'){
        char* nueva = calloc(letras + 1, sizeof(char));
        nueva[letras] = '\0';
        return nueva;
    }
    char* nueva = _copiar_clave(clave, letras + 1);
    nueva[letras] = clave[letras];
    return nueva;
}

lista_iter_t* buscar_objeto(lista_t* celda, const char* clave){
    lista_iter_t* iter = lista_iter_crear(celda);
    objeto_t* objeto;
    while(!lista_iter_al_final(iter)){
        objeto = (objeto_t*)lista_iter_ver_actual(iter);
        if(strcmp(objeto->clave, clave) == 0){
            return iter;
        }
        lista_iter_avanzar(iter);
    }
    return iter;
}


void destruir_objeto(objeto_t* objeto, hash_t* hash){
    if(hash->destructor) hash->destructor(objeto->elemento);
    free(objeto->clave);
    free(objeto);
}



objeto_t* crear_objeto(void* elem, const char*clave){
    objeto_t* objeto = calloc(1, sizeof(objeto_t));
    if(!objeto) return NULL;
    char* clave_char = (char*)clave;
    objeto->clave = _copiar_clave(clave_char, 0);
    //if(objeto->clave[0] == '\0') printf("crear : la clave esta vacia\n\n");
    objeto->elemento = elem;
    return objeto;

}




bool redimensionar(hash_t* hash){
    
    lista_t** nueva = calloc(hash->cant*2, sizeof(lista_t*));
    lista_t** aux = hash->tabla;
    hash->tabla = nueva;
    size_t tope = hash->tam;
    hash->tam = hash->cant*2;

    lista_iter_t* iter;
    for(size_t indice = 0; indice < tope; indice++){
        if(aux[indice]){
            iter = lista_iter_crear(aux[indice]);
            while(!lista_iter_al_final(iter)){

                objeto_t* objeto = lista_iter_borrar(iter);
                hash->cant--;

                //printf("La cantidad del hash es %ld\n", hash->cant);
                hash_guardar(hash, (const char*)objeto->clave, objeto->elemento);
                
                //printf("Se encontro\n ", hash_pertenece(hash, objeto->clave));
                free(objeto->clave);
                free(objeto);       
                
                
                //printf("La cantidad del hash es %ld\n", hash->cant);
            }
            lista_iter_destruir(iter);
        }
    }
    for(size_t indice = 0; indice < tope; indice++){
        if(aux[indice]){
            lista_destruir(aux[indice], NULL);
        }
    }
    free(aux);
    //printf("La cantidad del hash es %ld\n", hash->cant); Esto da invalid read obvio
    //printf("La cantidad del hash es %ld\n", hash->cant);
    return true;
}
/// PRIMITIVAS ///

/* Crea el hash
 */
hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
    hash_t* hash = calloc(1, sizeof(hash_t));
    if(!hash) return NULL;
    hash->tam = TAM_INIC;
    hash->tabla = calloc(TAM_INIC, sizeof(lista_t*));
    hash->destructor = destruir_dato;
    return hash;

}

/* Guarda un elemento en el hash, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura hash fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool hash_guardar(hash_t *hash, const char *clave, void *dato){
    //if(clave) printf("Estamos incertando %s,  ", clave);
    size_t indice = generar_indice(clave, hash->tam);
    //printf("%ld\n", indice);
    if(!hash->tabla[indice]) hash->tabla[indice] = lista_crear();
    lista_t* celda = hash->tabla[indice];
    //Chequeado que la lista se crea
    //if(lista_esta_vacia(celda)) printf("La lista se crea vacia");
    lista_iter_t* iter = buscar_objeto(celda, clave);
    objeto_t* objeto = NULL;
    bool exito = false;

    if(lista_iter_al_final(iter)) {
        //printf("No se encuentra uno igual\n\n");
        objeto = crear_objeto(dato, clave);
        if(!objeto) exito = false;
        exito = lista_insertar_ultimo(celda, objeto);
        hash->cant++;
    }
    //else(lista_iter_destruir(iter));
    
    else{
        //printf("Se encuentra uno igual");
        objeto = (objeto_t*)lista_iter_ver_actual(iter); //Aca anotar si la asignacion se puede hacer sin redefinir tipo
        //printf("el elemento es %s", (char*)objeto->elemento);
      
        if(objeto && hash->destructor) hash->destructor(objeto->elemento); //Aca esta el problema de segmentation fault
        objeto->elemento = dato;
        exito = true;
    }
    if(hash->tam * FACTOR_REDIM  == hash->cant) redimensionar(hash); //Este + 1 magico arreglo cosas
    lista_iter_destruir(iter);
    return exito;
    
     
}

/* Borra un elemento del hash y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura hash fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *hash_borrar(hash_t *hash, const char *clave){
    size_t indice = generar_indice(clave, hash->tam);
    lista_t* celda = hash->tabla[indice];
    if(!celda) return NULL;
    lista_iter_t* iter = buscar_objeto(celda, clave);
    objeto_t* objeto = NULL;
    objeto = lista_iter_borrar(iter);
    lista_iter_destruir(iter);
    if(objeto){
        
        void* dato =  objeto->elemento;
        free(objeto->clave);
        free(objeto);
        hash->cant--;
        return dato;
    }
    else return NULL;
    
    


}

/* Obtiene el valor de un elemento del hash, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura hash fue inicializada
 */
void *hash_obtener(const hash_t *hash, const char *clave){
    size_t indice = generar_indice(clave, hash->tam);
    lista_t* celda = hash->tabla[indice];
    if(!celda) return NULL;
    
    lista_iter_t* iter = buscar_objeto(celda, clave);
    objeto_t* objeto = NULL;
    objeto = lista_iter_ver_actual(iter);
    lista_iter_destruir(iter);
    if(objeto) return objeto->elemento;
    else return NULL;
}

/* Determina si clave pertenece o no al hash.
 * Pre: La estructura hash fue inicializada
 */
bool hash_pertenece(const hash_t *hash, const char *clave){
    size_t indice = generar_indice(clave, hash->tam);
    lista_t* celda = hash->tabla[indice];
    if(!celda) return false;
    lista_iter_t* iter = buscar_objeto(celda, clave);
    objeto_t* objeto = lista_iter_ver_actual(iter);
    lista_iter_destruir(iter);
    if(objeto) return true;
    else return false;

}

/* Devuelve la cantidad de elementos del hash.
 * Pre: La estructura hash fue inicializada
 */
size_t hash_cantidad(const hash_t *hash){
    return hash->cant;
}

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura hash fue inicializada
 * Post: La estructura hash fue destruida
 */
void hash_destruir(hash_t *hash){
    for(size_t indice = 0; indice < hash->tam; indice++){
        objeto_t* objeto;
        if(hash->tabla[indice]){
            while (!lista_esta_vacia(hash->tabla[indice]))
            {
                objeto = lista_borrar_primero(hash->tabla[indice]);
                destruir_objeto(objeto, hash);
            }
        lista_destruir(hash->tabla[indice], NULL);    
        }
        
    }
    free(hash->tabla);
    free(hash);
}




/// ITERADOR ///

/* Iterador del hash */

// Crea iterador
hash_iter_t *hash_iter_crear(const hash_t *hash){
    hash_iter_t* iterador = calloc(1, sizeof(hash_iter_t));
    bool sigo = true;
    iterador->actual = NULL;
    for(size_t indice = 0; indice < hash->tam && sigo; indice++){
        if(hash->tabla[indice] && !lista_esta_vacia(hash->tabla[indice])){
            iterador->actual = lista_iter_crear(hash->tabla[indice]);
            iterador->indice = indice;
            sigo = false;
        }
    }
    iterador->hash = hash;
    return iterador;
}

// Avanza iterador
bool hash_iter_avanzar(hash_iter_t *iter){
    if(hash_iter_al_final(iter)) return false;
    
    lista_iter_avanzar(iter->actual);
    if(!lista_iter_al_final(iter->actual)) {
        
        return true;
    }
    lista_iter_destruir(iter->actual);
    iter->actual = NULL;
    bool sigo = true;
    size_t indice = iter->indice + 1;
    while(indice < iter->hash->tam && sigo){
        
        if(iter->hash->tabla[indice] && !lista_esta_vacia(iter->hash->tabla[indice])){
            iter->actual = lista_iter_crear(iter->hash->tabla[indice]);
            iter->indice = indice;
            sigo = false;
            
        }
        else indice++;
        
    }
    return true;
}

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *hash_iter_ver_actual(const hash_iter_t *iter){
    if(hash_iter_al_final(iter)) return NULL;
    objeto_t* objeto = lista_iter_ver_actual(iter->actual);
    return objeto->clave;
}

// Comprueba si terminó la iteración
bool hash_iter_al_final(const hash_iter_t *iter){
    if(iter->actual == NULL) return true;
    return false;
}

// Destruye iterador
void hash_iter_destruir(hash_iter_t *iter){
    lista_iter_destruir(iter->actual);
    free(iter);
}
