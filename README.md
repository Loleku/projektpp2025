# projektpp2025

# Przykład użycia aplikacji

./main.exe dane.csv <Opcjonalnie: wynik.csv>

# Dwa tryby:
## Uczeń:
Kolumy CSV zawierające: nazwe przedmiotu, oceny z niego opcjonalnie z wagą

przykład bez wagi:

przedmiot,oceny
Biologia,[6,6,6,6,2,1,2,1,3]
Matematyka,[1,1,1,1,1,1]

przykład z wagą:

Oceny zapisujemy w formacie: [[ocena,waga],...]

przedmiot,oceny
Biologia,[[6,3],[6,2],[5,1]]
Matematyka,[[1,3],[1,3],[1,3]]

## Nauczyciel:
Kolumny CSV zawierające: imie, nazwisko, oceny

przykład bez wagi

imie,nazwisko,oceny
Jan,Kowalski,[1,1,1,1,1]
Marta,Łyjak,[3,3,3,1,2]

przykład z wagą

imie,nazwisko,oceny
Jan,Kowalski,[[1,1],[1,1],[1,3]]
Marta,Łyjak,[[3,3],[3,1],[2,2]]