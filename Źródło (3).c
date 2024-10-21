#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>	//standardowe wej�cie/wyj�cie
#include <stdlib.h>	//standardowe funkcje, rand(), malloc(), realloc(), itd.
#include <string.h> //do napis�w char
#include <conio.h>	//do funkcji _getch
#include <time.h>	//do funkcji obs�ugi czasu
#include <locale.h>	//do funkcji setlocale()

//"1. Struktur� na kontakt - imi�, nazwisko, numer, grupa (grupa to np. rodzina, praca, znajomi�);"
//struktura, w tym przypadku jako zmienna globalna(poza main)
//jest to tylko deklaracja struktury, nie zmiennej strukturalnej
struct kontakt
{
	char imie[50];
	char nazwisko[50];
	char numer[50];	//numer m�g�by by� w long int, ale w char jest dla mnie bardziej intuicyjnie
	char grupa[50];
};

//"2. Struktur� � element listy, zawieraj�cy kontakt i adresy do struktur tego samego typu � elementu poprzedniego i nast�pnego;"
//r�wnie� zmienna globalna
//wykonane tak, jak pokazano w instrukcji
struct element
{
	struct element* poprzedni; //wska�nik � adres poprzedniego elementu stosu
	struct kontakt aktualny;
	struct element* nastepny;
};

//"6. Funkcj� dodaj�c� kontakt do listy[...]"
void dodaj_do_listy(struct element** aktualny, struct kontakt pomocniczy)
{
	struct element* nowy;
	nowy = malloc(sizeof(struct element)); //alokacja pami�ci

	struct element* tymczasowy;
	tymczasowy = malloc(sizeof(struct element)); //alokacja pami�ci

	if (*aktualny != NULL) //je�li lista kontakt�w nie jest pusta to...
	{
		tymczasowy = *aktualny; //przepisujemy dane do tymczasowego
		while (tymczasowy->nastepny) //dop�ki istnieje jaki� nast�pny kontakt wykonujemy p�tl�, a� nie przejdziemy na sam koniec listy
		{
			tymczasowy = tymczasowy->nastepny;
		}
		nowy->poprzedni = tymczasowy; //w nowym elemencie poprzednim b�dzie ten tymczasowy
		nowy->aktualny = pomocniczy;  //w aktualnym b�dzie ten kt�ry chcemy doda�
		nowy->nastepny = NULL; //nast�pnego nie ma, bo dojechali�my do ko�ca
		tymczasowy->nastepny = nowy; //nast�pny poprzedniego kontaktu (wpisany do tymczasowego) to jest nasz nowy kontakt
	}

	else //a je�li jest pusta
	{
		nowy->poprzedni = NULL; //nie ma poprzedniego..
		nowy->aktualny = pomocniczy; //przepisujemy dane z pomocniczego 
		nowy->nastepny = NULL; //ani nast�pnego..
		*aktualny = nowy; //i od teraz aktualnym i jedynym elementem jest nowy element
		tymczasowy = *aktualny; //przepisujemy dane do tymczasowego
	}
}

//"3. Funkcje wczytuj�c� kontakty z pliku .CSV i formuj�c� list�;"
//"Chc�c zamieni� wska�niki na struktur� wewn�trz funkcji, nale�y przekazywa� do funkcji ich adres, czyli wska�nik na wska�nik na struktur�."
void wczytaj_z_listy(struct element** aktualny) //nazwa argumentu nie ma znaczenia; void - bo funkcja nic nie zwraca
{
	struct kontakt pomocniczy;
	FILE* plik;						//utworzenie struktury FILE* (uchwytu, identyfikatora) pliku, do kt�rego b�dziemy si� odwo�ywa�
	printf("Kt�r� list� chcesz wczyta�?\n1. Rozszerzon� (kontakty.csv).\n2. Podstawow� (lista_kontaktow.csv).\n");
	int wybor;
	do
	{
		char napis[20];
		scanf("%s", napis);
		int n = strlen(napis);
		for (int i = 0; i < n; i++)
		{
			if (napis[i] >= 49 && napis[i] <= 50) //przedzia� [1;2]
			{
				wybor = atoi(napis);
			}
			else
			{
				wybor = -1;
				printf("Niepoprawna warto��. Prosz� poda� liczb� z przedzia�u od 1 do 2.\n");
			}
		}
	} while (wybor <= 0);

	switch (wybor)
	{
	case 1:
	{
		plik = fopen("kontakty.csv", "r");	//"r" � otwiera plik do odczytu
		if (!plik)								//sprawdzamy czy plik istnieje
		{
			printf("\nB��d. Niestety nie uda�o si� otworzy� tego pliku. Zapis pliku nie b�dzie mo�liwy.\n");
		}
		else
		{
			while (feof(plik) == 0)
				/*
				"Funkcja zwraca warto�� niezerow� je�eli wcze�niej napotkano koniec pliku tekstowego (tzn. je�eli poprzedzaj�ca operacja przeczyta�a znacznik ko�ca pliku)."
				Czyli odczytujemy, a� nie napotkamy ko�ca pliku.
				Czyli zak�adaj�c, �e w pliku .CSV ci�gi rozdzielone s� �rednikami, mo�na u�y� funkcji fscanf
				Format "%[^;]" oznacza czytanie ci�gu znak�w a� do napotkania na �rednik.
				*/
			{
				fscanf(plik, "%[^;] ; %[^;] ; %[^;] ; %s\n", &pomocniczy.imie, &pomocniczy.nazwisko, &pomocniczy.numer, &pomocniczy.grupa);
				dodaj_do_listy(aktualny, pomocniczy); //teraz musimy doda� to wszystko do naszej listy
			}
			fclose(plik);	//zamkni�cie pliku czy�ci bufory danych powi�zane z nim, zwalnia uchwyt oraz pozwala na otwieranie pliku na zewn�trz lub jego usuni�cie;
		}
		break;
	}

	case 2:
	{
		plik = fopen("lista_kontaktow.csv", "r");//"r" � otwiera plik do odczytu
		if (!plik)								//sprawdzamy czy plik istnieje
		{
			printf("\nB��d. Niestety nie uda�o si� otworzy� tego pliku. Zapis pliku nie b�dzie mo�liwy.\n");
		}
		else
		{
			while (feof(plik) == 0)
				/*
				"Funkcja zwraca warto�� niezerow� je�eli wcze�niej napotkano koniec pliku tekstowego (tzn. je�eli poprzedzaj�ca operacja przeczyta�a znacznik ko�ca pliku)."
				Czyli odczytujemy, a� nie napotkamy ko�ca pliku.
				Czyli zak�adaj�c, �e w pliku .CSV ci�gi rozdzielone s� �rednikami, mo�na u�y� funkcji fscanf
				Format "%[^;]" oznacza czytanie ci�gu znak�w a� do napotkania na �rednik.
				*/
			{
				fscanf(plik, "%[^;] ; %[^;] ; %[^;] ; %s\n", &pomocniczy.imie, &pomocniczy.nazwisko, &pomocniczy.numer, &pomocniczy.grupa);
				dodaj_do_listy(aktualny, pomocniczy); //teraz musimy doda� to wszystko do naszej listy
			}
			fclose(plik);	//zamkni�cie pliku czy�ci bufory danych powi�zane z nim, zwalnia uchwyt oraz pozwala na otwieranie pliku na zewn�trz lub jego usuni�cie;
		}
		break;
	}

	default:
	{
		printf("Niestety takiej opcji nie ma w menu wyboru. Prosz� poda� ponownie:\n");
		break;
	}
	}
}

//4. "Funkcj� wy�wietlaj�ca list� w konsoli;"
void wyswietl_liste(struct element** aktualny)
{
	struct element* tymczasowy;
	tymczasowy = *aktualny; //wpisujemy do tymczasowego
	int i = 1; //deklarujemy przed p�tl�, numerowanie od jedynki
	printf("Wy�wietlam list� kontakt�w:\n");
	while (tymczasowy) //je�li tymczasowy istnieje to wykonuj
	{
		printf("%d. %s  |  %s  |  %s  |  %s\n", i, tymczasowy->aktualny.imie, tymczasowy->aktualny.nazwisko, tymczasowy->aktualny.numer, tymczasowy->aktualny.grupa);
		tymczasowy = tymczasowy->nastepny;
		i = i + 1;
	}
}

//"5. Funkcj� sortuj�c� list� wed�ug imienia, nazwiska lub grupy;"
void zamiana(struct element** aktualny)
{
	//zrobione wed�ug rozpiski w instrukcji
	if ((*aktualny)->nastepny->nastepny == NULL) //gdy zamiana nast�puje w dw�ch ostatnich elementach i nie ma elementu nast�pnego po (nast�pnym aktualnego) = [nie ma elementu nast�pnego po ostatnim]
	{
		(*aktualny)->poprzedni->nastepny = (*aktualny)->nastepny;
		(*aktualny)->nastepny->poprzedni = (*aktualny)->poprzedni;
		(*aktualny)->poprzedni = (*aktualny)->nastepny;
		(*aktualny)->nastepny = NULL; //tego nie b�dzie, bo PO ZAMIANIE przedostatniego z ostatnim, ten przedostatni nie b�dzie mia� nast�pnego
		(*aktualny)->poprzedni->nastepny = (*aktualny);
	}

	if ((*aktualny)->poprzedni == NULL) //gdy zamiana nast�puje w elementach pierszym z drugim i nie ma poprzedniego kontaktu (przed kontaktem pierwszym-aktualnym)
	{
		(*aktualny)->nastepny->poprzedni = NULL; //tego nie ma
		(*aktualny)->poprzedni = (*aktualny)->nastepny;
		(*aktualny)->nastepny = (*aktualny)->poprzedni->nastepny;
		(*aktualny)->poprzedni->nastepny = (*aktualny);
		(*aktualny)->nastepny->poprzedni = (*aktualny);
	}

	else //wszystkie inne przypadki, czyli gdy zamiana nast�puje gdzie� w �rodku, wtedy jest identycznie, jak pokazano w instrukcji
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
		while (tymczasowy->nastepny) //kiedy w og�le istnieje jaki� nast�pny
		{
			int a = strcoll(tymczasowy->aktualny.imie, tymczasowy->nastepny->aktualny.imie); //"Funkcja por�wnuje dwa �a�cuchy znak�w str1 i str2, oraz zwraca liczb� b�d�c� opisem zale�no�ci mi�dzy nimi."

			if (a == 1) //je�li nasz aktualny kontakt jest "mniejszy" od dolnego (nast�pnego), czyli np. aktualny zaczyna si� na b, a nast�pny na a to musimy je zamieni�
			{
				zamiana(&tymczasowy);
				b = b + 1; //ni�ej to b�dzie przydatne, bo to znaczy �e jak a != 1 to nie ma zamiany, b si� nie zmieni, jest zerem i nie potrzebna jest dalsza zamiana
			}
			else
			{
				tymczasowy = tymczasowy->nastepny; //je�li pierwszy z drugim si� zgadza kolejno�ci� to przechodzimy do nast�pnego elementu
			}
		}

		//kiedy nie ma ju� nast�pnych sprawdzamy to:
		while ((*aktualny)->poprzedni) //kiedy istnieje poprzedni
		{
			*aktualny = (*aktualny)->poprzedni; //i znowu sortujemy
		}
		tymczasowy = *aktualny;	//bez tego zmiany w tablicy zachodz� tylko raz i tablica nie jest dobrze posortowana

	} while (b != 0); //jak jest zero tzn �e dobra kolejno��
}

void sortowanie_nazwisko(struct element** aktualny)
{
	struct element* tymczasowy;
	tymczasowy = *aktualny;
	int b;
	do
	{
		b = 0;
		while (tymczasowy->nastepny) //kiedy w og�le istnieje jaki� nast�pny
		{
			int a = strcoll(tymczasowy->aktualny.nazwisko, tymczasowy->nastepny->aktualny.nazwisko); //"Funkcja por�wnuje dwa �a�cuchy znak�w str1 i str2, oraz zwraca liczb� b�d�c� opisem zale�no�ci mi�dzy nimi."

			if (a == 1) //je�li nasz aktualny kontakt jest "mniejszy" od dolnego (nast�pnego), czyli np. aktualny zaczyna si� na b, a nast�pny na a to musimy je zamieni�
			{
				zamiana(&tymczasowy);
				b = b + 1; //ni�ej to b�dzie przydatne, bo to znaczy �e jak a != 1 to nie ma zamiany, b si� nie zmieni, jest zerem i nie potrzebna jest dalsza zamiana
			}
			else
			{
				tymczasowy = tymczasowy->nastepny; //je�li pierwszy z drugim si� zgadza kolejno�ci� to przechodzimy do nast�pnego elementu
			}
		}

		//kiedy nie ma ju� nast�pnych sprawdzamy to:
		while ((*aktualny)->poprzedni) //kiedy istnieje poprzedni
		{
			*aktualny = (*aktualny)->poprzedni; //i znowu sortujemy
		}
		tymczasowy = *aktualny;	//bez tego zmiany w tablicy zachodz� tylko raz i tablica nie jest dobrze posortowana

	} while (b != 0); //jak jest zero tzn �e dobra kolejno��
}

void sortowanie_grupa(struct element** aktualny)
{
	struct element* tymczasowy;
	tymczasowy = *aktualny;
	int b;
	do
	{
		b = 0;
		while (tymczasowy->nastepny) //kiedy w og�le istnieje jaki� nast�pny
		{
			int a = strcoll(tymczasowy->aktualny.grupa, tymczasowy->nastepny->aktualny.grupa); //"Funkcja por�wnuje dwa �a�cuchy znak�w str1 i str2, oraz zwraca liczb� b�d�c� opisem zale�no�ci mi�dzy nimi."

			if (a == 1) //je�li nasz aktualny kontakt jest "mniejszy" od dolnego (nast�pnego), czyli np. aktualny zaczyna si� na b, a nast�pny na a to musimy je zamieni�
			{
				zamiana(&tymczasowy);
				b = b + 1; //ni�ej to b�dzie przydatne, bo to znaczy �e jak a != 1 to nie ma zamiany, b si� nie zmieni, jest zerem i nie potrzebna jest dalsza zamiana
			}

			else
			{
				tymczasowy = tymczasowy->nastepny; //je�li pierwszy z drugim si� zgadza kolejno�ci� to przechodzimy do nast�pnego elementu
			}
		}

		//kiedy nie ma ju� nast�pnych sprawdzamy to:
		while ((*aktualny)->poprzedni) //kiedy istnieje poprzedni
		{
			*aktualny = (*aktualny)->poprzedni; //i znowu sortujemy
		}
		tymczasowy = *aktualny;	//bez tego zmiany w tablicy zachodz� tylko raz i tablica nie jest dobrze posortowana

	} while (b != 0); //jak jest zero tzn �e dobra kolejno��
}

int sortowanie(struct element** aktualny, int* sposob)
{
	struct element* tymczasowy;
	tymczasowy = *aktualny;
	int wybor;
	//int sposob;
	printf("Wybierz sortowanie alfabetyczne, wed�ug:\n1. Imienia.\n2. Nazwiska.\n3. Grupy.\n4. Cofnij.\n");
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
					printf("Niepoprawna warto��. Prosz� poda� liczb� z przedzia�u od 1 do 4.\n");
				}
			}
		} while (wybor <= 0);

		switch (wybor)
		{
		case 1:
		{
			printf("Wyb�r 1. Sortowanie wed�ug imienia.");
			sortowanie_imie(aktualny);
			sposob = 1; //p�niej b�dziemy dodawa� kontakt ze wzgl�du na spos�b
			break;
		}

		case 2:
		{
			printf("Wyb�r 2. Sortowanie wed�ug nazwiska.");
			sortowanie_nazwisko(aktualny);
			sposob = 2;
			break;
		}

		case 3:
		{
			printf("Wyb�r 3. Sortowanie wed�ug grupy.");
			sortowanie_grupa(aktualny);
			sposob = 3;
			break;
		}

		case 4:
		{
			printf("Wyb�r 4. Cofnij.");
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

//"6. [...] w miejscu zale�nym od metody sortowania (np. je�li posortowano wed�ug nazwiska nowy kontakt r�wnie� ma by� umieszczony w odpowiednim miejscu)"
void dodaj_sortowanie(struct element** aktualny, int sposob)
{
	struct element* tymczasowy;
	tymczasowy = malloc(sizeof(struct element));
	printf("\nProsz� zapisa� imi�:");
	scanf("%s", (tymczasowy->aktualny.imie));
	printf("\nProsz� zapisa� nazwisko:");
	scanf("%s", (tymczasowy->aktualny.nazwisko));
	printf("\nProsz� wpisa� numer telefonu:");
	scanf("%s", (tymczasowy->aktualny.numer));
	printf("\nProsz� zapisa� grup�:");
	scanf("%s", (tymczasowy->aktualny.grupa));

	if (sposob == 0) //gdy 0 to nieposortowane, to wynika z funkcji sortowania
	{
		dodaj_do_listy(aktualny, tymczasowy->aktualny); //dodajemy na ko�cu, bo i tak nie jest posortowane
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

/*"7. Funkcj� wyszukuj�c� kontakty wed�ug imienia i nazwiska na zasadzie por�wnywania ci�g�w
znak�w(np.wyszukuj�c ci�giem ma znajduje kontakt zawieraj�cy Marcin, ale tak�e Tomasz albo Macierewicz);
8. Na powy�szej zasadzie � funkcj� wyszukuj�c� kontakty nale��ce do jednej grupy;"*/
void znajdz(struct element** aktualny, char* tab)
{
	for (int i = 0; tab[i] != '\0'; i = i + 1)	//'\0' - nic nie wpisali�my
	{
		if (tab[i] >= 65 && tab[i] <= 95) //je�li s� du�e litery
		{
			tab[i] = tab[i] + 32; //to zamie� na ma�e litery
		}
		else	//je�li s� ma�e
		{
			tab[i] = tab[i]; //nic nie r�b
		}
	}

	/*Funkcja strcpy kopiuje tekst z tablicy do tablicy. Funkcja kopiuje znak po znaku od pocz�tku,
	a� do ko�ca tablicy lub znaku '\0', kt�ry te� kopiuje.*/
	char imie[50];
	char nazwisko[50];
	strcpy(imie, (*aktualny)->aktualny.imie);
	strcpy(nazwisko, (*aktualny)->aktualny.nazwisko); //to nie ten sam element "aktualny", po najechaniu strza�k� wida� co jest argumentem funkcji, a co struktur� globaln�

	for (int i = 0; imie[i] != '\0'; i = i + 1)	//'\0' - nic nie wpisali�my
	{
		if (imie[i] >= 65 && imie[i] <= 95) //je�li s� du�e litery
		{
			imie[i] = imie[i] + 32; //to zamie� na ma�e litery
		}
		else	//je�li s� ma�e
		{
			imie[i] = imie[i]; //nic nie r�b
		}
	}

	for (int i = 0; nazwisko[i] != '\0'; i = i + 1)	//'\0' - nic nie wpisali�my
	{
		if (nazwisko[i] >= 65 && nazwisko[i] <= 95) //je�li s� du�e litery
		{
			nazwisko[i] = nazwisko[i] + 32; //to zamie� na ma�e litery
		}
		else	//je�li s� ma�e
		{
			nazwisko[i] = nazwisko[i]; //nic nie r�b
		}
	}

	/*Funkcja, kt�rej mo�na u�y�, by okre�li� czy jeden ci�g zawiera si� w drugim, to
	strstr( char * str1, char * str2 ); gdzie drugi ci�g znak�w jest wyszukiwany w pierwszym.
	Funkcja zwraca nast�puj�ce warto�ci:
	- Je�li fragment zostanie znaleziony - tablic� znak�w ( char * )
	- Je�li fragment nie zostanie znaleziony � NULL;*/
	if (strstr(imie, tab) || strstr(nazwisko, tab))  // || - lub
	{
		//printf("Znaleziono kontakt:\n");
		printf("%s  ;  %s  ;  %s  ;  %s\n", (*aktualny)->aktualny.imie, (*aktualny)->aktualny.nazwisko, (*aktualny)->aktualny.numer, (*aktualny)->aktualny.grupa);
	}
}

void znajdz_grupe(struct element** aktualny, char* tab)
{
	for (int i = 0; tab[i] != '\0'; i = i + 1)	//'\0' - nic nie wpisali�my
	{
		if (tab[i] >= 65 && tab[i] <= 95) //je�li s� du�e litery
		{
			tab[i] = tab[i] + 32; //to zamie� na ma�e litery
		}
		else	//je�li s� ma�e
		{
			tab[i] = tab[i]; //nic nie r�b
		}
	}

	/*Funkcja strcpy kopiuje tekst z tablicy do tablicy. Funkcja kopiuje znak po znaku od pocz�tku,
	a� do ko�ca tablicy lub znaku '\0', kt�ry te� kopiuje.*/
	char grupa[50];
	strcpy(grupa, (*aktualny)->aktualny.grupa); //to nie ten sam element "aktualny", po najechaniu strza�k� wida� co jest argumentem funkcji, a co struktur� globaln�

	for (int i = 0; grupa[i] != '\0'; i = i + 1)	//'\0' - nic nie wpisali�my
	{
		if (grupa[i] >= 65 && grupa[i] <= 95) //je�li s� du�e litery
		{
			grupa[i] = grupa[i] + 32; //to zamie� na ma�e litery
		}
		else	//je�li s� ma�e
		{
			grupa[i] = grupa[i]; //nic nie r�b
		}
	}

	/*Funkcja, kt�rej mo�na u�y�, by okre�li� czy jeden ci�g zawiera si� w drugim, to
	strstr( char * str1, char * str2 ); gdzie drugi ci�g znak�w jest wyszukiwany w pierwszym.
	Funkcja zwraca nast�puj�ce warto�ci:
	- Je�li fragment zostanie znaleziony - tablic� znak�w ( char * )
	- Je�li fragment nie zostanie znaleziony � NULL;*/
	if (strstr(grupa, tab))
	{
		//printf("Znaleziono grup�:\n");
		printf("%s  ;  %s  ;  %s  ;  %s\n", (*aktualny)->aktualny.imie, (*aktualny)->aktualny.nazwisko, (*aktualny)->aktualny.numer, (*aktualny)->aktualny.grupa);
	}
}

int zliczanie(struct element** aktualny)
{
	struct element* tymczasowy;
	tymczasowy = *aktualny; //wpisujemy do tymczasowego
	int i = 1; //deklarujemy przed p�tl�
	while (tymczasowy) //je�li tymczasowy istnieje to wykonuj
	{
		tymczasowy = tymczasowy->nastepny;
		i = i + 1;
	}
	return i;
}

//"9. Funkcje usuwaj�c� wybrany kontakt z listy;"
void usun_z_listy(struct element** aktualny, int ilosc)
{
	struct element* tymczasowy;
	int wybor;

	do
	{
		wyswietl_liste(aktualny);
		ilosc = zliczanie(aktualny);

		printf("Prosz� wybra� kontakt do usuni�cia. Wyb�r 0 oznacza cofni�cie.\n");
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
					printf("Niepoprawna warto��. Prosz� poda� ponownie.\n");
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
			*aktualny = tymczasowy; //teraz pierwszym jest ten co by� wcze�niej nast�pnym
		}

		else if (wybor == ilosc) //usuwamy ostatni, analogicznie do usuni�cia pierwszego
		{
			tymczasowy = (*aktualny)->poprzedni; //przedostatni przepisujemy do tymczasowego
			(*aktualny)->poprzedni->nastepny = NULL; //usuwamy wybrany
			*aktualny = tymczasowy; //teraz ostatnim jest ten przedostatni
		}

		else if (wybor > 1 && wybor < ilosc) //ca�a reszta opr�cz pierwszego i ostatniego
		{
			(*aktualny)->poprzedni->nastepny = (*aktualny)->nastepny; //ta cz�� jest wzi�ta z instrukcji, strona 5 u g�ry
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
			(*aktualny) = (*aktualny)->poprzedni; //powr�t
		}

		//inny spos�b, ale chyba nie jest do ko�ca poprawny, bynajmniej s� b��dy w programie Visual
		/*if (wybor == 1) //pierwszy element
		{
			struct element* tymczasowy = NULL; //tymczasowa warto��
			tymczasowy = aktualny; //za tymczasowego dajemy aktualny
			aktualny = tymczasowy->poprzedni;	//za aktualnego poprzedni czyli wcze�niej nast�pny
			free(tymczasowy);	//zwalniamy ten aktualny (tymczasowy)
		}

		else if (wybor == ilosc) //ostatni - dolny
		{
			struct element* tymczasowy = NULL; //tymczasowa warto��
			tymczasowy = aktualny; //za tymczasowego dajemy aktualny
			while (tymczasowy->poprzedni != NULL) //dop�ki poprzedni element nie b�dzie null
			{
				tymczasowy = tymczasowy->poprzedni;	//to wykonujemy zamian�
			}
			tymczasowy->nastepny->poprzedni = NULL;
			free(tymczasowy);	//zwalniamy pami�� tymczasowego, �eby nie by�o wyciek�w
		}

		else if (wybor > 1 && wybor < ilosc)
		{
			struct element* tymczasowy = NULL; //tymczasowa warto��
			tymczasowy = aktualny; //za tymczasowego dajemy aktualny
			for (int i = 1; i < wybor; i = i + 1) //w sumie tak jak wy�ej, ale nie jedziemy do samego ko�ca, tylko do wyboru
			{
				tymczasowy = tymczasowy->poprzedni;
			}
			tymczasowy->poprzedni->nastepny = tymczasowy->nastepny;
			tymczasowy->nastepny->poprzedni = tymczasowy->poprzedni;
			free(tymczasowy);	//zwalniamy pami�� tymczasowego, �eby nie by�o wyciek�w
		}*/

	} while (wybor == 0);
}

int main()
{
	struct element* aktualny = NULL; /*deklaracja zmiennej strukturalnej, jest to tablica, w kt�rej b�dziemy przechowywa� struktury
										wska�nik, aby mo�na by�o �atwo edytowa� ewentualne zmiany
										to jest w�a�nie nasza dynamiczna baza do przechowywania w pami�ci i obs�ugi obraz�w
										NULL, bo na poczatku po odpaleniu programu nie ma zadnego obrazu na liscie, jest ona pusta*/
										//int c, ilosc, i = 0, j = 0;
										//char znak;
										//char klawiatura[20] = { 0 };
	setlocale(LC_ALL, "polish_poland");	//do polskich znak�w
	wczytaj_z_listy(&aktualny); //tu wybieramy kt�r� list� chcemy wgra�
	int w; //wybor z menu
	char tab[50] = { 0 }; //do wyszukiwania, inicjowanie tablicy wymaga listy inicjalizatora w nawiasach klamrowych, czyli nie mo�e by� NULL
	int* sposob = 0; //wska�nik, �eby m�c p�niej si� odwo�ywa� do jakiej� zmiennej
	int ilosc = zliczanie(&aktualny);
	printf("				\nMENU WYBORU\nProsz� wybra� jedn� z poni�szych opcji i zatwierdzi� klawiszem enter:\n");
	do
	{
		while (aktualny->poprzedni) //wracamy do pocz�tku za ka�dym razem, aby wy�wietlanie kontak�w �adnie dzia�a�o
		{
			aktualny = aktualny->poprzedni;
		}

		printf("\n1. Wy�wietl list� kontakt�w.\n2. Dodaj kontakt.\n3. Usu� kontakt.\n4. Sortuj kontakty.\n5. Wyszukaj kontakt.\n6. Wyszukaj grup�.\n7. Zako�cz program.\n\n");
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
					printf("Niepoprawna warto��. Prosz� poda� liczb� z przedzia�u od 1 do 6.\n");
				}
			}
		} while (w <= 0);

		switch (w)
		{
		case 1:
		{
			printf("Wyb�r 1. Wy�wietlanie kontakt�w:\n");
			wyswietl_liste(&aktualny);

			/*notatka: argument musi byc zawsze tego samego poziomu, a skoro w main jest *akutalny, a w innych funkcjach **aktualny
			to w wywo�aniu w main musi byc &aktualny, �eby by� ten sam poziom; ale je�eli w funkcji jest **aktulany
			to w wywo�aniu funkcji musi byc np dodaj_do_listy(aktualny)*/

			break;
		}
		case 2:
		{
			printf("Wyb�r 2. Dodanie nowego kontaktu.\n");
			dodaj_sortowanie(&aktualny, sposob);
			ilosc = zliczanie(&aktualny);	//ilo�� si� powi�ksza
			wyswietl_liste(&aktualny);
			break;
		}
		case 3:
		{
			printf("Wyb�r 3. Usuni�cie kontaktu.\n");
			ilosc = zliczanie(&aktualny);
			usun_z_listy(&aktualny, ilosc);
			ilosc = zliczanie(&aktualny);	//ilo�� si� zmniejsza
			wyswietl_liste(&aktualny);
			break;
		}
		case 4:
		{
			printf("Wyb�r 4. Sortowanie kontakt�w w spos�b alfabetyczny.\n");
			sposob = sortowanie(&aktualny, sposob);
			wyswietl_liste(&aktualny);
			break;
		}
		case 5:
		{
			printf("Wyb�r 5. Wyszukiwanie kontaktu. Aby zako�czy� kliknij Enter.\n");
			printf("Wyszukiwarka:\n");
			int z = 0; //do zliczania
			char litera;
			do
			{
				while (aktualny->poprzedni)
				{
					aktualny = aktualny->poprzedni;
				}

				litera = _getch(); //Dla �dynamicznego� wyszukiwania mo�na u�y� funkcji getch() lub _getch(), kt�ra co prawda pobiera
				//z klawiatury po jednym znaku, ale dzia�a natychmiastowo bez potrzeby potwierdzania �enterem�

				if (litera >= 97 && litera <= 122) //ma�e litery
				{
					tab[z] = litera;
					z = z + 1;
				}
				else if (litera >= 65 && litera <= 95) //du�e litery
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
				} while (aktualny->nastepny); //a� dojdziemy do ostatniego kontaktu

			} while (litera != 13); //enter, wtedy koniec

			/*po zako�czeniu wykonywania si� funkcji wyszukiwania musz� wyczy�cic tablic� tab[] w main,
			bo przy nastepnym wyszukiwaniu to, co by�o wyszukane za poprzednim razem niestety zostaje*/
			for (int i = 0; i < 50; i = i + 1)
			{
				tab[i] = NULL;
			}
			break;
		}
		case 6:
		{
			printf("Wyb�r 6. Wyszukiwanie grupy. Aby zako�czy� kliknij Enter.\n");
			printf("Wyszukiwarka:\n");
			int z = 0; //do zliczania
			char litera;
			do
			{
				while (aktualny->poprzedni)
				{
					aktualny = aktualny->poprzedni;
				}

				litera = _getch(); //Dla �dynamicznego� wyszukiwania mo�na u�y� funkcji getch() lub _getch(), kt�ra co prawda pobiera
				//z klawiatury po jednym znaku, ale dzia�a natychmiastowo bez potrzeby potwierdzania �enterem�

				if (litera >= 65 && litera <= 95) //du�e litery
				{
					tab[z] = litera;
					z = z + 1;
				}
				else if (litera >= 97 && litera <= 122) //ma�e litery
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
				} while (aktualny->nastepny); //a� dojdziemy do ostatniego kontaktu

			} while (litera != 13); //enter, wtedy koniec

			/*po zako�czeniu wykonywania si� funkcji wyszukiwania musz� wyczy�cic tablic� tab[] w main,
			bo przy nastepnym wyszukiwaniu to, co by�o wyszukane za poprzednim razem niestety zostaje*/
			for (int i = 0; i < 50; i = i + 1)
			{
				tab[i] = NULL;
			}
			break;
		}
		case 7:
		{
			printf("Wyb�r 7. Zako�czenie programu.\n");
			while (aktualny->nastepny) //dop�ki istnieje jaki� nast�pny to zwalniamy pami��
			{
				aktualny = aktualny->nastepny;
				free(aktualny->poprzedni);
			}
			/*
			drugi spos�b, ale chyba nie do ko�ca poprawny, nie jestem pewna
			while (aktualny->nastepny)
			{
				struct element* tymczasowy = NULL; //tymczasowa warto��
				tymczasowy = aktualny; //za tymczasowego dajemy aktualny
				aktualny = tymczasowy->poprzedni;	//za aktualnego poprzedni czyli wcze�niej nast�pny
				free(tymczasowy);	//zwalniamy ten aktualny (tymczasowy)
				aktualny = aktualny->nastepny;
				free(aktualny->poprzedni);
			}
			*/
			printf("Program zako�czono.\n");
			break;
		}
		default:
		{
			printf("Niestety takiej opcji nie ma w menu wyboru. Prosz� poda� ponownie:\n");
			break;
		}
		}
	} while (w != 7);

	return 0;
}