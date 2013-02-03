#include "dl32Config.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_GENERAL_STRESS

#include "dl32Window.h"
#include "dl32Graphics.h"
#include "dl32Console.h"
#include "dl32Timing.h"

#define RANDOM(min,max) (rand()%(max-min))+min
#define RANDOM_COLOR COLOR_FromRGB(RANDOM(0,255),RANDOM(0,255),RANDOM(0,255))
#define RANDOM_POINT(minX,minY,maxX,maxY) dl32Point2D(RANDOM(minX,maxX),RANDOM(minY,maxY))

//Estructura que contiene las caracteristicas de un poligono regular:
typedef struct _POLYGONDATA
{
	dl32Point2D Center;//Centro
	float Radius;//Radio
	int Count;//Numero de lados
	dl32Color Color;//Color
	int Z;//Nivel Z para el render de dl32GraphicsClass
}POLYGONDATA;

struct TEXTDATA
{
	char* text;
	dl32Point2D position;
	dl32Color color;
};

void Render();
void DrawPolygon(dl32GraphicsClass* gfx,POLYGONDATA PolygonData,float Angle); //Dibuja un poligono regular con las caracteristicas especificadas
POLYGONDATA GetRandomPolygon(int WindowWidth,int WindowHeight,int MinRadius,int MaxRadius,int MaxCount,int Z=0);//Obtiene un poligono regular aleatorio dentro de los rangos especificados
void GetBoxTrapezoid(dl32VertexTrapezoid Trapezoid,float x, float y,float width, float height);//Obtiene un sprite rectangular con las caracteristicas especificadas
void GetRandomBoxTrapezoid(dl32VertexTrapezoid Trapezoid,int WindowWidth,int WindowHeight,int MinSize,int MaxSize); //Obtiene un sprite aleatorio dentro de los rangos especificados

const int WINDOWWIDTH=1440;//Ancho de la ventana
const int WINDOWHEIGHT=900;//Alto de la ventana

const int POLYCOUNT=1000;//Total de poligonos/sprites
const char* FILE1PATH="Texture.png";//Direccion de la imagen que usan los sprites
const char* FILE2PATH="MeshTexture.bmp";//Direccion de la imagen que usa la malla

//Variables globales wuahahaha!!!!
float Alfa=PI2+1; //Angulo de rotacion de poligonos/sprites
POLYGONDATA PDATA[POLYCOUNT];//Array de poligonos
dl32VertexTrapezoid Sprites[POLYCOUNT];//Array de sprites

int Texture1,Texture2,Font;

TEXTDATA texts[POLYCOUNT];

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
void OnMouseWheel(dl32MouseData MouseData);//Captura del evento "MouseWheel" de la ventana
void OnMove(dl32AABB2D WindowArea);//Captura del evento "Move" de la ventana
void OnKeyDown(dl32KeyboardData KeyboardData);//Captura del evento "KeyDown" de la ventana

void OnClose(bool *cancel);

bool DrawingSpline=false;
bool PolylineQueue=true;
const int POLYLINEQUEUESIZE = 100;
vector<dl32Point2D> nodes;
dl32Spline spline;
int selectedNode=0;

INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, INT )
{DL32TIMING_BEGIN

	dl32MeshPatch TransformationPatch(-1,3,3,3,3);
	Console.Open("dx_lib32 C++ (Debugging console)");
	Console.WriteLine("dx_lib32 setting up window...");

	try
	{
		Window = new dl32Window("dx_lib32 C++",0,0,WINDOWWIDTH,WINDOWHEIGHT);
		gfx = new dl32GraphicsClass(Window);

		Console << "Loading texture: '" << dl32String(FILE1PATH) << "'" << dl32endl;
		Texture1=gfx->MAP_Load(FILE1PATH);//Cargamos la imagen
		Console << "Loading texture: '" << dl32String(FILE2PATH) << "'" << dl32endl;
		Texture2=gfx->MAP_Load(FILE2PATH);//Cargamos la imagen

		Font=gfx->FONT_LoadSystemFont("Times New Roman",5,false,false);

		Console.WriteLine("Setting up event handlers...");

		Window->MouseMove.AddHandler(OnMouseMove);//Preparamos el manejador del evento MouseMove
		Window->MouseDown.AddHandler(OnMouseDown);//Preparamos el manejador del evento MouseDown

		Window->KeyDown.AddHandler(OnKeyDown);

		Window->Idle.AddHandler(Render);

		Window->Close.AddHandler(OnClose);

		Console.WriteLine("Setting up mesh patches (not used)");
		Patch=dl32MeshPatch(Texture1,0,0,10,10,DL32COLOR_WHITE);
		Mesh=dl32Mesh(dl32AABB2D(0,0,1024,768),10,10);

		Mesh.AddPatch(Patch);
		//Mesh.AddPatch(dl32MeshPatch(Texture1,5,0,5,10));

		Mesh.Transformation(dl32Transformation2D::Rotation(Mesh.GetPatchCenter(TransformationPatch),PI/4),TransformationPatch);

		Console.WriteLine("Starting message loop");

		for(float i=0;i<10*PI;i+=(PI/4))
			nodes.push_back(dl32Point2D(i*50,100+sin(float(i))*50));

		/*nodes.push_back(dl322DPoint(50,50));
		nodes.push_back(dl322DPoint(400,50));
		nodes.push_back(dl322DPoint(1000,100));*/

		dl32Window::Start();//Iniciamos el bucle de captura de mensajes
	}
	catch(dl32Exception ex)
	{
		MessageBox(GetDesktopWindow(),ex.what(),"dx_lib32 C++ - Error",MB_ICONERROR);
	}
	catch(dl32Direct3DInitFailedException)
	{
		MessageBox(GetDesktopWindow(),"ERROR: Direct3D cannot be inititialized","dx_lib32 C++ - Error",MB_ICONERROR);
	}
	catch(dl32WindowCreationFailedException)
	{
		MessageBox(GetDesktopWindow(),"ERROR: Cannot get HWND","dx_lib32 C++ - Error",MB_ICONERROR);
	}
	catch(dl32WindowClassRegistrationFailedException)
	{
		MessageBox(GetDesktopWindow(),"ERROR: Cannot register window class","dx_lib32 C++ - Error",MB_ICONERROR);
	}

	DL32MEMORY_SAFEDELETE(gfx);

	return 0;
DL32TIMING_END}

//ï¿½sta funciï¿½n se ejecuta automï¿½ticamente en cada ciclo de renderizado antes de dibujar la escena
void Render()
{DL32TIMING_BEGIN
	dl32Transformation2D Rotation;
	dl32Transformation2D InverseRotation;

	if(Alfa<=PI2) //Si no se ha completado la vuelta:
	{
		Alfa+=(PI/(1*gfx->FPS()));
		for(int i=0;i<POLYCOUNT;++i)
		{
			Rotation=dl32Transformation2D::Rotation(dl32Vertex::Baricenter(Sprites[i],4),Alfa); //Rotacion de alfa grados alrededor del centro de la textura
			InverseRotation=dl32Transformation2D::Rotation(dl32Vertex::Baricenter(Sprites[i],4),-Alfa); //Rotacion de -alfa grados alrededor del centro de la textura

			//Aplicamos la rotaciï¿½n al sprite: (Ver NOTA al principio si no se entiende la sintaxis)
			Rotation.Apply(&Sprites[i][0]);
			Rotation.Apply(&Sprites[i][1]);
			Rotation.Apply(&Sprites[i][2]);
			Rotation.Apply(&Sprites[i][3]);

			//Dibujamos el sprite:
			gfx->DRAW_VertexMap(Texture1,Sprites[i],1);
			//gfx->DRAW_Trapezoid(Sprites[i],false,2);

			//gfx->DRAW_Box(dl32AABB2D(100,100,300,300),DL32COLOR_DARKRED,true);

			//Devolvemos el sprite a su posición original: (Ver NOTA al principio si no se entiende la sintaxis)
			InverseRotation.Apply(&Sprites[i][0]);
			InverseRotation.Apply(&Sprites[i][1]);
			InverseRotation.Apply(&Sprites[i][2]);
			InverseRotation.Apply(&Sprites[i][3]);

			//Dibujamos el poligono actual:
			//DrawPolygon(gfx,PDATA[i],Alfa);
			//gfx->DRAW_Text(Font,texts[i].position,texts[i].text,texts[i].color);
		}
	}
	else
	{//Cuando se acaba la vuelta, alfa vuelve a cero, y se vuelven a generar nuevos poligonos/sprites:
		Alfa=0;
		for(int i=0;i<POLYCOUNT;++i)
		{
			GetRandomBoxTrapezoid(Sprites[i],WINDOWWIDTH,WINDOWHEIGHT,50,200);
			//PDATA[i]=GetRandomPolygon(WINDOWWIDTH,WINDOWHEIGHT,100,100,10);
			//texts[i].color=RANDOM_COLOR;
			//texts[i].position=RANDOM_POINT(0,0,WINDOWWIDTH,WINDOWHEIGHT);
			//texts[i].text="dx_lib32 C++ !!!";

			//Por supuesto, dibujamos. Si no, hay un ciclo de renderizado sin dibujar, lo que provoca parpadeos en la imagen
			//gfx->DRAW_VertexMap(Texture1,Sprites[i]);//Aquï¿½, el valor Z es uno, para que los sprites salgan delante de los poligonos
			//gfx->DRAW_Text(Font,texts[i].position,texts[i].text,texts[i].color);
			//DrawPolygon(gfx,PDATA[i],Alfa);
		}
	}

	Mesh.Transformation(dl32Transformation2D::Rotation(Mesh.GetMeshCenter(),PI/1000));
	//gfx->DRAW_Box(Window->GetClientArea(),COLOR_FromRGB(255,0,0),true,-4);
	gfx->DRAW_Mesh(Mesh,-5);


	gfx->DRAW_Text(Font,100,100,"dx_lib32" + dl32String(dl32endl) + "UPLEFT",COLOR_FromARGB(128,255,255,255),DL32TA_UPLEFT);
	gfx->DRAW_Box(98,98,4,4,DL32COLOR_RED,true);

	gfx->DRAW_Text(Font,100,130,"dx_lib32" + dl32String(dl32endl) + "UPRIGHT",COLOR_FromARGB(128,255,255,255),DL32TA_UPRIGHT);
	gfx->DRAW_Box(98,128,4,4,DL32COLOR_RED,true);

	gfx->DRAW_Text(Font,100,160,"dx_lib32" + dl32String(dl32endl) + "DOWNRIGHT",COLOR_FromARGB(128,255,255,255),DL32TA_DOWNRIGHT);
	gfx->DRAW_Box(98,158,4,4,DL32COLOR_RED,true);

	gfx->DRAW_Text(Font,100,190,"dx_lib32" + dl32String(dl32endl) + "DOWNLEFT",COLOR_FromARGB(128,255,255,255),DL32TA_DOWNLEFT);
	gfx->DRAW_Box(98,188,4,4,DL32COLOR_RED,true);

	gfx->DRAW_Text(Font,100,220,"dx_lib32" + dl32String(dl32endl) + "UP",COLOR_FromARGB(128,255,255,255),DL32TA_UP);
	gfx->DRAW_Box(98,218,4,4,DL32COLOR_RED,true);

	gfx->DRAW_Text(Font,100,250,"dx_lib32" + dl32String(dl32endl) + "DOWN",COLOR_FromARGB(128,255,255,255),DL32TA_DOWN);
	gfx->DRAW_Box(98,248,4,4,DL32COLOR_RED,true);

	gfx->DRAW_Text(Font,100,280,"dx_lib32" + dl32String(dl32endl) + "RIGHT",COLOR_FromARGB(128,255,255,255),DL32TA_RIGHT);
	gfx->DRAW_Box(98,278,4,4,DL32COLOR_RED,true);

	gfx->DRAW_Text(Font,100,310,"dx_lib32" + dl32String(dl32endl) + "LEFT",COLOR_FromARGB(128,255,255,255),DL32TA_LEFT);
	gfx->DRAW_Box(98,308,4,4,DL32COLOR_RED,true);

	gfx->DRAW_Text(Font,100,340,"dx_lib32" + dl32String(dl32endl) + "CENTER",COLOR_FromARGB(128,255,255,255),DL32TA_CENTER);
	gfx->DRAW_Box(98,338,4,4,DL32COLOR_RED,true);

	if(!DrawingSpline && nodes.size()>0)
		gfx->DRAW_Pixels(nodes.data(),DL32COLOR_GREEN,nodes.size());
	else
	{
		if(nodes.size()>=2)
		{
			//gfx->DRAW_Polyline(nodes.data(),nodes.size(),DL32COLOR_CYAN,10,false);
			gfx->DRAW_Spline(&spline,DL32COLOR_LIGHTGREEN);
			gfx->DRAW_Text(Font,10,10,"Selected node = " + dl32String(selectedNode),DL32COLOR_WHITE,DL32TA_UPLEFT);
		}
	}

	Window->SetText("dx_lib32 C++ (" + dl32String(gfx->FPS()) + " FPS)");

	gfx->Frame();
DL32TIMING_END}

void DrawPolygon(dl32GraphicsClass* gfx,POLYGONDATA PolygonData,float Angle)
{
	if(PolygonData.Count>=3)
	{
		dl32Point2D *Polygon=new dl32Point2D[PolygonData.Count]; //Aquï¿½ guardamos la geometria del poligono

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


		//dl322DPointTrapezoid obbVertex;
		//dl32OBB2D obb=dl32OBB2D(Polygon,PolygonData.Count);

		//obbVertex[0]=obb.GetUpLeftCorner();
		//obbVertex[1]=obb.GetUpRightCorner();
		//obbVertex[2]=obb.GetDownRightCorner();
		//obbVertex[3]=obb.GetDownLeftCorner();

		//gfx->DRAW_Polygon(obbVertex,4,DL32COLOR_GREEN,false);

		//Eliminamos el poligono:
		delete Polygon;
	}
}

POLYGONDATA GetRandomPolygon(int WindowWidth,int WindowHeight,int MinRadius,int MaxRadius,int MaxCount,int Z)
{
	POLYGONDATA RetVal;
	RetVal.Center=RANDOM_POINT(0,0,WindowWidth,WindowHeight);
	RetVal.Color=RANDOM_COLOR;
	RetVal.Radius=RANDOM(MinRadius,MaxRadius+1);
	RetVal.Count=RANDOM(3,MaxCount);
	RetVal.Z=Z;

	return RetVal;
}

void GetBoxTrapezoid(dl32VertexTrapezoid Trapezoid,float x, float y,float width, float height)
{
	//NOTA: El color difuso de los vertices del sprite es blanco, para que se vea la imagen original
	Trapezoid[0]=dl32Vertex(x,y,RANDOM_COLOR);
	Trapezoid[1]=dl32Vertex(x+width,y,RANDOM_COLOR);
	Trapezoid[2]=dl32Vertex(x+width,y+height,RANDOM_COLOR);
	Trapezoid[3]=dl32Vertex(x,y+height,RANDOM_COLOR);
}

void GetRandomBoxTrapezoid(dl32VertexTrapezoid Trapezoid,int WindowWidth,int WindowHeight,int MinSize,int MaxSize)
{
	GetBoxTrapezoid(Trapezoid,RANDOM(0,WindowWidth),RANDOM(0,WindowHeight),RANDOM(MinSize,MaxSize),RANDOM(MinSize,MaxSize));
}

void OnMouseMove(dl32MouseData MouseData)
{
	Window->SetText("dx_lib32 C++ (MouseMove!!!) {X=" + dl32String(MouseData.Location.x) + ",Y=" + dl32String(MouseData.Location.y) + "}");
	//gfx->Camera.SetPosition(MouseData.Location.x-(Window->GetWidth()/2),MouseData.Location.y-(Window->GetHeight()/2));

	if(DrawingSpline && MouseData.Button==DL32MOUSEBUTTON_LEFT)
	{
		nodes[selectedNode]=MouseData.Location;
		spline=dl32Spline(nodes.data(),nodes.size());
	}
	/*else if(PolylineQueue)
	{
		if(nodes.size()>=POLYLINEQUEUESIZE)
			nodes.erase(nodes.begin());
		
		if(nodes.size() == 0 ||dl32Vector2D(nodes[nodes.size()-1],MouseData.Location).GetLength() > 20)
			nodes.push_back(MouseData.Location);
	}*/

	/*if(true)
	{
	if(nodes.size()<100)
	nodes.push_back(MouseData.Location);
	else
	{
	nodes.erase(nodes.begin());
	nodes.push_back(MouseData.Location);
	}

	if(nodes.size()>=2) spline=dl322DSpline(nodes.data(),nodes.size());
	}*/
}

void OnMouseDown(dl32MouseData MouseData)
{
	Window->SetText("dx_lib32 C++ (MouseDown!!!)");

	switch(MouseData.Button)
	{
	case DL32MOUSEBUTTON_LEFT:
		Mesh.SetColor(DL32COLOR_RED,Patch);
		if(!DrawingSpline) nodes.push_back(MouseData.Location);
		break;
	case DL32MOUSEBUTTON_RIGHT:
		Mesh.SetColor(DL32COLOR_BLUE,Patch);
		break;
	case DL32MOUSEBUTTON_CENTER:
		Mesh.SetColor(DL32COLOR_WHITE,Patch);
		break;
	}
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
	case 'c':
		if(!DrawingSpline)
		{
			spline=dl32Spline(nodes.data(),nodes.size());
			DrawingSpline=true;
		}
		else
		{
			nodes.clear();
			DrawingSpline=false;
		}
		break;
	case '+':
		if((unsigned int)selectedNode<nodes.size()-1) selectedNode++;
		break;
	case '-':
		if(selectedNode>0) selectedNode--;
		break;
	case 'e':
		Window->DeleteWindow();
		break;
	case 'E':
		if(gfx->CAMERA_IsEnabled())
			gfx->CAMERA_Disable();
		else
			gfx->CAMERA_Enable();
	}
}

void OnClose(bool *cancel)
{
	*cancel=false;
}
#endif
