# SnakeNN Daniel Krywult 1TR2 23.05.2023 - Trenowanie Sieci Neuronowej do gry Snake
Discord - danihek#6538

// --------------------------- Tytul
Projekt AI do gry Snake z uzyciem sieci neuronowych trenowanych algorytmem genetycznym(obecnie funckja crossover jest wylaczona, zamiast tego selfmutate) - wersja niedokonczona

// Budowa sieci neuronowej - na ten czas nie mozna jej ustawic przy wlaczaniu i jest ustawiona w kodzie domyslnie

NN = {16, 8, 6 ,4}

INPUT LAYER - 16
	{
		16 promieni (raycasty) wyprowadzone w 8 kierunkach(jak w kompasie) n,ne,e,se,s,sw,w,nw
		
		- pierwsze 8 jest wysylane dopoki nie trafi na sciane
		
		- drugie osiem jest wysylane dopoki nie trafi na jablko
		
		W przypadku natrafienia na obiekt jest obliczana odleglosc miedzy PIERWSZYM SEGMENTEM snake,
		a obiektem i wprowadzona do inputu jako - "1/distance"
		
		
		//W przyszlosci bedzie dodatkowe osiem inputow,
		ktore wysyla kolejne osiem raycastow i sprawdza odleglosc pomiedzy "glowa" snake,
		a reszta jego ciala, by nie zabijal sie o siebie
	}
	
HIDDEN LAYER - {8, 6}
	{
		8 - suma wartosci wszystkich 16 inputow pomnozonych przez wage ich polaczenia
		
		6 - suma wartosci poprzedniej warstwy pomnozonych przez wage ich polaczenia
	}
	
OUTPUT LAYER - 4
	{
		1 - suma wartosci poprzedniej warstwy pomnozonych przez wage ich polaczenia
		2 - suma wartosci poprzedniej warstwy pomnozonych przez wage ich polaczenia
		3 - suma wartosci poprzedniej warstwy pomnozonych przez wage ich polaczenia
		4 - suma wartosci poprzedniej warstwy pomnozonych przez wage ich polaczenia// najwyzsza wartosc jest wybierana jako ruch snake
	}

// --------------------------- Jak poprawnie odczytywac zapisane modele?

1180-27040000 - pierwsza czesc to liczba generacji = "1180"
+ "-"
druga czesc to najlepszy wynik konkretnego modelu  = "27040000" - im wyzszy wynik tym lepiej

	-0.929735 // przykladowa zawartosc
	-0.603564
	0.341047
	-0.339219
	1.000000
	0.794609
	-0.105155
	-1.000000
	1.000000
	1.000000
	-0.491314

// --------------------------- Jak uzywac?
Po wlaczeniu aplikacji trzeba wpisac parametry startowe

// zalecane dla trenowania
screenwidth  - 800
screenheight - 800

screen blockSize(px) - 16

screen rows - 30
screen cols - 30

player or ai? - 1
Population size - 500
trained model - 0

// --------------------------- Przykladowe do odczytania juz wytrenowanego modelu
screenwidth  - 800
screenheight - 800

screen blockSize(px) - 16

screen rows - 30
screen cols - 30

player or ai? - 1
Population size - 500
trained model - 1

model filename - "modelname" - aktualnie najlepszy znajduje sie w folderze "models" pod nazwa "5720-362989568"

//--------------------------- Sterowanie

[przycisk na klawiaturze] - funkcja

[SPACE] - szybka symulacja / wolna symulacja
	[0] i [9] - sterowanie szybkoscia symulacji (tylko gdy jest wolna) - jezeli ![SPACE]
	[Q] - wylacza rysowanie tylko gdy jest WOLNA symulacja(by przyspieszyc proces trenowania - jezeli [SPACE]
	
[R] - rysowanie wszystkich/jednego najlepszego snake
	[U] - wyswietlenie inputu i outputu najlepszego snake w konsoli - tylko jezeli [R]
[T] - rysowanie raycastow on/off

//Jezeli zostal wybrany tryb gracza, a nie AI - strzalki do sterowania

