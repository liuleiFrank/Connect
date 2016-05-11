#include <iostream>
#include "Point.h"
#include "Strategy.h"
#include "Judge.h"
#include "MCTPoint.h"
#include <ctime>
#include <cmath>
#include <conio.h>
#include <atlstr.h>
const int MAXTREE = 25000000;
const double epsilon = 1e-6;
MCTNode nodes[MAXTREE];
int m,n;//�����ȺͿ������Ϊȫ�ֱ������ڵ���
int nox,noy;
int **temp;//TreePolicy ��default�е���ģ��ʹ��
int **Temp;
int *TOP;
int* Top;
int index;
using namespace std;

/*
	���Ժ����ӿ�,�ú������Կ�ƽ̨����,ÿ�δ��뵱ǰ״̬,Ҫ�����������ӵ�,�����ӵ������һ��������Ϸ��������ӵ�,��Ȼ�Կ�ƽ̨��ֱ����Ϊ��ĳ�������
	
	input:
		Ϊ�˷�ֹ�ԶԿ�ƽ̨ά����������ɸ��ģ����д���Ĳ�����Ϊconst����
		M, N : ���̴�С M - ���� N - ���� ����0��ʼ�ƣ� ���Ͻ�Ϊ����ԭ�㣬����x��ǣ�����y���
		top : ��ǰ����ÿһ���ж���ʵ��λ��. e.g. ��i��Ϊ��,��_top[i] == M, ��i������,��_top[i] == 0
		_board : ���̵�һά�����ʾ, Ϊ�˷���ʹ�ã��ڸú����տ�ʼ���������Ѿ�����ת��Ϊ�˶�ά����board
				��ֻ��ֱ��ʹ��board���ɣ����Ͻ�Ϊ����ԭ�㣬�����[0][0]��ʼ��(����[1][1])
				board[x][y]��ʾ��x�С���y�еĵ�(��0��ʼ��)
				board[x][y] == 0/1/2 �ֱ��Ӧ(x,y)�� ������/���û�����/�г������,�������ӵ㴦��ֵҲΪ0
		lastX, lastY : �Է���һ�����ӵ�λ��, ����ܲ���Ҫ�ò�����Ҳ������Ҫ�Ĳ������ǶԷ�һ����
				����λ�ã���ʱ��������Լ��ĳ����м�¼�Է������ಽ������λ�ã�����ȫȡ�������Լ��Ĳ���
		noX, noY : �����ϵĲ������ӵ�(ע:��ʵ���������top�Ѿ����㴦���˲������ӵ㣬Ҳ����˵���ĳһ��
				������ӵ�����ǡ�ǲ������ӵ㣬��ôUI�����еĴ�����Ѿ������е�topֵ�ֽ�����һ�μ�һ������
				��������Ĵ�����Ҳ���Ը�����ʹ��noX��noY��������������ȫ��Ϊtop������ǵ�ǰÿ�еĶ�������,
				��Ȼ�������ʹ��lastX,lastY�������п��ܾ�Ҫͬʱ����noX��noY��)
		���ϲ���ʵ���ϰ����˵�ǰ״̬(M N _top _board)�Լ���ʷ��Ϣ(lastX lastY),��Ҫ���ľ�������Щ��Ϣ�¸������������ǵ����ӵ�
	output:
		������ӵ�Point
*/
extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* top, const int* _board, 
	const int lastX, const int lastY, const int noX, const int noY){
	/*
		��Ҫ������δ���
	*/
	int x = -1, y = -1;//���ս�������ӵ�浽x,y��
	int** board = new int*[M];
	for(int i = 0; i < M; i++){
		board[i] = new int[N];
		for(int j = 0; j < N; j++){
			board[i][j] = _board[i * N + j];
		}
	}
	
	/*
		�������Լ��Ĳ������������ӵ�,Ҳ���Ǹ�����Ĳ�����ɶ�x,y�ĸ�ֵ
		�ò��ֶԲ���ʹ��û�����ƣ�Ϊ�˷���ʵ�֣�����Զ����Լ��µ��ࡢ.h�ļ���.cpp�ļ�
	*/
	//Add your own code below
	nox = noX;
	noy = noY;
	m = M;
	n = N;
	Top = new int[n];
	TOP = new int[n];
	temp = new int*[m];
	Temp = new int*[m];
	for(int i=0;i<m;++i){
		temp[i] = new int[n];
		Temp[i] = new int[n];
		for(int j=0;j<n;++j){
			Top[j] = top[j];
			temp[i][j] = board[i][j];
			Temp[i][j] = board[i][j];
		}
	}
	nodes[0].left = nodes[0].right = 1;
	nodes[0].isUser = true;//�Է�Ϊuser
	nodes[0].father = NULL;
	nodes[0].x = lastX;
	nodes[0].y = lastY;
	index = 1;
	int start = clock();
	int end = clock();
	int count = 0;
	srand((int)time(0));
	while((end - start)<4500){
		for(int i = 0; i < n; ++i){
			Top[i] = top[i];
			TOP[i] = top[i];
			for(int j=0;j<m;++j){
				Temp[j][i] = board[j][i];
				temp[j][i] = board[j][i];
			}
		}
		int chosen_index = TreePolicy(0);
		//_cprintf("chosen_index  = %d\n", chosen_index);
		DefaultPolicy(chosen_index);
		end = clock();
		++count;
	}
	int best_choice = BestChild(0,0);
	x = nodes[best_choice].x;
	y = nodes[best_choice].y;
	clearArray(M, N, board);
	clear();
	return new Point(x, y);
}


/*
	getPoint�������ص�Pointָ�����ڱ�dllģ���������ģ�Ϊ��������Ѵ���Ӧ���ⲿ���ñ�dll�е�
	�������ͷſռ䣬����Ӧ�����ⲿֱ��delete
*/
extern "C" __declspec(dllexport) void clearPoint(Point* p){
	delete p;
	return;
}

/*
	���top��board����
*/
void clearArray(int M, int N, int** board){
	for(int i = 0; i < M; i++){
		delete[] board[i];
	}
	delete[] board;
}


/*
	������Լ��ĸ�������������������Լ����ࡢ����������µ�.h .cpp�ļ�������ʵ������뷨
*/

//TreePolicy���ж��Ƿ�Ϊ��ֹ�ڵ�
bool notEnd(int v){
	if(nodes[v].x==-1||nodes[v].y==-1)return true;
	if((nodes[v].isUser && userWin(nodes[v].x, nodes[v].y, m, n, Temp))||(!nodes[v].isUser&& machineWin(nodes[v].x, nodes[v].y, m,n, Temp))||isTie(n, TOP))
		return false;
	return true;

}

//��չ�ýڵ�
int Expand(int v){
	//_cprintf("Expand v=%d\n",v);
	int i= nodes[v].right - nodes[v].left;	//����
	int index_ = nodes[v].right; //��ǰ��չ�ڵ���±�
	while(TOP[i]==0)++i;  //�ų��޷����ӵ���
	if(i>n||index_>MAXTREE)return v;
	nodes[index_].y = i;
	nodes[index_].x = TOP[i]-1;
	if((TOP[i]-2) == nox && i == noy){TOP[i] -= 2;Top[i] -= 2;}
	else {--TOP[i];--Top[i];}
	nodes[index_].interest = nodes[index_].totalRound = 0;
	nodes[index_].father = v;
	index += n;//�����ӽڵ���±�
	nodes[index_].left = nodes[index_].right =index;
	nodes[index_].isUser = !nodes[v].isUser;
	Temp[nodes[index_].x][i] = (nodes[index_].isUser)?1:2;
	temp[nodes[index_].x][i] = Temp[nodes[index_].x][i];
	++nodes[v].right;
	return index_;
}


//ѡ�����õ��ӽڵ�
int BestChild(int v , double c){
	int i= nodes[v].left;
	int best = i;
	long double father_round = 2 * log((long double)nodes[v].totalRound);
	double Q = nodes[i].interest/((double)nodes[i].totalRound+epsilon) + c * sqrt(father_round/(double)nodes[i].totalRound);
	for(++i;i<nodes[v].right;++i){
		double temp_Q = nodes[i].interest/(double)nodes[i].totalRound + c * sqrt(father_round/((double)nodes[i].totalRound)+epsilon);
		if(temp_Q > Q){
			Q = temp_Q;
			best = i;
		}
	}
	return best;
}

//����������
int TreePolicy(int v){
	//_cprintf("Tree Policy!\n");
	int index = v;
	while(notEnd(v)){	 //������ֹ�ڵ�
		int choice = 0;//����չ�ڵ���Ŀ
		for(int i=0;i<n;++i)
			if(TOP[i]>0)
				++choice;
		if((nodes[v].right - nodes[v].left) < choice){	
			return Expand(v);
		}
		else {
			v = BestChild(v,0.7);
			--TOP[nodes[v].y];
			--Top[nodes[v].y];
			if((nodes[v].x-1)==nox && nodes[v].y==noy){--TOP[nodes[v].y];--Top[nodes[v].y];}
			Temp[nodes[v].x][nodes[v].y] = (nodes[v].isUser)?1:2;
			temp[nodes[v].x][nodes[v].y] = Temp[nodes[v].x][nodes[v].y];
		}
	}
	return v;
}
//Ĭ�ϲ���
//����vΪ��ģ���״̬�ڵ�
void DefaultPolicy(int v){
	//_cprintf("Default Ploicy \n");
	double value = 0;
	int user = (nodes[v].isUser)?1:2;
	bool isWin = (user == 1)? userWin(nodes[v].x, nodes[v].y, m, n, temp): machineWin(nodes[v].x, nodes[v].y, m,n, temp);
	int rand_x = nodes[v].x,rand_y = nodes[v].y;
	
	if(isWin && user == 1) BackUp(v,-1);
	else if(isWin &&user == 2) BackUp(v,1);
	else if(isTie(n,Top)) BackUp(v,0);
	else {
		for(int i=0;i<2*n;++i){
			for(int k=0;k<m;++k)
				for(int j=0;j<n;++j){
					Top[j] = TOP[j];
					temp[k][j] = Temp[k][j];
				}
			bool isWin = (user == 1)? userWin(nodes[v].x, nodes[v].y, m, n, temp): machineWin(nodes[v].x, nodes[v].y, m,n, temp);
			int rand_x = nodes[v].x,rand_y = nodes[v].y;
			while(!isWin && !isTie(n,Top)){	
				rand_y = rand()%n;
				while(Top[rand_y]== 0)
					rand_y = rand()%n;
				rand_x = Top[rand_y]-1;
				if(rand_x ==nox && rand_y==noy)Top[rand_y]-=2;
				else --Top[rand_y];			
				user = 3 - user;
				temp[rand_x][rand_y] = user;
				if(user == 2)isWin = machineWin(rand_x, rand_y, m, n, temp);
				else isWin =  userWin(rand_x, rand_y, m, n, temp);
				
			}
			if(isTie(n,Top)) {value = 0;/*_cprintf("Tie\n");*/BackUp(v, value);}
			else if( user == 1 ){value = -1;/* _cprintf("userwin\n");*/BackUp(v,value);}
			else if(user == 2){value = 1; /*_cprintf("machineWin\n");*/BackUp(v,value);}
		}
	}	
	return ;	
}


//�����޸ĸ����Ĺ���ֵ�ͷ��ʴ���
void BackUp(int v, double value){	
	if( value > 0){
		while(v != 0){
			++nodes[v].totalRound;
			if(!nodes[v].isUser)++nodes[v].interest;
			v = nodes[v].father;
		}
		++nodes[0].totalRound;
	}
	else if(value < 0){
		while(v != 0){
			++nodes[v].totalRound;
			if(nodes[v].isUser)++nodes[v].interest;
			v = nodes[v].father;
		}
		++nodes[0].totalRound;
		++nodes[0].interest;
	}
	else {
		while(v != 0){
			++nodes[v].totalRound;
			++nodes[v].interest;
			v = nodes[v].father;
		}
		++nodes[0].totalRound;
		++nodes[0].interest;
	}
}


void clear(){
	for(int i=0;i<m;++i){
		delete temp[i];
		delete Temp[i];
	}
	delete temp;
	delete Temp;
	delete Top;
	delete TOP;
}