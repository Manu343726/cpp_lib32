#include "dl32Preprocessor.h"

#if DL32DEBUG_DEBUGTEST == 1

#include "dl32Windows.h"
 
#define RANDOM(min,max) (rand()%(max-min))+min
#define RANDOM_COLOR COLOR_FromRGB(RANDOM(0,255),RANDOM(0,255),RANDOM(0,255))
#define RANDOM_POINT(minX,minY,maxX,maxY) dl322DPoint(RANDOM(minX,maxX),RANDOM(minY,maxY))

//Estructura que contiene las caracteristicas de un poligono regular:
typedef struct _POLYGONDATA
{
	dl322DPoint Center;//Centro
	float Radius;//Radio
	int Count;//Numero de lados
	dl32Color Color;//Color
	int Z;//Nivel Z para el render de dl32GraphicsClass
}POLYGONDATA;

void PreDrawProc(PTDL32GRAPHICSCLASS gfx); //Funcion que se ejecuta antes del dibujo de la escena
void DrawPolygon(PTDL32GRAPHICSCLASS gfx,POLYGONDATA PolygonData,float Angle); //Dibuja un poligono regular con las caracteristicas especificadas
POLYGONDATA GetRandomPolygon(int WindowWidth,int WindowHeight,int MinRadius,int MaxRadius,int MaxCount,int Z=0);//Obtiene un poligono regular aleatorio dentro de los rangos especificados
void GetBoxTrapezoid(dl32VertexTrapezoid Trapezoid,float x, float y,float width, float height);//Obtiene un sprite rectangular con las caracteristicas especificadas
void GetRandomBoxTrapezoid(dl32VertexTrapezoid Trapezoid,int WindowWidth,int WindowHeight,int MinSize,int MaxSize); //Obtiene un sprite aleatorio dentro de los rangos especificados

const int WINDOWWIDTH=1440;//Ancho de la ventana
const int WINDOWHEIGHT=900;//Alto de la ventana

const int POLYCOUNT=100;//Total de poligonos/sprites
const dl32String FILE1PATH="Texture.png";//Direccion de la imagen que usan los sprites
const dl32String FILE2PATH="MeshTexture.bmp";//Direccion de la imagen que usa la malla

//Variables globales wuahahaha!!!!
float Alfa=PI2+1; //Angulo de rotacion de poligonos/sprites
POLYGONDATA PDATA[POLYCOUNT];//Array de poligonos
dl32VertexTrapezoid Sprites[POLYCOUNT];//Array de sprites

int Texture1,Texture2;

dl32GraphicsClass *gfx;//Objeto de graficos
dl32Window *Window;//Ventana de la aplicación

dl32Mesh Mesh;
dl32MeshPatch Patch;
dl32MeshPatch Patches[4];

void OnIdle();//Captura del Evento "Idle" de la ventana (Está a la espera de mensajes)
void OnPaint();//Captura del evento "Paint" de la ventana (La ventana necesita repintarse)
void OnMouseMove(dl32MouseData MouseData);//Captura del evento "MouseMove" de la ventana
void OnMouseUp(dl32MouseData MouseData);//Captura del evento "MouseUp" de la ventana
void OnMouseDown(dl32MouseData MouseData);//Captura del evento "MouseDown" de la ventana
void OnMove(dl322DAABB WindowArea);//Captura del evento "Move" de la ventana
void OnKeyDown(dl32KeyboardData KeyboardData);//Captura del evento "KeyDown" de la ventana

INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
	dl32MeshPatch TransformPatch(-1,3,3,3,3);

	Window = new dl32Window("dx_lib32 C++",0,0,WINDOWWIDTH,WINDOWHEIGHT);
	if(Window->Ready())
	{
		gfx=Window->GetGraphics();

		switch(gfx->InitError())//dl32GraphicsClass contiene la funciï¿½n "InitError()" que devuelve un valor del tipo DL32GRAPHICSCLASS_CTORERRORS que indica el tipo de error que ocurrio durante la ejecuciï¿½n del constructor
		{
		case DL32GCCE_NOERRORS://Todo bien!!!
			gfx->DEVICE_SetPreDrawProc(PreDrawProc);//Asignamos la funciï¿½n que se ejecuta antes del dibujo de la escena

			Texture1=gfx->MAP_Load(FILE1PATH);//Cargamos la imagen
			Texture2=gfx->MAP_Load(FILE2PATH);//Cargamos la imagen

			Window->MouseMove.AddHandler(OnMouseMove);//Preparamos el manejador del evento MouseMove
			Window->MouseUp.AddHandler(OnMouseUp);//Preparamos el manejador del evento MouseUp
			Window->MouseDown.AddHandler(OnMouseDown);//Preparamos el manejador del evento MouseDown
			
			Window->Paint.AddHandler(OnPaint);//Preparamos el manejador del evento Paint
			Window->Idle.AddHandler(OnIdle);//Preparamos el manejador del evento Idle
			Window->Move.AddHandler(OnMove);

			Window->KeyDown.AddHandler(OnKeyDown);

			Patch=dl32MeshPatch(Texture1,0,0,10,10,DL32COLOR_WHITE);
			Mesh=dl32Mesh(dl322DAABB(0,0,1024,768),10,10);

			Mesh.AddPatch(Patch);
			//Mesh.AddPatch(dl32MeshPatch(Texture1,5,0,5,10));
			
			Mesh.Transform(dl322DTransform::Rotation(Mesh.GetPatchCenter(TransformPatch),PI/4),TransformPatch);


			dl32Window::Start();//Iniciamos el bucle de captura de mensajes
			break;
		case DL32GCCE_D3DINITFAILED://La inicializaciï¿½n de Direct3D a fallado (Vete tu a saber por quï¿½)
			MessageBox(Window->GetHwnd(),"ERROR: DL32GCCE_D3DINITFAILED","dx_lib32 C++ - dl32GraphicsClass CTOR ERROR",MB_ICONERROR);
			break;
		}
	}

	delete Window;

	return 0;
}

//ï¿½sta funciï¿½n se ejecuta automï¿½ticamente en cada ciclo de renderizado antes de dibujar la escena
void PreDrawProc(PTDL32GRAPHICSCLASS gfx)
{
	dl322DTransform Rotation;
	dl322DTransform InverseRotation;

	if(Alfa<=PI2) //Si no se ha completado la vuelta:
	{
		Alfa+=(PI/100);
		for(int i=0;i<POLYCOUNT;++i)
		{
			Rotation=dl322DTransform::Rotation(dl32Vertex::Baricenter(Sprites[i],4),Alfa); //Rotacion de alfa grados alrededor del centro de la textura
			InverseRotation=dl322DTransform::Rotation(dl32Vertex::Baricenter(Sprites[i],4),-Alfa); //Rotacion de -alfa grados alrededor del centro de la textura

			//Aplicamos la rotaciï¿½n al sprite: (Ver NOTA al principio si no se entiende la sintaxis)
			Rotation.Apply(&Sprites[i][0]);
			Rotation.Apply(&Sprites[i][1]);
			Rotation.Apply(&Sprites[i][2]);
			Rotation.Apply(&Sprites[i][3]);

			//Dibujamos el sprite:
			gfx->DRAW_VertexMap(Texture1,Sprites[i],1);

			//Devolvemos el sprite a su posición original: (Ver NOTA al principio si no se entiende la sintaxis)
			InverseRotation.Apply(&Sprites[i][0]);
			InverseRotation.Apply(&Sprites[i][1]);
			InverseRotation.Apply(&Sprites[i][2]);
			InverseRotation.Apply(&Sprites[i][3]);

			//Dibujamos el poligono actual:
			DrawPolygon(gfx,PDATA[i],Alfa);
		}
	}
	else
	{//Cuando se acaba la vuelta, alfa vuelve a cero, y se vuelven a generar nuevos poligonos/sprites:
		Alfa=0;
		for(int i=0;i<POLYCOUNT;++i)
		{
			GetRandomBoxTrapezoid(Sprites[i],WINDOWWIDTH,WINDOWHEIGHT,50,200);
			PDATA[i]=GetRandomPolygon(WINDOWWIDTH,WINDOWHEIGHT,10,100,10);
			
			//Por supuesto, dibujamos. Si no, hay un ciclo de renderizado sin dibujar, lo que provoca parpadeos en la imagen
			gfx->DRAW_VertexMap(Texture1,Sprites[i]);//Aquï¿½, el valor Z es uno, para que los sprites salgan delante de los poligonos
			//DrawPolygon(gfx,PDATA[i],Alfa);
		}
	}

	Mesh.Transform(dl322DTransform::Rotation(Mesh.GetMeshCenter(),PI/1000));
	//gfx->DRAW_Box(Window->GetClientArea(),COLOR_FromRGB(255,0,0),true,-2);
	//gfx->DRAW_Mesh(Mesh,5);
}

void DrawPolygon(PTDL32GRAPHICSCLASS gfx,POLYGONDATA PolygonData,float Angle)
{
	if(PolygonData.Count>=3)
	{
		dl322DPoint *Polygon=new dl322DPoint[PolygonData.Count]; //Aquï¿½ guardamos la geometria del poligono

		//Mï¿½todo para dibujar poligonos regulares (No era el mï¿½todo de Euclides???):
		////////////////////////////////////////////////////////////////////////////////

		//Calculamos el angulo entre vertice y vertice:
		const double ALFA=PI2/PolygonData.Count;

		for(int i=0;i<PolygonData.Count;++i)
		{
			//Las coordenadas del vertice son las razones trigonometricas del angulo que le corresponde multiplicadas por el radio (Trigonometria basica)
			Polygon[i].x=cos(Angle+ALFA*i)*PolygonData.Radius + PolygonData.Center.x; //NOTA: Angle+(ALFA*i): ALFA*i es el angulo que le corresponde al vertice, y Angle el angulo de rotacion que aplicamos al poligono
			Polygon[i].y=sin(Angle+ALFA*i)*PolygonData.Radius + PolygonData.Center.y;//NOTA: Se suman las coordenadas del centro, ya que ï¿½ste mï¿½todo generea el poligono centrado en el origen de coordenadas
		}

		//Dibujamos el poligono:
		gfx->DRAW_Polygon(Polygon,PolygonData.Count,PolygonData.Color,true,PolygonData.Z);

		//Eliminamos el poligono:
		delete Polygon;
	}
}

POLYGONDATA GetRandomPolygon(int WindowWidth,int WindowHeight,int MinRadius,int MaxRadius,int MaxCount,int Z)
{
	POLYGONDATA RetVal;
	RetVal.Center=RANDOM_POINT(0,0,WindowWidth,WindowHeight);
	RetVal.Color=RANDOM_COLOR;
	RetVal.Radius=RANDOM(MinRadius,MaxRadius);
	RetVal.Count=RANDOM(3,MaxCount);
	RetVal.Z=Z;

	return RetVal;
}

void GetBoxTrapezoid(dl32VertexTrapezoid Trapezoid,float x, float y,float width, float height)
{
	//NOTA: El color difuso de los vertices del sprite es blanco, para que se vea la imagen original
	Trapezoid[0]=dl32Vertex(x,y,COLOR_FromRGB(255,255,255));
	Trapezoid[1]=dl32Vertex(x+width,y,COLOR_FromRGB(255,255,255));
	Trapezoid[2]=dl32Vertex(x+width,y+width,COLOR_FromRGB(255,255,255));
	Trapezoid[3]=dl32Vertex(x,y+width,COLOR_FromRGB(255,255,255));
}

void GetRandomBoxTrapezoid(dl32VertexTrapezoid Trapezoid,int WindowWidth,int WindowHeight,int MinSize,int MaxSize)
{
	GetBoxTrapezoid(Trapezoid,RANDOM(0,WindowWidth),RANDOM(0,WindowHeight),RANDOM(MinSize,MaxSize),RANDOM(MinSize,MaxSize));
}

void OnPaint()
{
	Window->SetText("dx_lib32 C++ (Paint!!!)");
}

void OnIdle()
{
	Window->SetText("dx_lib32 C++ (Idle!!!)");
}

void OnMouseMove(dl32MouseData MouseData)
{
	Window->SetText("dx_lib32 C++ (MouseMove!!!) {X=" + dl32String(MouseData.Location.x) + ",Y=" + dl32String(MouseData.Location.y) + "}");
	gfx->Camera.SetPosition(MouseData.Location.x-(Window->GetWidth()/2),MouseData.Location.y-(Window->GetHeight()/2));
}

void OnMouseUp(dl32MouseData MouseData)
{
	Window->SetText("dx_lib32 C++ (MouseUp!!!)");	
}

void OnMouseDown(dl32MouseData MouseData)
{
	Window->SetText("dx_lib32 C++ (MouseDown!!!)");

	switch(MouseData.Button)
	{
	case DL32MOUSEBUTTON_LEFT:
		Mesh.SetColor(DL32COLOR_RED,Patch);
		break;
	case DL32MOUSEBUTTON_RIGHT:
		Mesh.SetColor(DL32COLOR_BLUE,Patch);
		break;
	case DL32MOUSEBUTTON_CENTER:
		Mesh.SetColor(DL32COLOR_WHITE,Patch);
		break;
	}
}

void OnMove(dl322DAABB WindowArea)
{
	Window->SetText("dx_lib32 C++ (Move!!!)");
}

void OnKeyDown(dl32KeyboardData KeyboardData)
{
	if(!KeyboardData.PreviousPressed)
		Window->SetText("dx_lib32 C++ (KeyDown!!! [NO PREV PRESSED]) {'" + dl32String(KeyboardData.Key) + "'}");
	else
		Window->SetText("dx_lib32 C++ (KeyDown!!! [PREV PRESSED]) {'" + dl32String(KeyboardData.Key) + "'}");

	switch(KeyboardData.Key)
	{
	case 'Q':
	case 'q':
		dl32Window::Exit();
		break;
	case 'z':
		gfx->Camera.Rotate(gfx->Camera.GetPosition(),PI/100);
		break;
	case 'x':
		gfx->Camera.Rotate(gfx->Camera.GetPosition(),-PI/100);
		break;
	case 'a':
		gfx->Camera.Traslate(-10,0);
		break;
	case 'd':
		gfx->Camera.Traslate(10,0);
		break;
	case 'w':
		gfx->Camera.Traslate(0,-10);
		break;
	case 's':
		gfx->Camera.Traslate(0,10);
		break;
	case ' ':
		gfx->Camera=dl322DCamera();
		break;
	}
}
#endif
