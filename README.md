# Projekt: Książka Telefoniczna

## Opis Projektu

Projekt polega na stworzeniu programu do zarządzania książką telefoniczną. Program umożliwia dodawanie, wyświetlanie, sortowanie oraz wyszukiwanie kontaktów, a także ich usuwanie. Kontakty są przechowywane w formie listy, co pozwala na efektywne zarządzanie danymi.

## Funkcjonalności programu:

1. **Struktura na kontakt**  
   Definicja struktury `Kontakt`, która przechowuje dane o osobie, w tym imię, nazwisko, numer telefonu oraz grupę (np. rodzina, praca, znajomi).

2. **Struktura elementu listy**  
   Implementacja struktury `ElementListy`, która zawiera kontakt oraz wskaźniki do poprzedniego i następnego elementu, tworząc w ten sposób podwójnie powiązaną listę.

3. **Wczytywanie kontaktów z pliku .CSV**  
   Funkcja do wczytywania kontaktów z pliku CSV, formatująca je i tworząca listę kontaktów.

4. **Wyświetlanie listy kontaktów**  
   Funkcja, która wyświetla wszystkie kontakty na konsoli w czytelny sposób.

5. **Sortowanie listy**  
   Funkcja sortująca kontakty według imienia, nazwiska lub grupy, umożliwiająca łatwe zarządzanie danymi.

6. **Dodawanie kontaktów**  
   Funkcja dodająca nowy kontakt do listy w odpowiednim miejscu, zgodnie z wybraną metodą sortowania, aby zachować porządek.

7. **Wyszukiwanie kontaktów**  
   Funkcja wyszukująca kontakty według imienia i nazwiska, stosując porównywanie ciągów znaków. Umożliwia wyszukiwanie częściowe, np. wyszukując "ma" znajdzie kontakty zawierające "Marcin", "Tomasz" czy "Macierewicz".

8. **Wyszukiwanie według grupy**  
   Funkcja, która umożliwia wyszukiwanie kontaktów przynależących do określonej grupy.

9. **Usuwanie kontaktów**  
   Funkcja do usuwania wybranego kontaktu z listy, umożliwiająca aktualizację struktury danych.
