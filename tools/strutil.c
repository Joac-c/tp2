#include <string.h>
#include "strutil.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

char *substr(const char *str, size_t n){
    char* nueva = calloc(n + 1, sizeof(char));
    for(int i = 0; i < n; i++){
        nueva[i] = str[i];
        if(str[i] == '\0') return nueva;
    }
    nueva[n] = '\0';
    return nueva;
}

char** _split(const char* str, char sep, size_t palabras){
    bool sigo = true;
    size_t letras = 0;
    char* palabra;
    while(sigo){
        if(str[letras] == sep || str[letras] == '\0'){
            palabras++;
            palabra = substr(str, letras);
            if(str[letras] == '\0'){
                char** lista = calloc(palabras + 1, sizeof(char*));
                lista[palabras] = NULL;
                lista[palabras - 1] = palabra;
                return lista;
            }
            sigo = false;
        }
        letras++;
    }

    char** lista = _split(str+letras, sep, palabras);
    lista[palabras - 1] = palabra;
    return lista;
    

}

char **split(const char *str, char sep){ 
    char * puntero = (char*)str;
    size_t palabras = 0;
    return _split(puntero, sep, palabras);
}

/*
char* _join(char **strv, char sep, size_t caracteres, size_t indice,  size_t palabra){
    if(strv[palabra] == NULL) {
        
        if(palabra == 0) caracteres = 1; 
        char* cadena = calloc(caracteres , sizeof(char*));
        cadena[caracteres]  = '\0';
        return cadena;  
    }
    bool saltear = false;
    bool fin = false;
    if(strv[palabra][indice] == '\0'){
        palabra++;
        indice = 0;
        saltear = (sep == '\0');
        fin = true;   
        
    }
    else indice++;
    char* cadena = _join(strv, sep, caracteres + !saltear, indice, palabra);
    if(!saltear && !fin) cadena[caracteres] = strv[palabra][indice - 1];
    else if(!saltear && fin && strv[palabra] != NULL) cadena[caracteres] = sep;
    return cadena;

}

*/






char *join(char **strv, char sep){
    size_t palabra = 0;
    size_t indice = 0;
    size_t caracteres = 0;

    //Recorro las listas para contar caracteres
    bool saltear;
   
    while(strv[palabra] != NULL){
        saltear = false;
        
        if(strv[palabra][indice] == '\0'){
            palabra++;
            indice = 0;
            saltear = (sep == '\0');
        }
        else indice++;
        if(!saltear) caracteres++;
    }
    caracteres++;
    //Armo la cadena con el total de caracteres
    if(palabra == 0) caracteres = 1; 
    char* cadena = calloc(caracteres, sizeof(char*));
    cadena[caracteres]  = '\0';

    //coloco los caracteres
    palabra = 0;
    indice = 0;
    caracteres = 0;
    char letra;
    while(strv[palabra] != NULL){
        letra = strv[palabra][indice];
        saltear = false;
        
        if(letra == '\0'){
            palabra++;
            indice = 0;
            letra = sep;
            saltear = (sep == '\0');
        }
        else indice++;
        if(!saltear && strv[palabra] != NULL){
            cadena[caracteres] = letra;
            caracteres++;
        }

    }



    //char* cadena = _join(strv, sep, caracter, indice, palabra);
    return cadena;
}


void free_strv(char *strv[]){
    int indice = 0;
    while(strv[indice]){
        free(strv[indice]);
        indice++;

}
    free(strv);
}


