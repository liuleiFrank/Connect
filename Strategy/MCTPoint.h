#ifndef MCTNODE_H_
#define MCTNODE_H_

extern int m;
extern int n;
class MCTNode{
public:
	int **board;
	int *top;
	int left,right,father;
	int x,y;
	bool isLeaf,isUser;
	double interest;//收益，暂定为赢的次数
	int totalRound;

	MCTNode(){
		interest = 0;
		totalRound = 0;
		x = y = -1;		
		isLeaf = true;
	}
	void  ini(int **board,int* t,int lastX,int lastY){
		top = t;
		this->board = board;
		x = lastX;
		y = lastY;
	}
};

#endif
