#include <stdio.h>
#include "abb.h"
#include "pa2mm.h"

#define ERROR -1
#define EXITO 0

typedef struct pokemon {
	int numero_pokedex;
	size_t peso;
} pokemon_t;

pokemon_t* crear_pokemon(int numero_pokedex) {
    pokemon_t* pokemon = malloc(1 * sizeof(pokemon_t));
   	if (pokemon) {
   		pokemon->numero_pokedex = numero_pokedex;
   	}
  
    return pokemon;
}

/* Recibe 2 Pokémon, compara sus números de Pokédex y devuelve 0 si son iguales, 1 si el primero es mayor 
que el segundo y -1 si el segundo es mayor que el primero. */
int comparador_pokemon_numero_pokedex(void* pokemon1, void* pokemon2) {
	if ((((pokemon_t*)pokemon1)->numero_pokedex) == (((pokemon_t*)pokemon2)->numero_pokedex)) {
		return 0;
	} else if ((((pokemon_t*)pokemon1)->numero_pokedex) > (((pokemon_t*)pokemon2)->numero_pokedex)) {
		return 1;
	} else {
		return -1;
	}
}

/* Recibe un elemento y lo libera. */
void destruir_pokemon(pokemon_t* pokemon) {
	free(pokemon);
}

/* Verifica que el elemento enviado se pueda liberar y llama a la función que lo libera. */
void destructor_elementos(void* elemento) {
	if (!elemento) {
		return;
	}

	destruir_pokemon((pokemon_t*)elemento);
}

void pruebas_arbol_crear() {
	abb_t* arbol = NULL;

	pa2m_afirmar(((arbol = arbol_crear(NULL, destructor_elementos)) == NULL), "Le paso un comparador NULL, y no me puede crear el árbol.");
	pa2m_afirmar(((arbol = arbol_crear(comparador_pokemon_numero_pokedex, NULL)) != NULL), "Le paso un destructor NULL, y me crea el árbol.");
	if (!arbol) {
		return;
	}
	pa2m_afirmar(((arbol->destructor) == NULL), "El destructor del árbol es NULL.");

	arbol_destruir(arbol);

	pa2m_afirmar(((arbol = arbol_crear(comparador_pokemon_numero_pokedex, destructor_elementos)) != NULL), "Le paso un comparador y un destructor, y me crea el árbol.");
	if (!arbol) {
		return;
	}
	pa2m_afirmar(((arbol->nodo_raiz) == NULL), "El nodo raíz se mantiene en NULL.");
	pa2m_afirmar(((arbol->comparador) == comparador_pokemon_numero_pokedex), "El comparador del árbol es el correcto.");
	pa2m_afirmar(((arbol->destructor) == destructor_elementos), "El destructor del árbol es el correcto.");

	arbol_destruir(arbol);
}

/* Asumo que la función arbol_crear funciona correctamente. */
void pruebas_arbol_insertar() {
	abb_t* arbol = arbol_crear(comparador_pokemon_numero_pokedex, NULL);

	pokemon_t pokemon1 = {10, 5};
	pokemon_t pokemon2 = {20, 100};
	pokemon_t pokemon3 = {4, 20};

	pa2m_afirmar(((arbol_insertar(NULL, &pokemon1)) == ERROR), "Le paso un árbol NULL y me devuelve error.");
	pa2m_afirmar(((arbol_insertar(arbol, &pokemon1)) == EXITO), "Le paso un árbol vacío e inserta correctamente el elemento en el nodo raíz.");
	pa2m_afirmar(arbol->nodo_raiz->elemento == &pokemon1, "El elemento insertado en el nodo raíz es el correcto.");

	pa2m_afirmar(((arbol_insertar(arbol, &pokemon2)) == EXITO), "Le paso un elemento mayor al nodo raíz, e inserta correctamente el elemento a la derecha de este.");
	pa2m_afirmar(arbol->nodo_raiz->derecha->elemento == &pokemon2, "El elemento insertado a la derecha del nodo raíz es el correcto.");

	pa2m_afirmar(((arbol_insertar(arbol, &pokemon3)) == EXITO), "Le paso un elemento menor al nodo raíz, e inserta correctamente el elemento a la izquierda de este.");
	pa2m_afirmar(arbol->nodo_raiz->izquierda->elemento == &pokemon3, "El elemento insertado a la izquierda del  nodo raíz es el correcto.");

	pa2m_afirmar(((arbol_insertar(arbol, &pokemon3)) == EXITO), "Le paso un elemento igual al anterior, e inserta correctamente el elemento a la izquierda de este.");
	pa2m_afirmar(arbol->nodo_raiz->izquierda->izquierda->elemento == &pokemon3, "El elemento insertado a la izquierda del anterior es el correcto.");

	pa2m_afirmar(((arbol_insertar(arbol, &pokemon2)) == EXITO), "Le paso un elemento igual al nodo raíz derecho, e inserta correctamente el elemento a la izquierda de este.");
	pa2m_afirmar(arbol->nodo_raiz->derecha->izquierda->elemento == &pokemon2, "El elemento insertado a la izquierda del nodo raíz derecho es el correcto.");

	arbol_destruir(arbol);

	arbol = arbol_crear(comparador_pokemon_numero_pokedex, destructor_elementos);

	pokemon_t* pokemon = crear_pokemon(151);
	if (!pokemon) {
		arbol_destruir(arbol);
	}

	pa2m_afirmar(((arbol_insertar(arbol, pokemon)) == EXITO), "Le paso un elemento que reservó memoria y lo inserta correctamente en el nodo raíz.");
	pa2m_afirmar(arbol->nodo_raiz->elemento == pokemon, "El elemento insertado en el nodo raíz es el correcto.");

	arbol_destruir(arbol);

	arbol = arbol_crear(comparador_pokemon_numero_pokedex, NULL);

	bool estado_insercion = ERROR;

	for (int i = 0; i < 1000; i++) {
		estado_insercion = arbol_insertar(arbol, &i);
	}

	pa2m_afirmar(estado_insercion == EXITO, "Intento insertar 1000 elementos y se insertan correctamente.");

	arbol_destruir(arbol);
}

/* Asumo que la función arbol_insertar funciona correctamente. */
void pruebas_arbol_borrar() {
	abb_t* arbol = arbol_crear(comparador_pokemon_numero_pokedex, NULL);

	pokemon_t pokemon1 = {1, 10};
	pokemon_t pokemon2 = {4, 10};
	pokemon_t pokemon3 = {4, 10};
	pokemon_t pokemon4 = {7, 10};
	pokemon_t pokemon5 = {5, 10};
	pokemon_t pokemon6 = {8, 10};
	pokemon_t pokemon7 = {3, 10};
	pokemon_t pokemon8 = {6, 10};
	pokemon_t pokemon_auxiliar = {0, 10};

	pokemon_auxiliar.numero_pokedex = pokemon1.numero_pokedex;
	pa2m_afirmar(((arbol_borrar(NULL, &pokemon_auxiliar)) == ERROR), "Le paso un árbol NULL y me devuelve error.");
	pa2m_afirmar(((arbol_borrar(arbol, &pokemon_auxiliar)) == ERROR), "Le paso un árbol vacío y me devuelve error.");
	arbol_insertar(arbol, &pokemon1);
	pa2m_afirmar(((arbol_borrar(arbol, &pokemon_auxiliar)) == EXITO), "Le paso un árbol con un elemento y borra el nodo raíz correctamente.");
	pa2m_afirmar(arbol->nodo_raiz == NULL, "El nodo raíz del árbol es NULL.");
	arbol_insertar(arbol, &pokemon1);
	arbol_insertar(arbol, &pokemon2);
	arbol_insertar(arbol, &pokemon3);
	pokemon_auxiliar.numero_pokedex = pokemon3.numero_pokedex;
	pa2m_afirmar(((arbol_borrar(arbol, &pokemon_auxiliar)) == EXITO), "Le paso un árbol con tres elementos (ninguno con dos hijos) y borra el anteúltimo correctamente.");
	pa2m_afirmar(arbol->nodo_raiz->derecha->izquierda == NULL, "El anterior al último elemento apunta a NULL.");
	pokemon_auxiliar.numero_pokedex = pokemon2.numero_pokedex;
	pa2m_afirmar(((arbol_borrar(arbol, &pokemon_auxiliar)) == EXITO), "Le paso un árbol con dos elementos y borra el último correctamente.");
	pa2m_afirmar(arbol->nodo_raiz->derecha == NULL, "El anterior al último apunta a NULL.");
	pa2m_afirmar(((arbol_borrar(arbol, &pokemon_auxiliar)) == ERROR), "Le paso un elemento que no existe y me devuelve error.");
	arbol_insertar(arbol, &pokemon2);
	arbol_insertar(arbol, &pokemon3);
	pokemon_auxiliar.numero_pokedex = pokemon1.numero_pokedex;
	pa2m_afirmar(((arbol_borrar(arbol, &pokemon_auxiliar)) == EXITO), "Le paso un árbol con tres elementos (ninguno con dos hijos) y borra el nodo raíz correctamente.");
	pa2m_afirmar(arbol->nodo_raiz->elemento == &pokemon2, "El nodo raíz apunta al que era el segundo elemento.");
	arbol_insertar(arbol, &pokemon1);
	pokemon_auxiliar.numero_pokedex = pokemon3.numero_pokedex;
	pa2m_afirmar(((arbol_borrar(arbol, &pokemon_auxiliar)) == EXITO), "Le paso un árbol con tres elementos (ninguno con dos hijos) y borra el anteúltimo correctamente.");
	pa2m_afirmar(arbol->nodo_raiz->izquierda->elemento == &pokemon1, "El último elemento ahora esta en el lugar del que era el anteúltimo.");
	arbol_insertar(arbol, &pokemon4);
	arbol_insertar(arbol, &pokemon5);
	arbol_insertar(arbol, &pokemon6);
	arbol_insertar(arbol, &pokemon1);
	arbol_insertar(arbol, &pokemon7);
	pokemon_auxiliar.numero_pokedex = pokemon2.numero_pokedex;
	pa2m_afirmar(((arbol_borrar(arbol, &pokemon_auxiliar)) == EXITO), "Le paso un árbol con un nodo raíz con dos hijos y borra este correctamente.");
	pa2m_afirmar(arbol->nodo_raiz->elemento == &pokemon7, "El nodo raíz apunta al elemento correcto.");
	pa2m_afirmar(((arbol->nodo_raiz->derecha->elemento == &pokemon4) && (arbol->nodo_raiz->derecha->derecha->elemento == &pokemon6) && (arbol->nodo_raiz->derecha->izquierda->elemento == &pokemon5) && (arbol->nodo_raiz->izquierda->elemento == &pokemon1) && (arbol->nodo_raiz->izquierda->izquierda->elemento == &pokemon1) && (arbol->nodo_raiz->izquierda->derecha == NULL)), "El árbol está como debería estar.");
	arbol_insertar(arbol, &pokemon8);
	pokemon_auxiliar.numero_pokedex = pokemon4.numero_pokedex;
	pa2m_afirmar(((arbol_borrar(arbol, &pokemon_auxiliar)) == EXITO), "Le paso un árbol con un nodo con dos hijos y borra este correctamente.");
	pa2m_afirmar(arbol->nodo_raiz->derecha->elemento == &pokemon8, "El elemento mayor de los menores ahora esta en el lugar del elemento borrado.");
	pa2m_afirmar(((arbol->nodo_raiz->elemento == &pokemon7) && (arbol->nodo_raiz->derecha->elemento == &pokemon8) && (arbol->nodo_raiz->derecha->derecha->elemento == &pokemon6) && (arbol->nodo_raiz->derecha->izquierda->elemento == &pokemon5) && (arbol->nodo_raiz->izquierda->elemento == &pokemon1) && (arbol->nodo_raiz->izquierda->izquierda->elemento == &pokemon1) && (arbol->nodo_raiz->derecha->izquierda->derecha == NULL)), "El árbol está como debería estar.");

	arbol_destruir(arbol);

	arbol = arbol_crear(comparador_pokemon_numero_pokedex, destructor_elementos);

	pokemon_t* pokemon = crear_pokemon(500);
	if (!pokemon) {
		arbol_destruir(arbol);
	}

	arbol_insertar(arbol, pokemon);
	pa2m_afirmar(((arbol_borrar(arbol, pokemon)) == EXITO), "Le paso un elemento que reservó memoria y lo borra correctamente.");
	pa2m_afirmar(arbol->nodo_raiz == NULL, "El nodo raíz es NULL.");

	arbol_destruir(arbol);
}

void pruebas_arbol_buscar() {
	abb_t* arbol = arbol_crear(comparador_pokemon_numero_pokedex, NULL);

	pokemon_t pokemon1 = {44, 23};
	pokemon_t pokemon2 = {76, 10};
	pokemon_t pokemon3 = {55, 532};
	pokemon_t pokemon4 = {33, 23};
	pokemon_t pokemon_auxiliar = {0, 10};

	pokemon_auxiliar.numero_pokedex = pokemon3.numero_pokedex;
	pa2m_afirmar(((arbol_buscar(NULL, &pokemon_auxiliar)) == NULL), "Le paso un árbol NULL y me devuelve NULL.");
	pa2m_afirmar(((arbol_buscar(arbol, &pokemon_auxiliar)) == NULL), "Le paso un árbol vacío y me devuelve NULL.");
	arbol_insertar(arbol, &pokemon1);
	arbol_insertar(arbol, &pokemon2);
	arbol_insertar(arbol, &pokemon3);
	pa2m_afirmar(((arbol_buscar(arbol, &pokemon_auxiliar)) == &pokemon3), "Le paso un árbol con tres elemento y me devuelve el elemento buscado.");
	pokemon_auxiliar.numero_pokedex = pokemon4.numero_pokedex;
	pa2m_afirmar(((arbol_buscar(arbol, &pokemon_auxiliar)) == NULL), "Intenta buscar un elemento que no existe y me devuelve NULL.");

	arbol_destruir(arbol);
}

void pruebas_arbol_raiz() {
	abb_t* arbol = arbol_crear(comparador_pokemon_numero_pokedex, NULL);

	pokemon_t pokemon = {1, 10};

	pa2m_afirmar(((arbol_raiz(NULL)) == NULL), "Le paso un árbol NULL y me devuelve NULL.");
	pa2m_afirmar(((arbol_raiz(arbol)) == NULL), "Le paso un árbol vacío y me devuelve NULL.");
	arbol_insertar(arbol, &pokemon);
	pa2m_afirmar(((arbol_raiz(arbol)) == &pokemon), "Le paso un árbol con un elemento y me devuelve la raíz del árbol correctamente.");

	arbol_destruir(arbol);
}

void pruebas_arbol_vacio() {
	abb_t* arbol = arbol_crear(comparador_pokemon_numero_pokedex, NULL);

	pokemon_t pokemon = {9, 50};

	pa2m_afirmar(((arbol_vacio(NULL)) == true), "Le paso un árbol NULL y me devuelve true.");
	pa2m_afirmar(((arbol_vacio(arbol)) == true), "Le paso un árbol vacío y me devuelve true.");
	arbol_insertar(arbol, &pokemon);
	pa2m_afirmar(((arbol_vacio(arbol)) == false), "Le paso un árbol con un elemento y me devuelve que false.");

	arbol_destruir(arbol);
}

void pruebas_arbol_recorrido_inorden() {
	abb_t* arbol = arbol_crear(comparador_pokemon_numero_pokedex, NULL);

	pokemon_t pokemon1 = {1, 20};
	pokemon_t pokemon2 = {2, 20};
	pokemon_t pokemon3 = {3, 20};
	pokemon_t pokemon4 = {4, 20};
	pokemon_t pokemon5 = {5, 20};
	pokemon_t pokemon6 = {6, 20};
	pokemon_t pokemon7 = {7, 20};
	pokemon_t pokemon8 = {8, 20};
	pokemon_t* vector_pokemon[8];

	arbol_insertar(arbol, &pokemon5);
	arbol_insertar(arbol, &pokemon2);
	arbol_insertar(arbol, &pokemon7);
	arbol_insertar(arbol, &pokemon3);
	arbol_insertar(arbol, &pokemon1);
	arbol_insertar(arbol, &pokemon6);
	arbol_insertar(arbol, &pokemon8);
	arbol_insertar(arbol, &pokemon4);

	printf("Recorrido inorden, debería salir: 1 2 3 4 5 6 7 8.\n");
	size_t cantidad = arbol_recorrido_inorden(arbol, (void**)vector_pokemon, 8);
	for (size_t i = 0; i < cantidad; i++) {
		printf("%i ", vector_pokemon[i]->numero_pokedex);
	}
	printf("\n");

	arbol_destruir(arbol);
}

void pruebas_arbol_recorrido_preorden() {
	abb_t* arbol = arbol_crear(comparador_pokemon_numero_pokedex, NULL);

	pokemon_t pokemon1 = {1, 20};
	pokemon_t pokemon2 = {2, 20};
	pokemon_t pokemon3 = {3, 20};
	pokemon_t pokemon4 = {4, 20};
	pokemon_t pokemon5 = {5, 20};
	pokemon_t pokemon6 = {6, 20};
	pokemon_t pokemon7 = {7, 20};
	pokemon_t pokemon8 = {8, 20};
	pokemon_t* vector_pokemon[8];

	arbol_insertar(arbol, &pokemon5);
	arbol_insertar(arbol, &pokemon2);
	arbol_insertar(arbol, &pokemon7);
	arbol_insertar(arbol, &pokemon3);
	arbol_insertar(arbol, &pokemon1);
	arbol_insertar(arbol, &pokemon6);
	arbol_insertar(arbol, &pokemon8);
	arbol_insertar(arbol, &pokemon4);

	printf("Recorrido postorden, debería salir: 5 2 1 3 4 7 6 8.\n");
	size_t cantidad = arbol_recorrido_preorden(arbol, (void**)vector_pokemon, 8);
	for (size_t i = 0; i < cantidad; i++) {
		printf("%i ", vector_pokemon[i]->numero_pokedex);
	}
	printf("\n");

	arbol_destruir(arbol);
}

void pruebas_arbol_recorrido_postorden() {
	abb_t* arbol = arbol_crear(comparador_pokemon_numero_pokedex, NULL);

	pokemon_t pokemon1 = {1, 20};
	pokemon_t pokemon2 = {2, 20};
	pokemon_t pokemon3 = {3, 20};
	pokemon_t pokemon4 = {4, 20};
	pokemon_t pokemon5 = {5, 20};
	pokemon_t pokemon6 = {6, 20};
	pokemon_t pokemon7 = {7, 20};
	pokemon_t pokemon8 = {8, 20};
	pokemon_t* vector_pokemon[8];

	arbol_insertar(arbol, &pokemon5);
	arbol_insertar(arbol, &pokemon2);
	arbol_insertar(arbol, &pokemon7);
	arbol_insertar(arbol, &pokemon3);
	arbol_insertar(arbol, &pokemon1);
	arbol_insertar(arbol, &pokemon6);
	arbol_insertar(arbol, &pokemon8);
	arbol_insertar(arbol, &pokemon4);

	printf("Recorrido postorden, debería salir: 1 4 3 2 6 8 7 5.\n");
	size_t cantidad = arbol_recorrido_postorden(arbol, (void**)vector_pokemon, 8);
	for (size_t i = 0; i < cantidad; i++) {
		printf("%i ", vector_pokemon[i]->numero_pokedex);
	}
	printf("\n");

	printf("Recorrido postorden, debería salir: 1 4 3 2 6 8.\n");
	cantidad = arbol_recorrido_postorden(arbol, (void**)vector_pokemon, 6);
	for (size_t i = 0; i < cantidad; i++) {
		printf("%i ", vector_pokemon[i]->numero_pokedex);
	}
	printf("\n");

	arbol_destruir(arbol);
}

/* Suma los elementos de cada nodo. */
bool sumar_elementos(void *elemento, void *suma) {
	if((elemento) && (suma)) {
		*(int*)suma += *(int*)elemento;
		printf("%i ", ((pokemon_t*)elemento)->numero_pokedex);
	}

    return false;
}

/* Muestra hasta el elemento 6. */
bool mostrar_hasta_6(void* elemento, void* extra) {
    extra=extra;
    if (elemento) {
    	printf("%i ", ((pokemon_t*)elemento)->numero_pokedex);
    	if (((pokemon_t*)elemento)->numero_pokedex == 6) {
    		return true;
    	}
    }

    return false;
}

void prueba_arbol_iterador_interno() {
	abb_t* arbol = arbol_crear(comparador_pokemon_numero_pokedex, NULL);

	pokemon_t pokemon1 = {1, 20};
	pokemon_t pokemon2 = {2, 20};
	pokemon_t pokemon3 = {3, 20};
	pokemon_t pokemon4 = {4, 20};
	pokemon_t pokemon5 = {5, 20};
	pokemon_t pokemon6 = {6, 20};
	pokemon_t pokemon7 = {7, 20};
	pokemon_t pokemon8 = {8, 20};

	int suma = 0;
	size_t elementos_recorridos = 0;

	pa2m_afirmar((abb_con_cada_elemento(NULL, ABB_RECORRER_PREORDEN, sumar_elementos, (void*)&suma) == 0), "Le paso un árbol NULL y me devuelve 0.");
	pa2m_afirmar((abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, sumar_elementos, (void*)&suma) == 0), "Le paso un árbol vacío y me devuelve 0.");
	arbol_insertar(arbol, &pokemon5);
	arbol_insertar(arbol, &pokemon2);
	arbol_insertar(arbol, &pokemon7);
	arbol_insertar(arbol, &pokemon3);
	arbol_insertar(arbol, &pokemon1);
	arbol_insertar(arbol, &pokemon6);
	arbol_insertar(arbol, &pokemon8);
	arbol_insertar(arbol, &pokemon4);
	pa2m_afirmar((abb_con_cada_elemento(arbol, 6, sumar_elementos, (void*)&suma) == 0), "Le paso un recorrido inexistente y me devuelve 0.");
	pa2m_afirmar(((elementos_recorridos = (abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, sumar_elementos, (void*)&suma))) == 8), "Le paso una lista con elementos, los recorre correctamente y los suma.");
	printf("Recorri %lu elementos con el iterador interno y la suma de elementos total es %i\n", elementos_recorridos, suma);

	pa2m_afirmar(((elementos_recorridos = (abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, mostrar_hasta_6, NULL))) == 6), "Le paso una lista con elementos y los recorre correctamente hasta el 6.");
	printf("Recorri %lu elementos con el iterador interno.\n", elementos_recorridos);

	pa2m_afirmar(((elementos_recorridos = (abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, mostrar_hasta_6, NULL))) == 7), "Le paso una lista con elementos y los recorre correctamente hasta el 6.");
	printf("Recorri %lu elementos con el iterador interno.\n", elementos_recorridos);

	arbol_destruir(arbol);
}

int main() {
	pa2m_nuevo_grupo("Pruebas de crear un árbol.");
	pruebas_arbol_crear();

	pa2m_nuevo_grupo("Pruebas de insertar un elemento en un árbol.");
	pruebas_arbol_insertar();

	pa2m_nuevo_grupo("Pruebas de borrar un elemento en un árbol.");
	pruebas_arbol_borrar();

	pa2m_nuevo_grupo("Pruebas de buscar elemento en un árbol.");
	pruebas_arbol_buscar();

	pa2m_nuevo_grupo("Pruebas de devolver la raíz de un árbol.");
	pruebas_arbol_raiz();

	pa2m_nuevo_grupo("Pruebas de devolver si el árbol esta vacío.");
	pruebas_arbol_vacio();

	pa2m_nuevo_grupo("Pruebas de recorrer el árbol en secuencia inorden.");
	pruebas_arbol_recorrido_inorden();

	pa2m_nuevo_grupo("Pruebas de recorrer el árbol en secuencia preorden.");
	pruebas_arbol_recorrido_preorden();

	pa2m_nuevo_grupo("Pruebas de recorrer el árbol en secuencia postorden.");
	pruebas_arbol_recorrido_postorden();

	pa2m_nuevo_grupo("Pruebas de iterador interno del árbol.");
	prueba_arbol_iterador_interno();
}