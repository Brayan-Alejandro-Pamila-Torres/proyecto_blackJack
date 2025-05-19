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
    vector<Carta> mano;
    int puntuacion;
    bool enJuego = true; //esta variable indica si el jugador sigue en juego o no
};

vector<Carta> crearMazo();
void barajar(vector<Carta>& mazo);
Carta tomarCarta(vector<Carta> & mazo);
void calcularPuntos(Jugador& jugador);
void guardarPartida(const Jugador & jugador, const string& resultado);

int main(){
    vector<Carta> mazo = crearMazo();
    barajar(mazo);
    cout<<"Bienvenido al juego de Blackjack!"<<endl;
    cout<< "REGLAS DEL JUEGO"<<endl;
    cout<<"El objetivo del juego es llegar a 21 puntos o lo mas cercano posible sin pasarse."<<endl;
    cout<<"Si el jugador se pasa de 21 puntos, pierde automaticamente."<<endl;
    cout<<"Si la banca se pasa de 21 puntos, el jugador gana automaticamente."<<endl;
    cout<<"Si el jugador y la banca tienen la misma puntuacion, gana la banca."<<endl;
    cout<<"EMPEZEMOS EL JUEGO!"<<endl;
    //limpiar pantalla para comenzar el juego
    system("Pause");
    system("cls");
    cout<<"Ingrese su nombre: ";
    string nombre;
    cin>>nombre;
    //se le pide al jugador que ingrese su nombre

    Jugador jugador = {nombre}; //se crea el jugador
    Jugador banca = {"Banca"};
    //se le asigna el nombre al jugador y a la banca
    jugador.mano.push_back(tomarCarta(mazo));
    jugador.mano.push_back(tomarCarta(mazo));
    banca.mano.push_back(tomarCarta(mazo));
    banca.mano.push_back(tomarCarta(mazo));
    // Mostrar cartas iniciales del jugador
    cout<< nombre;
    cout << " tus dos cartas iniciales son:\n";
    for (const Carta& carta : jugador.mano) {
        cout << "- " << carta.nombre << " de " << carta.palo << " (Valor: " << carta.valor << ")\n";
    }
    
    cout << "\nLas cartas iniciales de la banca son:\n";
    for (const Carta& carta : banca.mano) {
      cout << "- " << carta.nombre << " de " << carta.palo << " (Valor: " << carta.valor << ")\n";
    }
    calcularPuntos(jugador);
    calcularPuntos(banca);
    //turno del jugador
    while (jugador.puntuacion < 21 && jugador.enJuego) {
        char opcion;
        cout << "La puntuacion de la banca es: " << banca.puntuacion << endl;
        cout << "Tu puntuacion es: " << jugador.puntuacion <<endl << "Quieres pedir carta (h) o plantarte (s) ";
        cin >> opcion;
        if( opcion == 'h'){
           jugador.mano.push_back(tomarCarta(mazo));
            calcularPuntos(jugador);
        }else if (opcion == 's'){
            cout << "Te plantaste con " << jugador.puntuacion << " puntos." << endl;
            jugador.enJuego = false; //el jugador se planta y ya no puede pedir mas cartas
            break;
        }
    }
    //turno de la banca
    while (banca.puntuacion < 17){
        banca.mano.push_back(tomarCarta(mazo));
        calcularPuntos(banca);
    }
    //determinar el resultado
    string resultado;
    if(jugador.puntuacion > 21){
        resultado = "Perdiste!";
    }else if(banca.puntuacion > 21 || jugador.puntuacion > banca.puntuacion){
        resultado = "Ganaste!";
    }else {
        resultado = "Perdiste!";
    }
    cout<< "Resultado: " << resultado << endl;
    guardarPartida(jugador, resultado);
    return 0;
}
//se declaran las funciones

//implementacion de funciones
vector<Carta> crearMazo() {
    vector<Carta> mazo;
    string palos[] = {"Corazones", "Diamantes", "Treboles", "Picas"};
    string nombres[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    int valores [] = {11, 2,3,4,5,6,7,8,9,10,10,10,10};
    for(const string& palo : palos) {
            for(int i = 0 ; i < 13; i++){
                mazo.push_back({palo, nombres[i], valores[i]});
            }
        }
        return mazo;
    }
void barajar(vector<Carta>& mazo){
    srand(time(0)); // Inicializa la semilla con el tiempo actual
    for (size_t i = 0; i < mazo.size(); i++) {
        int j = rand() % mazo.size(); // Genera un índice aleatorio
        swap(mazo[i], mazo[j]); // Intercambia dos cartas
    }
    
}
Carta tomarCarta(vector<Carta> & mazo){
    Carta carta = mazo.back(); //toma la carta de arriba del mazo
    mazo.pop_back(); //la quita del mazo
    //cout<< "Tomaste la carta   " << carta.nombre << " de " << carta.palo << endl;
    //cout<< "Su valor es: " << carta.valor << endl;
    return carta; //devuelve la carta
}
void calcularPuntos(Jugador& jugador) {
    int total = 0;
    int ases = 0;
    for(const Carta& carta : jugador.mano) {
        total += carta.valor;
        if(carta.nombre == "A") {
            ases++;
        }
    }
        while (total > 21 && ases > 0){
            total -= 10; //si el total es mayor a 21 y hay ases en la mano, se le resta 10 al total
            ases--;
        }
        jugador.puntuacion = total; //se le asigna el total a la puntuacion del jugador

}
void guardarPartida(const Jugador & jugador, const string& resultado){
    ofstream archivo("partida.txt", ios::app); //abre el archivo en modo appeend
    if(archivo.is_open()){
        archivo << "Nombre: " << jugador.nombre << endl;
        archivo << "Puntuacion: " << jugador.puntuacion << endl;
        archivo << "Resultado: " << resultado << endl;
        archivo.close();
    } else {
        cout << "Error al abrir el archivo" << endl;
    }
}



