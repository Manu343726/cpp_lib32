float MATH_Mul(dl322DVector V1, dl322DVector V2){return V1.x*V2.x+V1.y*V2.y;};
dl322DPoint MATH_Div(dl322DPoint Point,float Divisor);

float MATH_CrossProduct(dl322DPoint P1, dl322DPoint P2, dl322DPoint P3){return (MATH_Mul(dl322DVector(P1,P2),dl322DVector(P2,P3)));};
float MATH_GetAngle(dl322DVector V1, dl322DVector V2){return (acos(MATH_Mul(V1,V2)/(V1.GetLenght()*V2.GetLenght())));};
dl322DPoint MATH_GetBaricenter(vector<dl322DPoint> &PointCloud);

dl322DPoint operator+(dl322DPoint P1, dl322DPoint P2){return MATH_Add(P1,P2);};
dl322DPoint operator-(dl322DPoint Point){return dl322DPoint(-Point.x,-Point.y);};
dl322DVector operator-(dl322DVector Vector){return dl322DVector(-Vector.x,-Vector.y);};
dl322DPoint operator-(dl322DPoint P1, dl322DPoint P2){return MATH_Sub(P1,P2);};
dl322DPoint operator*(dl322DPoint Point, float Factor){return MATH_Mul(Point,Factor);};
dl322DPoint operator*(float Factor,dl322DPoint Point){return MATH_Mul(Point,Factor);}; //Se repite la sobrecarga para permitir la asociatividad
dl322DPoint operator/(dl322DPoint Point, float Divisor){return MATH_Div(Point,Divisor);};

float operator*(dl322DVector V1, dl322DVector V2){return MATH_Mul(V1,V2);}; //MATH_Mul vectorial
float operator^(dl322DVector V1, dl322DVector V2){return MATH_GetAngle(V1,V2);}; //MATH_GetAngle

bool operator==(dl322DPoint P1, dl322DPoint P2){return (P1.x==P2.x && P1.y==P2.y);};
bool operator!=(dl322DPoint P1, dl322DPoint P2){return !(P1==P2);};



bool MATH_Collide(dl322DAABB A1, dl322DAABB A2);
dl322DOrientation MATH_MATH_GetOrientation(dl322DAABB Origin, dl322DAABB AABB);

bool operator& (dl322DAABB A1, dl322DAABB A2){return MATH_Collide(A1,A2);}; //Indica si hay colision
bool operator& (dl322DAABB AABB, dl322DPoint Point){return AABB.BelongTo(Point);}; //Indica si el Point pertenece al espacio de la caja
bool operator& (dl322DPoint Point, dl322DAABB AABB){return AABB.BelongTo(Point);}; //Indica si el Point pertenece al espacio de la caja



const int DL323X3MATRIX_SIZE=sizeof(dl323x3Matrix);

dl323x3Matrix MATH_Get3x3Unity();
dl323x3Matrix MATH_Add(dl323x3Matrix m1,dl323x3Matrix m2);
dl323x3Matrix MATH_Sub(dl323x3Matrix m1,dl323x3Matrix m2);
dl323x3Matrix MATH_Mul(dl323x3Matrix m1,dl323x3Matrix m2);
dl323x3Matrix MATH_Mul(dl323x3Matrix matrix,float mul);
float MATH_GetDeterminant(dl323x3Matrix matrix);
dl323x3Matrix operator+(dl323x3Matrix m1,dl323x3Matrix m2){return MATH_Add(m1,m2);};
dl323x3Matrix operator-(dl323x3Matrix m1,dl323x3Matrix m2){return MATH_Sub(m1,m2);};
dl323x3Matrix operator*(dl323x3Matrix m1,dl323x3Matrix m2){return MATH_Mul(m1,m2);};
dl323x3Matrix operator*(dl323x3Matrix matrix,float mul){return MATH_Mul(matrix,mul);};

dl323DPoint MATH_Add(dl323DPoint P1, dl323DPoint P2){return dl323DPoint(P1.x+P2.x,P1.y+P2.y,P1.z+P2.z);};
dl323DPoint MATH_Sub(dl323DPoint P1, dl323DPoint P2){return dl323DPoint(P1.x+P2.x,P1.y+P2.y,P1.z+P2.z);};
dl323DPoint MATH_Mul(dl323DPoint Point,float Factor){return dl323DPoint(Point.x*Factor,Point.y*Factor,Point.z*Factor);};
float MATH_Mul(dl323DVector V1, dl323DVector V2){return V1.x*V2.x+V1.y*V2.y+V1.z*V2.z;};
dl323DVector MATH_VectorialMul(dl323DVector V1,dl323DVector V2);
dl323DPoint MATH_Div(dl323DPoint Point,float Divisor);

float MATH_CrossProduct(dl323DPoint P1, dl323DPoint P2, dl323DPoint P3);
float MATH_GetAngle(dl323DVector V1, dl323DVector V2){return acos(MATH_Mul(V1,V2)/(V1.GetLenght()*V2.GetLenght()));};
dl323DPoint MATH_GetBaricenter(vector<dl323DPoint> &PointCloud){return dl323DPoint();};

dl323DPoint operator+(dl323DPoint P1, dl323DPoint P2){return MATH_Add(P1,P2);};
dl323DPoint operator-(dl323DPoint Point){return dl323DPoint(-Point.x,-Point.y,-Point.z);};
dl323DVector operator-(dl323DVector Vector){return dl323DVector(-Vector.x,-Vector.y,-Vector.z);};
dl323DPoint operator-(dl323DPoint P1, dl323DPoint P2){return MATH_Sub(P1,P2);};
dl323DPoint operator*(dl323DPoint Point, float Factor){return MATH_Mul(Point,Factor);};
dl323DPoint operator*(float Factor,dl323DPoint Point){return MATH_Mul(Point,Factor);}; //Se repite la sobrecarga para permitir la asociatividad
dl323DPoint operator/(dl323DPoint Point, float Divisor){return MATH_Div(Point,Divisor);};

float operator*(dl323DVector V1, dl323DVector V2){return MATH_Mul(V1,V2);}; //MATH_Mul vectorial
float operator^(dl323DVector V1, dl323DVector V2){return MATH_GetAngle(V1,V2);}; //MATH_GetAngle
