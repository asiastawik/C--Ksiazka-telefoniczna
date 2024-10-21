#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>	//standardowe wejœcie/wyjœcie
#include <stdlib.h>	//standardowe funkcje, rand(), malloc(), realloc(), itd.
#include <string.h> //do napisów char
#include <conio.h>	//do funkcji _getch
#include <time.h>	//do funkcji obs³ugi czasu
#include <locale.h>	//do funkcji setlocale()

//"1. Strukturê na kontakt - imiê, nazwisko, numer, grupa (grupa to np. rodzina, praca, znajomi…);"
//struktura, w tym przypadku jako zmienna globalna(poza main)
//jest to tylko deklaracja struktury, nie zmiennej strukturalnej
struct kontakt
{
	char imie[50];
	char nazwisko[50];
	char numer[50];	//numer móg³by byæ w long int, ale w char jest dla mnie bardziej intuicyjnie
	char grupa[50];
};

//"2. Strukturê – element listy, zawieraj¹cy kontakt i adresy do struktur tego samego typu – elementu poprzedniego i nastêpnego;"
//równie¿ zmienna globalna
//wykonane tak, jak pokazano w instrukcji
struct element
{
	struct element* poprzedni; //wskaŸnik – adres poprzedniego elementu stosu
	struct kontakt aktualny;
	struct element* nastepny;
};

//"6. Funkcjê dodaj¹c¹ kontakt do listy[...]"
void dodaj_do_listy(struct element** aktualny, struct kontakt pomocniczy)
{
	struct element* nowy;
	nowy = malloc(sizeof(struct element)); //alokacja pamiêci

	struct element* tymczasowy;
	tymczasowy = malloc(sizeof(struct element)); //alokacja pamiêci

	if (*aktualny != NULL) //jeœli lista kontaktów nie jest pusta to...
	{
		tymczasowy = *aktualny; //przepisujemy dane do tymczasowego
		while (tymczasowy->nastepny) //dopóki istnieje jakiœ nastêpny kontakt wykonujemy pêtlê, a¿ nie przejdziemy na sam koniec listy
		{
			tymczasowy = tymczasowy->nastepny;
		}
		nowy->poprzedni = tymczasowy; //w nowym elemencie poprzednim bêdzie ten tymczasowy
		nowy->aktualny = pomocniczy;  //w aktualnym bêdzie ten który chcemy dodaæ
		nowy->nastepny = NULL; //nastêpnego nie ma, bo dojechaliœmy do koñca
		tymczasowy->nastepny = nowy; //nastêpny poprzedniego kontaktu (wpisany do tymczasowego) to jest nasz nowy kontakt
	}

	else //a jeœli jest pusta
	{
		nowy->poprzedni = NULL; //nie ma poprzedniego..
		nowy->aktualny = pomocniczy; //przepisujemy dane z pomocniczego 
		nowy->nastepny = NULL; //ani nastêpnego..
		*aktualny = nowy; //i od teraz aktualnym i jedynym elementem jest nowy element
		tymczasowy = *aktualny; //przepisujemy dane do tymczasowego
	}
}

//"3. Funkcje wczytuj¹c¹ kontakty z pliku .CSV i formuj¹c¹ listê;"
//"Chc¹c zamieniæ wskaŸniki na strukturê wewn¹trz funkcji, nale¿y przekazywaæ do funkcji ich adres, czyli wskaŸnik na wskaŸnik na strukturê."
void wczytaj_z_listy(struct element** aktualny) //nazwa argumentu nie ma znaczenia; void - bo funkcja nic nie zwraca
{
	struct kontakt pomocniczy;
	FILE* plik;						//utworzenie struktury FILE* (uchwytu, identyfikatora) pliku, do którego bêdziemy siê odwo³ywaæ
	printf("Któr¹ listê chcesz wczytaæ?\n1. Rozszerzon¹ (kontakty.csv).\n2. Podstawow¹ (lista_kontaktow.csv).\n");
	int wybor;
	do
	{
		char napis[20];
		scanf("%s", napis);
		int n = strlen(napis);
		for (int i = 0; i < n; i++)
		{
			if (napis[i] >= 49 && napis[i] <= 50) //przedzia³ [1;2]
			{
				wybor = atoi(napis);
			}
			else
			{
				wybor = -1;
				printf("Niepoprawna wartoœæ. Proszê podaæ liczbê z przedzia³u od 1 do 2.\n");
			}
		}
	} while (wybor <= 0);

	switch (wybor)
	{
	case 1:
	{
		plik = fopen("kontakty.csv", "r");	//"r" – otwiera plik do odczytu
		if (!plik)								//sprawdzamy czy plik istnieje
		{
			printf("\nB³¹d. Niestety nie uda³o siê otworzyæ tego pliku. Zapis pliku nie bêdzie mo¿liwy.\n");
		}
		else
		{
			while (feof(plik) == 0)
				/*
				"Funkcja zwraca wartoœæ niezerow¹ je¿eli wczeœniej napotkano koniec pliku tekstowego (tzn. je¿eli poprzedzaj¹ca operacja przeczyta³a znacznik koñca pliku)."
				Czyli odczytujemy, a¿ nie napotkamy koñca pliku.
				Czyli zak³adaj¹c, ¿e w pliku .CSV ci¹gi rozdzielone s¹ œrednikami, mo¿na u¿yæ funkcji fscanf
				Format "%[^;]" oznacza czytanie ci¹gu znaków a¿ do napotkania na œrednik.
				*/
			{
				fscanf(plik, "%[^;] ; %[^;] ; %[^;] ; %s\n", &pomocniczy.imie, &pomocniczy.nazwisko, &pomocniczy.numer, &pomocniczy.grupa);
				dodaj_do_listy(aktualny, pomocniczy); //teraz musimy dodaæ to wszystko do naszej listy
			}
			fclose(plik);	//zamkniêcie pliku czyœci bufory danych powi¹zane z nim, zwalnia uchwyt oraz pozwala na otwieranie pliku na zewn¹trz lub jego usuniêcie;
		}
		break;
	}

	case 2:
	{
		plik = fopen("lista_kontaktow.csv", "r");//"r" – otwiera plik do odczytu
		if (!plik)								//sprawdzamy czy plik istnieje
		{
			printf("\nB³¹d. Niestety nie uda³o siê otworzyæ tego pliku. Zapis pliku nie bêdzie mo¿liwy.\n");
		}
		else
		{
			while (feof(plik) == 0)
				/*
				"Funkcja zwraca wartoœæ niezerow¹ je¿eli wczeœniej napotkano koniec pliku tekstowego (tzn. je¿eli poprzedzaj¹ca operacja przeczyta³a znacznik koñca pliku)."
				Czyli odczytujemy, a¿ nie napotkamy koñca pliku.
				Czyli zak³adaj¹c, ¿e w pliku .CSV ci¹gi rozdzielone s¹ œrednikami, mo¿na u¿yæ funkcji fscanf
				Format "%[^;]" oznacza czytanie ci¹gu znaków a¿ do napotkania na œrednik.
				*/
			{
				fscanf(plik, "%[^;] ; %[^;] ; %[^;] ; %s\n", &pomocniczy.imie, &pomocniczy.nazwisko, &pomocniczy.numer, &pomocniczy.grupa);
				dodaj_do_listy(aktualny, pomocniczy); //teraz musimy dodaæ to wszystko do naszej listy
			}
			fclose(plik);	//zamkniêcie pliku czyœci bufory danych powi¹zane z nim, zwalnia uchwyt oraz pozwala na otwieranie pliku na zewn¹trz lub jego usuniêcie;
		}
		break;
	}

	default:
	{
		printf("Niestety takiej opcji nie ma w menu wyboru. Proszê podaæ ponownie:\n");
		break;
	}
	}
}

//4. "Funkcjê wyœwietlaj¹ca listê w konsoli;"
void wyswietl_liste(struct element** aktualny)
{
	struct element* tymczasowy;
	tymczasowy = *aktualny; //wpisujemy do tymczasowego
	int i = 1; //deklarujemy przed pêtl¹, numerowanie od jedynki
	printf("Wyœwietlam listê kontaktów:\n");
	while (tymczasowy) //jeœli tymczasowy istnieje to wykonuj
	{
		printf("%d. %s  |  %s  |  %s  |  %s\n", i, tymczasowy->aktualny.imie, tymczasowy->aktualny.nazwisko, tymczasowy->aktualny.numer, tymczasowy->aktualny.grupa);
		tymczasowy = tymczasowy->nastepny;
		i = i + 1;
	}
}

//"5. Funkcjê sortuj¹c¹ listê wed³ug imienia, nazwiska lub grupy;"
void zamiana(struct element** aktualny)
{
	//zrobione wed³ug rozpiski w instrukcji
	if ((*aktualny)->nastepny->nastepny == NULL) //gdy zamiana nastêpuje w dwóch ostatnich elementach i nie ma elementu nastêpnego po (nastêpnym aktualnego) = [nie ma elementu nastêpnego po ostatnim]
	{
		(*aktualny)->poprzedni->nastepny = (*aktualny)->nastepny;
		(*aktualny)->nastepny->poprzedni = (*aktualny)->poprzedni;
		(*aktualny)->poprzedni = (*aktualny)->nastepny;
		(*aktualny)->nastepny = NULL; //tego nie bêdzie, bo PO ZAMIANIE przedostatniego z ostatnim, ten przedostatni nie bêdzie mia³ nastêpnego
		(*aktualny)->poprzedni->nastepny = (*aktualny);
	}

	if ((*aktualny)->poprzedni == NULL) //gdy zamiana nastêpuje w elementach pierszym z drugim i nie ma poprzedniego kontaktu (przed kontaktem pierwszym-aktualnym)
	{
		(*aktualny)->nastepny->poprzedni = NULL; //tego nie ma
		(*aktualny)->poprzedni = (*aktualny)->nastepny;
		(*aktualny)->nastepny = (*aktualny)->poprzedni->nastepny;
		(*aktualny)->poprzedni->nastepny = (*aktualny);
		(*aktualny)->nastepny->poprzedni = (*aktualny);
	}

	else //wszystkie inne przypadki, czyli gdy zamiana nastêpuje gdzieœ w œrodku, wtedy jest identycznie, jak pokazano w instrukcji
	{
		(*aktualny)->poprzedni->nastepny = (*aktualny)->nastepny;
		(*aktualny)->nastepny->poprzedni = (*aktualny)->poprzedni;
		(*aktualny)->poprzedni = (*aktualny)->nastepny;
		(*aktualny)->nastepny = (*aktualny)->poprzedni->nastepny;
		(*aktualny)->poprzedni->nastepny = (*aktualny);
		(*aktualny)->nastepny->poprzedni = (*aktualny);
	}
}

void sortowanie_imie(struct element** aktualny)
{
	struct element* tymczasowy;
	tymczasowy = *aktualny;
	int b;
	do
	{
		b = 0;
		while (tymczasowy->nastepny) //kiedy w ogóle istnieje jakiœ nastêpny
		{
			int a = strcoll(tymczasowy->aktualny.imie, tymczasowy->nastepny->aktualny.imie); //"Funkcja porównuje dwa ³añcuchy znaków str1 i str2, oraz zwraca liczbê bêd¹c¹ opisem zale¿noœci miêdzy nimi."

			if (a == 1) //jeœli nasz aktualny kontakt jest "mniejszy" od dolnego (nastêpnego), czyli np. aktualny zaczyna siê na b, a nastêpny na a to musimy je zamieniæ
			{
				zamiana(&tymczasowy);
				b = b + 1; //ni¿ej to bêdzie przydatne, bo to znaczy ¿e jak a != 1 to nie ma zamiany, b siê nie zmieni, jest zerem i nie potrzebna jest dalsza zamiana
			}
			else
			{
				tymczasowy = tymczasowy->nastepny; //jeœli pierwszy z drugim siê zgadza kolejnoœci¹ to przechodzimy do nastêpnego elementu
			}
		}

		//kiedy nie ma ju¿ nastêpnych sprawdzamy to:
		while ((*aktualny)->poprzedni) //kiedy istnieje poprzedni
		{
			*aktualny = (*aktualny)->poprzedni; //i znowu sortujemy
		}
		tymczasowy = *aktualny;	//bez tego zmiany w tablicy zachodz¹ tylko raz i tablica nie jest dobrze posortowana

	} while (b != 0); //jak jest zero tzn ¿e dobra kolejnoœæ
}

void sortowanie_nazwisko(struct element** aktualny)
{
	struct element* tymczasowy;
	tymczasowy = *aktualny;
	int b;
	do
	{
		b = 0;
		while (tymczasowy->nastepny) //kiedy w ogóle istnieje jakiœ nastêpny
		{
			int a = strcoll(tymczasowy->aktualny.nazwisko, tymczasowy->nastepny->aktualny.nazwisko); //"Funkcja porównuje dwa ³añcuchy znaków str1 i str2, oraz zwraca liczbê bêd¹c¹ opisem zale¿noœci miêdzy nimi."

			if (a == 1) //jeœli nasz aktualny kontakt jest "mniejszy" od dolnego (nastêpnego), czyli np. aktualny zaczyna siê na b, a nastêpny na a to musimy je zamieniæ
			{
				zamiana(&tymczasowy);
				b = b + 1; //ni¿ej to bêdzie przydatne, bo to znaczy ¿e jak a != 1 to nie ma zamiany, b siê nie zmieni, jest zerem i nie potrzebna jest dalsza zamiana
			}
			else
			{
				tymczasowy = tymczasowy->nastepny; //jeœli pierwszy z drugim siê zgadza kolejnoœci¹ to przechodzimy do nastêpnego elementu
			}
		}

		//kiedy nie ma ju¿ nastêpnych sprawdzamy to:
		while ((*aktualny)->poprzedni) //kiedy istnieje poprzedni
		{
			*aktualny = (*aktualny)->poprzedni; //i znowu sortujemy
		}
		tymczasowy = *aktualny;	//bez tego zmiany w tablicy zachodz¹ tylko raz i tablica nie jest dobrze posortowana

	} while (b != 0); //jak jest zero tzn ¿e dobra kolejnoœæ
}

void sortowanie_grupa(struct element** aktualny)
{
	struct element* tymczasowy;
	tymczasowy = *aktualny;
	int b;
	do
	{
		b = 0;
		while (tymczasowy->nastepny) //kiedy w ogóle istnieje jakiœ nastêpny
		{
			int a = strcoll(tymczasowy->aktualny.grupa, tymczasowy->nastepny->aktualny.grupa); //"Funkcja porównuje dwa ³añcuchy znaków str1 i str2, oraz zwraca liczbê bêd¹c¹ opisem zale¿noœci miêdzy nimi."

			if (a == 1) //jeœli nasz aktualny kontakt jest "mniejszy" od dolnego (nastêpnego), czyli np. aktualny zaczyna siê na b, a nastêpny na a to musimy je zamieniæ
			{
				zamiana(&tymczasowy);
				b = b + 1; //ni¿ej to bêdzie przydatne, bo to znaczy ¿e jak a != 1 to nie ma zamiany, b siê nie zmieni, jest zerem i nie potrzebna jest dalsza zamiana
			}

			else
			{
				tymczasowy = tymczasowy->nastepny; //jeœli pierwszy z drugim siê zgadza kolejnoœci¹ to przechodzimy do nastêpnego elementu
			}
		}

		//kiedy nie ma ju¿ nastêpnych sprawdzamy to:
		while ((*aktualny)->poprzedni) //kiedy istnieje poprzedni
		{
			*aktualny = (*aktualny)->poprzedni; //i znowu sortujemy
		}
		tymczasowy = *aktualny;	//bez tego zmiany w tablicy zachodz¹ tylko raz i tablica nie jest dobrze posortowana

	} while (b != 0); //jak jest zero tzn ¿e dobra kolejnoœæ
}

int sortowanie(struct element** aktualny, int* sposob)
{
	struct element* tymczasowy;
	tymczasowy = *aktualny;
	int wybor;
	//int sposob;
	printf("Wybierz sortowanie alfabetyczne, wed³ug:\n1. Imienia.\n2. Nazwiska.\n3. Grupy.\n4. Cofnij.\n");
	do
	{
		do
		{
			char napis[20];
			scanf("%s", napis);
			int n = strlen(napis);
			for (int i = 0; i < n; i++)
			{
				if (napis[i] >= 48 && napis[i] <= 57)
				{
					wybor = atoi(napis);
				}
				else
				{
					wybor = -1;
					printf("Niepoprawna wartoœæ. Proszê podaæ liczbê z przedzia³u od 1 do 4.\n");
				}
			}
		} while (wybor <= 0);

		switch (wybor)
		{
		case 1:
		{
			printf("Wybór 1. Sortowanie wed³ug imienia.");
			sortowanie_imie(aktualny);
			sposob = 1; //póŸniej bêdziemy dodawaæ kontakt ze wzglêdu na sposób
			break;
		}

		case 2:
		{
			printf("Wybór 2. Sortowanie wed³ug nazwiska.");
			sortowanie_nazwisko(aktualny);
			sposob = 2;
			break;
		}

		case 3:
		{
			printf("Wybór 3. Sortowanie wed³ug grupy.");
			sortowanie_grupa(aktualny);
			sposob = 3;
			break;
		}

		case 4:
		{
			printf("Wybór 4. Cofnij.");
			*sposob = 0;
			break;
		}

		default:
		{
			printf("Wybrales opcji ktorej nie bylo w MENU! Wybierz raz jeszcze!\n");
			break;
		}
		}
	} while (wybor != 1 && wybor != 2 && wybor != 3 && wybor != 0);

	return sposob;
}

//"6. [...] w miejscu zale¿nym od metody sortowania (np. jeœli posortowano wed³ug nazwiska nowy kontakt równie¿ ma byæ umieszczony w odpowiednim miejscu)"
void dodaj_sortowanie(struct element** aktualny, int sposob)
{
	struct element* tymczasowy;
	tymczasowy = malloc(sizeof(struct element));
	printf("\nProszê zapisaæ imiê:");
	scanf("%s", (tymczasowy->aktualny.imie));
	printf("\nProszê zapisaæ nazwisko:");
	scanf("%s", (tymczasowy->aktualny.nazwisko));
	printf("\nProszê wpisaæ numer telefonu:");
	scanf("%s", (tymczasowy->aktualny.numer));
	printf("\nProszê zapisaæ grupê:");
	scanf("%s", (tymczasowy->aktualny.grupa));

	if (sposob == 0) //gdy 0 to nieposortowane, to wynika z funkcji sortowania
	{
		dodaj_do_listy(aktualny, tymczasowy->aktualny); //dodajemy na koñcu, bo i tak nie jest posortowane
	}

	else if (sposob == 1) //sortowanie wdg imion
	{
		dodaj_do_listy(aktualny, tymczasowy->aktualny);
		sortowanie_imie(aktualny);
	}

	else if (sposob == 2) //wdg nazwisk
	{
		dodaj_do_listy(aktualny, tymczasowy->aktualny);
		sortowanie_nazwisko(aktualny);
	}

	else if (sposob == 3) //wdg grup
	{
		dodaj_do_listy(aktualny, tymczasowy->aktualny);
		sortowanie_grupa(aktualny);
	}
}

/*"7. Funkcjê wyszukuj¹c¹ kontakty wed³ug imienia i nazwiska na zasadzie porównywania ci¹gów
znaków(np.wyszukuj¹c ci¹giem ma znajduje kontakt zawieraj¹cy Marcin, ale tak¿e Tomasz albo Macierewicz);
8. Na powy¿szej zasadzie – funkcjê wyszukuj¹c¹ kontakty nale¿¹ce do jednej grupy;"*/
void znajdz(struct element** aktualny, char* tab)
{
	for (int i = 0; tab[i] != '\0'; i = i + 1)	//'\0' - nic nie wpisaliœmy
	{
		if (tab[i] >= 65 && tab[i] <= 95) //jeœli s¹ du¿e litery
		{
			tab[i] = tab[i] + 32; //to zamieñ na ma³e litery
		}
		else	//jeœli s¹ ma³e
		{
			tab[i] = tab[i]; //nic nie rób
		}
	}

	/*Funkcja strcpy kopiuje tekst z tablicy do tablicy. Funkcja kopiuje znak po znaku od pocz¹tku,
	a¿ do koñca tablicy lub znaku '\0', który te¿ kopiuje.*/
	char imie[50];
	char nazwisko[50];
	strcpy(imie, (*aktualny)->aktualny.imie);
	strcpy(nazwisko, (*aktualny)->aktualny.nazwisko); //to nie ten sam element "aktualny", po najechaniu strza³k¹ widaæ co jest argumentem funkcji, a co struktur¹ globaln¹

	for (int i = 0; imie[i] != '\0'; i = i + 1)	//'\0' - nic nie wpisaliœmy
	{
		if (imie[i] >= 65 && imie[i] <= 95) //jeœli s¹ du¿e litery
		{
			imie[i] = imie[i] + 32; //to zamieñ na ma³e litery
		}
		else	//jeœli s¹ ma³e
		{
			imie[i] = imie[i]; //nic nie rób
		}
	}

	for (int i = 0; nazwisko[i] != '\0'; i = i + 1)	//'\0' - nic nie wpisaliœmy
	{
		if (nazwisko[i] >= 65 && nazwisko[i] <= 95) //jeœli s¹ du¿e litery
		{
			nazwisko[i] = nazwisko[i] + 32; //to zamieñ na ma³e litery
		}
		else	//jeœli s¹ ma³e
		{
			nazwisko[i] = nazwisko[i]; //nic nie rób
		}
	}

	/*Funkcja, której mo¿na u¿yæ, by okreœliæ czy jeden ci¹g zawiera siê w drugim, to
	strstr( char * str1, char * str2 ); gdzie drugi ci¹g znaków jest wyszukiwany w pierwszym.
	Funkcja zwraca nastêpuj¹ce wartoœci:
	- Jeœli fragment zostanie znaleziony - tablicê znaków ( char * )
	- Jeœli fragment nie zostanie znaleziony – NULL;*/
	if (strstr(imie, tab) || strstr(nazwisko, tab))  // || - lub
	{
		//printf("Znaleziono kontakt:\n");
		printf("%s  ;  %s  ;  %s  ;  %s\n", (*aktualny)->aktualny.imie, (*aktualny)->aktualny.nazwisko, (*aktualny)->aktualny.numer, (*aktualny)->aktualny.grupa);
	}
}

void znajdz_grupe(struct element** aktualny, char* tab)
{
	for (int i = 0; tab[i] != '\0'; i = i + 1)	//'\0' - nic nie wpisaliœmy
	{
		if (tab[i] >= 65 && tab[i] <= 95) //jeœli s¹ du¿e litery
		{
			tab[i] = tab[i] + 32; //to zamieñ na ma³e litery
		}
		else	//jeœli s¹ ma³e
		{
			tab[i] = tab[i]; //nic nie rób
		}
	}

	/*Funkcja strcpy kopiuje tekst z tablicy do tablicy. Funkcja kopiuje znak po znaku od pocz¹tku,
	a¿ do koñca tablicy lub znaku '\0', który te¿ kopiuje.*/
	char grupa[50];
	strcpy(grupa, (*aktualny)->aktualny.grupa); //to nie ten sam element "aktualny", po najechaniu strza³k¹ widaæ co jest argumentem funkcji, a co struktur¹ globaln¹

	for (int i = 0; grupa[i] != '\0'; i = i + 1)	//'\0' - nic nie wpisaliœmy
	{
		if (grupa[i] >= 65 && grupa[i] <= 95) //jeœli s¹ du¿e litery
		{
			grupa[i] = grupa[i] + 32; //to zamieñ na ma³e litery
		}
		else	//jeœli s¹ ma³e
		{
			grupa[i] = grupa[i]; //nic nie rób
		}
	}

	/*Funkcja, której mo¿na u¿yæ, by okreœliæ czy jeden ci¹g zawiera siê w drugim, to
	strstr( char * str1, char * str2 ); gdzie drugi ci¹g znaków jest wyszukiwany w pierwszym.
	Funkcja zwraca nastêpuj¹ce wartoœci:
	- Jeœli fragment zostanie znaleziony - tablicê znaków ( char * )
	- Jeœli fragment nie zostanie znaleziony – NULL;*/
	if (strstr(grupa, tab))
	{
		//printf("Znaleziono grupê:\n");
		printf("%s  ;  %s  ;  %s  ;  %s\n", (*aktualny)->aktualny.imie, (*aktualny)->aktualny.nazwisko, (*aktualny)->aktualny.numer, (*aktualny)->aktualny.grupa);
	}
}

int zliczanie(struct element** aktualny)
{
	struct element* tymczasowy;
	tymczasowy = *aktualny; //wpisujemy do tymczasowego
	int i = 1; //deklarujemy przed pêtl¹
	while (tymczasowy) //jeœli tymczasowy istnieje to wykonuj
	{
		tymczasowy = tymczasowy->nastepny;
		i = i + 1;
	}
	return i;
}

//"9. Funkcje usuwaj¹c¹ wybrany kontakt z listy;"
void usun_z_listy(struct element** aktualny, int ilosc)
{
	struct element* tymczasowy;
	int wybor;

	do
	{
		wyswietl_liste(aktualny);
		ilosc = zliczanie(aktualny);

		printf("Proszê wybraæ kontakt do usuniêcia. Wybór 0 oznacza cofniêcie.\n");
		do
		{
			char napis[20];
			scanf("%s", napis);
			int n = strlen(napis);
			for (int i = 0; i < n; i++)
			{
				if (napis[i] >= 49 && napis[i] <= 57)
				{
					wybor = atoi(napis);
				}
				else
				{
					wybor = -1;
					printf("Niepoprawna wartoœæ. Proszê podaæ ponownie.\n");
				}
			}
		} while (wybor <= 0 || wybor >= ilosc);

		for (int i = 1; i < wybor; i = i + 1)
		{
			*aktualny = (*aktualny)->nastepny; //przechodzimy do wybranego kontaktu
		}
		tymczasowy = (*aktualny)->nastepny; //przepisujemy dane kolejnego do tymczasowego

		if (wybor == 1) //usuwamy pierwszy element
		{
			tymczasowy = (*aktualny)->nastepny; //przepisujemy dane kolejnego do tymczasowego
			(*aktualny)->nastepny->poprzedni = NULL; //usuwamy ten co chcemy
			*aktualny = tymczasowy; //teraz pierwszym jest ten co by³ wczeœniej nastêpnym
		}

		else if (wybor == ilosc) //usuwamy ostatni, analogicznie do usuniêcia pierwszego
		{
			tymczasowy = (*aktualny)->poprzedni; //przedostatni przepisujemy do tymczasowego
			(*aktualny)->poprzedni->nastepny = NULL; //usuwamy wybrany
			*aktualny = tymczasowy; //teraz ostatnim jest ten przedostatni
		}

		else if (wybor > 1 && wybor < ilosc) //ca³a reszta oprócz pierwszego i ostatniego
		{
			(*aktualny)->poprzedni->nastepny = (*aktualny)->nastepny; //ta czêœæ jest wziêta z instrukcji, strona 5 u góry
			(*aktualny)->nastepny->poprzedni = (*aktualny)->poprzedni;
			(*aktualny) = tymczasowy;	//ten tymczasowy z linii 560
		}

		else //cofnij
		{
			wybor = 0;
			printf("Cofnij.\n");
		}

		while ((*aktualny)->poprzedni)
		{
			(*aktualny) = (*aktualny)->poprzedni; //powrót
		}

		//inny sposób, ale chyba nie jest do koñca poprawny, bynajmniej s¹ b³êdy w programie Visual
		/*if (wybor == 1) //pierwszy element
		{
			struct element* tymczasowy = NULL; //tymczasowa wartoœæ
			tymczasowy = aktualny; //za tymczasowego dajemy aktualny
			aktualny = tymczasowy->poprzedni;	//za aktualnego poprzedni czyli wczeœniej nastêpny
			free(tymczasowy);	//zwalniamy ten aktualny (tymczasowy)
		}

		else if (wybor == ilosc) //ostatni - dolny
		{
			struct element* tymczasowy = NULL; //tymczasowa wartoœæ
			tymczasowy = aktualny; //za tymczasowego dajemy aktualny
			while (tymczasowy->poprzedni != NULL) //dopóki poprzedni element nie bêdzie null
			{
				tymczasowy = tymczasowy->poprzedni;	//to wykonujemy zamianê
			}
			tymczasowy->nastepny->poprzedni = NULL;
			free(tymczasowy);	//zwalniamy pamiêæ tymczasowego, ¿eby nie by³o wycieków
		}

		else if (wybor > 1 && wybor < ilosc)
		{
			struct element* tymczasowy = NULL; //tymczasowa wartoœæ
			tymczasowy = aktualny; //za tymczasowego dajemy aktualny
			for (int i = 1; i < wybor; i = i + 1) //w sumie tak jak wy¿ej, ale nie jedziemy do samego koñca, tylko do wyboru
			{
				tymczasowy = tymczasowy->poprzedni;
			}
			tymczasowy->poprzedni->nastepny = tymczasowy->nastepny;
			tymczasowy->nastepny->poprzedni = tymczasowy->poprzedni;
			free(tymczasowy);	//zwalniamy pamiêæ tymczasowego, ¿eby nie by³o wycieków
		}*/

	} while (wybor == 0);
}

int main()
{
	struct element* aktualny = NULL; /*deklaracja zmiennej strukturalnej, jest to tablica, w której bêdziemy przechowywaæ struktury
										wskaŸnik, aby mo¿na by³o ³atwo edytowaæ ewentualne zmiany
										to jest w³aœnie nasza dynamiczna baza do przechowywania w pamiêci i obs³ugi obrazów
										NULL, bo na poczatku po odpaleniu programu nie ma zadnego obrazu na liscie, jest ona pusta*/
										//int c, ilosc, i = 0, j = 0;
										//char znak;
										//char klawiatura[20] = { 0 };
	setlocale(LC_ALL, "polish_poland");	//do polskich znaków
	wczytaj_z_listy(&aktualny); //tu wybieramy któr¹ listê chcemy wgraæ
	int w; //wybor z menu
	char tab[50] = { 0 }; //do wyszukiwania, inicjowanie tablicy wymaga listy inicjalizatora w nawiasach klamrowych, czyli nie mo¿e byæ NULL
	int* sposob = 0; //wskaŸnik, ¿eby móc póŸniej siê odwo³ywaæ do jakiejœ zmiennej
	int ilosc = zliczanie(&aktualny);
	printf("				\nMENU WYBORU\nProszê wybraæ jedn¹ z poni¿szych opcji i zatwierdziæ klawiszem enter:\n");
	do
	{
		while (aktualny->poprzedni) //wracamy do pocz¹tku za ka¿dym razem, aby wyœwietlanie kontaków ³adnie dzia³a³o
		{
			aktualny = aktualny->poprzedni;
		}

		printf("\n1. Wyœwietl listê kontaktów.\n2. Dodaj kontakt.\n3. Usuñ kontakt.\n4. Sortuj kontakty.\n5. Wyszukaj kontakt.\n6. Wyszukaj grupê.\n7. Zakoñcz program.\n\n");
		do
		{
			char napis[20];
			scanf("%s", napis);
			int n = strlen(napis);
			for (int i = 0; i < n; i++)
			{
				if (napis[i] >= 48 && napis[i] <= 57)
				{
					w = atoi(napis);
				}
				else
				{
					w = -1;
					printf("Niepoprawna wartoœæ. Proszê podaæ liczbê z przedzia³u od 1 do 6.\n");
				}
			}
		} while (w <= 0);

		switch (w)
		{
		case 1:
		{
			printf("Wybór 1. Wyœwietlanie kontaktów:\n");
			wyswietl_liste(&aktualny);

			/*notatka: argument musi byc zawsze tego samego poziomu, a skoro w main jest *akutalny, a w innych funkcjach **aktualny
			to w wywo³aniu w main musi byc &aktualny, ¿eby by³ ten sam poziom; ale je¿eli w funkcji jest **aktulany
			to w wywo³aniu funkcji musi byc np dodaj_do_listy(aktualny)*/

			break;
		}
		case 2:
		{
			printf("Wybór 2. Dodanie nowego kontaktu.\n");
			dodaj_sortowanie(&aktualny, sposob);
			ilosc = zliczanie(&aktualny);	//iloœæ siê powiêksza
			wyswietl_liste(&aktualny);
			break;
		}
		case 3:
		{
			printf("Wybór 3. Usuniêcie kontaktu.\n");
			ilosc = zliczanie(&aktualny);
			usun_z_listy(&aktualny, ilosc);
			ilosc = zliczanie(&aktualny);	//iloœæ siê zmniejsza
			wyswietl_liste(&aktualny);
			break;
		}
		case 4:
		{
			printf("Wybór 4. Sortowanie kontaktów w sposób alfabetyczny.\n");
			sposob = sortowanie(&aktualny, sposob);
			wyswietl_liste(&aktualny);
			break;
		}
		case 5:
		{
			printf("Wybór 5. Wyszukiwanie kontaktu. Aby zakoñczyæ kliknij Enter.\n");
			printf("Wyszukiwarka:\n");
			int z = 0; //do zliczania
			char litera;
			do
			{
				while (aktualny->poprzedni)
				{
					aktualny = aktualny->poprzedni;
				}

				litera = _getch(); //Dla „dynamicznego” wyszukiwania mo¿na u¿yæ funkcji getch() lub _getch(), która co prawda pobiera
				//z klawiatury po jednym znaku, ale dzia³a natychmiastowo bez potrzeby potwierdzania „enterem”

				if (litera >= 97 && litera <= 122) //ma³e litery
				{
					tab[z] = litera;
					z = z + 1;
				}
				else if (litera >= 65 && litera <= 95) //du¿e litery
				{
					tab[z] = litera;
					z = z + 1;
				}
				else
				{
					printf("Wprowadzono niepoprawny znak.\n");
				}

				printf("%s\n\n", tab);

				do
				{
					znajdz(&aktualny, tab);
					aktualny = aktualny->nastepny;
				} while (aktualny->nastepny); //a¿ dojdziemy do ostatniego kontaktu

			} while (litera != 13); //enter, wtedy koniec

			/*po zakoñczeniu wykonywania siê funkcji wyszukiwania muszê wyczyœcic tablicê tab[] w main,
			bo przy nastepnym wyszukiwaniu to, co by³o wyszukane za poprzednim razem niestety zostaje*/
			for (int i = 0; i < 50; i = i + 1)
			{
				tab[i] = NULL;
			}
			break;
		}
		case 6:
		{
			printf("Wybór 6. Wyszukiwanie grupy. Aby zakoñczyæ kliknij Enter.\n");
			printf("Wyszukiwarka:\n");
			int z = 0; //do zliczania
			char litera;
			do
			{
				while (aktualny->poprzedni)
				{
					aktualny = aktualny->poprzedni;
				}

				litera = _getch(); //Dla „dynamicznego” wyszukiwania mo¿na u¿yæ funkcji getch() lub _getch(), która co prawda pobiera
				//z klawiatury po jednym znaku, ale dzia³a natychmiastowo bez potrzeby potwierdzania „enterem”

				if (litera >= 65 && litera <= 95) //du¿e litery
				{
					tab[z] = litera;
					z = z + 1;
				}
				else if (litera >= 97 && litera <= 122) //ma³e litery
				{
					tab[z] = litera;
					z = z + 1;
				}
				else
				{
					printf("Wprowadzono niepoprawny znak.\n");
				}

				printf("%s\n\n", tab);

				do
				{
					znajdz_grupe(&aktualny, tab);
					aktualny = aktualny->nastepny;
				} while (aktualny->nastepny); //a¿ dojdziemy do ostatniego kontaktu

			} while (litera != 13); //enter, wtedy koniec

			/*po zakoñczeniu wykonywania siê funkcji wyszukiwania muszê wyczyœcic tablicê tab[] w main,
			bo przy nastepnym wyszukiwaniu to, co by³o wyszukane za poprzednim razem niestety zostaje*/
			for (int i = 0; i < 50; i = i + 1)
			{
				tab[i] = NULL;
			}
			break;
		}
		case 7:
		{
			printf("Wybór 7. Zakoñczenie programu.\n");
			while (aktualny->nastepny) //dopóki istnieje jakiœ nastêpny to zwalniamy pamiêæ
			{
				aktualny = aktualny->nastepny;
				free(aktualny->poprzedni);
			}
			/*
			drugi sposób, ale chyba nie do koñca poprawny, nie jestem pewna
			while (aktualny->nastepny)
			{
				struct element* tymczasowy = NULL; //tymczasowa wartoœæ
				tymczasowy = aktualny; //za tymczasowego dajemy aktualny
				aktualny = tymczasowy->poprzedni;	//za aktualnego poprzedni czyli wczeœniej nastêpny
				free(tymczasowy);	//zwalniamy ten aktualny (tymczasowy)
				aktualny = aktualny->nastepny;
				free(aktualny->poprzedni);
			}
			*/
			printf("Program zakoñczono.\n");
			break;
		}
		default:
		{
			printf("Niestety takiej opcji nie ma w menu wyboru. Proszê podaæ ponownie:\n");
			break;
		}
		}
	} while (w != 7);

	return 0;
}