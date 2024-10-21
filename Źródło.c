#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>	//standardowe wejście/wyjście
#include <stdlib.h>	//standardowe funkcje, rand(), malloc(), realloc(), itd.
#include <string.h> //do napisów char
#include <conio.h>	//do funkcji _getch
#include <time.h>	//do funkcji obsługi czasu
#include <locale.h>	//do funkcji setlocale()

//"1. Strukturę na kontakt - imię, nazwisko, numer, grupa (grupa to np. rodzina, praca, znajomi…);"
//struktura, w tym przypadku jako zmienna globalna(poza main)
//jest to tylko deklaracja struktury, nie zmiennej strukturalnej
struct kontakt
{
	char imie[50];
	char nazwisko[50];
	char numer[50];	//numer mógłby być w long int, ale w char jest dla mnie bardziej intuicyjnie
	char grupa[50];
};

//"2. Strukturę – element listy, zawierający kontakt i adresy do struktur tego samego typu – elementu poprzedniego i następnego;"
//również zmienna globalna
//wykonane tak, jak pokazano w instrukcji
struct element
{
	struct element* poprzedni; //wskaźnik – adres poprzedniego elementu stosu
	struct kontakt aktualny;
	struct element* nastepny;
};

//"6. Funkcję dodającą kontakt do listy[...]"
void dodaj_do_listy(struct element** aktualny, struct kontakt pomocniczy)
{
	struct element* nowy;
	nowy = malloc(sizeof(struct element)); //alokacja pamięci

	struct element* tymczasowy;
	tymczasowy = malloc(sizeof(struct element)); //alokacja pamięci

	if (*aktualny != NULL) //jeśli lista kontaktów nie jest pusta to...
	{
		tymczasowy = *aktualny; //przepisujemy dane do tymczasowego
		while (tymczasowy->nastepny) //dopóki istnieje jakiś następny kontakt wykonujemy pętlę, aż nie przejdziemy na sam koniec listy
		{
			tymczasowy = tymczasowy->nastepny;
		}
		nowy->poprzedni = tymczasowy; //w nowym elemencie poprzednim będzie ten tymczasowy
		nowy->aktualny = pomocniczy;  //w aktualnym będzie ten który chcemy dodać
		nowy->nastepny = NULL; //następnego nie ma, bo dojechaliśmy do końca
		tymczasowy->nastepny = nowy; //następny poprzedniego kontaktu (wpisany do tymczasowego) to jest nasz nowy kontakt
	}

	else //a jeśli jest pusta
	{
		nowy->poprzedni = NULL; //nie ma poprzedniego..
		nowy->aktualny = pomocniczy; //przepisujemy dane z pomocniczego 
		nowy->nastepny = NULL; //ani następnego..
		*aktualny = nowy; //i od teraz aktualnym i jedynym elementem jest nowy element
		tymczasowy = *aktualny; //przepisujemy dane do tymczasowego
	}
}

//"3. Funkcje wczytującą kontakty z pliku .CSV i formującą listę;"
//"Chcąc zamienić wskaźniki na strukturę wewnątrz funkcji, należy przekazywać do funkcji ich adres, czyli wskaźnik na wskaźnik na strukturę."
void wczytaj_z_listy(struct element** aktualny) //nazwa argumentu nie ma znaczenia; void - bo funkcja nic nie zwraca
{
	struct kontakt pomocniczy;
	FILE* plik;						//utworzenie struktury FILE* (uchwytu, identyfikatora) pliku, do którego będziemy się odwoływać
	printf("Którą listę chcesz wczytać?\n1. Rozszerzoną (kontakty.csv).\n2. Podstawową (lista_kontaktow.csv).\n");
	int wybor;
	do
	{
		char napis[20];
		scanf("%s", napis);
		int n = strlen(napis);
		for (int i = 0; i < n; i++)
		{
			if (napis[i] >= 49 && napis[i] <= 50) //przedział [1;2]
			{
				wybor = atoi(napis);
			}
			else
			{
				wybor = -1;
				printf("Niepoprawna wartość. Proszę podać liczbę z przedziału od 1 do 2.\n");
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
			printf("\nBłąd. Niestety nie udało się otworzyć tego pliku. Zapis pliku nie będzie możliwy.\n");
		}
		else
		{
			while (feof(plik) == 0)
				/*
				"Funkcja zwraca wartość niezerową jeżeli wcześniej napotkano koniec pliku tekstowego (tzn. jeżeli poprzedzająca operacja przeczytała znacznik końca pliku)."
				Czyli odczytujemy, aż nie napotkamy końca pliku.
				Czyli zakładając, że w pliku .CSV ciągi rozdzielone są średnikami, można użyć funkcji fscanf
				Format "%[^;]" oznacza czytanie ciągu znaków aż do napotkania na średnik.
				*/
			{
				fscanf(plik, "%[^;] ; %[^;] ; %[^;] ; %s\n", &pomocniczy.imie, &pomocniczy.nazwisko, &pomocniczy.numer, &pomocniczy.grupa);
				dodaj_do_listy(aktualny, pomocniczy); //teraz musimy dodać to wszystko do naszej listy
			}
			fclose(plik);	//zamknięcie pliku czyści bufory danych powiązane z nim, zwalnia uchwyt oraz pozwala na otwieranie pliku na zewnątrz lub jego usunięcie;
		}
		break;
	}

	case 2:
	{
		plik = fopen("lista_kontaktow.csv", "r");//"r" – otwiera plik do odczytu
		if (!plik)								//sprawdzamy czy plik istnieje
		{
			printf("\nBłąd. Niestety nie udało się otworzyć tego pliku. Zapis pliku nie będzie możliwy.\n");
		}
		else
		{
			while (feof(plik) == 0)
				/*
				"Funkcja zwraca wartość niezerową jeżeli wcześniej napotkano koniec pliku tekstowego (tzn. jeżeli poprzedzająca operacja przeczytała znacznik końca pliku)."
				Czyli odczytujemy, aż nie napotkamy końca pliku.
				Czyli zakładając, że w pliku .CSV ciągi rozdzielone są średnikami, można użyć funkcji fscanf
				Format "%[^;]" oznacza czytanie ciągu znaków aż do napotkania na średnik.
				*/
			{
				fscanf(plik, "%[^;] ; %[^;] ; %[^;] ; %s\n", &pomocniczy.imie, &pomocniczy.nazwisko, &pomocniczy.numer, &pomocniczy.grupa);
				dodaj_do_listy(aktualny, pomocniczy); //teraz musimy dodać to wszystko do naszej listy
			}
			fclose(plik);	//zamknięcie pliku czyści bufory danych powiązane z nim, zwalnia uchwyt oraz pozwala na otwieranie pliku na zewnątrz lub jego usunięcie;
		}
		break;
	}

	default:
	{
		printf("Niestety takiej opcji nie ma w menu wyboru. Proszę podać ponownie:\n");
		break;
	}
	}
}

//4. "Funkcję wyświetlająca listę w konsoli;"
void wyswietl_liste(struct element** aktualny)
{
	struct element* tymczasowy;
	tymczasowy = *aktualny; //wpisujemy do tymczasowego
	int i = 1; //deklarujemy przed pętlą, numerowanie od jedynki
	printf("Wyświetlam listę kontaktów:\n");
	while (tymczasowy) //jeśli tymczasowy istnieje to wykonuj
	{
		printf("%d. %s  |  %s  |  %s  |  %s\n", i, tymczasowy->aktualny.imie, tymczasowy->aktualny.nazwisko, tymczasowy->aktualny.numer, tymczasowy->aktualny.grupa);
		tymczasowy = tymczasowy->nastepny;
		i = i + 1;
	}
}

//"5. Funkcję sortującą listę według imienia, nazwiska lub grupy;"
void zamiana(struct element** aktualny)
{
	//zrobione według rozpiski w instrukcji
	if ((*aktualny)->nastepny->nastepny == NULL) //gdy zamiana następuje w dwóch ostatnich elementach i nie ma elementu następnego po (następnym aktualnego) = [nie ma elementu następnego po ostatnim]
	{
		(*aktualny)->poprzedni->nastepny = (*aktualny)->nastepny;
		(*aktualny)->nastepny->poprzedni = (*aktualny)->poprzedni;
		(*aktualny)->poprzedni = (*aktualny)->nastepny;
		(*aktualny)->nastepny = NULL; //tego nie będzie, bo PO ZAMIANIE przedostatniego z ostatnim, ten przedostatni nie będzie miał następnego
		(*aktualny)->poprzedni->nastepny = (*aktualny);
	}

	if ((*aktualny)->poprzedni == NULL) //gdy zamiana następuje w elementach pierszym z drugim i nie ma poprzedniego kontaktu (przed kontaktem pierwszym-aktualnym)
	{
		(*aktualny)->nastepny->poprzedni = NULL; //tego nie ma
		(*aktualny)->poprzedni = (*aktualny)->nastepny;
		(*aktualny)->nastepny = (*aktualny)->poprzedni->nastepny;
		(*aktualny)->poprzedni->nastepny = (*aktualny);
		(*aktualny)->nastepny->poprzedni = (*aktualny);
	}

	else //wszystkie inne przypadki, czyli gdy zamiana następuje gdzieś w środku, wtedy jest identycznie, jak pokazano w instrukcji
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
		while (tymczasowy->nastepny) //kiedy w ogóle istnieje jakiś następny
		{
			int a = strcoll(tymczasowy->aktualny.imie, tymczasowy->nastepny->aktualny.imie); //"Funkcja porównuje dwa łańcuchy znaków str1 i str2, oraz zwraca liczbę będącą opisem zależności między nimi."

			if (a == 1) //jeśli nasz aktualny kontakt jest "mniejszy" od dolnego (następnego), czyli np. aktualny zaczyna się na b, a następny na a to musimy je zamienić
			{
				zamiana(&tymczasowy);
				b = b + 1; //niżej to będzie przydatne, bo to znaczy że jak a != 1 to nie ma zamiany, b się nie zmieni, jest zerem i nie potrzebna jest dalsza zamiana
			}
			else
			{
				tymczasowy = tymczasowy->nastepny; //jeśli pierwszy z drugim się zgadza kolejnością to przechodzimy do następnego elementu
			}
		}

		//kiedy nie ma już następnych sprawdzamy to:
		while ((*aktualny)->poprzedni) //kiedy istnieje poprzedni
		{
			*aktualny = (*aktualny)->poprzedni; //i znowu sortujemy
		}
		tymczasowy = *aktualny;	//bez tego zmiany w tablicy zachodzą tylko raz i tablica nie jest dobrze posortowana

	} while (b != 0); //jak jest zero tzn że dobra kolejność
}

void sortowanie_nazwisko(struct element** aktualny)
{
	struct element* tymczasowy;
	tymczasowy = *aktualny;
	int b;
	do
	{
		b = 0;
		while (tymczasowy->nastepny) //kiedy w ogóle istnieje jakiś następny
		{
			int a = strcoll(tymczasowy->aktualny.nazwisko, tymczasowy->nastepny->aktualny.nazwisko); //"Funkcja porównuje dwa łańcuchy znaków str1 i str2, oraz zwraca liczbę będącą opisem zależności między nimi."

			if (a == 1) //jeśli nasz aktualny kontakt jest "mniejszy" od dolnego (następnego), czyli np. aktualny zaczyna się na b, a następny na a to musimy je zamienić
			{
				zamiana(&tymczasowy);
				b = b + 1; //niżej to będzie przydatne, bo to znaczy że jak a != 1 to nie ma zamiany, b się nie zmieni, jest zerem i nie potrzebna jest dalsza zamiana
			}
			else
			{
				tymczasowy = tymczasowy->nastepny; //jeśli pierwszy z drugim się zgadza kolejnością to przechodzimy do następnego elementu
			}
		}

		//kiedy nie ma już następnych sprawdzamy to:
		while ((*aktualny)->poprzedni) //kiedy istnieje poprzedni
		{
			*aktualny = (*aktualny)->poprzedni; //i znowu sortujemy
		}
		tymczasowy = *aktualny;	//bez tego zmiany w tablicy zachodzą tylko raz i tablica nie jest dobrze posortowana

	} while (b != 0); //jak jest zero tzn że dobra kolejność
}

void sortowanie_grupa(struct element** aktualny)
{
	struct element* tymczasowy;
	tymczasowy = *aktualny;
	int b;
	do
	{
		b = 0;
		while (tymczasowy->nastepny) //kiedy w ogóle istnieje jakiś następny
		{
			int a = strcoll(tymczasowy->aktualny.grupa, tymczasowy->nastepny->aktualny.grupa); //"Funkcja porównuje dwa łańcuchy znaków str1 i str2, oraz zwraca liczbę będącą opisem zależności między nimi."

			if (a == 1) //jeśli nasz aktualny kontakt jest "mniejszy" od dolnego (następnego), czyli np. aktualny zaczyna się na b, a następny na a to musimy je zamienić
			{
				zamiana(&tymczasowy);
				b = b + 1; //niżej to będzie przydatne, bo to znaczy że jak a != 1 to nie ma zamiany, b się nie zmieni, jest zerem i nie potrzebna jest dalsza zamiana
			}

			else
			{
				tymczasowy = tymczasowy->nastepny; //jeśli pierwszy z drugim się zgadza kolejnością to przechodzimy do następnego elementu
			}
		}

		//kiedy nie ma już następnych sprawdzamy to:
		while ((*aktualny)->poprzedni) //kiedy istnieje poprzedni
		{
			*aktualny = (*aktualny)->poprzedni; //i znowu sortujemy
		}
		tymczasowy = *aktualny;	//bez tego zmiany w tablicy zachodzą tylko raz i tablica nie jest dobrze posortowana

	} while (b != 0); //jak jest zero tzn że dobra kolejność
}

int sortowanie(struct element** aktualny, int* sposob)
{
	struct element* tymczasowy;
	tymczasowy = *aktualny;
	int wybor;
	//int sposob;
	printf("Wybierz sortowanie alfabetyczne, według:\n1. Imienia.\n2. Nazwiska.\n3. Grupy.\n4. Cofnij.\n");
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
					printf("Niepoprawna wartość. Proszę podać liczbę z przedziału od 1 do 4.\n");
				}
			}
		} while (wybor <= 0);

		switch (wybor)
		{
		case 1:
		{
			printf("Wybór 1. Sortowanie według imienia.");
			sortowanie_imie(aktualny);
			sposob = 1; //później będziemy dodawać kontakt ze względu na sposób
			break;
		}

		case 2:
		{
			printf("Wybór 2. Sortowanie według nazwiska.");
			sortowanie_nazwisko(aktualny);
			sposob = 2;
			break;
		}

		case 3:
		{
			printf("Wybór 3. Sortowanie według grupy.");
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

//"6. [...] w miejscu zależnym od metody sortowania (np. jeśli posortowano według nazwiska nowy kontakt również ma być umieszczony w odpowiednim miejscu)"
void dodaj_sortowanie(struct element** aktualny, int sposob)
{
	struct element* tymczasowy;
	tymczasowy = malloc(sizeof(struct element));
	printf("\nProszę zapisać imię:");
	scanf("%s", (tymczasowy->aktualny.imie));
	printf("\nProszę zapisać nazwisko:");
	scanf("%s", (tymczasowy->aktualny.nazwisko));
	printf("\nProszę wpisać numer telefonu:");
	scanf("%s", (tymczasowy->aktualny.numer));
	printf("\nProszę zapisać grupę:");
	scanf("%s", (tymczasowy->aktualny.grupa));

	if (sposob == 0) //gdy 0 to nieposortowane, to wynika z funkcji sortowania
	{
		dodaj_do_listy(aktualny, tymczasowy->aktualny); //dodajemy na końcu, bo i tak nie jest posortowane
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

/*"7. Funkcję wyszukującą kontakty według imienia i nazwiska na zasadzie porównywania ciągów
znaków(np.wyszukując ciągiem ma znajduje kontakt zawierający Marcin, ale także Tomasz albo Macierewicz);
8. Na powyższej zasadzie – funkcję wyszukującą kontakty należące do jednej grupy;"*/
void znajdz(struct element** aktualny, char* tab)
{
	for (int i = 0; tab[i] != '\0'; i = i + 1)	//'\0' - nic nie wpisaliśmy
	{
		if (tab[i] >= 65 && tab[i] <= 95) //jeśli są duże litery
		{
			tab[i] = tab[i] + 32; //to zamień na małe litery
		}
		else	//jeśli są małe
		{
			tab[i] = tab[i]; //nic nie rób
		}
	}

	/*Funkcja strcpy kopiuje tekst z tablicy do tablicy. Funkcja kopiuje znak po znaku od początku,
	aż do końca tablicy lub znaku '\0', który też kopiuje.*/
	char imie[50];
	char nazwisko[50];
	strcpy(imie, (*aktualny)->aktualny.imie);
	strcpy(nazwisko, (*aktualny)->aktualny.nazwisko); //to nie ten sam element "aktualny", po najechaniu strzałką widać co jest argumentem funkcji, a co strukturą globalną

	for (int i = 0; imie[i] != '\0'; i = i + 1)	//'\0' - nic nie wpisaliśmy
	{
		if (imie[i] >= 65 && imie[i] <= 95) //jeśli są duże litery
		{
			imie[i] = imie[i] + 32; //to zamień na małe litery
		}
		else	//jeśli są małe
		{
			imie[i] = imie[i]; //nic nie rób
		}
	}

	for (int i = 0; nazwisko[i] != '\0'; i = i + 1)	//'\0' - nic nie wpisaliśmy
	{
		if (nazwisko[i] >= 65 && nazwisko[i] <= 95) //jeśli są duże litery
		{
			nazwisko[i] = nazwisko[i] + 32; //to zamień na małe litery
		}
		else	//jeśli są małe
		{
			nazwisko[i] = nazwisko[i]; //nic nie rób
		}
	}

	/*Funkcja, której można użyć, by określić czy jeden ciąg zawiera się w drugim, to
	strstr( char * str1, char * str2 ); gdzie drugi ciąg znaków jest wyszukiwany w pierwszym.
	Funkcja zwraca następujące wartości:
	- Jeśli fragment zostanie znaleziony - tablicę znaków ( char * )
	- Jeśli fragment nie zostanie znaleziony – NULL;*/
	if (strstr(imie, tab) || strstr(nazwisko, tab))  // || - lub
	{
		//printf("Znaleziono kontakt:\n");
		printf("%s  ;  %s  ;  %s  ;  %s\n", (*aktualny)->aktualny.imie, (*aktualny)->aktualny.nazwisko, (*aktualny)->aktualny.numer, (*aktualny)->aktualny.grupa);
	}
}

void znajdz_grupe(struct element** aktualny, char* tab)
{
	for (int i = 0; tab[i] != '\0'; i = i + 1)	//'\0' - nic nie wpisaliśmy
	{
		if (tab[i] >= 65 && tab[i] <= 95) //jeśli są duże litery
		{
			tab[i] = tab[i] + 32; //to zamień na małe litery
		}
		else	//jeśli są małe
		{
			tab[i] = tab[i]; //nic nie rób
		}
	}

	/*Funkcja strcpy kopiuje tekst z tablicy do tablicy. Funkcja kopiuje znak po znaku od początku,
	aż do końca tablicy lub znaku '\0', który też kopiuje.*/
	char grupa[50];
	strcpy(grupa, (*aktualny)->aktualny.grupa); //to nie ten sam element "aktualny", po najechaniu strzałką widać co jest argumentem funkcji, a co strukturą globalną

	for (int i = 0; grupa[i] != '\0'; i = i + 1)	//'\0' - nic nie wpisaliśmy
	{
		if (grupa[i] >= 65 && grupa[i] <= 95) //jeśli są duże litery
		{
			grupa[i] = grupa[i] + 32; //to zamień na małe litery
		}
		else	//jeśli są małe
		{
			grupa[i] = grupa[i]; //nic nie rób
		}
	}

	/*Funkcja, której można użyć, by określić czy jeden ciąg zawiera się w drugim, to
	strstr( char * str1, char * str2 ); gdzie drugi ciąg znaków jest wyszukiwany w pierwszym.
	Funkcja zwraca następujące wartości:
	- Jeśli fragment zostanie znaleziony - tablicę znaków ( char * )
	- Jeśli fragment nie zostanie znaleziony – NULL;*/
	if (strstr(grupa, tab))
	{
		//printf("Znaleziono grupę:\n");
		printf("%s  ;  %s  ;  %s  ;  %s\n", (*aktualny)->aktualny.imie, (*aktualny)->aktualny.nazwisko, (*aktualny)->aktualny.numer, (*aktualny)->aktualny.grupa);
	}
}

int zliczanie(struct element** aktualny)
{
	struct element* tymczasowy;
	tymczasowy = *aktualny; //wpisujemy do tymczasowego
	int i = 1; //deklarujemy przed pętlą
	while (tymczasowy) //jeśli tymczasowy istnieje to wykonuj
	{
		tymczasowy = tymczasowy->nastepny;
		i = i + 1;
	}
	return i;
}

//"9. Funkcje usuwającą wybrany kontakt z listy;"
void usun_z_listy(struct element** aktualny, int ilosc)
{
	struct element* tymczasowy;
	int wybor;

	do
	{
		wyswietl_liste(aktualny);
		ilosc = zliczanie(aktualny);

		printf("Proszę wybrać kontakt do usunięcia. Wybór 0 oznacza cofnięcie.\n");
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
					printf("Niepoprawna wartość. Proszę podać ponownie.\n");
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
			*aktualny = tymczasowy; //teraz pierwszym jest ten co był wcześniej następnym
		}

		else if (wybor == ilosc) //usuwamy ostatni, analogicznie do usunięcia pierwszego
		{
			tymczasowy = (*aktualny)->poprzedni; //przedostatni przepisujemy do tymczasowego
			(*aktualny)->poprzedni->nastepny = NULL; //usuwamy wybrany
			*aktualny = tymczasowy; //teraz ostatnim jest ten przedostatni
		}

		else if (wybor > 1 && wybor < ilosc) //cała reszta oprócz pierwszego i ostatniego
		{
			(*aktualny)->poprzedni->nastepny = (*aktualny)->nastepny; //ta część jest wzięta z instrukcji, strona 5 u góry
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

		//inny sposób, ale chyba nie jest do końca poprawny, bynajmniej są błędy w programie Visual
		/*if (wybor == 1) //pierwszy element
		{
			struct element* tymczasowy = NULL; //tymczasowa wartość
			tymczasowy = aktualny; //za tymczasowego dajemy aktualny
			aktualny = tymczasowy->poprzedni;	//za aktualnego poprzedni czyli wcześniej następny
			free(tymczasowy);	//zwalniamy ten aktualny (tymczasowy)
		}

		else if (wybor == ilosc) //ostatni - dolny
		{
			struct element* tymczasowy = NULL; //tymczasowa wartość
			tymczasowy = aktualny; //za tymczasowego dajemy aktualny
			while (tymczasowy->poprzedni != NULL) //dopóki poprzedni element nie będzie null
			{
				tymczasowy = tymczasowy->poprzedni;	//to wykonujemy zamianę
			}
			tymczasowy->nastepny->poprzedni = NULL;
			free(tymczasowy);	//zwalniamy pamięć tymczasowego, żeby nie było wycieków
		}

		else if (wybor > 1 && wybor < ilosc)
		{
			struct element* tymczasowy = NULL; //tymczasowa wartość
			tymczasowy = aktualny; //za tymczasowego dajemy aktualny
			for (int i = 1; i < wybor; i = i + 1) //w sumie tak jak wyżej, ale nie jedziemy do samego końca, tylko do wyboru
			{
				tymczasowy = tymczasowy->poprzedni;
			}
			tymczasowy->poprzedni->nastepny = tymczasowy->nastepny;
			tymczasowy->nastepny->poprzedni = tymczasowy->poprzedni;
			free(tymczasowy);	//zwalniamy pamięć tymczasowego, żeby nie było wycieków
		}*/

	} while (wybor == 0);
}

int main()
{
	struct element* aktualny = NULL; /*deklaracja zmiennej strukturalnej, jest to tablica, w której będziemy przechowywać struktury
										wskaźnik, aby można było łatwo edytować ewentualne zmiany
										to jest właśnie nasza dynamiczna baza do przechowywania w pamięci i obsługi obrazów
										NULL, bo na poczatku po odpaleniu programu nie ma zadnego obrazu na liscie, jest ona pusta*/
										//int c, ilosc, i = 0, j = 0;
										//char znak;
										//char klawiatura[20] = { 0 };
	setlocale(LC_ALL, "polish_poland");	//do polskich znaków
	wczytaj_z_listy(&aktualny); //tu wybieramy którą listę chcemy wgrać
	int w; //wybor z menu
	char tab[50] = { 0 }; //do wyszukiwania, inicjowanie tablicy wymaga listy inicjalizatora w nawiasach klamrowych, czyli nie może być NULL
	int* sposob = 0; //wskaźnik, żeby móc później się odwoływać do jakiejś zmiennej
	int ilosc = zliczanie(&aktualny);
	printf("				\nMENU WYBORU\nProszę wybrać jedną z poniższych opcji i zatwierdzić klawiszem enter:\n");
	do
	{
		while (aktualny->poprzedni) //wracamy do początku za każdym razem, aby wyświetlanie kontaków ładnie działało
		{
			aktualny = aktualny->poprzedni;
		}

		printf("\n1. Wyświetl listę kontaktów.\n2. Dodaj kontakt.\n3. Usuń kontakt.\n4. Sortuj kontakty.\n5. Wyszukaj kontakt.\n6. Wyszukaj grupę.\n7. Zakończ program.\n\n");
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
					printf("Niepoprawna wartość. Proszę podać liczbę z przedziału od 1 do 6.\n");
				}
			}
		} while (w <= 0);

		switch (w)
		{
		case 1:
		{
			printf("Wybór 1. Wyświetlanie kontaktów:\n");
			wyswietl_liste(&aktualny);

			/*notatka: argument musi byc zawsze tego samego poziomu, a skoro w main jest *akutalny, a w innych funkcjach **aktualny
			to w wywołaniu w main musi byc &aktualny, żeby był ten sam poziom; ale jeżeli w funkcji jest **aktulany
			to w wywołaniu funkcji musi byc np dodaj_do_listy(aktualny)*/

			break;
		}
		case 2:
		{
			printf("Wybór 2. Dodanie nowego kontaktu.\n");
			dodaj_sortowanie(&aktualny, sposob);
			ilosc = zliczanie(&aktualny);	//ilość się powiększa
			wyswietl_liste(&aktualny);
			break;
		}
		case 3:
		{
			printf("Wybór 3. Usunięcie kontaktu.\n");
			ilosc = zliczanie(&aktualny);
			usun_z_listy(&aktualny, ilosc);
			ilosc = zliczanie(&aktualny);	//ilość się zmniejsza
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
			printf("Wybór 5. Wyszukiwanie kontaktu. Aby zakończyć kliknij Enter.\n");
			printf("Wyszukiwarka:\n");
			int z = 0; //do zliczania
			char litera;
			do
			{
				while (aktualny->poprzedni)
				{
					aktualny = aktualny->poprzedni;
				}

				litera = _getch(); //Dla „dynamicznego” wyszukiwania można użyć funkcji getch() lub _getch(), która co prawda pobiera
				//z klawiatury po jednym znaku, ale działa natychmiastowo bez potrzeby potwierdzania „enterem”

				if (litera >= 97 && litera <= 122) //małe litery
				{
					tab[z] = litera;
					z = z + 1;
				}
				else if (litera >= 65 && litera <= 95) //duże litery
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
				} while (aktualny->nastepny); //aż dojdziemy do ostatniego kontaktu

			} while (litera != 13); //enter, wtedy koniec

			/*po zakończeniu wykonywania się funkcji wyszukiwania muszę wyczyścic tablicę tab[] w main,
			bo przy nastepnym wyszukiwaniu to, co było wyszukane za poprzednim razem niestety zostaje*/
			for (int i = 0; i < 50; i = i + 1)
			{
				tab[i] = NULL;
			}
			break;
		}
		case 6:
		{
			printf("Wybór 6. Wyszukiwanie grupy. Aby zakończyć kliknij Enter.\n");
			printf("Wyszukiwarka:\n");
			int z = 0; //do zliczania
			char litera;
			do
			{
				while (aktualny->poprzedni)
				{
					aktualny = aktualny->poprzedni;
				}

				litera = _getch(); //Dla „dynamicznego” wyszukiwania można użyć funkcji getch() lub _getch(), która co prawda pobiera
				//z klawiatury po jednym znaku, ale działa natychmiastowo bez potrzeby potwierdzania „enterem”

				if (litera >= 65 && litera <= 95) //duże litery
				{
					tab[z] = litera;
					z = z + 1;
				}
				else if (litera >= 97 && litera <= 122) //małe litery
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
				} while (aktualny->nastepny); //aż dojdziemy do ostatniego kontaktu

			} while (litera != 13); //enter, wtedy koniec

			/*po zakończeniu wykonywania się funkcji wyszukiwania muszę wyczyścic tablicę tab[] w main,
			bo przy nastepnym wyszukiwaniu to, co było wyszukane za poprzednim razem niestety zostaje*/
			for (int i = 0; i < 50; i = i + 1)
			{
				tab[i] = NULL;
			}
			break;
		}
		case 7:
		{
			printf("Wybór 7. Zakończenie programu.\n");
			while (aktualny->nastepny) //dopóki istnieje jakiś następny to zwalniamy pamięć
			{
				aktualny = aktualny->nastepny;
				free(aktualny->poprzedni);
			}
			/*
			drugi sposób, ale chyba nie do końca poprawny, nie jestem pewna
			while (aktualny->nastepny)
			{
				struct element* tymczasowy = NULL; //tymczasowa wartość
				tymczasowy = aktualny; //za tymczasowego dajemy aktualny
				aktualny = tymczasowy->poprzedni;	//za aktualnego poprzedni czyli wcześniej następny
				free(tymczasowy);	//zwalniamy ten aktualny (tymczasowy)
				aktualny = aktualny->nastepny;
				free(aktualny->poprzedni);
			}
			*/
			printf("Program zakończono.\n");
			break;
		}
		default:
		{
			printf("Niestety takiej opcji nie ma w menu wyboru. Proszę podać ponownie:\n");
			break;
		}
		}
	} while (w != 7);

	return 0;
}
