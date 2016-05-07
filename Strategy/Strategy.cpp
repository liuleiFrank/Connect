#include <iostream>
#include "Point.h"
#include "Strategy.h"
#include "Judge.h"
#include "MCTPoint.h"
#include <ctime>
#include <cmath>
#include <conio.h>
#include <atlstr.h>
const int MAXTREE = 8500000;
MCTNode nodes[MAXTREE];
int m,n;//�����ȺͿ������Ϊȫ�ֱ������ڵ���
int nox,noy;
int **temp;//TreePolicy ��default�е���ģ��ʹ��
int **Temp;
int *TOP;
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
	/*int index = 0;*/
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

	//��ʼ�����ڵ�
	/*for(int i=0;i<m;++i){
		for(int j=0;j<n;++j)
			_cprintf("%d ",board[i][j]);
		_cprintf("\n");
	}*/
	nodes[0].left = nodes[0].right = 1;
	nodes[0].isUser = true;//�Է�Ϊuser
	nodes[0].father = NULL;
	nodes[0].x = lastX;
	nodes[0].y = lastY;
	
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
		/*_cprintf("chosen_index  = %d\n", chosen_index);
		for(int i=0;i<m;++i){
			for(int j=0;j<n;++j)
				_cprintf("%d ",Temp[i][j]);
			_cprintf("\n");
		}*/
		/*if(chosen_index > index) index = chosen_index;*/
		//_cprintf("index  = %d\n", index);
		double value = DefaultPolicy(chosen_index);
		// _cprintf("value  = %f\n", value);
		BackUp(chosen_index , value);
		end = clock();
		++count;
	}
	//_cprintf("%d \n",count);
	int best_choice = BestChild(0,0);
	x = nodes[best_choice].x;
	y = nodes[best_choice].y;
	/*for(int i=1;i<=n;++i)
		_cprintf("%d  interest=%f  round=%d \n",i,nodes[i].interest,nodes[i].totalRound);
	_cprintf("bestchoice=%d best_x=%d best_y=%d \n",best_choice,x,y);*/
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
	if(nodes[v].x==-1||nodes[v].y==-1)return true;
	if((nodes[v].isUser && userWin(nodes[v].x, nodes[v].y, m, n, Temp))||(!nodes[v].isUser&& machineWin(nodes[v].x, nodes[v].y, m,n, Temp))||isTie(n, TOP))
		return false;
	return true;

}

//��չ�ýڵ�
int Expand(int v){
	//_cprintf("1.interest=%f \n",nodes[1].interest);
	int i= nodes[v].right - nodes[v].left;	//����
	int index_ = nodes[v].right; //��ǰ��չ�ڵ���±�
	while(TOP[i]==0)++i;  //�ų��޷����ӵ���
	if(i>n||index_>MAXTREE)return v;
	nodes[index_].y = i;
	nodes[index_].x = TOP[i]-1;
	if((TOP[i]-2) == nox && i == noy)TOP[i] -= 2;
	else --TOP[i];
	nodes[index_].interest = nodes[index_].totalRound = 0;
	nodes[index_].father = v;
	nodes[index_].left = nodes[index_].right = n * index_ + 1;
	nodes[index_].isUser = !nodes[v].isUser;
	Temp[nodes[index_].x][i] = (nodes[index_].isUser)?1:2;
	++nodes[v].right;
	return index_;
}


//ѡ�����õ��ӽڵ�
int BestChild(int v , double c){
	 // _cprintf("BestChild \n");
	int best = 0;
	double Q = -10000000;
	for(int i= nodes[v].left;i<nodes[v].right;++i){
		double temp_Q =0;
		if(nodes[i].isUser)temp_Q = 100-nodes[i].interest/nodes[i].totalRound + c * sqrt(2 * log((long double)nodes[v].totalRound)/(double)nodes[i].totalRound);
		else temp_Q = nodes[i].interest/nodes[i].totalRound + c * sqrt(2 * log((long double)nodes[v].totalRound)/(double)nodes[i].totalRound);
		if(temp_Q > Q){
			Q = temp_Q;
			best = i;
		}
		//_cprintf("father=%d i=%d interest=%f round=%d temp=%f\n",v,i,nodes[i].interest,nodes[i].totalRound,temp_Q);
	}
	//_cprintf("BestChild = %d\n",best);
	return best;
}

//����������
int TreePolicy(int v){
	  //_cprintf("Tree Policy!\n");
	int index = v;
	while(true){		
		if(index != v){
			--TOP[nodes[v].y];
			if((nodes[v].x-1)==nox && nodes[v].y==noy)--TOP[nodes[v].y];
			Temp[nodes[v].x][nodes[v].y] = (nodes[v].isUser)?1:2;
		}
		if(notEnd(v)){  //������ֹ�ڵ�
			int choice = 0;//����չ�ڵ���Ŀ
			for(int i=0;i<n;++i)
				if(TOP[i]>0)
					++choice;
			if((nodes[v].right - nodes[v].left) < choice){			
				return Expand(v);
			}
			else v = BestChild(v,0.7);
		}
		else break;		
	}
	return v;
	//_cprintf("v = %d\n", v);
}
//Ĭ�ϲ���
//����vΪ��ģ���״̬�ڵ�
double DefaultPolicy(int v){
	  //_cprintf("Default Ploicy \n");
	double value = 0;
	int user = (nodes[v].isUser)?1:2;
	for(int i=0;i<n;++i)Top[i] = TOP[i];
	for(int i=0;i<m;++i)
		for(int j=0;j<n;++j)
			temp[i][j] = Temp[i][j];
	bool isWin = (user == 1)? userWin(nodes[v].x, nodes[v].y, m, n, temp): machineWin(nodes[v].x, nodes[v].y, m,n, temp);
	int rand_x = nodes[v].x,rand_y = nodes[v].y;
	// _cprintf("1 \n");
	if(isWin && user == 1) return -1;
	else if(isWin &&user == 2) return 1;
	else if(isTie(n,Top)) return 0;
	else {
		for(int i=0;i<2*n;++i){
			for(int k=0;k<m;++k)
				for(int j=0;j<n;++j){
					Top[j] = TOP[j];
					temp[k][j] = Temp[k][j];
				}
			rand_y = i%n;
			while(!isWin && !isTie(n,Top)){				
				while(Top[rand_y]== 0)
					rand_y = rand()%n;
				rand_x = Top[rand_y]-1;
				if(rand_x ==nox && rand_y==noy)Top[rand_y]-=2;
				else --Top[rand_y];			
				user = 3 - user;
				temp[rand_x][rand_y] = user;
				if(user == 2)isWin = machineWin(rand_x, rand_y, m, n, temp);
				else isWin =  userWin(rand_x, rand_y, m, n, temp);
				rand_y = rand()%n;
			}
			if( user == 2 && userWin(rand_x,rand_y,m,n,temp))--value;
			else if(user == 1 && machineWin(rand_x,rand_y,m,n,temp))++value;
		}
		return value;
	}	
	
}


//�����޸ĸ����Ĺ���ֵ�ͷ��ʴ���
void BackUp(int v, double value){
	 //_cprintf("BckpUp \n");
	while(v != 0){
		++nodes[v].totalRound;
		nodes[v].interest += value;
		value = -value;
		//_cprintf("%d %d %f %d\n",v,nodes[v].father,nodes[v].interest,nodes[v].totalRound);
		v = nodes[v].father;
	}
	//_cprintf(" \n");
	++nodes[0].totalRound;
	nodes[0].interest+=value;
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