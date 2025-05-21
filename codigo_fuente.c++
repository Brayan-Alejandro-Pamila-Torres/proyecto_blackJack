#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

struct Carta {
    string nombre;
    string palo;
    int valor;
};

struct Jugador {
    string nombre;
    vector<Carta>* mano = new vector<Carta>(); // Memoria dinámica para la mano
    int puntuacion;
    bool enJuego = true; // Indica si el jugador sigue en juego o no

    // Destructor para liberar memoria
    ~Jugador() {
        delete mano;
    }
};

vector<Carta>* crearMazo();
void barajar(vector<Carta>* mazo);
Carta tomarCarta(vector<Carta>* mazo, vector<Carta>* mano);
void calcularPuntos(Jugador& jugador);
void guardarPartida(const Jugador& jugador, const Jugador& banca, const string& resultado);
void jugarBlackjack();
void verArchivoPuntajes();

int main() {
    int opcion;
    do {
        cout << "Bienvenido al juego de Blackjack!" << endl;
        cout << "1. Jugar Blackjack" << endl;
        cout << "2. Ver archivo de puntajes" << endl;
        cout << "3. Salir" << endl;
        cout << "Elige una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                jugarBlackjack();
                break;
            case 2:
                verArchivoPuntajes();
                break;
            case 3:
                cout << "Gracias por jugar Blackjack. Hasta la proxima" << endl;
                break;
            default:
                cout << "Opcion invalida. Intentalo de nuevo." << endl;
        }

    } while (opcion != 3);

    return 0;
}

void jugarBlackjack() {
    vector<Carta>* mazo = crearMazo();
    barajar(mazo);
    string nombre;
    string resultado;
    cout << "Ingrese su nombre: ";
    cin >> nombre;

    Jugador jugador = {nombre};
    Jugador banca = {"Banca", new vector<Carta>(), 0, true};

    jugador.mano->push_back(tomarCarta(mazo, jugador.mano));
    jugador.mano->push_back(tomarCarta(mazo, jugador.mano));
    banca.mano->push_back(tomarCarta(mazo, banca.mano));
    banca.mano->push_back(tomarCarta(mazo, banca.mano));


    cout << jugador.nombre << ", tus dos cartas iniciales son:\n";
    for (const Carta& carta : *jugador.mano) {
        cout << "- " << carta.nombre << " de " << carta.palo << " (Valor: " << carta.valor << ")\n";
    }
    cout<< endl<< endl;

    cout<< "La primera carta de la banca es:\n";
    cout << "- " << banca.mano->at(0).nombre << " de " << banca.mano->at(0).palo << " (Valor: " << banca.mano->at(0).valor << ")\n";
    cout<<endl;
    calcularPuntos(jugador);
    calcularPuntos(banca);

    while (jugador.puntuacion < 21 && jugador.enJuego) {
        char opcion; 
        cout << "Tu puntuacion es: " << jugador.puntuacion << endl << "Quieres pedir carta (h) o plantarte (s)? ";
        cin >> opcion;
        if (opcion != 'h' && opcion != 's') {
            cout << "Opcion invalida. Intentalo de nuevo." << endl;
            continue; // Volver a preguntar si la opción es inválida
        }

        if (opcion == 'h') {
            jugador.mano->push_back(tomarCarta(mazo, jugador.mano));
            calcularPuntos(jugador);
            cout << "Has tomado una carta: " << jugador.mano->back().nombre << " de " << jugador.mano->back().palo << endl;
            if (jugador.mano->back().nombre == "A") {
            cout << "Has tomado un As! quieres obtener 1 o 11 puntos?" << endl;
            int puntos;
            cin >> puntos;
            if (puntos == 1) {
            jugador.puntuacion += 1;
            } else if (puntos == 11) {
            jugador.puntuacion += 10;
            }
        }
            if(jugador.puntuacion >21){
                cout << "Te has pasado de 21. La banca gana!" << endl;
                // Revelar la segunda carta de la banca
                cout << "\nLa segunda carta oculta de la banca era:\n";
                cout << "- " << banca.mano->at(1).nombre << " de " << banca.mano->at(1).palo 
                << " (Valor: " << banca.mano->at(1).valor << ")\n";
                cout << endl;

                resultado = "Perdiste!";
    
                // Mostrar puntuaciones finales
                cout << "Resultado: " << resultado << endl;
                cout << "PUNTUACIONES FINALES:" << endl;
                cout << jugador.nombre << ": " << jugador.puntuacion << endl;
                cout << banca.nombre << ": " << banca.puntuacion << endl;

                guardarPartida(jugador, banca, resultado);
                delete mazo;

        return; 
            }

        } else if (opcion == 's') {
            jugador.enJuego = false;
            cout << "Te has plantado. Tu puntuacion es: " << jugador.puntuacion << endl;
            break;
        }
    }
    
    cout<< "\nLa banca juega ahora..."<<endl;
    cout<< "La 2da carta de la banca es:\n";
    cout << "- " << banca.mano->at(1).nombre << " de " << banca.mano->at(1).palo << " (Valor: " << banca.mano->at(1).valor << ")\n";
    
    while (banca.puntuacion < 17) {
        banca.mano->push_back(tomarCarta(mazo, banca.mano));
        cout << "La banca toma una carta: " << banca.mano->back().nombre << " de " << banca.mano->back().palo << endl;
        calcularPuntos(banca);
    }
    if (jugador.puntuacion > 21) {
        resultado = "Perdiste!"; 
        cout<<"La banca gana!"<<endl;
        cout<<endl;
    } else if (banca.puntuacion > 21 || jugador.puntuacion > banca.puntuacion) {
        resultado = "Ganaste!";
    } else if (jugador.enJuego == false && jugador.puntuacion == banca.puntuacion) {
        cout<<"Has empatado con la banca por lo tanto la banca gana!"<<endl;
        cout<<endl;
        resultado = "Perdiste!";
    }else{
        resultado = "Perdiste!";
        cout<<"La banca gana!"<<endl;
        cout<<endl;
    }
    if(jugador.puntuacion == 21){
        cout<< "BLACKJACK del jugador !";;
        cout<<endl;
    }else if(banca.puntuacion == 21){
        cout<<"La banca tiene BLACKJACK!"<<endl;
        cout<<endl;
    }

    cout << "Resultado: " << resultado << endl;
    cout<< "PUNTUACIONES FINALES:"<<endl;
    cout << jugador.nombre << ": " << jugador.puntuacion << endl;
    cout << banca.nombre << ": " << banca.puntuacion << endl;
    cout<<endl;
    guardarPartida(jugador,banca, resultado);

    delete mazo;
}

vector<Carta>* crearMazo() {
    vector<Carta>* mazo = new vector<Carta>();
    string palos[] = {"Corazones", "Diamantes", "Treboles", "Picas"};
    string nombres[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    int valores[] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

    for (const string& palo : palos) {
        for (int i = 0; i < 13; i++) {
            mazo->push_back({nombres[i], palo, valores[i]});
        }
    }
    return mazo;
}

void barajar(vector<Carta>* mazo) {
    srand(time(0));
    for (size_t i = 0; i < mazo->size(); i++) {
        int j = rand() % mazo->size();
        swap((*mazo)[i], (*mazo)[j]);
    }
}

Carta tomarCarta(vector<Carta>* mazo, vector<Carta>* mano) {
    Carta nuevaCarta;
    bool repetido;
    
    do {
        nuevaCarta = mazo->back();
        mazo->pop_back();
        repetido = false;
        
        for (const Carta& carta : *mano) {
            if (carta.nombre == nuevaCarta.nombre && carta.palo == nuevaCarta.palo) {
                repetido = true;
                break;
            }
        }
        
        if (repetido) {
            mazo->insert(mazo->begin(), nuevaCarta); // Volver a insertar la carta repetida en el mazo
        }

    } while (repetido);

    return nuevaCarta;
}

void calcularPuntos(Jugador& jugador) {
    int total = 0, ases = 0;

    for (const Carta& carta : *jugador.mano) {
        total += carta.valor;
        if (carta.nombre == "A") ases++;
    }

    while (total > 21 && ases > 0) {
        total -= 10;
        ases--;
    }

    jugador.puntuacion = total;
}

void guardarPartida(const Jugador& jugador,const Jugador& banca, const string& resultado) {
    ofstream archivo("partida.txt", ios::app);
    if (archivo.is_open()) {
        archivo << "Nombre: " << jugador.nombre << endl;
        archivo << "Puntuación del jugador: " << jugador.puntuacion << endl;
        archivo << "Puntuación de la banca: " << banca.puntuacion << endl;
        archivo << "Resultado: " << resultado << endl;
        archivo << "-----------------------------" << endl; // Separador entre partidas
        archivo.close();
    } else {
        cout << "Error al abrir el archivo" << endl;
    }
}

void verArchivoPuntajes() {
    ifstream archivo("partida.txt");
    string linea;
    
    if (archivo.is_open()) {
        cout << "Puntajes guardados:\n";
        while (getline(archivo, linea)) {
            cout << linea << endl;
        }
        archivo.close();
    } else {
        cout << "No se pudo abrir el archivo de puntajes." << endl;
    }
}
