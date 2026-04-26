#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include <string.h>
#include <stdlib.h>

struct Magazin {
	int id;
	char* denumire;
	char* localitate;
	float suprafata;
	int numar_angajati;
};
typedef struct Magazin Magazin;
typedef struct Nod Nod;
typedef struct HashTable HashTable;
struct Nod {
	Nod* next;
	Magazin info;
};
struct HashTable {
	Magazin** vector;
	int dimensiune;
};

Magazin citireMagazinDinFisier(FILE* file){
	char buffer[100];
	char sep[3] = "\n,";
	Magazin m;
	fgets(buffer, 100, file);
	char* aux;
	aux = strtok(buffer, sep);
	m.id = atoi(aux);
	aux = strtok(NULL, sep);
	m.denumire = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m.denumire, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	m.localitate= (char*)malloc(strlen(aux) + 1);
	strcpy_s(m.localitate, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	m.suprafata = atof(aux);
	aux = strtok(NULL, sep);
	m.numar_angajati = atoi(aux);
	return m;
}

void afisareMagazin(Magazin m) {
	printf("Id:%d\n", m.id);
	printf("Denumire:%s\n", m.denumire);
	printf("Localitate:%s\n", m.localitate);
	printf("Suprafata:%.2f\n", m.suprafata);
	printf("Numar angajati:%d\n", m.numar_angajati);
	printf("\n");
}

void adaugareMagazinInLista(Nod** lista, Magazin magazinNou) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = magazinNou;
	nou->next = NULL;
	if (*lista == NULL) {
		*lista = nou;
	} else if ((*lista) != NULL) {
		Nod* p=(*lista);
		while (p->next!=NULL && p->next->info.id<magazinNou.id) {
			p = p->next;
		}
		if (p->next!=NULL) {
			nou->next = p->next;
			p->next = nou;
		}
		else {
			p->next = nou;
		}
	}
}
Nod* citireListaMagazineDinFisier(const char* numeFisier) {
	Nod* lista = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			Magazin m = citireMagazinDinFisier(f);
			adaugareMagazinInLista(&lista, m);
		}
	}
	fclose(f);
	return lista;

}
void afisareListaMagazine(Nod* lista) {
	while (lista) {
		afisareMagazin(lista->info);
		lista = lista->next;
	}

}
//Scrieţi şi apelaţi funcţia pentru modificarea denumirii unui magazin specificat prin denumire şi localitate.
void modificareDenumireMagazin(Nod* lista, const char* denumireCautata, const char* localitateCautata, const char* denumireModificata) {
	Nod* p = lista;
	while (p) {
		if (strcmp(p->info.denumire, denumireCautata) == 0 && strcmp(p->info.localitate, localitateCautata) == 0) {
			free(p->info.denumire);
			p->info.denumire = (char*)malloc(strlen(denumireModificata) + 1);
			strcpy_s(p->info.denumire, strlen(denumireModificata) + 1, denumireModificata);
		}
	
		p = p->next;
	}
}

//Scrieţi şi apelaţi funcţia care determină numărul mediu de angajaţi dintr - o localitate specificată.
float numarMediuAngajati(Nod* lista, const char* localitateCautata) {
	Nod* p = lista;
	float suma = 0;
	int contor = 0;
	while (p) {
		if (strcmp(p->info.localitate, localitateCautata) == 0) {
			suma += p->info.numar_angajati;
			contor++;
		}

		p = p->next;
	}
	if (contor != 0) {
		suma /= contor;
		return suma;
	}
	else {
		return 0;
	}
	
}

//Scrieţi secvenţa de cod care copiază datele din Lista Simplă creată anterior într - o structură Tabelă de
//Dispersie, având Linear Probing ca mecanism de tratare a coliziunilor.Cheia de câutare este localitate.În caz
//de coliziune, căutarea primei poziţii disponibile în Tabela de Dispersie se efectuează cu pasul - 1 (la stânga
//punctului de coliziune).Cele două structuri de date NU partajează zone de memorie heap. 


HashTable initHashtable(int size) {
	HashTable tabela;
	tabela.dimensiune = size;
	tabela.vector = (Magazin**)malloc(sizeof(Magazin*)*size);
	for (int i = 0;i < size;i++) {
		tabela.vector[i] = NULL;
	}
	return tabela;
}

int hash(const char* localitate, int dimensiune) {
	int suma = 0;
	for (int i = 0;i < strlen(localitate);i++) {
		suma += localitate[i];
	}
	return suma % dimensiune;
}

void inserareInHashtable(HashTable tabela, Magazin m) {
	int pozitie = hash(m.localitate, tabela.dimensiune);
	while (tabela.vector[pozitie] != NULL) {
		pozitie--;
		if (pozitie < 0) {
			pozitie = tabela.dimensiune - 1;
		}
	}
	tabela.vector[pozitie] = (Magazin*)malloc(sizeof(Magazin));
	tabela.vector[pozitie]->id = m.id;
	tabela.vector[pozitie]->denumire = (char*)malloc(strlen(m.denumire)+1);
	strcpy_s(tabela.vector[pozitie]->denumire, strlen(m.denumire) + 1, m.denumire);
	tabela.vector[pozitie]->localitate = (char*)malloc(strlen(m.localitate) + 1);
	strcpy_s(tabela.vector[pozitie]->localitate, strlen(m.localitate) + 1, m.localitate);
	tabela.vector[pozitie]->suprafata = m.suprafata;
	tabela.vector[pozitie]->numar_angajati = m.numar_angajati;
}

void afisareHashtable(HashTable tabela) {
	for (int i = 0;i < tabela.dimensiune;i++) {
		printf("\nPozitia:%d\n", i);
		if (tabela.vector[i] != NULL) {
			afisareMagazin(*(tabela.vector[i]));
		}
		
	}
}
void dezalocareLista(Nod** cap) {
	while ((*cap) != NULL) {
		free((*cap)->info.denumire);
		free((*cap)->info.localitate);
		Nod* copie = *cap;
		(*cap) = (*cap)->next;
		free(copie);
	}
}
void dezalocareHashTable(HashTable tabela) {
	for (int i = 0;i < tabela.dimensiune;i++) {
		dezalocareLista(&(tabela.vector[i]));
	}
	free(tabela.vector);
}

int main() {
	Nod* lista = citireListaMagazineDinFisier("magazine.txt");
	afisareListaMagazine(lista);


	printf("Modificarea denumirii unui magazin:\n");
	modificareDenumireMagazin(lista, "Auchan", "Constanta", "Mega");
	afisareListaMagazine(lista);

	printf("Numarul mediu de angajati din localitatea data este de:%.2f\n", numarMediuAngajati(lista, "Iasi"));

	printf("\nHashtable creat cu ajutorul listei:\n");
	HashTable tabela = initHashtable(12);
	Nod* p = lista;
	while (p) {
		inserareInHashtable(tabela, p->info);
		p = p->next;
	}
	afisareHashtable(tabela);

	dezalocareHashTable(tabela);
	dezalocareLista(&lista);


	


}