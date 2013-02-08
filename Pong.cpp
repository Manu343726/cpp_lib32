#include "dl32Config.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_LAURA_PONG_PSEUDOPLAYABLE
#include "dl32Window.h"
#include "dl32Graphics.h"
#include "dl32Console.h"

//MACROS: (De �sto no te preocupes, ya te lo explicar�. Cosas de C...
#define RANDOM_FLOAT(HI,LO) LO + (float)rand()/((float)RAND_MAX/(HI-LO))
#define RANDOM_INT(min,max) ((rand()%(max-min))+min)
#define RANDOM_COLOR COLOR_FromRGB(RANDOM(0,255),RANDOM_INT(0,255),RANDOM_INT(0,255))
#define RANDOM_POINT(minX,minY,maxX,maxY) dl32Point2D(RANDOM_FLOAT(minX,maxX),RANDOM_FLOAT(minY,maxY))

//Constantes del juego:
const float VIDASINICIALES=5;
const float VELOCIDADINICIAL=1;
const float AUMENTOVELOCIDAD=0;
const float ANCHOPALETA=30;
const float ALTOPALETA=300;

const bool DOBLEIA = true;

/*Leeme por favor!
Un AABB (Siglas en ingles de "Axis Aligned Bounding Box") es, como su nombre indica, una caja alineada con los ejes de coordenadas; que define un �rea del espacio.
Est� definida por sus coordenadas y sus dimensiones. M�s informaci�n: http://matesyvideojuegos.blogspot.com.es/

La estructura dl322DAABB representa un AABB bidimensional. Contiene sus coordenadas (Campo p�blico "Position",de tipo dl322DPoint), y sus dimensiones (Privadas).
Sus coordenadas se corresponden con las de su esquina superior izquierda.
M�todos principales:
- float GetWidth(): Obtiene el ancho del AABB
- float GetHeight(): Obtiene el alto del AABB
- void SetWidth(float Width): Establece el ancho del AABB
- void SetHeight(float Height): Establece el alto del AABB

- dl322DPoint GetCenter(): Obtiene el centro del AABB
- void SetCenter(dl322DPoint Center): Establece el centro del AABB

- bool BelongTo(dl322DPoint point): Devuelve verdadero si el punto especificado pertenece al AABB. Devuelve falso en caso contrario.
- bool Collide(dl322DAABB A1,dl322DAABB A2): Devuelve verdadero si los dos AABBs especificados colisionan. Devuelve falso en caso contrario. 
*/

//Definimos una bola como su area y su velocidad
struct Bola
{
	dl32AABB2D AABB;//El �rea de la bola.
	dl32Vector2D velocidad;
};

//Definimos a un jugador como el �rea de su paleta, su puntuaci�n, sus vidas, y un identificador
struct Jugador
{
	dl32AABB2D AABB;
	int puntuacion;
	int vidas;
	int ID;
	Jugador(){puntuacion=0;vidas=VIDASINICIALES;ID=0;};
	Jugador(dl32AABB2D AABB){this->AABB=AABB;puntuacion=0;vidas=VIDASINICIALES;};
};

/*
"Breve" introducci�n a la programaci�n dirigida por eventos!

La programaci�n dirigida por eventos es un paradigma de programaci�n en el cual el flujo de ejecuci�n del programa no est� determinado por el programador,
sino que depende de las acciones del usuario. M�s informaci�n: http://es.wikipedia.org/wiki/Programaci%C3%B3n_dirigida_por_eventos

En el caso de un lenguaje orientado a objetos, un objeto puede lanzar un evento; y otros objetos estar a la ecucha de �ste. Cuando el evento es lanzado,
los objetos que est�n a la escucha ejecutar�n la acci�n oportuna.
�sto es util para programas con interfaz gr�fica (En adelante GUI, de sus siglas en ingl�s "Graphical User Interface"),
en los que se utilizan los eventos para responder a la interacci�n con el usuario.

Por ejemplo: Un programa con GUI en un lenguaje orientado a objetos normalmente contiene un objeto ventana (Ya profundizaremos en �l m�s adelante) y �ste objeto
contiene varios objetos hijos, que se corresponden con los controles que forman su interfaz (Ya sea un bot�n, una barra de progreso, un cuadro de texto,etc).
Si el usuario pulsa un bot�n de la ventana, el objeto que representa dicho bot�n lanzar� un evento indicandolo. Para poder responder a dicho evento, es necesario
que nuestra aplicaci�n sea capaz de capturarlo. Los eventos se suelen capturar mediante funciones. Es decir, le indicamos al evento que cuando se lance, ejecute autom�ticamente
la funci�n que le indiquemos. Un ejemplo es la funci�n "OnMouseMove()" definida m�s adelante. �sta se ejecutar� autom�ticamente al mover el rat�n por encima de la ventana.

La clase dl32Window representa una ventana. Permite realizar las operaciones m�s importantes, ya sean cambiar su posici�n/tama�o, cambiar el t�tulo, dibujar en la ventana,etc.
Adem�s, los objetos de �sta clase contienen varios campos, que se corresponden con los eventos que la ventana puede lanzar.
�stos son los m�s importantes:
- MouseMove: Se lanza al mover el rat�n por la ventana. Tiene como par�metro una estructura del tipo dl32MouseData que contiene los datos del rat�n (Coordenadas, boton pulsado, y rotaci�n de la rueda)
- MouseDown: Se lanza al pulsar un bot�n del rat�n. Tambien tiene un par�metro dl32MouseData
- MouseUp: Se lanza al soltar un bot�n del rat�n. Tambien tiene un par�metro dl32MouseData
- KeyDown: Se lanza al pulsar una tecla. Contiene un par�metro del tipo dl32KeyboardData con los datos del teclado (Tecla pulsada, n�mero de pulsaciones anteriores, y si la tecla ya estaba pulsada antes)
NOTA: Cuando mantienes pulsada una tecla, el sistema avisa de que se ha pulsado la tecla una y otra vez, hasta que se deje de presionar. �sto provoca que el evento KeyDown se ejecute todo el
rato mientras tengas pulsada la tecla. Por ello, dl32KeyboardData contiene dos campos, "PreviousPressed" y "RepeatCount", que indican si la tecla ya estaba pulsada antes, y en ese caso el n�mero de pulsaciones capturadas.
- KeyUp: Se lanza al dejar de presionar una tecla. Aunque tenga un parametro dl32KeyboardData, los campos "PreviousPressed" y "RepeatCount" no guardan informaci�n util (No tienen sentido)

Como dec�a antes, para capturar un evento, es necesario indicar una funci�n (o m�s) que lo manejen. Para ello se llama a la funci�n "AddHandler()" del evento.
Por ejemplo: Window->MouseMove.AddHandler(OnMouseMove) A�adimos la funci�n "OnmouseMove()" a la lista de manejadores del evento "MouseMove" de la ventana.
*/

//NOTA: Por el tipo de implementaci�n que hice de la clase dl32Window, no es bueno usar variables (instancias) de la clase, mejor usar punteros:
dl32Window *Window;
dl32GraphicsClass *gfx;
int Fuente;
Bola bola;
Jugador Jugador1,Jugador2;
float velocidadJuego=VELOCIDADINICIAL;
bool FinJuego=false;

//FUNCIONES DEL JUEGO:
void InicializarJuego();
void Puntuar(int Ganador);
void Reestablecer();
void IA();
dl32Vector2D CalcularRebote(Bola bola,Jugador jugador);
bool TestColisionJugadores();

//MANEJADORES DE EVENTOS:
void OnMouseMove(dl32MouseData data);
void OnResize(dl32AABB2D area);
void OnDraw();

//HERRAMIENTAS VARIAS:
dl32String ToString(dl32Point2D point);//Devuelve la representaci�n en cadena de caracteres de un punto 2D

int AllocHook(int allocType, void* userData, size_t size, int blockType, long RequestNumber, const unsigned char* fileName, int lineNumber);

//�ste es el punto de entrada de una aplicaci�n gr�fica en Windows:
INT WINAPI wWinMain(HINSTANCE,HINSTANCE,LPWSTR,INT)
{
	Console.Open("PONG! (Debug)");
	Console << "Inicializando...";

	dl32LeakDebugger::Start();
	dl32LeakDebugger::SetHook(AllocHook);

	try
	{
		Window=new dl32Window("PONG!",0,0,1440,900);
		gfx=new dl32GraphicsClass(Window);

		Window->MouseMove.AddHandler(OnMouseMove);
		Window->Idle.AddHandler(OnDraw);

		//Cargamos la fuente que usaremos para pintar texto:
		Fuente=gfx->FONT_LoadSystemFont("Lucilda Console",5,false,false);

		//Inicializamos los elementos del juego (Jugadores y bola):
		InicializarJuego();

		Console.WriteLine("Listo. Iniciando bucle de mensajes");
		dl32Window::Start();

		Console.WriteLine("dl32Window MessageLoop stopped count: " + dl32String(dl32Window::DEBUG_GetMessageLoopStoppedCount()));
		Console.Close();
	}
	catch(dl32Direct3DInitFailedException)
	{
		MessageBox(Window->GetHwnd(),"ERROR: Direct3D cannot be inititialized","dx_lib32 C++ - Error",MB_ICONERROR);
	}

	DL32MEMORY_SAFEDELETE(gfx);

	if(dl32LeakDebugger::Check())
	{
		Console.WriteLine("MEMORY LEAK!!!");
		_CrtDumpMemoryLeaks();
	}
}

void InicializarJuego()
{DL32TIMING_BEGIN
	//Establecemos las dimensiones de la bola, de manera que es un cuadrado con lado el ancho de las paletas:
	bola.AABB.SetWidth(ANCHOPALETA);
	bola.AABB.SetHeight(ANCHOPALETA);
	Reestablecer();
	//Establecemos los identificadores de los jugadores:
	Jugador1.ID=1;
	Jugador2.ID=2;
DL32TIMING_END}

void OnDraw()
{DL32TIMING_BEGIN
	if(!TestColisionJugadores())
	{
		//dl322DOrientation representa la posici�n relativa entre dos AABBs 2D. Se supone que uno se usa como "origen", de manera que es como si tuvieramos una br�jula.
		//Es decir, uno hace de centro de la "br�jula", y la enumeraci�n indica donde est� el otro respecto a ese. Ten en cuenta que tienen que colisionar, si no devuelve OUT
		//Norte es arriba, sur abajo, y as�. Las compuestas (NorEste, SurOeste, etc) significan que est� en una esquina.

		//En �ste caso, usamos el area de la ventana como origen, para sacar la posici�n relativa de la bola:
		//Si no sale INSIDE significa que, o est� en un borde (y por tanto hay que hacerla rebotar), o se le ha escapado a alguno de los jugadores.
		//Como excepci�n est� el caso OUTSIDE, que significa que ha habido un error (La bola va tan rapido que sali� de la ventana antes de que detect�ramos el fallo de un jugador)

		switch(dl32AABB2D::Orientation(Window->GetClientArea(),bola.AABB))
		{
		case dl32Orientation2D::NORTH:
			//Le damos a la parte vertical de la velocidad sentido positivo, para que vuelva hacia abajo
			//Recuerdas las leyes de Snell de �ptica? Pues eso.
			bola.velocidad.y=abs(bola.velocidad.y);	Console.WriteLine("REBOTE (SUPERIOR): Velocidad " + ToString(bola.velocidad) + "(Posicion " + ToString(bola.AABB.GetCenter()) + ")",DL32CP_AQUA);
			break;
		case dl32Orientation2D::SOUTH:
			//Le damos a la parte vertical de la velocidad sentido negativo, para que vuelva hacia abajo
			//Recuerdas las leyes de Snell de �ptica? Pues eso.
			bola.velocidad.y=-abs(bola.velocidad.y); Console.WriteLine("REBOTE (INFERIOR): Velocidad " + ToString(bola.velocidad) + "(Posicion " + ToString(bola.AABB.GetCenter()) + ")",DL32CP_AQUA);
			break;
		case dl32Orientation2D::WEST:
		case dl32Orientation2D::NORTHWEST:
		case dl32Orientation2D::SOUTHWEST:
			Puntuar(2); Console.WriteLine("FALLO DE JUGADOR 1",DL32CP_LIGHTYELLOW);
			break;
		case dl32Orientation2D::EAST:
		case dl32Orientation2D::NORTHEAST:
		case dl32Orientation2D::SOUTHEAST:
			Puntuar(1); Console.WriteLine("FALLO DE JUGADOR 2",DL32CP_LIGHTYELLOW);
			break;
		case dl32Orientation2D::OUTSIDE:
			Console.WriteLine("PERDIDA DE BOLA (Pero que cojones ha pasado???",DL32CP_LIGHTRED);
			velocidadJuego=VELOCIDADINICIAL;
			Reestablecer();
		}
	}

	if(!FinJuego)
	{
		//La nueva posici�n de la bola es la actual m�s el vector velocidad:
		bola.AABB.SetCenter(bola.AABB.GetCenter()+bola.velocidad);

		//Ejecutamos la "IA":
		IA();

		//Dibujamos:
		gfx->DRAW_Box(bola.AABB,DL32COLOR_WHITE,true);
		gfx->DRAW_Box(Jugador1.AABB,DL32COLOR_WHITE,true);
		gfx->DRAW_Box(Jugador2.AABB,DL32COLOR_WHITE,true);

		//Escribimos las puntuaciones:
		gfx->DRAW_Text(Fuente,0,0,"Jugador 1: " + dl32String(Jugador1.puntuacion) + " puntos (" + dl32String(Jugador1.vidas) + " vidas)",DL32COLOR_WHITE,DL32TA_UPLEFT);
		gfx->DRAW_Text(Fuente,Window->GetClientArea().GetWidth(),0,"Jugador 2: " + dl32String(Jugador2.puntuacion) + " puntos (" + dl32String(Jugador2.vidas) + " vidas)",DL32COLOR_WHITE,DL32TA_UPRIGHT);

		Window->SetText("PONG! (" + dl32String(gfx->FPS()) + " FPS)");

		gfx->Frame();

		DL32TIMING_END
	}
	else
		dl32Window::Exit();DL32TIMING_END//Finalizamos la captura de mensajes ("eventos") para finalizar la aplicaci�n
};

bool TestColisionJugadores()
{DL32TIMING_BEGIN
	if(dl32AABB2D::Collide(bola.AABB,Jugador1.AABB))//Si la bola ha chocado con la paleta del jugador 1...
	{
		Console.Write("COLISION (JUGADOR 1): Vin " + ToString(bola.velocidad),DL32CP_PURPLE);
		bola.velocidad=CalcularRebote(bola,Jugador1);
		velocidadJuego+=AUMENTOVELOCIDAD;//Cada vez que la bola rebota en una paleta, aumentamos la velocidad de la bola para darle m�s gracia
		Jugador1.puntuacion++;
		Console.WriteLine(" Vout " + ToString(bola.velocidad),DL32CP_PURPLE);
	}
	else if(dl32AABB2D::Collide(bola.AABB,Jugador2.AABB))//Si la bola ha chocado con la paleta del jugador 2...
	{
		Console.Write("COLISION (JUGADOR 2): Vin " + ToString(bola.velocidad),DL32CP_PURPLE);
		bola.velocidad=CalcularRebote(bola,Jugador2);
		velocidadJuego+=AUMENTOVELOCIDAD;//Cada vez que la bola rebota en una paleta, aumentamos la velocidad de la bola para darle m�s gracia
		Jugador2.puntuacion++;
		Console.WriteLine(" Vout " + ToString(bola.velocidad),DL32CP_PURPLE);
	}
	else
		return false;//Si no, pues nada, devolvemos falso para calcular la posici�n relativa (Ver "OnDraw()")

	return true;
DL32TIMING_END}

//Calcula la velocidad de salida de la bola cuando rebota con una paleta:
//Para poder controlar la bola, dependiendo de con que zona de la paleta le demos, sale con un �ngulo diferente (M�ximo 45�)
dl32Vector2D CalcularRebote(Bola bola,Jugador jugador)
{DL32TIMING_BEGIN
	dl32Vector2D retorno;

	//�sto es pura trigonometr�a. Imagina el caso del jugador1: Hacemos un tri�ngulo rect�ngulo, un cateto horizontal positivo (retorno.x), otro vertical (retorno.y)
	//y la hipotenusa es la velocidad. El vertice del �ngulo rect�ngulo en el centro de la paleta.
	//Recapitulemos: Queremos que salga como m�ximo a 45� cuando damos con los extremos no? En ese caso, la distancia vertical de la bola al centro de 
	//la paleta ser� ALTOPALETA/2. Entonces, hacemos que el cateto horizontal valga ALTOPALETA/2,as� en los extremos los catetos ser�n iguales, y por tanto 45�:

	if(jugador.ID==1)
		retorno.x=ALTOPALETA/2;//Si es el jugador 1, la bola tiene que salir con sentido horizontal positivo para que rebote.
	else
		retorno.x=-ALTOPALETA/2;//Si es el jugador 2, la bola tiene que salir con sentido horizontal negativo para que rebote.


	//Recuerda: El cateto vertical es la distancia centro bola-centro paleta:
	retorno.y=bola.AABB.GetCenter().y-jugador.AABB.GetCenter().y;

	retorno.Normalize();//Normalizamos el vector...
	return retorno*velocidadJuego;//... y devolvemos un vector cuyo m�dulo es la velocidad actual de juego
DL32TIMING_END}

void Puntuar(int Ganador)
{DL32TIMING_BEGIN
	if(Ganador==1)
	{
		Jugador2.vidas--;
		if(Jugador2.vidas<=0)
		{
			FinJuego=true;
		}
		else
			Reestablecer();
	}
	else
	{
		Jugador1.vidas--;
		if(Jugador1.vidas<=0)
		{
			FinJuego=true;
		}
		else
			Reestablecer();
	}
DL32TIMING_END}

void Reestablecer()
{DL32TIMING_BEGIN
	//Movemos la bola al centro de la ventana, y las paletas a sus bordes y a media altura:
	bola.AABB.SetCenter(Window->GetClientArea().GetCenter());
	bola.velocidad.x=-velocidadJuego;
	bola.velocidad.y=0;

	Jugador1.AABB=dl32AABB2D(3,(Window->GetClientArea().GetHeight()-ALTOPALETA)/2,ANCHOPALETA,ALTOPALETA);
	Jugador2.AABB=dl32AABB2D(Window->GetClientArea().GetWidth()-ANCHOPALETA-3,(Window->GetClientArea().GetHeight()-ALTOPALETA)/2,ANCHOPALETA,ALTOPALETA);
DL32TIMING_END}

void IA()
{DL32TIMING_BEGIN
	if(bola.AABB.GetCenter().y>ALTOPALETA/2 && bola.AABB.GetCenter().y<Window->GetClientArea().GetHeight()-ALTOPALETA/2) //Si la paleta no se sale de la pantalla al moverla...
	{
		Jugador2.AABB.SetCenter(Jugador2.AABB.GetCenter().x,bola.AABB.GetCenter().y+RANDOM_FLOAT(-ANCHOPALETA,ANCHOPALETA));//...entonces la movemos de manera que su centro quede a la altura del cursor
		if(DOBLEIA) Jugador1.AABB.SetCenter(Jugador1.AABB.GetCenter().x,bola.AABB.GetCenter().y+RANDOM_FLOAT(-ANCHOPALETA,ANCHOPALETA));//Si DOBLEIA est� a true,
		//juega ordenador vs ordenador, y por tanto hacemos lo mismo con la paleta del jugador 1.
	}
DL32TIMING_END}

void OnMouseMove(dl32MouseData data)
{
	if(!DOBLEIA && data.Location.y>ALTOPALETA/2 && data.Location.y<Window->GetClientArea().GetHeight()-ALTOPALETA/2)//Si la paleta no se sale de la pantalla al moverla...
		Jugador1.AABB.SetCenter(Jugador1.AABB.GetCenter().x,data.Location.y);//...entonces la movemos de manera que su centro quede a la altura del cursor

	//NOTA: Si DOBLEIA est� a true, significa que juega ordenador vs ordenador, por eso no movemos la paleta
}

dl32String ToString(dl32Point2D point)
{
	return "{X=" + dl32String(point.x) + ",Y=" + dl32String(point.y) + "}";
}

int AllocHook(int allocType, void* userData, size_t size, int blockType, long RequestNumber, const unsigned char* fileName, int lineNumber)
{
	switch(allocType)
	{
	case _HOOK_ALLOC:
		Console.WriteLine("Memory allocation. File '" + dl32String(fileName) + "' line " + dl32String(lineNumber) + " (" + dl32String(userData) + ")");
	case _HOOK_REALLOC:
		Console.WriteLine("Memory reallocation. File '" + dl32String(fileName) + "' line " + dl32String(lineNumber) + " (" + dl32String(userData) + ")");
	case _HOOK_FREE:
		Console.WriteLine("Memory free. File '" + dl32String(fileName) + "' line " + dl32String(lineNumber) + " (" + dl32String(userData) + ")");
	}

	return TRUE;
}
#endif