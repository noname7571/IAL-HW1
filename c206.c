/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) { // initializes the list by setting active. first and last element to null
	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) { // dispose works on the same principle as in c201
	DLLElementPtr elemPtr;
	
	while (list->firstElement != NULL) {
		elemPtr = list->firstElement;
		list->firstElement = elemPtr->nextElement;
		free(elemPtr);
	}
	
	list->activeElement = NULL;
	list->lastElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
	DLLElementPtr newElem = (DLLElementPtr)malloc(sizeof(struct DLLElement)); // allocs memory for a new element
	
	if (newElem != NULL) {
		newElem->data = data;
		newElem->nextElement = list->firstElement;
		newElem->previousElement = NULL;
		
		if(list->firstElement != NULL) {
			list->firstElement->previousElement = newElem; // if the list isnt empty, set previous element of first element to new element
		} else {
			list->lastElement = newElem; // if the list is empty, the new element is first element and the last element at the same time
		}

		list->firstElement = newElem;
	
	} else { // if malloc fails, call error function
		DLL_Error();
		return;
	}
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
	DLLElementPtr newElem = (DLLElementPtr)malloc(sizeof(struct DLLElement)); // allocs memory for a new element
	
	if (newElem != NULL) {
		newElem->data = data;
		newElem->nextElement = NULL;
		newElem->previousElement = list->lastElement;
		
		if(list->firstElement != NULL) {
			list->lastElement->nextElement = newElem; // if the list isnt empty, set next element of a last element to the new element
		} else {
			list->firstElement = newElem; // if the list is empty, the new element is last element and a first element at the same time
		}

		list->lastElement = newElem;
	
	} else { // if malloc fails, call error function
		DLL_Error();
		return;
	}
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement; // sets active element to the first element of the list
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement; // sets active element to the last element of the list
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
	if (list->firstElement != NULL) {
		*dataPtr = list->firstElement->data; // returns value of the first element in the list
	
	} else { // if the list is empty, call error function
		DLL_Error();
		return;
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
	if (list->firstElement != NULL) {
		*dataPtr = list->lastElement->data; // returns value of the last element in the list
	
	} else { // if the list is empty, call error function
		DLL_Error();
		return;
	}
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	if (list->firstElement != NULL) { // deletes the first element if the list is not empty
		DLLElementPtr elemPtr = list->firstElement; // helping element that will be freed

		if (list->firstElement == list->activeElement) {
			list->activeElement = NULL; // sets active element to null if it is the first element
		}
		
		if (list->firstElement == list->lastElement) { // if there is only one element in the list, set first and last element to null
			free(elemPtr);
			list->firstElement = NULL;
			list->lastElement = NULL;
		
		} else {
			list->firstElement = list->firstElement->nextElement; // otherwise set the first element to the second element of the list
			list->firstElement->previousElement = NULL;
			free(elemPtr);
		}
	}
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	if (list->firstElement != NULL) { // deletes the last element of the list if the list isnt empty
		DLLElementPtr elemPtr = list->lastElement; // helping element that will be freed
		
		if (list->lastElement == list->activeElement) {
			list->activeElement = NULL; // sets active element if it is the last element
		}

		if (list->firstElement == list->lastElement) { // if there is only one element in the list, set first and last element to null
			free(elemPtr);
			list->firstElement = NULL;
			list->lastElement = NULL;
		
		} else {
			list->lastElement = elemPtr->previousElement; // otherwise set the last element to the second to last element
			list->lastElement->nextElement = NULL;
			free(elemPtr);
		}
	}
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	if ((list->activeElement != NULL) || (list->activeElement != list->lastElement)) { // removes the element after active element if the list is not empty or the active element isnt the last element
		DLLElementPtr elemPtr = list->activeElement->nextElement; // helping element is set to the element after the active element that will be freed
		list->activeElement->nextElement = elemPtr->nextElement; // sets next element from the active element to the element next to helping element
		
		if (elemPtr == list->lastElement) {
			list->lastElement = list->activeElement; // if helping element is the last element, set last element to active element
		} else {
			elemPtr->nextElement->previousElement = list->activeElement; // otherwise previous element of next element to the helping one will be active element
		}

		free(elemPtr);
	}
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	if ((list->activeElement != NULL) || (list->activeElement != list->firstElement)) { // removes the element before active element if the list is not empty or the active element isnt the first element
		DLLElementPtr elemPtr = list->activeElement->previousElement; // helping element is set to the element before the active element that will be freed
		list->activeElement->previousElement = elemPtr->previousElement; // sets next element from the active element to the element before helping element

		if (elemPtr == list->firstElement) {
			list->firstElement = list->activeElement; // if helping element is the first element, set first element to active element
		} else {
			elemPtr->previousElement->nextElement = list->activeElement; // otherwise next element of previous element to the helping one will be active element
		}

		free(elemPtr);
	}
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
	if (list->activeElement != NULL) {
		DLLElementPtr newElem = (DLLElementPtr)malloc(sizeof(struct DLLElement)); // allocs memory for new element
		
		if (newElem != NULL) {
			newElem->data = data;
			newElem->nextElement = list->activeElement->nextElement;
			newElem->previousElement = list->activeElement;
			list->activeElement->nextElement = newElem;
		
			if (list->activeElement == list->lastElement) { // sets last element to new element if the active element is the last element
				list->lastElement = newElem;
			} else {
				newElem->nextElement->previousElement = newElem;
			}

		} else { // if malloc fails, call error function
			DLL_Error();
			return;
		}
	}
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
	if (list->activeElement != NULL) {
		DLLElementPtr newElem = (DLLElementPtr)malloc(sizeof(struct DLLElement)); // allocs memory for new element
		
		if (newElem != NULL) {
			newElem->data = data;
			newElem->previousElement = list->activeElement->previousElement;
			newElem->nextElement = list->activeElement;
			list->activeElement->previousElement = newElem;
		
			if (list->activeElement == list->firstElement) { // sets new element to first element if the active element is the frist element
				list->firstElement = newElem;
			} else {
				newElem->previousElement->nextElement = newElem;
			}
		
		} else { // if malloc fails, call error function
			DLL_Error();
			return;
		}
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
	if (list->activeElement != NULL) {
		*dataPtr = list->activeElement->data; // returns value of the active element
	
	} else { // if the list isnt active, call error function
		DLL_Error();
		return;
	}
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
	if (list->activeElement != NULL) {
		list->activeElement->data = data; // sets data of an active element to the value data given
	}
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if (list->activeElement != NULL) { // moves active element to the next element if the list is active
		
		if (list->activeElement != list->lastElement) {
			list->activeElement = list->activeElement->nextElement;
		
		} else { // if the active element is the last element, set active element to null
			list->activeElement = NULL;
		}
	}
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	if (list->activeElement != NULL) { // moves active element to previous element if the list is active
		
		if (list->activeElement != list->firstElement) {
			list->activeElement = list->activeElement->previousElement;
		
		} else { // if the active element is the first element, set active element to null
			list->activeElement = NULL;
		}
	}
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	return list->activeElement != NULL; // returns 0 if the list is not active
}

/* Konec c206.c */
