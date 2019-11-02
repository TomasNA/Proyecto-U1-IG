#include <iostream>
#include <math.h>
#include <vector>
#include <time.h>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#endif

#define ANCHO 600
#define ALTURA 500

#define NUM_PALOS 4
#define NUM_VAL 13

using namespace std;

float altura;
int i = 0;
int b = 0;
int palo = 0;
int valor = 0;
int palos[NUM_PALOS * NUM_VAL];
int valores[NUM_PALOS * NUM_VAL];
bool paloRepetido = false;
bool valorRepetido = false;
int multiContador[] = {0, 0, 0, 0};
int posicionesTablero[28][3];
int contadorMazo = NUM_PALOS * NUM_VAL - 1;
int contador = 0;
int indice = 0;
int lim = 7;
bool inicio = false;

float refresh = 100.0;


class Carta{
	private:
		int palo;
		int valor;
		bool alReves;
	public:
		int posX;
		int posY;
		int posZ;
		Carta(){
			palo = -1;
			valor = -1;
			posX = 0;
			posY = 0;
			posZ = 0;
			alReves = true;
		}
		Carta(int p, int v, int x, int y, int z){
			palo = p;
			valor = v;
			posX = x;
			posY = y;
			posZ = z;
			alReves = true;
		}
		void dibujarCarta();
		void voltear();
		int getValor();
		int getPalo();
};

void Carta::dibujarCarta(){
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(posX,       posY,        posZ);
	glVertex3f(posX + 8.5, posY,        posZ);
	glVertex3f(posX + 8.5, posY + 13.5, posZ);
	glVertex3f(posX,       posY + 13.5, posZ);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glVertex3f(posX      , posY + 0.1, posZ);
	glVertex3f(posX + 8.3, posY + 0.1, posZ);
	glVertex3f(posX + 8.3, posY + 13.4, posZ);
	glVertex3f(posX      , posY + 13.4, posZ);
	glEnd();
}

void Carta::voltear(){
	if(alReves)
		alReves = false;
	else
		alReves = true;
}

int Carta::getValor(){
	return valor;
}

int Carta::getPalo(){
	return palo;
}

vector<Carta> cartas;
vector<Carta>::iterator cartasIterator; 
Carta mazo[NUM_PALOS * NUM_VAL - 28];

void dibujarCartas(){
	cartasIterator = cartas.begin();
	while(cartasIterator != cartas.end()){
		cartasIterator -> dibujarCarta();
		cartasIterator++;
	}
}

void writeBitmapString(void *font, char *string)
{  
   char *c;
   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

void windowInfo(){
	char valor[10];
	//sprintf(valor, "%f", alpha);
}

void generarMazo(){
	// 4 Palos: 1 - Corazones, 2 - Diamantes, 3 - Treboles, 4 - Picas
	// 13 Valores, 1 / As, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 / J, 12 / Q, 13 / K

	for(int z = 0; z < NUM_PALOS * NUM_VAL; z++){ // Limpiar arrays
		palos[z] = 0;
		valores[z] = 0;
	}

	while(i < NUM_PALOS * NUM_VAL){
		paloRepetido = false;

		if(!valorRepetido){ // Evitar que se cargue otro palo cuando se detecto un valor repetido
			palo = 1 + rand() % NUM_PALOS;
			
			// Un palo solo puede tener 13 cartas, por lo cual se debe solo repetir 13 veces
			// Si toca un palo con 13 cartas se debe ignorar y volver a empezar
			if(palo == 1){
				if(multiContador[0] == 13) paloRepetido = true;
				else multiContador[0]++;
			} else if(palo == 2){
				if(multiContador[1] == 13) paloRepetido = true;
				else multiContador[1]++;
			} else if(palo == 3){
				if(multiContador[2] == 13) paloRepetido = true;
				else multiContador[2]++;
			} else if(palo == 4){
				if(multiContador[3] == 13) paloRepetido = true; 
				else multiContador[3]++;
			}

			if(!paloRepetido) palos[i] = palo; // No es un palo repetido
		}

		if(!paloRepetido){ // Se tiene un palo no repetido
			valor = 1 + rand() % NUM_VAL;
			valorRepetido = false;
			

			if(b == 0){ // No se verifica el valor de la primera iteracion
				valores[i] = valor;
			} else{
				for(int rep = 0; valores[rep]; rep++){ // Se buscan valores repetidos en el mismo palo actual
					if(valores[rep] == valor && palos[rep] == palos[i]){
						valorRepetido = true; // Valor repetido
						break;
					}
				}

				if(!valorRepetido){ // No es un valor repetido en el mismo palo
					valores[i] = valor;
				}
			}

			if(!valorRepetido) i++; // Se tiene un palo y valor no generados, se pasa a la siguiente carta
		}
	}
}

void dibujarTablero(){
	indice = 0;
	contador = 0;
	lim = 7;

	// Dibujar cartas en el tablero
	for(int j = 1; j <= 7; j++){
		for(int k = 1; k <= 7; k++){
			cout << indice << " Tablero - Palo: " << palos[k - 1] << " Valor: " << valores[k - 1] << endl;

			posicionesTablero[indice][0] = (95 - 12 * k);
			posicionesTablero[indice][1] = (60 - 4 * j);
			posicionesTablero[indice][2] = (-0.1 * j);
			

			cartas.push_back(Carta(palos[k - 1], 			// Palo
					       valores[k - 1], 			// Valor
					       posicionesTablero[indice][0], 	// Posicion en X
					       posicionesTablero[indice][1],	// Posicion en Y
					       posicionesTablero[indice][2]));	// Posicion en Z
			contador++;
			indice++;

			if(contador == lim){
				lim--;
				contador = 0;
				break;
			}
		}
	}

	dibujarCartas();
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glVertex3f(mazo[51].posX, mazo[51].posY, mazo[51].posZ);
	glEnd();
}

void dibujarMazo(){
	// Dibujar Mazo
	for(int j = indice; j < NUM_PALOS * NUM_VAL; j++){
		mazo[j] = Carta(palos[j], valores[j], posicionesTablero[6][0], 80, 0);
		
		mazo[j].dibujarCarta();
		cout << j << " Mazo - Palo: " << palos[j] << " Valor: " << valores[j] 
		     << " x: " << mazo[j].posX << " y: " << mazo[j].posY << endl;
	}
}

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);
	glLoadIdentity();
	
	if(!inicio){
		dibujarTablero();
		inicio = true;
	} else{
		dibujarCartas();
	}

	dibujarMazo();

	glFlush();
}

// Initialization routine.
void setup(void) 
{
	glClearColor(0.0, 1.0, 0.0, 10.0);

	generarMazo();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 100, 0, 100, -1, 1);

	altura = h;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Timer(int value) {
	
	glutPostRedisplay();
	glutTimerFunc(refresh, Timer, 0);
}

void mouseControl(int button, int state, int x, int y){
	//cout << contadorMazo << " objetivo: " << mazo[contadorMazo].posX << " " << mazo[contadorMazo].posY << endl;
	//cout << x << " " << (x / 6) << " " << y << " " << altura / 5 - y / 5 << endl;

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		if(x / 6 >= mazo[contadorMazo].posX && x / 6 <= mazo[contadorMazo].posX + 8.5 
		&& altura / 5 - y / 5 >= mazo[contadorMazo].posY && altura / 5 - y / 5 <= mazo[contadorMazo].posY + 13.5) {
			cout << "Carta seleccionada: " << mazo[contadorMazo].getPalo() 
				<< " " << mazo[contadorMazo].getValor() << endl;
			
			mazo[contadorMazo].voltear();
			mazo[contadorMazo].posX += 10;
			
			mazo[contadorMazo].dibujarCarta();
			contadorMazo--;
			
			if(contadorMazo < 28){
				contadorMazo = 51;
				drawScene();
			}

			glFlush();
		}
	}
}

void keyInput(unsigned char key, int x, int y)
{
	switch(key) 
	{
		case 27:
			exit(0);
			break;
		default:
			break;
	}
}


void printInteraction(){
	cout << "Triangle centers" << endl;
	cout << "Author: Oscar Francisco Flores Gallegos" << endl;
	cout << "Use space key for switch the diagrams" << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
	srand(time(NULL));
	glutInit(&argc, argv);

	glutInitContextVersion(2, 1); 
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); 
	glutInitWindowSize(ANCHO, ALTURA);
	glutInitWindowPosition(0, 0); 
	glutCreateWindow("Proyect");
	glutDisplayFunc(drawScene); 
	glutReshapeFunc(resize);  
	glutMouseFunc(mouseControl);
	//glutKeyboardFunc(keyInput);
	//glutTimerFunc(0, Timer, 0);
	glewInit(); 
	//printInteraction();

	setup(); 

	glutMainLoop();   	
}
