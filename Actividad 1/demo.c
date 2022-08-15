/*Juan Jose Marin y Stefania Hurtado */
/*Sistemas operativos*/

#include <stdlib.h>
#include <stdio.h>

typedef struct Nodo {
   int variable;
   struct Nodo *siguiente;
} tipoNodo;


typedef tipoNodo *pNodo;
typedef tipoNodo *Lista;


void insert(Lista *l, int v);
void delete(Lista *l, int v);
int vacia(Lista l);

int main() {
   Lista lista = NULL;
   pNodo p;
   insert(&lista, 20);
   insert(&lista, 10);
   insert(&lista, 30);
   delete(&lista, 10);
   imprint();
}

void insert(Lista *lista, int v) {
   pNodo nuevo, anterior;

   /* Crear un nodo nuevo */
   nuevo = (pNodo)malloc(sizeof(tipoNodo));
   nuevo->variable = v;
   
   if(vacia(*lista) || (*lista)->variable > v) {
                                                    /* Añadimos la lista a nuevo nodo */
      nuevo->siguiente = *lista;
                                                    /* Inicio lista es en nuevo nodo */
      *lista = nuevo;
   } else {
      anterior = *lista;
                                                    /* Avanzamos hasta el último elemento o hasta que el siguiente tenga
                                                    un variable mayor que v */
      while(anterior->siguiente && anterior->siguiente->variable <= v)
         anterior = anterior->siguiente;
                                                    /* Insertamos el nuevo nodo */
      nuevo->siguiente = anterior->siguiente;
      anterior->siguiente = nuevo;
   }
}

void delete(Lista *lista, int v) {
   pNodo anterior, nodo;

   nodo = *lista;
   anterior = NULL;
   while(nodo && nodo->variable < v) {
      anterior = nodo;
      nodo = nodo->siguiente;
   }
   if(!nodo || nodo->variable != v) return;
   else {                                   /* Borra el nodo */
      if(!anterior)                         /* Primera posicion */
         *lista = nodo->siguiente;
      else                                  /* un elemento cualquiera */
         anterior->siguiente = nodo->siguiente;
      free(nodo);
   }

}

int vacia(Lista lista) {
   return (lista == NULL);
}

void imprint(){
    int Theint = 10 ; int * thePointer ;
    thePointer = &Theint;
    printf ( " el puntero es % d" , * thePointer ) ;  /* Apuntador*/
    printf ( " El espacio de dirección para el puntero es % d" , thePointer ) ; /*posicion de memoria*/
    
}