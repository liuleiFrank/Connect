#include <iostream>
#include "Point.h"
#include "Strategy.h"
#include "Judge.h"
#include "MCTPoint.h"
#include <ctime>
#include <cmath>
#include <conio.h>
#include <atlstr.h>
const int MAXTREE = 85000;
MCTNode nodes[MAXTREE];
int m,n;//�����ȺͿ������Ϊȫ�ֱ������ڵ���
int nox,noy;
int **temp;//TreePolicy ��default�е���ģ��ʹ��
int* Top;
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
	AllocConsole();
	nox = noX;
	noy = noY;
	m = M;
	n = N;
	int index = 0;
	Top = new int[n];
	temp = new int*[m];
	for(int i=0;i<m;++i){
		temp[i] = new int[n];
		for(int j=0;j<n;++j){
			Top[j] = top[j];
			temp[i][j] = board[i][j];
		}
	}
	//��ʼ�����ڵ�
	nodes[0].left = nodes[0].right = 1;
	nodes[0].isUser = true;//�Է�Ϊuser
	nodes[0].father = NULL;
	nodes[0].ini(board,Top,lastX,lastY);
	temp = new int*[m];
	Top = new int[n];
	for(int i = 0; i < m; ++i){
		temp[i] = new int[n];
	} 	
	srand((int)time(0));
	int start = clock();
	int end = clock();
	int count = 0;
	while((end - start)<4500){
		for(int i = 0; i < n; ++i){
			Top[i] = top[i];
			for(int j=0;j<m;++j){
				temp[j][i] = board[j][i];
			}
		}
		int chosen_index = TreePolicy(0);
		//_cprintf("chosen_index  = %d\n", chosen_index);
		if(chosen_index > index) index = chosen_index;
		//_cprintf("index  = %d\n", index);
		double value = DefaultPolicy(chosen_index);
		//_cprintf("value  = %f\n", value);
		BackUp(chosen_index , value);
		end = clock();
		++count;
	}
	_cprintf("%d \n",count);
	int best_choice = BestChild(0,0);
	x = nodes[best_choice].x;
	y = nodes[best_choice].y;
	_cprintf("best_x=%d best_y=%d \n",x,y);
	/*
		��Ҫ������δ���
	*/
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
	//_cprintf("judge whether end\n");
	//_cprintf("x=%d y=%d \n",nodes[v].x,nodes[v].y);
	if(nodes[v].x==-1||nodes[v].y==-1)return true;
	if(userWin(nodes[v].x, nodes[v].y, m, n, nodes[v].board)||machineWin(nodes[v].x, nodes[v].y, m,n, nodes[v].board)||isTie(n, nodes[v].top))
		return false;
	
	return true;

}

//��չ�ýڵ�
int Expand(int v){
	//_cprintf("Expand \n");
	Top = nodes[v].top;	
	temp = nodes[v].board;	
	int i= nodes[v].right - nodes[v].left;	
	int index_ = nodes[v].right;
	while(Top[i]==0)++i;
	if(i>n)return v;
	int y = i;
	int x = Top[y]-1;
	if((x-1) == nox && y == noy)Top[y] -= 2;
	else --Top[y];
	
	//_cprintf("node x y = %d %d\n",nodes[index_].x,nodes[index_].y);
	//_cprintf("TOP[i] = %d\n", TOP[i]);
	nodes[index_].father = v;
	nodes[index_].left = nodes[index_].right = n * index_ + 1;
	nodes[index_].isUser = !nodes[v].isUser;
	temp[x][y] = (nodes[index_].isUser)?1:2;
	nodes[index_].ini(temp,Top,x,y);
	++nodes[v].right;
	
	return index_;
}


//ѡ�����õ��ӽڵ�
int BestChild(int v , int c){
	//_cprintf("BestChild \n");
	int best = 0;
	double Q = -10000000;
	for(int i= nodes[v].left;i<nodes[v].right;++i){
		double temp_Q = nodes[i].interest/nodes[i].totalRound + c * sqrt(2 * log((long double)nodes[v].totalRound)/nodes[i].totalRound);
		if(temp_Q > Q){
			Q = temp_Q;
			best = i;
		}
	}
	//_cprintf("BestChild = %d\n",best);
	return best;
}

//����������
int TreePolicy(int v){
	//_cprintf("Tree Policy!\n");
	while(notEnd(v)){		//������ֹ�ڵ�
		int choice = 0;
		for(int i=0;i<n;++i)
			if(nodes[v].top[i]>0)
				++choice;
		if((nodes[v].right - nodes[v].left) < choice){			
			return Expand(v);
		}
		else v = BestChild(v,1.5);
	}
	return v;
	//_cprintf("v = %d\n", v);
}
//Ĭ�ϲ���
//����vΪ����չ��״̬�ڵ�
double DefaultPolicy(int v){
	//_cprintf("Default Ploicy \n");
	int user = (nodes[v].isUser)?1:2;;
	bool isWin = userWin(nodes[v].x, nodes[v].y, m, n, temp)||machineWin(nodes[v].x, nodes[v].y, m,n, temp)||isTie(n, Top);
	int rand_x = nodes[v].x,rand_y = nodes[v].y;
	while(!isWin){
		rand_y = rand()%n;
		while(Top[rand_y]== 0)
			rand_y = rand()%n;
		rand_x = Top[rand_y]-1;
		--Top[rand_y];
		user = 3 - user;
		temp[rand_x][rand_y] = user;
		isWin = userWin(rand_x, rand_y, m, n, temp)||machineWin(rand_x, rand_y, m, n, temp)||isTie(n, Top);
	}
	/*_cprintf("end While \n deafult final result\n");
	for(int i=0;i<m ;++i){
		for(int j=0;j<n;++j){
			_cprintf("%d ",temp[i][j]);
		}
		_cprintf(" \n");
	}*/
	if( userWin(rand_x,rand_y,m,n,temp)){
		//_cprintf("user Win \n");
		return -1;
	}
	else if(machineWin(rand_x,rand_y,m,n,temp)){
		//_cprintf("machine Win \n");
		return 1;
	}
	else if(isTie(n,Top)){
		//_cprintf("Tie \n");
		return 0;
	}
	//else return 0;
}


//�����޸ĸ����Ĺ���ֵ�ͷ��ʴ���
void BackUp(int v, double value){
	//_cprintf("BckpUp \n");
	while(v != 0){
		//_cprintf("%d %d \n",v,nodes[v].father);
		++nodes[v].totalRound;
		nodes[v].interest += value;
		value = -value;
		v = nodes[v].father;
	}
	//_cprintf(" \n");
	++nodes[0].totalRound;
}


void clear(){
	for(int i=0;i<m;++i){
		delete temp[i];
	}
	delete temp;
	delete Top;
}