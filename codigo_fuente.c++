//Brayan Alejandro Pamila Torres y Diego Abraham Delgado Rodriguez
//EQUIPO: LAMINE YABIEN
//PROYECTO: BLACKJACK
#include <iostream>// para usar cout y cin
#include <vector>// para usar vectores
#include <cstdlib>// para usar rand y srand
#include <ctime>// para usar srand y time
#include <fstream>//uso de archivos
#include <windows.h> //para uso de colores en la consola


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

    // funcion que nos ayuda a liberar la memoria del jugador
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
void cambiarColor(int color);
int main() {
    int opcion;
    cambiarColor(11);
    cout<<"------------------------------=====------------------------------"<<endl;
    cout<<" Brayan Alejandro Pamila Torres y Diego Abraham Delgado Rodriguez"<<endl;
    cout<<"           PROYECTO FINAL  PROGRAMACION II: BLACKJACK "<<endl;
    cout<<"------------------------------=====------------------------------"<<endl;
    do {
        //mostrar en verde el mensaje de bienvenida
        cambiarColor(10);
        cout << "Bienvenido al juego de Blackjack!" << endl;
        //mostrar en azul el mensaje de opciones
        cambiarColor(9);
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
    //volver al color normal
        cambiarColor(7);

    Jugador jugador = {nombre};
    Jugador banca = {"Banca", new vector<Carta>(), 0, true};

    jugador.mano->push_back(tomarCarta(mazo, jugador.mano));
    jugador.mano->push_back(tomarCarta(mazo, jugador.mano));
    banca.mano->push_back(tomarCarta(mazo, banca.mano));
    banca.mano->push_back(tomarCarta(mazo, banca.mano));


    cout << jugador.nombre << ", tus dos cartas iniciales son:\n";
    cambiarColor(9);
    for (const Carta& carta : *jugador.mano) {
        cout << "- " << carta.nombre << " de " << carta.palo << " (Valor: " << carta.valor << ")\n";
    }
    cout<< endl<< endl;
    cambiarColor(7);
    cout<< "La primera carta de la banca es:\n";
    cambiarColor(9);
    cout << "- " << banca.mano->at(0).nombre << " de " << banca.mano->at(0).palo << " (Valor: " << banca.mano->at(0).valor << ")\n";
    cout<<endl;
    cambiarColor(7);
    calcularPuntos(jugador);
    calcularPuntos(banca);

    while (jugador.puntuacion < 21 && jugador.enJuego) {
        char opcion; 
        cout << "Tu puntuacion es: " << jugador.puntuacion << endl << "Quieres pedir carta (h) o plantarte (s)? ";
        cin >> opcion;
        if (opcion != 'h' && opcion != 's') {
            cambiarColor(12);
            cout << "Opcion invalida. Intentalo de nuevo." << endl;
            cambiarColor(7);
            continue; // Volver a preguntar si la opción es inválida

        }

        if (opcion == 'h') {
            jugador.mano->push_back(tomarCarta(mazo, jugador.mano));
            calcularPuntos(jugador);
            cambiarColor(9);
            cout << "Has tomado una carta: " << jugador.mano->back().nombre << " de " << jugador.mano->back().palo << endl;
            cambiarColor(7);
            if (jugador.mano->back().nombre == "A") {
            cambiarColor(9);
            cout << "Has tomado un As! quieres obtener 1 o 11 puntos?" << endl;
            cambiarColor(7);
            int puntos;
            cin >> puntos;
            jugador.mano->back().valor = puntos;
            // Actualizar la puntuación del jugador
            // Si el jugador elige 1, se suma 1 a la puntuación
            calcularPuntos(jugador);
        }
            if(jugador.puntuacion >21){
                cout << "Te has pasado de 21. La banca gana!" << endl;
                // Revelar la segunda carta de la banca
                cout << "\nLa segunda carta oculta de la banca era:\n";
                cout << "- " << banca.mano->at(1).nombre << " de " << banca.mano->at(1).palo 
                << " (Valor: " << banca.mano->at(1).valor << ")\n";
                cout << endl;
                //poner perdiste en rojo
                cambiarColor(12);
                resultado = "Perdiste!";
                cambiarColor(7);
                // Mostrar puntuaciones finales
                cambiarColor(9);
                cout << "Resultado: " << resultado << endl;
                cout << "PUNTUACIONES FINALES:" << endl;
                cout << jugador.nombre << ": " << jugador.puntuacion << endl;
                cout << banca.nombre << ": " << banca.puntuacion << endl;
                cambiarColor(7);
                guardarPartida(jugador, banca, resultado);
                delete mazo;

        return; 
            }

        } else if (opcion == 's') {
            jugador.enJuego = false;
            cambiarColor(9);
            cout << "Te has plantado. Tu puntuacion es: " << jugador.puntuacion << endl;
            cambiarColor(7);
            break;
        }
    }
    
    cout<< "\nLa banca juega ahora..."<<endl;
    cout<< "La 2da carta de la banca es:\n";
    cambiarColor(9);
    cout << "- " << banca.mano->at(1).nombre << " de " << banca.mano->at(1).palo << " (Valor: " << banca.mano->at(1).valor << ")\n";
    cambiarColor(7);
    while (banca.puntuacion < 17) {
        banca.mano->push_back(tomarCarta(mazo, banca.mano));
        cambiarColor(9);
        cout << "La banca toma una carta: " << banca.mano->back().nombre << " de " << banca.mano->back().palo << endl;
        cambiarColor(7);
        calcularPuntos(banca);
    }
    if (jugador.puntuacion > 21) {
        //poner perdiste en rojo
        cambiarColor(12);
        resultado = "Perdiste!"; 
        cout<<"La banca gana!"<<endl;
        cambiarColor(7);
        cout<<endl;
    } else if (banca.puntuacion > 21 || jugador.puntuacion > banca.puntuacion) {
        //poner ganaste en verde
        cambiarColor(10);
        resultado = "Ganaste!";
        cambiarColor(7);
    } else if (jugador.enJuego == false && jugador.puntuacion == banca.puntuacion) {
        cambiarColor(9);
        cout<<"Has empatado con la banca por lo tanto la banca gana!"<<endl;
        cout<<endl;
        //poner perdiste en rojo
        cambiarColor(12);
        resultado = "Perdiste!";
        cambiarColor(7);
    }else{
        cambiarColor(12);
        resultado = "Perdiste!";
        cout<<"La banca gana!"<<endl;
        cambiarColor(7);
        cout<<endl;
    }
    if(jugador.puntuacion == 21){
        cambiarColor(9);
        cout<< "BLACKJACK del jugador !";
        cambiarColor(7);
        cout<<endl;
    }else if(banca.puntuacion == 21){
        cambiarColor(9);
        cout<<"La banca tiene BLACKJACK!"<<endl;
        cambiarColor(7);
        cout<<endl;
    }
    cambiarColor(9);
    cout << "Resultado: " << resultado << endl;
    cout<< "PUNTUACIONES FINALES:"<<endl;
    cout << jugador.nombre << ": " << jugador.puntuacion << endl;
    cout << banca.nombre << ": " << banca.puntuacion << endl;
    cout<<endl;
    cambiarColor(7);
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
    int total = 0;
    
    for (const Carta& carta : *jugador.mano) {
        total += carta.valor;  // Se usa el valor asignado a cada carta
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
        //mostrar en rojo el mensaje de error
        cambiarColor(12);
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
void cambiarColor(int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
