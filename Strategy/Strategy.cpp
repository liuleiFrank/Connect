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
int m,n;//将长度和宽度设置为全局变量便于调用
int nox,noy;
int **temp;//TreePolicy 和default中调用模拟使用
int **Temp;
int *TOP;
int* Top;
int index;
using namespace std;

/*
	策略函数接口,该函数被对抗平台调用,每次传入当前状态,要求输出你的落子点,该落子点必须是一个符合游戏规则的落子点,不然对抗平台会直接认为你的程序有误
	
	input:
		为了防止对对抗平台维护的数据造成更改，所有传入的参数均为const属性
		M, N : 棋盘大小 M - 行数 N - 列数 均从0开始计， 左上角为坐标原点，行用x标记，列用y标记
		top : 当前棋盘每一列列顶的实际位置. e.g. 第i列为空,则_top[i] == M, 第i列已满,则_top[i] == 0
		_board : 棋盘的一维数组表示, 为了方便使用，在该函数刚开始处，我们已经将其转化为了二维数组board
				你只需直接使用board即可，左上角为坐标原点，数组从[0][0]开始计(不是[1][1])
				board[x][y]表示第x行、第y列的点(从0开始计)
				board[x][y] == 0/1/2 分别对应(x,y)处 无落子/有用户的子/有程序的子,不可落子点处的值也为0
		lastX, lastY : 对方上一次落子的位置, 你可能不需要该参数，也可能需要的不仅仅是对方一步的
				落子位置，这时你可以在自己的程序中记录对方连续多步的落子位置，这完全取决于你自己的策略
		noX, noY : 棋盘上的不可落子点(注:其实这里给出的top已经替你处理了不可落子点，也就是说如果某一步
				所落的子的上面恰是不可落子点，那么UI工程中的代码就已经将该列的top值又进行了一次减一操作，
				所以在你的代码中也可以根本不使用noX和noY这两个参数，完全认为top数组就是当前每列的顶部即可,
				当然如果你想使用lastX,lastY参数，有可能就要同时考虑noX和noY了)
		以上参数实际上包含了当前状态(M N _top _board)以及历史信息(lastX lastY),你要做的就是在这些信息下给出尽可能明智的落子点
	output:
		你的落子点Point
*/
extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* top, const int* _board, 
	const int lastX, const int lastY, const int noX, const int noY){
	/*
		不要更改这段代码
	*/
	int x = -1, y = -1;//最终将你的落子点存到x,y中
	int** board = new int*[M];
	for(int i = 0; i < M; i++){
		board[i] = new int[N];
		for(int j = 0; j < N; j++){
			board[i][j] = _board[i * N + j];
		}
	}
	
	/*
		根据你自己的策略来返回落子点,也就是根据你的策略完成对x,y的赋值
		该部分对参数使用没有限制，为了方便实现，你可以定义自己新的类、.h文件、.cpp文件
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
	nodes[0].isUser = true;//对方为user
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
	getPoint函数返回的Point指针是在本dll模块中声明的，为避免产生堆错误，应在外部调用本dll中的
	函数来释放空间，而不应该在外部直接delete
*/
extern "C" __declspec(dllexport) void clearPoint(Point* p){
	delete p;
	return;
}

/*
	清除top和board数组
*/
void clearArray(int M, int N, int** board){
	for(int i = 0; i < M; i++){
		delete[] board[i];
	}
	delete[] board;
}


/*
	添加你自己的辅助函数，你可以声明自己的类、函数，添加新的.h .cpp文件来辅助实现你的想法
*/

//TreePolicy中判断是否为终止节点
bool notEnd(int v){
	if(nodes[v].x==-1||nodes[v].y==-1)return true;
	if((nodes[v].isUser && userWin(nodes[v].x, nodes[v].y, m, n, Temp))||(!nodes[v].isUser&& machineWin(nodes[v].x, nodes[v].y, m,n, Temp))||isTie(n, TOP))
		return false;
	return true;

}

//扩展该节点
int Expand(int v){
	//_cprintf("Expand v=%d\n",v);
	int i= nodes[v].right - nodes[v].left;	//列数
	int index_ = nodes[v].right; //当前扩展节点的下标
	while(TOP[i]==0)++i;  //排除无法落子的列
	if(i>n||index_>MAXTREE)return v;
	nodes[index_].y = i;
	nodes[index_].x = TOP[i]-1;
	if((TOP[i]-2) == nox && i == noy){TOP[i] -= 2;Top[i] -= 2;}
	else {--TOP[i];--Top[i];}
	nodes[index_].interest = nodes[index_].totalRound = 0;
	nodes[index_].father = v;
	index += n;//控制子节点的下标
	nodes[index_].left = nodes[index_].right =index;
	nodes[index_].isUser = !nodes[v].isUser;
	Temp[nodes[index_].x][i] = (nodes[index_].isUser)?1:2;
	temp[nodes[index_].x][i] = Temp[nodes[index_].x][i];
	++nodes[v].right;
	return index_;
}


//选择出最好的子节点
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

//搜索树策略
int TreePolicy(int v){
	//_cprintf("Tree Policy!\n");
	int index = v;
	while(notEnd(v)){	 //不是终止节点
		int choice = 0;//可扩展节点数目
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
//默认策略
//其中v为待模拟的状态节点
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


//回溯修改父辈的估计值和访问次数
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