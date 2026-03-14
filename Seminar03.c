#include<stdio.h>
#include<malloc.h>
//stiva si coada
//stocam date omogene, de acelasi tip
//stiva e o structura de date dar aici conteaza principiul de inserare si de extragere
//stiva-LIFO coada FIFO
//La stiva inserarea si extragerea se face din acelasi capat,loc:inserare la inceput si extragere de la inceput SAU inserare la final si extragere de la final
//push-inserare la inceput
//pop
//la coada inserare la sfarsit si extragere la inceput
typedef struct Test Test;
typedef struct Nod Nod;
struct Test {
	char* materie;
	int nrStudenti;
	float medie;
};
struct Nod {
	Test info;
	Nod* next;
};
Test initTest(const char* materie, int nrStudenti, float medie) {
	Test t;
	t.nrStudenti = nrStudenti;
	t.medie = medie;
	t.materie = (char*)malloc(sizeof(char)*(strlen(materie) + 1));
	strcpy_s(t.materie, strlen(materie) + 1, materie);
	return t;

}
void afisareTest(Test t) {
	printf("\n La testul de la materia %s au participat %d studenti si au obstinut media %.2f.\n ", 
		t.materie, t.nrStudenti, t.medie);
}
void push(Nod** varf, Test t) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->next = (*varf);
	nou->info = t; //shallow copy
	(*varf) = nou;
}

Test pop(Nod** varf) {
	if ((*varf) == NULL) {
		return initTest(NULL, 0, 0);
	}
	Test t;
	t = (*varf)->info;
	Nod* aux = *varf;
	*varf = (*varf)->next;
	free(aux);
	return t;
}

void put(Nod** coada, Test t) { //identica cu inserarea la final
	Nod* nou= (Nod*)malloc(sizeof(Nod));
	nou->next = NULL;
	nou->info = t;
	if ((*coada) == NULL) {
		(*coada) = nou;
	}
	else {
		Nod* aux = *coada;
		while (aux->next!=NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
}
//nu mai facem get pt ca e acelasi cu pop-doar in acest caz
int main() {
	Nod* stiva = NULL;
	push(&stiva, initTest("SDD", 60, 2.5));
	push(&stiva, initTest("POO", 75, 6.5));
	push(&stiva, initTest("Java", 48, 5.5));

	Test t;
	printf("\nTraversare stiva:");
	while (stiva) {
		t = pop(&stiva);
		afisareTest(t);
		free(t.materie);
	}

	Nod* coada = NULL;
	put(&coada, initTest("SDD", 60, 2.5));
	put(&coada, initTest("POO", 75, 6.5));
	put(&coada, initTest("Java", 48, 5.5));

	printf("\nTraversare stiva:");
	while (coada) {
		t = pop(&coada);
		afisareTest(t);
		free(t.materie);
	}

	return 0;
}
//tema implementari pe lista dublu inlatuita
//tema sa facem celelalte inserari si extrageri