#include <iostream>
#include "Point.h"
#include "Strategy.h"
#include "Judge.h"
#include "MCTPoint.h"
#include <ctime>
#include <cmath>
#include <conio.h>
#include <atlstr.h>
const int MAXTREE = 8500050;
MCTNode nodes[MAXTREE];
int m,n;//将长度和宽度设置为全局变量便于调用
int nox,noy;
int **TEMP;//记录default后的棋盘
int **temp;//TreePolicy 和default中调用模拟使用
int* TOP;//可落子点
int* Top;
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
	AllocConsole();
	int index = 0;
	_cprintf("lastX= %d\n", lastX);
	_cprintf("lastY= %d\n", lastY);
	nodes[0].x = lastX;
	nodes[0].y = lastY;
	nodes[0].left = nodes[0].right = 1;
	nodes[0].isLeaf = true;
	nodes[0].interest = nodes[0].totalRound = 0;
	nodes[0].isUser = false;
	m = M;
	n = N;
	temp = new int*[m];
	TEMP = new int*[m];
	for(int i = 0; i < m; ++i){
		temp[i] = new int[n];
		TEMP[i] = new int[n];
		for(int j = 0; j < n; j++){
			temp[i][j] = _board[i * N + j];
			TEMP[i][j] = _board[i * N + j];
			TOP[j] = top[j];
			Top[j] = top[j];
		}
	} 
	int start = clock();
	int end = clock();
	_cprintf("start= %d\n", start);
	_cprintf("end  = %d\n", end);
	while((end - start)<4500){
		int chosen_index = TreePolicy(0);
		if(chosen_index > index) index = chosen_index;
		double value = DefaultPolicy(chosen_index);
		BackUp(chosen_index , value);
		end = clock();
		for(int i=0;i < m; ++i)
			for(int j=0;j<n;++j){
				temp[i][j] = TEMP[i][j];
				Top[j] = TOP[j];
			}
	}
	int best_choice = BestChild(0,0);
	x = nodes[best_choice].x;
	y = nodes[best_choice].y;
	/*
		不要更改这段代码
	*/
	clearArray(M, N, board);
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
bool notEnd(){
	for(int i=0 ;i<n;++i){
		if(TEMP[0][i]==0 && nox!=0 && noy!=i)return true;
	}
	return false;

}

//扩展该节点
int Expand(int v){
	for(int i=0 ;i<n;++i){
		if(TOP[i]>0){
			int index_ = nodes[v].right;
			nodes[index_].y = i;
			nodes[index_].x = TOP[i];
			--TOP[i];
			nodes[index_].father = v;
			nodes[index_].isLeaf = true;
			nodes[index_].isUser = true;
			TEMP[TOP[i]][i] = 2;
			++nodes[v].right;
			nodes[v].isLeaf = false;
			return index_;
		}
	}
	return v;
}


//选择出最好的子节点
int BestChild(int v , int c){
	int best = 0;
	double Q = 0;
	for(int i= nodes[v].left;i<=nodes[v].right;++i){
		double temp_Q = nodes[i].interest/nodes[i].totalRound + c * sqrt(2 * log((long double)nodes[v].totalRound)/nodes[i].totalRound);
		if(temp_Q > Q){
			temp_Q = Q;
			best = i;
		}
	}
	return best;
}

//搜索树策略
int TreePolicy(int v){
	while(notEnd()){
		if((nodes[v].right - nodes[v].left) < n){
			return Expand(v);
		}
		else v = BestChild(v,0);
	}
	return v;
}
//默认策略
//其中v为待扩展的状态节点
double DefaultPolicy(int v){
	temp[nodes[v].x][nodes[v].y] = 2;
	int user = 2;
	bool isWin = userWin(nodes[v].x, nodes[v].y, m, n, temp)||machineWin(nodes[v].x, nodes[v].y, m,n, temp)||isTie(n, Top);
	int rand_x = 0,rand_y = 0;
	while(!isWin){
		rand_y = rand()%n;
		rand_x = Top[rand_y];
		--Top[rand_y];
		user = 3 - user;
		temp[rand_x][rand_y] = user;
	}
	if(user == 2&& userWin(rand_x,rand_y,m,n,temp))return 1;
	else if(user == 1 && machineWin(rand_x,rand_y,m,n,temp))return -1;
	else if(isTie(n,Top))return 0;
	else return 0;
}


//回溯修改父辈的估计值和访问次数
void BackUp(int v, double value){
	while(v!=0){
		++nodes[v].totalRound;
		nodes[v].interest += value;
		value = -value;
		v = nodes[v].father;
	}
}