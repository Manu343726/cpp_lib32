#include "dl32Config.h"

#if DL32TESTS_CURRENTTEST == DL32TEST_POLYLINE_SIMPLETEST

#include "dl32Graphics.h"
#include "dl32Window.h"

dl32Window* window;
dl32GraphicsClass* gfx;
int Font;

vector<dl32Vertex> nodes;
const int QUEUESIZE = 50;
const int MINNODEDISTANCE = 50;

bool viewDebugData = false;
bool viewStrip = false;
bool drawfillStrip = false;

const float MAXPOLYLINEWIDTH = 100.0f;
const float MINPOLYLINEWIDTH = 10.0f;
const float POLYLINEWIDTHSTEP = 5.0f; 
float polylineWidth = MINPOLYLINEWIDTH;

int colorCounter = 0;

void OnIdle();
void OnKeyDown(dl32KeyboardData keyboardData);
void OnMouseMove(dl32MouseData mouseData);
void OnMouseWheel(dl32MouseData mouseData);

struct PolylineJoint
{
	dl32Point2D Up;
	dl32Point2D Down;

	PolylineJoint(){}

	PolylineJoint(dl32Point2D up, dl32Point2D down)
	{
		Up=up;
		Down=down;
	}
}

void CoputePolylineSegment(dl32Point2D P1, dl32Point2D P2,float width,dl322DPointTrapezoid out);
void DrawPolylineSegments(dl32Point2D points[],int pointCount,float width,dl32Color color);

void _ComputePolyline(dl32Point2D points[],int pointCount,float width,PolylineJoint* &out,bool DrawDebug = false);
void _ParseJointsToVertexBuffer(PolylineJoint joints[],int JointCount,dl32Point2D* &out);
void _ParseJointsToVertexBuffer(PolylineJoint joints[],dl32Vertex colors[],int JointCount,dl32Vertex* &out);
dl32Point2D* _vertexArrayToPoint2D(dl32Vertex vertexArray[],int Count);

INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
	window = new dl32Window("dx_lib32 C++ - Polyline Test",0,0,1024,768);

	try
	{
		gfx = new dl32GraphicsClass(window);
		Font = gfx->FONT_LoadSystemFont("Times New Roman",5,false,false);

		window->Idle.AddHandler(OnIdle);
		window->KeyDown.AddHandler(OnKeyDown);
		window->MouseMove.AddHandler(OnMouseMove);
		window->MouseWheel.AddHandler(OnMouseWheel);

		dl32Window::Start();

		DL32MEMORY_SAFEDELETE(gfx);
	}catch(dl32Direct3DInitFailedException)
	{
		MessageBox(GetDesktopWindow(),"ERROR: Direct3D cannot be inititialized","dx_lib32 C++ - Error",MB_ICONERROR);
	}
}

void OnIdle()
{
	PolylineJoint* joints;
	dl32Vertex* vertices;
	dl32Point2D* points = _vertexArrayToPoint2D(nodes.data(),nodes.size());

	gfx->DRAW_Lines(points,nodes.size(),DL32COLOR_BLUE);

	_ComputePolyline(points,nodes.size(),polylineWidth,joints,viewDebugData);
	_ParseJointsToVertexBuffer(joints,nodes.data(),nodes.size(),vertices);

	if(viewStrip) gfx->DRAW_TriangleStrip(vertices,nodes.size()*2,drawfillStrip);
	//DrawPolylineSegments(points,POINTCOUNT,WIDTH,DL32COLOR_GREEN);

	DL32MEMORY_SAFEDELETE_ARRAY(joints);
	DL32MEMORY_SAFEDELETE_ARRAY(vertices);
	DL32MEMORY_SAFEDELETE_ARRAY(points);

	gfx->Frame();

	window->SetText("dx_lib32 C++ - Polyline Test (" + (dl32String)gfx->FPS() + " FPS)");
}

void DrawPolylineSegments(dl32Point2D points[],int pointCount,float width,dl32Color color)
{
	dl322DPointTrapezoid trapezoid;

	for(int i=0;i<pointCount-1;++i)
	{
		CoputePolylineSegment(points[i],points[i+1],width,trapezoid);
		gfx->DRAW_Trapezoid(trapezoid,color,false);
	}
}

void CoputePolylineSegment(dl32Point2D P1, dl32Point2D P2,float width,dl322DPointTrapezoid out)
{
	dl32Line2D p1p2(P1,P2);
	dl32Vector2D normal = p1p2.GetDirection().GetNormal();
	dl32Line2D normalp1(P1,normal),normalp2(P2,normal);

	out[0] = normalp1.GetPointByParameter(width);//Arriba izquierda
	out[1] = normalp2.GetPointByParameter(width);//Arriba derecha
	out[2] = normalp2.GetPointByParameter(-width);//Abajo derecha
	out[3] = normalp1.GetPointByParameter(-width);//Abajo izquierda
}


void _ComputePolyline(dl32Point2D points[],int pointCount,float width,PolylineJoint* &out,bool DrawDebug)
{
	dl32Line2D supi,supi_1,infi,infi_1;//Recta del borde superior i, recta del borde superior i-1, recta del borde inferior i, recta del borde inferior i-1
	dl32Line2D jointLine;//Recta de la articulación
	dl32Vector2D direction;//Direccion del segmento
	dl32Point2D upcenter,downcenter;//Centro del borde superior, centro del borde inferior
	dl32Line2D centerLine;//Linea perpendicular al segmento por su centro

	PolylineJoint* joints = new PolylineJoint[pointCount]; //Array de joints (Lo que queremos calcular)

	for(int i=0; i<pointCount;++i)
	{
		if(i==0)//Caso 1: extremo inicial: El joint es un joint perpendicular normal
		{
			direction = dl32Vector2D(points[i],points[i+1]);
			jointLine = dl32Line2D(points[i],dl32Vector2D(points[i],points[i+1]).GetNormal()); //Recta perpendicular al segmento 
			joints[i] = PolylineJoint(jointLine.GetPointByParameter(width),
									  jointLine.GetPointByParameter(-width));

			centerLine = dl32Line2D((points[i]+points[i+1])/2,direction.GetNormal()); //Recta perpendicular al segmento (centrada en él)

			supi = dl32Line2D(centerLine.GetPointByParameter(width),direction);//Borde superior
			infi = dl32Line2D(centerLine.GetPointByParameter(-width),direction);//Borde inferior
		}
		else if(i<pointCount-1)//Caso 2: Caso general
		{
			direction = dl32Vector2D(points[i],points[i+1]);
			centerLine = dl32Line2D((points[i]+points[i+1])/2,direction.GetNormal()); //Recta perpendicular al segmento (centrada en él)

			supi = dl32Line2D(centerLine.GetPointByParameter(width),direction);//Borde superior
			infi = dl32Line2D(centerLine.GetPointByParameter(-width),direction);//Borde inferior

			if(dl32Line2D::AreParallel(supi,supi_1))//Si los segmentos contiguos al joint son paralelos, el joint tambien es un joint perpendicular normal
			{
				jointLine = dl32Line2D(points[i],centerLine.GetDirection());
				joints[i] = PolylineJoint(jointLine.GetPointByParameter(width),
									      jointLine.GetPointByParameter(-width));
			}
			else//Si no, los extremos del joint son las intersecciones entre los bordes de los segmentos contíguos:
			{
				joints[i] = PolylineJoint(dl32Line2D::Intersection(supi,supi_1,true),//Ya hemos chequeado antes el paralelismo
										  dl32Line2D::Intersection(infi,infi_1,true));//Ya hemos chequeado antes el paralelismo
			}
		}
		else//Caso 3:  extremo inicial: El joint es un joint perpendicular normal
		{
			direction = dl32Vector2D(points[i-1],points[i]);
			jointLine = dl32Line2D(points[i],direction.GetNormal()); //Recta perpendicular al segmento

			joints[i] = PolylineJoint(jointLine.GetPointByParameter(width),
									  jointLine.GetPointByParameter(-width));
		}

		if(DrawDebug)
		{
			gfx->DRAW_Line(joints[i].Up,joints[i].Down,DL32COLOR_PURPLE);

			gfx->DRAW_Box(joints[i].Up.x-5,joints[i].Up.y-5,10,10,DL32COLOR_BLUE);
			gfx->DRAW_Box(joints[i].Down.x-5,joints[i].Down.y-5,10,10,DL32COLOR_YELLOW);


			gfx->DRAW_Line(supi.GetPosition(),infi.GetPosition(),DL32COLOR_GREEN);

			gfx->DRAW_Line(supi.GetPointByParameter(-direction.GetLength()/2),
						   supi.GetPointByParameter(direction.GetLength()/2),
						   DL32COLOR_ORANGE);
			gfx->DRAW_Line(infi.GetPointByParameter(-direction.GetLength()/2),
						   infi.GetPointByParameter(direction.GetLength()/2),
						   DL32COLOR_ORANGE);

			gfx->DRAW_Text(Font,(joints[i].Up+joints[i].Down)/2,(dl32String)i,DL32COLOR_RED,DL32TA_CENTER);
			gfx->DRAW_Text(Font,points[i],(dl32String)i,DL32COLOR_WHITE,DL32TA_CENTER);
		}		

		supi_1 = supi;
		infi_1 = infi;
	}

	out = joints;
}

void _ParseJointsToVertexBuffer(PolylineJoint joints[],int JointCount,dl32Point2D* &out)
{
	out = new dl32Point2D[JointCount*2];

	for(int i=0;i<JointCount;++i)
	{
		out[i*2] = joints[i].Down;
		out[i*2+1] = joints[i].Up;
	}
}

void _ParseJointsToVertexBuffer(PolylineJoint joints[],dl32Vertex colors[],int JointCount,dl32Vertex* &out)
{
	out = new dl32Vertex[JointCount*2];

	for(int i=0;i<JointCount;++i)
	{
		out[i*2] = dl32Vertex(joints[i].Down,colors[i].color);
		out[i*2+1] = dl32Vertex(joints[i].Up,colors[i].color);
	}
}

void OnKeyDown(dl32KeyboardData KeyboardData)
{
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
		gfx->Camera=dl32Camera2D();
		break;
	case 'e':
		window->DeleteWindow();
		break;
	case 'v':
		viewStrip = !viewStrip;
		break;
	case 'D':
		viewDebugData = !viewDebugData;
		break;
	case 'f':
		drawfillStrip = !drawfillStrip;
		break;
	}
}

void OnMouseMove(dl32MouseData mouseData)
{
	if(nodes.size()>=QUEUESIZE)
			nodes.erase(nodes.begin());

	if(nodes.size() == 0 || dl32Vector2D(nodes.back(),mouseData.Location).GetLength() > MINNODEDISTANCE)
	{
		nodes.push_back(dl32Vertex(mouseData.Location,COLOR_RainbowGradient(colorCounter*DL32COLOR_MAXRAINBOWVALUE/QUEUESIZE)));
		
		colorCounter++;
		
		if(colorCounter > QUEUESIZE)
			colorCounter = 0;
	}
}

void OnMouseWheel(dl32MouseData mouseData)
{
	if(mouseData.Delta > 0)
	{
		if(polylineWidth <= (MAXPOLYLINEWIDTH - POLYLINEWIDTHSTEP))
			polylineWidth += POLYLINEWIDTHSTEP;
	}
	else if(mouseData.Delta < 0)
		if(polylineWidth >= (MINPOLYLINEWIDTH + POLYLINEWIDTHSTEP))
			polylineWidth -= POLYLINEWIDTHSTEP;
}

dl32Point2D* _vertexArrayToPoint2D(dl32Vertex vertexArray[],int Count)
{
	dl32Point2D* points = new dl32Point2D[Count];

	for(int i=0;i<Count;++i)
		points[i] = dl32Point2D(vertexArray[i].x,vertexArray[i].y);

	return points;
}

#endif