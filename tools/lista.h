#ifndef LISTA_H
#define LISTA_H
#include <stdbool.h>
#include <stdio.h>


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* Lista de nodos simplemente enlazada con puteros genéricos. */
struct lista;
typedef struct lista lista_t;

/* Iterador externo para la lista. */
struct lista_iter;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía, tanto el primer elemento como el ultimo son NULL.
lista_t *lista_crear(void);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

// Devuelve verdadero si la lista no tiene elementos, false en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento a la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al principio
// de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento a la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del ultimo elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del ultimo, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el ultimo elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t *lista);

// Devuelve el largo de la lista.
// Pre: la lista fue creada.
// Post: devolvió el largo de la lista.
size_t lista_largo(const lista_t *lista);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía. Se actualizó el primer elemento de la lista
void* lista_borrar_primero(lista_t *lista);

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/
//Crea un iterador para una lista
//Pre: La lista está creada
//Post: Se creó el iterador para la lista que fue pasada como parámetro
//Se devuelve un puntero al iterador o NULL en caso de falla.
//Se considera que el iterador está al final de la lista cuando su posición actual es NULL
lista_iter_t *lista_iter_crear(lista_t *lista);

// Obtiene el valor de la posición en la que se encuentre el iterador. Si la lista tiene
// elementos, se devuelve el valor de esa posición, si está vacía o al final de ella devuelve NULL.
// Pre: la lista y el iterador fueron creados.
// Post: se devolvió el elemento de la posición del iterador, cuando no está vacía ni se encuentra en el final.
void* lista_iter_ver_actual(const lista_iter_t *iter);

//Avanza a la siguiente posición de la lista.
//Pre: la lista y el iterador fueron creados.
//Post: Devuelve False si el iterador está al final de la lista o la lista está vacía, sino True.
bool lista_iter_avanzar(lista_iter_t *iter);

//Indica si el iterador se encuentra al final de la lista.
//Pre: La lista fue creada.
//Post: Devuelve True si el iterador está al final de la lista o la lista está vacía, sino True.
bool lista_iter_al_final(const lista_iter_t *iter);

// Agrega un nuevo elemento a la lista en la posición en la que se encuentra el iterador. Devuelve falso en caso de error.
// Pre: la lista y el iterador fueron creados.
// Post: se agregó un nuevo elemento a la lista en la posición del iterador.
// El elemento insertado va a tomar la posicion del elemento al que se apunta.
// Luego de una insercion, el iterador va a apuntar al nuevo elemento.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Saca el elemento de la lista en la posición en la que se encuentra el iterador. Si la lista tiene elementos y el iterador no está al final, 
//se quita el elemento apuntado de la lista, y se devuelve su valor, si está vacía o el iterador está al final, devuelve NULL.
// Pre: la lista y el iterador fueron creados.
// Post: se devolvió el valor del primer elemento que se pretendía eliminar, la lista
// contiene un elemento menos, si la lista no estaba vacía.
// Si el elemento eliminado fue el primero o el último, se actualiza esa información en la lista
void* lista_iter_borrar(lista_iter_t *iter);

// Destruye el iterador.
// Pre: El iterador fue creado.
// Post: Se eliminó el iterador.
void lista_iter_destruir(lista_iter_t *iter);

//Permite recorrer toda la lista, recibe como parámetros un puntero al operador que se le desea aplicar a cada elemento
//y otro puntero extra que el usuario puede decidir para qué lo usa. Sino se desea hacer uso del mismo, pasar NULL.
//Pre: La lista está creada.
//Post: El iterador recorre la lista aplicando el operador pasado como parámetro. Si se pasó NULL no hace nada.
//La lista dejará de recorrerse si visitar() devuelve false o se llega al final de la misma.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);


/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación realizada.
//
// Las pruebas deben emplazarse en el archivo ‘pruebas_lista.c’, y
// solamente pueden emplear la interfaz pública tal y como aparece en lista.h
// (esto es, las pruebas no pueden acceder a los miembros del struct lista).
//
// Para la implementación de las pruebas se debe emplear la función
// print_test(), como se ha visto en TPs anteriores.
void pruebas_lista_estudiante(void);

#endif  // LISTA_H