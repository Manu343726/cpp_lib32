#include "dl32Console.h"

#ifndef DL32TIMING_H
#define DL32TIMING_H

struct dl32TimingFrame
{
	LARGE_INTEGER begin;
	LARGE_INTEGER end;
	double lapse;
	int depth;
	string function;

	dl32TimingFrame(char* function,LARGE_INTEGER begin, int depth){this->begin = begin; this->depth = depth; this->function = function;}
	void Finalize(LARGE_INTEGER end){this->end = end; lapse = getLapse();}

private:
	double getLapse();
};

class dl32TimingTreeNode
{
private:
	vector<dl32TimingTreeNode*> _subnodes;
	int _frame;
	dl32TimingTreeNode* _parent;
public: 
	dl32TimingTreeNode() : _parent(NULL) , _frame(-1) {}
	dl32TimingTreeNode(dl32TimingTreeNode* parent, int frame) : _parente(parent) , _frame(frame) {}
	~dl32TimingTreeNode();
	
	bool isRoot() const {return _parent == NULL;}
	bool isLeaf() const {return _subnodes.size() == 0;}
	int frame()   const {return _frame;}

	dl32TimingTreeNode* parent() const {return _parent;}

	bool isValid(){return _frame >= 0;}
	void clear(){_subnodes.clear(); _frame = 0; _parent = NULL;}

	void add(dl32TimingTreeNode* node){_subnodes.push_back(node);}
};

//NOTA: Ahora que lo veo igual debería haber hecho un singleton en vez de tanta mierda estática...
class dl32Timing
{
private:
	static dl32TimingTreeNode _root;
	static vector<int> _timingStack; 
	static vector<dl32TimingFrame> _frames;

	static dl32TimingTreeNode* _actual;
	
	dl32Timing(){}
public:
	static void push(char* function = "Unspecified function");
	static void pop();

	static dl32TimingFrame frame(int index){return _frames.at(index);}
	static vector<dl32TimingFrame> getFrames(){return _frames;}
	static dl32TimingTreeNode getTimingTree(){return _root;}
};

#if DL32DEBUG_TIMING
#define DL32TIMING_BEGIN dl32Timing::push(__FUNCTION__);
#define DL32TIMING_END dl32Timing::pop();
#else
#define DL32TIMING_BEGIN
#define DL32TIMING_END
#endif
#endif