#ifndef MCTNODE_H_
#define MCTNODE_H_

extern int m;
extern int n;
class MCTNode{
public:
	int left,right,father;
	int x,y;
	bool isUser;
	double interest;//���棬�ݶ�ΪӮ�Ĵ���
	int totalRound;

	MCTNode(){
		interest = 0;
		totalRound = 0;
		x = y = -1;		
	}
};

#endif
