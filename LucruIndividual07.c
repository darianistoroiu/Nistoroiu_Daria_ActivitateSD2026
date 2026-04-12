#include<stdio.h>
#include<malloc.h>

struct Farmacie {
	char* numeFarmacie;
	int nrAngajati;
	float adaosComercial;
};
typedef struct Farmacie Farmacie;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;
struct Nod {
	Farmacie info;
	Nod* next;
	Nod* prev;
};

struct ListaDubla {
	Nod* prim;
	Nod* ultim;
};

Farmacie initializareFarmacie(const char* numeFarmacie, int nrAngajati, float adaosComercial) {
	Farmacie f;
	f.numeFarmacie = (char*)malloc(sizeof(char) * (strlen(numeFarmacie) + 1));
	strcpy_s(f.numeFarmacie, strlen(numeFarmacie) + 1, numeFarmacie);
	f.nrAngajati = nrAngajati;
	f.adaosComercial = adaosComercial;
	return f;
}

//functia de inserare intr o LDI
void inserareInLista(ListaDubla* lista, Farmacie farmacieNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = farmacieNoua;
	nou->next = nou->prev = NULL;
	if (lista->ultim != NULL) {
		nou->prev = lista->ultim;
		lista->ultim->next = nou;
		lista->ultim = nou;
	}
	else {
		lista->prim = lista->ultim = nou;
	}
}

void afisareLista(ListaDubla lista) {
	Nod* p = lista.prim;
	while(p){
		printf("\nFarmacia %s are un numar de anagajati de %d si un adaos comercial de %.2f%%.\n", p->info.numeFarmacie, p->info.nrAngajati, p->info.adaosComercial);
		p = p->next;
		
	}
}

void afisareInversaLista(ListaDubla lista) {
	Nod* p = lista.ultim;
	while (p) {
		printf("\nFarmacia %s are un numar de anagajati de %d si un adaos comercial de %.2f%%.\n", p->info.numeFarmacie, p->info.nrAngajati, p->info.adaosComercial);
		p = p->prev;
	}

}
//functia care returneaza numele farmaciei cu numarul cel mai mic de angajati dintr o LDI. daca sunt doua
//cu nr egal de angajati se alege prima farmacie din lista 


char* farmacieCautata(ListaDubla lista) {
	Nod* p = lista.prim;
	int ok = 0, minim;
	minim = lista.prim->info.nrAngajati;
	p = lista.prim->next;
	while (p) {
		if (p->info.nrAngajati < minim) {
			minim = p->info.nrAngajati;
		}
		p=p->next;
	}
	p = lista.prim;
	while (p && !ok) {
		if (p->info.nrAngajati==minim) {
			return p->info.numeFarmacie;
			ok = 1;
		}
		p = p->next;
	}
}
//cautare farmacie in lista dubla inlantuita-deep copy(daca exista doua cu acelasi nume se returneaza prima aparitie) 

Farmacie cautareFarmacieInLista(ListaDubla lista, const char* numeCautat) {
	Farmacie f;
	f.numeFarmacie = NULL;
	f.nrAngajati = 0;
	f.adaosComercial = 0;
	Nod* p = lista.prim;
	int ok = 0;
	while (p && !ok) {
		if (strcmp(p->info.numeFarmacie, numeCautat)) {
			p=p->next;
		}
		else {
			ok = 1;
		}
	}
	if (p) {
		f.numeFarmacie =(char*)malloc(sizeof(char)*(strlen(p->info.numeFarmacie)+1));
		strcpy_s(f.numeFarmacie, strlen(p->info.numeFarmacie) + 1, p->info.numeFarmacie);
		f.nrAngajati= p->info.nrAngajati;
		f.adaosComercial = p->info.adaosComercial;
		return f;
	}
	else {
		return f;
	}
}
void dezalocareLD(ListaDubla** lista) {
	Nod* p = (*lista)->prim;
	while (p) {
		free(p->info.numeFarmacie);
		Nod* aux = p;
		p = p->next;
		free(aux);
	}
	free(*lista);
	(*lista) = NULL;
}


int main() {

	Farmacie f1 = initializareFarmacie("Catena", 20, 25);
	Farmacie f2 = initializareFarmacie("DrMax", 10, 25);
	Farmacie f3 = initializareFarmacie("Ana", 10, 50);
	Farmacie f4 = initializareFarmacie("MaxiFarma", 60, 70);
	Farmacie f5 = initializareFarmacie("Tei", 30, 40);
	ListaDubla lista;
	lista.prim = lista.ultim = NULL;
	inserareInLista(&lista, f1);
	inserareInLista(&lista, f2);
	inserareInLista(&lista, f3);
	inserareInLista(&lista, f4);
	inserareInLista(&lista, f5);
	afisareLista(lista);
	printf("\n");
	afisareInversaLista(lista);
	printf("\n");
	printf("Farmacia cu cei mai putini angajati este %s.\n", farmacieCautata(lista));
	printf("\n");
	Farmacie farmacieCautataDupaNume = cautareFarmacieInLista(lista, "Ana");
	printf("Farmacia cautata se numeste %s, are %d angajati si un adaos comercial de %.2f%%.\n", farmacieCautataDupaNume.numeFarmacie, farmacieCautataDupaNume.nrAngajati, farmacieCautataDupaNume.adaosComercial);
	dezalocareLD(&lista);
}