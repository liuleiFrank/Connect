#ifndef MCTNODE_H_
#define MCTNODE_H_


class MCTNode{
public:
	int left,right,father;
	int x,y;
	bool isLeaf,isUser;
	double interest;//���棬�ݶ�ΪӮ�Ĵ���
	int totalRound;

	MCTNode(){
		interest = 0;
		totalRound = 0;
	}
};

#endif