#include<stdio.h>
#include<malloc.h>

typedef struct Examen Examen;
typedef struct Nod Nod;
struct Examen {
	char* curs;
	int anStudiu;
};

struct Nod {
	Examen info;
	Nod* next;
};


Examen initializare(const char* curs, int anStudiu) {
	Examen e;
	e.anStudiu = anStudiu;
	e.curs = (char*)malloc(sizeof(char) * (strlen(curs) + 1));
	strcpy_s(e.curs, strlen(curs) + 1, curs);
	return e;
}

void afisare(Examen e) {
	printf("\nExamenul la cursul %s se va da in anul %d de studiu.\n", e.curs, e.anStudiu);
}

//functii stiva 

void push(Nod** varf, Examen e) {  // inserare la inceput in stiva
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = e;
	nou->next = (*varf);
	(*varf) = nou;
}

Examen pop(Nod** varf) {  //extragere de la inceput in stiva
	if ((*varf) == NULL) {
		return initializare(NULL, 0);
	}
	Examen e;
	e = (*varf)->info;
	Nod* aux = (*varf);
	(*varf) = (*varf)->next;
	free(aux);
	return e;
}

void pushBack(Nod** varf, Examen e) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = e;
	if ((*varf) == NULL) {
		nou->next = NULL;
		(*varf) = nou;
	}
	else {
		nou->next = (*varf);
		(*varf) = nou;
	}
}


void put(Nod** coada, Examen e) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = e;
	nou->next = NULL;
	if ((*coada) == NULL) {
		(*coada) = nou;
	}
	else {
		Nod* aux = (*coada);
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
}




int main() {
	Nod* stiva = NULL;
	Examen e1 = initializare("Algebra", 1);
	afisare(e1);
	push(&stiva, e1);
	Examen e2 = initializare("Analiza", 1);
	push(&stiva, e2);
	
	Examen e;
	printf("\nAfisare examene din stiva:\n");
	while (stiva) {
		e = pop(&stiva);
		afisare(e);
		free(e.curs);
	}

	Nod* stiva2 = NULL;
	Examen e3 = initializare("Algebra", 3);
	pushBack(&stiva2, e3);
	Examen e4 = initializare("Analiza", 3);
	pushBack(&stiva2, e4);

	printf("\nAfisare examene din stiva 2:\n");
	while (stiva2) {
		e = pop(&stiva2);
		afisare(e);
		free(e.curs);
	}


	Nod* coada = NULL;
	Examen e5 = initializare("Algebra", 2);
	Examen e6 = initializare("Analiza", 2);
	put(&coada, e5);
	put(&coada, e6);
	printf("\nAfisare examene din coada:\n");
	while (coada) {
		e = pop(&coada);
		afisare(e);
		free(e.curs);
	}
	return 0;

	
}