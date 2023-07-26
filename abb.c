#include <stdio.h>
#include <stdlib.h>
#include "abb.h"

#define ERROR -1
#define EXITO 0

abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor) {
	if (!comparador) {
		return NULL;
	}

	abb_t* arbol = malloc(1 * sizeof(abb_t));
	if (!arbol) {
		return NULL;
	}

	arbol->nodo_raiz = NULL;
	arbol->comparador = comparador;
	arbol->destructor = destructor;

	return arbol;
}

/* Recibe un nodo y lo inserta recursivamente dependiendo de 
si es mayor o menor que el nodo anterior. Si son iguales, se inserta en la izquierda. */
nodo_abb_t* nodo_insertar(abb_comparador comparador, nodo_abb_t* nodo, void* elemento) {
	if (!nodo) {
		nodo = calloc(1, sizeof(nodo_abb_t));
		if (!nodo) {
			return NULL;
		}
		nodo->elemento = elemento;
		return nodo;
	}

	if ((comparador(elemento, nodo->elemento)) > 0) {
		nodo->derecha = nodo_insertar(comparador, nodo->derecha, elemento);
	} else {
		nodo->izquierda = nodo_insertar(comparador, nodo->izquierda, elemento);
	}

	return nodo;
}

int arbol_insertar(abb_t* arbol, void* elemento) {
	if (!arbol) {
		return ERROR;
	}

	nodo_abb_t* nodo_nuevo = NULL;

	if (!arbol->nodo_raiz) {
		nodo_nuevo = nodo_insertar(arbol->comparador, nodo_nuevo, elemento);
		arbol->nodo_raiz = nodo_nuevo;
		return EXITO;
	}

	nodo_nuevo = arbol->nodo_raiz;
	nodo_nuevo = nodo_insertar(arbol->comparador, nodo_nuevo, elemento);

	return EXITO;
}

bool arbol_vacio(abb_t* arbol) {
	if ((!arbol) || (!arbol->nodo_raiz)) {
		return true;
	}

	return false;
}

/* Busca en el árbol el nodo que tenga el elemento buscado y lo devuelve. */
nodo_abb_t* buscar_nodo(abb_comparador comparador, nodo_abb_t* nodo, void* elemento) {
	if (!nodo) {
		return NULL;
	}

	if ((comparador(elemento, nodo->elemento)) == 0) {
		return nodo;
	}

	if ((comparador(elemento, nodo->elemento)) > 0) {
		nodo = buscar_nodo(comparador, nodo->derecha, elemento);
	} else {
		nodo = buscar_nodo(comparador, nodo->izquierda, elemento);
	}

	return nodo;
}

/* Busca en el árbol el nodo que tenga el elemento buscado y devuelve el anterior a este. */
nodo_abb_t* buscar_nodo_anterior(abb_comparador comparador, nodo_abb_t* nodo, void* elemento) {
	if ((comparador(elemento, nodo->elemento)) > 0) {
		if ((comparador(elemento, nodo->derecha->elemento)) == 0) {
			return nodo;
		}
		nodo = buscar_nodo_anterior(comparador, nodo->derecha, elemento);	
	} else {
		if ((comparador(elemento, nodo->izquierda->elemento)) == 0) {
			return nodo;
		}
		nodo = buscar_nodo_anterior(comparador, nodo->izquierda, elemento);
	}

	return nodo;
}

/* Asigna el nodo correspondiente a la derecha o izquierda del anterior 
al elemento borrar, dependiendo de donde esta. */
void asignar_nodo_izquierda_derecha(nodo_abb_t* nodo_correspondiente, nodo_abb_t* nodo_a_borrar, nodo_abb_t** nodo_derecha, nodo_abb_t** nodo_izquierda) {
	if (*nodo_derecha == nodo_a_borrar) {
		*nodo_derecha = nodo_correspondiente;
	} else {
		*nodo_izquierda = nodo_correspondiente;
	}
}

/* Borra el elemento en el nodo raíz del arbol. Cuando el nodo tiene dos hijos, 
lo reemplaza por el mayor de los menores. */
void arbol_borrar_nodo_raiz(abb_t* arbol, void* elemento) {
	nodo_abb_t* nodo_a_borrar = arbol->nodo_raiz;

	if ((!arbol->nodo_raiz->derecha) && (!arbol->nodo_raiz->izquierda)) {
		arbol->nodo_raiz = NULL;
	} else if (!arbol->nodo_raiz->derecha) {
		arbol->nodo_raiz = arbol->nodo_raiz->izquierda;
	} else if (!arbol->nodo_raiz->izquierda) {
		arbol->nodo_raiz = arbol->nodo_raiz->derecha;
	} else {
		nodo_abb_t* nodo_aux = arbol->nodo_raiz->izquierda;
		nodo_abb_t* nodo_anterior_aux = arbol->nodo_raiz;
		while (nodo_aux->derecha) {
			nodo_anterior_aux = nodo_aux;
			nodo_aux = nodo_aux->derecha;
		}
		arbol->nodo_raiz = nodo_aux;

		if (nodo_a_borrar->derecha != nodo_aux) {
			nodo_aux->derecha = nodo_a_borrar->derecha;
		}

		if (nodo_a_borrar->izquierda != nodo_aux) {
			nodo_aux->izquierda = nodo_a_borrar->izquierda;
		}

		asignar_nodo_izquierda_derecha(NULL, nodo_aux, &nodo_anterior_aux->derecha, &nodo_anterior_aux->izquierda);
	}

	if (arbol->destructor) {
		arbol->destructor(nodo_a_borrar->elemento);
	}
	free(nodo_a_borrar);
}

int arbol_borrar_nodo(abb_t* arbol, void* elemento) {
	nodo_abb_t* nodo_a_borrar = arbol->nodo_raiz;
	nodo_abb_t* nodo_anterior = arbol->nodo_raiz;

	nodo_a_borrar = buscar_nodo(arbol->comparador, nodo_a_borrar, elemento);
	if (!nodo_a_borrar) {
		return ERROR;
	}

	nodo_anterior = buscar_nodo_anterior(arbol->comparador, nodo_anterior, elemento);

	if ((!nodo_a_borrar->derecha) && (!nodo_a_borrar->izquierda)) {
		if (nodo_anterior->derecha == nodo_a_borrar) {
			nodo_anterior->derecha = NULL;
		} else {
			nodo_anterior->izquierda = NULL;
		}
		
	} else if (!nodo_a_borrar->derecha) {
		asignar_nodo_izquierda_derecha(nodo_a_borrar->izquierda, nodo_a_borrar, &nodo_anterior->derecha, &nodo_anterior->izquierda);
	} else if (!nodo_a_borrar->izquierda) {
		asignar_nodo_izquierda_derecha(nodo_a_borrar->derecha, nodo_a_borrar, &nodo_anterior->derecha, &nodo_anterior->izquierda);
	} else {
		nodo_abb_t* nodo_aux = nodo_a_borrar->izquierda;
		nodo_abb_t* nodo_anterior_aux = nodo_a_borrar;
		while (nodo_aux->derecha) {
			nodo_anterior_aux = nodo_aux;
			nodo_aux = nodo_aux->derecha;
		}

		asignar_nodo_izquierda_derecha(nodo_aux, nodo_a_borrar, &nodo_anterior->derecha, &nodo_anterior->izquierda);

		if (nodo_a_borrar->derecha != nodo_aux) {
			nodo_aux->derecha = nodo_a_borrar->derecha;
		}
		if (nodo_a_borrar->izquierda != nodo_aux) {
			nodo_aux->izquierda = nodo_a_borrar->izquierda;
		}

		if (nodo_anterior_aux != nodo_a_borrar) {
			if (nodo_anterior_aux->derecha == nodo_aux) {
				nodo_anterior_aux->derecha = NULL;
			} else {
				nodo_anterior_aux->izquierda = NULL;
			}
		}
	}

	if (arbol->destructor) {
		arbol->destructor(nodo_a_borrar->elemento);
	}
	free(nodo_a_borrar);
	return EXITO;
}

int arbol_borrar(abb_t* arbol, void* elemento) {
	if (arbol_vacio(arbol)) {
		return ERROR;
	}

	if ((arbol->comparador(elemento, arbol->nodo_raiz->elemento)) == 0) {
		arbol_borrar_nodo_raiz(arbol, elemento);
		return EXITO;
	}

	if ((arbol_borrar_nodo(arbol, elemento)) == ERROR) {
		return ERROR;
	}

	return EXITO;
}

void* arbol_buscar(abb_t* arbol, void* elemento) {
	if (arbol_vacio(arbol)) {
		return NULL;
	}

	nodo_abb_t* nodo = arbol->nodo_raiz;

	nodo = buscar_nodo(arbol->comparador, nodo, elemento);
	if (!nodo) {
		return NULL;
	}

	return nodo->elemento;
}

void* arbol_raiz(abb_t* arbol) {
	if (arbol_vacio(arbol)) {
		return NULL;
	}

	return arbol->nodo_raiz->elemento;
}

/* Recorre en secuencia inorden el árbol e inserta cada elemento en el vector. */
size_t arbol_insertar_vector_inorden(nodo_abb_t* nodo, void** array, size_t cantidad_elementos, size_t tamanio_array) {
	if ((!nodo) || (cantidad_elementos == tamanio_array)) {
		return cantidad_elementos;
	}

	cantidad_elementos = arbol_insertar_vector_inorden(nodo->izquierda, array, cantidad_elementos, tamanio_array);

	array[cantidad_elementos] = nodo->elemento;
	cantidad_elementos += 1;

	cantidad_elementos = arbol_insertar_vector_inorden(nodo->derecha, array, cantidad_elementos, tamanio_array);

	return cantidad_elementos;
}

size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array) {
	if (arbol_vacio(arbol)) {
		return 0;
	}

	nodo_abb_t* nodo = arbol->nodo_raiz;
	size_t cantidad_elementos = 0;

	cantidad_elementos = arbol_insertar_vector_inorden(nodo, array, cantidad_elementos, tamanio_array);

	return cantidad_elementos;
}

/* Recorre en secuencia preorden el árbol e inserta cada elemento en el vector. */
size_t arbol_insertar_vector_preorden(nodo_abb_t* nodo, void** array, size_t cantidad_elementos, size_t tamanio_array) {
	if ((!nodo) || (cantidad_elementos == tamanio_array)) {
		return cantidad_elementos;
	}

	array[cantidad_elementos] = nodo->elemento;
	cantidad_elementos += 1;

	cantidad_elementos = arbol_insertar_vector_preorden(nodo->izquierda, array, cantidad_elementos, tamanio_array);
	cantidad_elementos = arbol_insertar_vector_preorden(nodo->derecha, array, cantidad_elementos, tamanio_array);

	return cantidad_elementos;
}

size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array) {
	if (arbol_vacio(arbol)) {
		return 0;
	}

	nodo_abb_t* nodo = arbol->nodo_raiz;
	size_t cantidad_elementos = 0;

	cantidad_elementos = arbol_insertar_vector_preorden(nodo, array, cantidad_elementos, tamanio_array);

	return cantidad_elementos;
}

/* Recorre en secuencia postorden el árbol e inserta cada elemento en el vector. */
size_t arbol_insertar_vector_postorden(nodo_abb_t* nodo, void** array, size_t cantidad_elementos, size_t tamanio_array) {
	if ((!nodo) || (cantidad_elementos == tamanio_array)) {
		return cantidad_elementos;
	}

	cantidad_elementos = arbol_insertar_vector_postorden(nodo->izquierda, array, cantidad_elementos, tamanio_array);
	cantidad_elementos = arbol_insertar_vector_postorden(nodo->derecha, array, cantidad_elementos, tamanio_array);

	if (cantidad_elementos != tamanio_array) {
		array[cantidad_elementos] = nodo->elemento;
		cantidad_elementos += 1;
	}

	return cantidad_elementos;
}

size_t arbol_recorrido_postorden(abb_t* arbol, void** array, size_t tamanio_array) {
	if (arbol_vacio(arbol)) {
		return 0;
	}

	nodo_abb_t* nodo = arbol->nodo_raiz;
	size_t cantidad_elementos = 0;

	cantidad_elementos = arbol_insertar_vector_postorden(nodo, array, cantidad_elementos, tamanio_array);

	return cantidad_elementos;
}

void liberar_nodo(abb_t* arbol, nodo_abb_t* nodo) {
	if (!nodo) {
		return;
	}

	liberar_nodo(arbol, nodo->izquierda);
	liberar_nodo(arbol, nodo->derecha);

	if (arbol->destructor) {
		arbol->destructor(nodo->elemento);
	}

	free(nodo);
}

void arbol_destruir(abb_t* arbol) {
	if (!arbol) {
		return;
	}

	nodo_abb_t* nodo_a_eliminar = arbol->nodo_raiz;
	liberar_nodo(arbol, nodo_a_eliminar);

	free(arbol);
}

/* Itera en secuencia inorden el árbol. */
size_t arbol_iterador_inorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra, size_t elementos_iterados, bool* seguir_iterando) {
	if ((!nodo) || (*seguir_iterando == true)) {
		return elementos_iterados;
	}

	elementos_iterados = arbol_iterador_inorden(nodo->izquierda, funcion, extra, elementos_iterados, seguir_iterando);
	
	if (*seguir_iterando != true) {
		*seguir_iterando = funcion(nodo->elemento, extra);
		elementos_iterados += 1;
	}

	elementos_iterados = arbol_iterador_inorden(nodo->derecha, funcion, extra, elementos_iterados, seguir_iterando);

	return elementos_iterados;
}

/* Itera en secuencia preorden el árbol. */
size_t arbol_iterador_preorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra, size_t elementos_iterados, bool* seguir_iterando) {
	if ((!nodo) || (*seguir_iterando == true)) {
		return elementos_iterados;
	}

	if (*seguir_iterando != true) {
		*seguir_iterando = funcion(nodo->elemento, extra);
		elementos_iterados += 1;
	}

	elementos_iterados = arbol_iterador_preorden(nodo->izquierda, funcion, extra, elementos_iterados, seguir_iterando);
	elementos_iterados = arbol_iterador_preorden(nodo->derecha, funcion, extra, elementos_iterados, seguir_iterando);

	return elementos_iterados;
}

/* Itera en secuencia postorden el árbol. */
size_t arbol_iterador_postorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra, size_t elementos_iterados, bool* seguir_iterando) {
	if ((!nodo) || (*seguir_iterando == true)) {
		return elementos_iterados;
	}

	elementos_iterados = arbol_iterador_postorden(nodo->izquierda, funcion, extra, elementos_iterados, seguir_iterando);
	elementos_iterados = arbol_iterador_postorden(nodo->derecha, funcion, extra, elementos_iterados, seguir_iterando);

	if (*seguir_iterando != true) {
		*seguir_iterando = funcion(nodo->elemento, extra);
		elementos_iterados += 1;
	}

	return elementos_iterados;
}

size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra) {
	if (arbol_vacio(arbol)) {
		return 0;
	}

	if (funcion == NULL) {
		return 0;
	}

	nodo_abb_t* nodo_actual = arbol->nodo_raiz;

	bool seguir_iterando = false;
	size_t elementos_iterados = 0;

	if (recorrido == ABB_RECORRER_INORDEN) {
		elementos_iterados = arbol_iterador_inorden(nodo_actual, funcion, extra, elementos_iterados, &seguir_iterando);
	} else if (recorrido == ABB_RECORRER_PREORDEN) {
		elementos_iterados = arbol_iterador_preorden(nodo_actual, funcion, extra, elementos_iterados, &seguir_iterando);
	} else if (recorrido == ABB_RECORRER_POSTORDEN) {
		elementos_iterados = arbol_iterador_postorden(nodo_actual, funcion, extra, elementos_iterados, &seguir_iterando);
	} else {
		return 0;
	}

	return elementos_iterados;
}