#include <iostream>  //Permite imprimir en pantalla (cout)
#include <vector> // Permite usar vectores (arreglos dinamicos)
#include <queue> // Permite usar colas (estructuraa FIFO)
#include <cstdlib> // Permite convertir texto a numero (atoi)
#include <ctime> // Permite usar funciones rerelacionadas con el tiempo
#include <chrono> // Permite medir tiempo de ejecucion 
#include <algorithm> // Permite usar shuffle (mezclar elementos)
#include <random> // Permite generar numeros aleatorios modernos (pseudoaleatorios)

using namespace std; // Usamos el espacio de nombres estandar

// CONSTANTES DEL LABERINTO
const char PARED = '#'; 
const char CAMINO = ' '; 
const char ENTRADA = 'E'; 
const char SALIDA = 'S';
const char SOLUCION = '.';

// VARIABLES GLOBALES
int TAM; // Tamaño del laberinto
vector<vector<char>> laberinto; // Matriz dinamico
// Movimientos posibles para generar el laberinto
// Se mueve de 2 en 2 para saltar paredes
int dx[4] = {-2, 2, 0, 0};   // Movimiento vertical
int dy[4] = {0, 0, -2, 2};   // Movimiento horizontal

// FUNCIÓN PARA VERIFICAR SI UNA POSICIÓN ES VÁLIDA
bool esValido(int x, int y) {
    return x > 0 && x < TAM - 1 && y > 0 && y < TAM - 1; //verifica que la psoicion este dentro de los limites
}
// FUNCIÓN PARA GENERAR EL LABERINTO (DFS RECURSIVO)
void generarLaberinto(int x, int y) {

    laberinto[x][y] = CAMINO; // Marcamos la posicion actual como camino
    vector<int> direcciones = {0,1,2,3}; // Creamos un vector con las 4 direcciones posibles

    random_device rd; // creamos generador aleatorio
    mt19937 generador(rd()); // semilla 

    shuffle(direcciones.begin(), direcciones.end(), generador); // Mezclamos las direcciones para que sean aleatorios

    for (int i : direcciones) { // Recorremos cada direccion 
        int nx = x + dx[i]; // calculamos nueva posicion
        int ny = y + dy[i];

        if (esValido(nx, ny) && laberinto[nx][ny] == PARED) { // si la posicion es valida y todavia es pared
            laberinto[x + dx[i]/2][y + dy[i]/2] = CAMINO; // Eliminamos la pared intermedia
            generarLaberinto(nx, ny); // Llamamos recursivamente para seguir explorando
        }
    }
}

// FUNCIÓN PARA IMPRIMIR EL LABERINTO
void imprimirLaberinto() {
    for (int i = 0; i < TAM; i++) { // Recorremos filas

        for (int j = 0; j < TAM; j++) { // Recorremos columnas
            cout << laberinto[i][j] << " "; //imprimimos cada caracter
        }
        cout << endl; // salto de linea
    }
}
// FUNCIÓN PARA RESOLVER EL LABERINTO (BFS)
void resolverLaberinto() {

    vector<vector<bool>> visitado(TAM, vector<bool>(TAM, false)); // Matriz para saber que posicion ya visitamos

    vector<vector<pair<int,int>>> padre(TAM, vector<pair<int,int>>(TAM)); // Matriz para guardar el "padre" (de donde venimos)

    queue<pair<int,int>> cola; // Creamos cola

    cola.push({1,1}); // Insertamos la posicion inicial (1,1)

    visitado[1][1] = true; // Marcamos como visitado

    int movimientos[4][2] = {{-1,0},{1,0},{0,-1},{0,1}}; // Movimientos posibles (arriba, abajo, izquierda, derecha)

    while (!cola.empty()) { // Mientras la cola no este vacia

        pair<int,int> actual = cola.front(); // Obtenemos el primer elemento
        cola.pop();

        int x = actual.first;
        int y = actual.second;

        if (x == TAM-2 && y == TAM-2) // Si llegamos a la salida 
            break;

        
          // Revisamos los 4 movimientos
            for (int i = 0; i < 4; i++) {

                int nx = x + movimientos[i][0];
                int ny = y + movimientos[i][1];

                if (esValido(nx,ny) && !visitado[nx][ny] && // si es valido, no visitado y es camino libre
                    (laberinto[nx][ny] == CAMINO || laberinto[nx][ny] == SALIDA)) {

                    visitado[nx][ny] = true;           // Lo marcamos visitado
                    padre[nx][ny] = {x,y};             // Guardamos de dónde vino
                    cola.push({nx,ny});                // Lo agregamos a la cola
                }
        }
    }

    // Reconstrucción del camino desde la salida // investigar bien
    int x = TAM-2;
    int y = TAM-2;

    while (!(x == 1 && y == 1)) {

        if (laberinto[x][y] != SALIDA)
            laberinto[x][y] = SOLUCION;

        pair<int,int> p = padre[x][y];
        x = p.first;
        y = p.second;
    }
}

// FUNCIÓN PRINCIPAL
int main() {

    cout << "Ingrese el tamaño del laberinto (numero impar mayor o igual a 5): ";
    cin >> TAM; // Leemos desde teclado

    if (TAM < 5) TAM = 5; // Aseguramos tamaño minimo

    if (TAM % 2 == 0) TAM++; // Si es par, lo hacemos impar

    laberinto = vector<vector<char>>(TAM, vector<char>(TAM, PARED)); 
    // Inicializamos matriz llena de paredes

    generarLaberinto(1,1); // Generamos el laberinto desde (1,1)

    laberinto[1][1] = ENTRADA; 
    laberinto[TAM-2][TAM-2] = SALIDA;

    cout << "\n=== LABERINTO GENERADO ===\n\n";
    imprimirLaberinto();

    // Medición de tiempo
    auto inicio = chrono::high_resolution_clock::now(); 

    resolverLaberinto();

    auto fin = chrono::high_resolution_clock::now();

    chrono::duration<double> tiempo = fin - inicio;

    cout << "\n=== LABERINTO RESUELTO ===\n\n";
    imprimirLaberinto();

    cout << "\nTiempo de resolución: " 
         << tiempo.count() 
         << " segundos\n";

    return 0;
}

// para llamar a ejecutar g++ -std=c++17 laberinto.cpp -o laberinto
// g++ -std=c++17 laberinto.cpp -o laberinto && ./laberinto