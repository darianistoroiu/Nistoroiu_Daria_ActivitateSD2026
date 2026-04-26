//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
////trebuie sa folositi fisierul masini.txt
////sau va creati un alt fisier cu alte date
//
//struct StructuraMasina {
//	int id;
//	int nrUsi;
//	float pret;
//	char* model;
//	char* numeSofer;
//	unsigned char serie;
//};
//typedef struct StructuraMasina Masina;
//
////creare structura pentru un nod dintr-un arbore binar de cautare
//
//struct Nod {
//	Masina info;
//	struct Nod* st;
//	struct Nod* dr;
//};
//
//typedef struct Nod Nod;
//
//
//void afisareMasina(Masina masina) {
//	printf("Id: %d\n", masina.id);
//	printf("Nr. usi : %d\n", masina.nrUsi);
//	printf("Pret: %.2f\n", masina.pret);
//	printf("Model: %s\n", masina.model);
//	printf("Nume sofer: %s\n", masina.numeSofer);
//	printf("Serie: %c\n\n", masina.serie);
//}
//
//Masina initMasina(int id, int nrUsi, float pret, const char* model, const char* numeSofer, unsigned char serie) {
//	Masina m;
//	m.id = id;
//	m.nrUsi = nrUsi;
//	m.pret = pret;
//	m.serie = serie;
//	m.model =(char*) malloc(sizeof(char) * (strlen(model) + 1));
//	strcpy(m.model, model);
//	m.numeSofer = (char*)malloc(sizeof(char) * (strlen(numeSofer) + 1));
//	strcpy(m.numeSofer, numeSofer);
//
//	return m;
//}
//
//int calculDiferentaInaltimi(Nod* rad) {
//	if (rad != NULL) {
//		return calculeazaInaltimeArbore(rad->st) - calculeazaInaltimeArbore(rad->dr);
//	}
//	else {
//		return 0;
//	}
//}
//
//void rotireLaStanga(Nod** rad) {
//	Nod* aux = (*rad)->dr;
//	//incepem sa setam legaturile de jos in sus
//	(*rad)->dr = aux->st;
//	aux->st = (*rad);
//	(*rad) = aux;
//}
//void rotireLaDreapta(Nod** rad) { //in oglinda fata de stanga
//	Nod* aux = (*rad)->st;
//	//incepem sa setam legaturile de jos in sus
//	(*rad)->st = aux->dr;
//	aux->dr = (*rad);
//	(*rad) = aux;
//}
//
//void adaugaMasinaInArbore(Nod** rad, Masina masinaNoua) { //dublu pointer pt ca se poate modifica radacina
//	//adauga o noua masina pe care o primim ca parametru in arbore,
//	//astfel incat sa respecte principiile de arbore binar de cautare
//	//dupa o anumita cheie pe care o decideti - poate fi ID
//
//	//divide et impera=> recursivitate
//
//	if (*rad == NULL) {
//		Nod* nod = malloc(sizeof(Nod));
//		nod->info = masinaNoua;
//		nod->dr = NULL;
//		nod->st = NULL;
//		*rad = nod;
//	}
//	else {
//		if ((*rad)->info.id > masinaNoua.id) {
//			adaugaMasinaInArbore(&((*rad)->st), masinaNoua);
//		}
//		if ((*rad)->info.id < masinaNoua.id) {
//			adaugaMasinaInArbore(&((*rad)->dr), masinaNoua);
//		}
//	}
//
//	int diferentaInaltimi = calculDiferentaInaltimi(*rad);
//	if (diferentaInaltimi == 2) {
//		//dezechilibru pe partea stanga
//		if (calculDiferentaInaltimi((*rad)->st) == -1) {
//			rotireLaStanga(&(*rad)->st);
//		}
//		rotireLaDreapta(rad);
//	}
//	if (diferentaInaltimi == -2) {
//		//dezechilibru pe partea dreapta
//		if (calculDiferentaInaltimi((*rad)->dr) == 1) {   //optimizarea procesului
//			rotireLaDreapta(&(*rad)->dr);
//		}
//		rotireLaStanga(rad);
//	}
//
//}
//
//void afisareMasiniDinArbore(Nod* rad) { //tot recursiv SRD
//	//afiseaza toate elemente de tip masina din arborele creat
//	//prin apelarea functiei afisareMasina()
//	//parcurgerea arborelui poate fi realizata in TREI moduri
//	//folositi toate cele TREI moduri de parcurgere
//	if (rad) {
//		afisareMasiniDinArbore(rad->st);
//		afisareMasina(rad->info);
//		afisareMasiniDinArbore(rad->dr);
//	}
//
//}
//
//void afisarePreordine(Nod* rad) {
//	if (rad) {
//		afisareMasina(rad->info);
//		afisarePreordine(rad->st);
//		afisarePreordine(rad->dr);
//	}
//}
//
//
//void dezalocareArboreDeMasini(Nod** rad) {
//	//sunt dezalocate toate masinile si arborele de elemente
//	if (rad) { //toate functiile care sunt pt arbori trebuie sa aibe conditia de oprire-daca exista arbore
//		dezalocareArboreDeMasini(&(*rad)->st);
//		dezalocareArboreDeMasini(&(*rad)->dr);
//		free((*rad)->info.model);
//		free((*rad)->info.numeSofer);
//		free(*rad);
//		*rad = NULL;
//	}
//}
//
//Masina getMasinaByID(Nod* rad, int id) {
//	Masina m;
//	m.id = -1;
//	if (rad) {
//		if (rad->info.id == id) {
//			m = rad->info;
//			m.model = malloc(sizeof(char) * (strlen(rad->info.model) + 1));
//			strcpy(m.model, rad->info.model);
//			m.numeSofer = malloc(sizeof(char) * (strlen(rad->info.numeSofer) + 1));
//			strcpy(m.numeSofer, rad->info.numeSofer);
//		}
//		if (id < rad->info.id) {
//			m = getMasinaByID(rad->st, id);
//		}
//		if (id > rad->info.id) {
//			m = getMasinaByID(rad->dr, id);
//		}
//	}
//	return m;
//}
//
//int determinaNumarNoduri(Nod* rad) {
//	//calculeaza numarul total de noduri din arborele binar de cautare
//	if (rad) {
//		return determinaNumarNoduri(rad->st) + determinaNumarNoduri(rad->dr) + 1;
//	}
//	return 0;
//}
//int maxim(int a, int b) {
//	return (a > b ? a : b);
//}
//int calculeazaInaltimeArbore(Nod* rad) {
//	//calculeaza inaltimea arborelui care este data de 
//	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
//	if (rad) {
//		return maxim(calculeazaInaltimeArbore(rad->st), calculeazaInaltimeArbore(rad->dr)) + 1;
//	}
//	return 0;
//}
//
//float calculeazaPretTotal(Nod* rad) {
//	//calculeaza pretul tuturor masinilor din arbore.
//	if (rad) {
//		return rad->info.pret + calculeazaPretTotal(rad->st) + calculeazaPretTotal(rad->dr);
//	}
//	return 0;
//}
//
//float calculeazaPretulMasinilorUnuiSofer(Nod* rad, const char* numeSofer) {
//	//calculeaza pretul tuturor masinilor unui sofer.
//	if (rad) {
//		float suma = calculeazaPretulMasinilorUnuiSofer(rad->st, numeSofer) + calculeazaPretulMasinilorUnuiSofer(rad->dr, numeSofer);
//		if (strcmp(rad->info.numeSofer, numeSofer) == 0) {
//			suma += rad->info.pret;
//		}
//		return suma;
//	}
//	return 0;
//}
//
////arbore echilibrat:diferenta intre inaltimea din st si inaltimea din dr sa fie -1, 0, 1 => AVL
////la afisarea in ordine SRD-> se afiseaza nodurile in ordine crescatoare
//
//int main() {
//	Nod* rad = NULL;
//	adaugaMasinaInArbore(&rad, initMasina(1, 4, 5000, "Lexus", "Ion", "L"));
//	adaugaMasinaInArbore(&rad, initMasina(2, 4, 7000, "Skoda", "Costel", "S"));
//	adaugaMasinaInArbore(&rad, initMasina(3, 4, 10000, "BMW", "Ciki", "B"));
//	adaugaMasinaInArbore(&rad, initMasina(4, 4, 9000, "Audi", "Stefan", "A"));
//	adaugaMasinaInArbore(&rad, initMasina(5, 4, 8000, "Dacia", "Vasile", "D"));
//
//
//	afisarePreordine(rad);
//	printf("\nMasina cautata: \n");
//	afisareMasina(getMasinaByID(rad, 4));
//	printf("\nNumar noduri:%d \n", determinaNumarNoduri(rad));
//	printf("\nInaltime arbore:%d \n", calculeazaInaltimeArbore(rad));
//	printf("\nPret total:%.2f \n", calculeazaPretTotal(rad));
//	printf("\nPretul masinilor unui sofer este de:%.2f \n", calculeazaPretulMasinilorUnuiSofer(rad, "Ionescu"));
//	dezalocareArboreDeMasini(&rad);
//
//	return 0;
//}