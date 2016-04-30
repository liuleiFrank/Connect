#include <iostream>
#include "Point.h"
#include "Strategy.h"
#include "Judge.h"
#include "MCTPoint.h"
#include <ctime>
#include <cmath>
#include <conio.h>
#include <atlstr.h>
const int MAXTREE = 8500;
MCTNode nodes[MAXTREE];
int m,n;//将长度和宽度设置为全局变量便于调用
int nox,noy;
int **TEMP;//记录default后的棋盘
int **temp;//TreePolicy 和default中调用模拟使用
int* const_TOP;//可落子点//常量
int* TOP;
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
	nox = noX;
	noy = noY;
	int index = 0;
	//_cprintf("lastX= %d\n", lastX);
	//_cprintf("lastY= %d\n", lastY);
	//for(int i=0;i<N;++i)_cprintf("%d ", top[i]);
	//_cprintf("\n");
	nodes[0].x = lastX;
	nodes[0].y = lastY;
	nodes[0].left = nodes[0].right = 1;
	nodes[0].isLeaf = true;
	nodes[0].interest = nodes[0].totalRound = 0;
	nodes[0].isUser = false;//对方为user
	nodes[0].father = NULL;
	m = M;
	n = N;
	/*_cprintf("M = %d\n", m);
	_cprintf("N = %d\n", n);*/
	temp = new int*[m];
	TEMP = new int*[m];
	TOP = new int[n];
	Top = new int[n];
	const_TOP = new int[n];
	for(int i = 0; i < m; ++i){
		temp[i] = new int[n];
		for(int j = 0; j < n; j++){
			temp[i][j] = _board[i * N + j];
		}
	} 
	for(int i = 0; i < m; ++i){
		TEMP[i] = new int[n];
		for(int j = 0; j < n; ++j){
			TEMP[i][j] = _board[i * N + j];
		}
	}	
	for(int i = 0; i < n; ++i){
		TOP[i] = top[i];
		Top[i] = top[i];
		const_TOP[i] = top[i];
	}
	int start = clock();
	int end = clock();
	//_cprintf("nox = %d\n", nox);
	//_cprintf("noy = %d\n", noy);
	while((end - start)<4500){
		int chosen_index = TreePolicy(0);
		//_cprintf("chosen_index  = %d\n", chosen_index);
		if(chosen_index > index) index = chosen_index;
		//_cprintf("index  = %d\n", index);
		double value = DefaultPolicy(chosen_index);
		//_cprintf("value  = %f\n", value);
		BackUp(chosen_index , value);
		for(int i=0;i < m; ++i)
			for(int j=0;j<n;++j){
				temp[i][j] = TEMP[i][j];
				Top[j] = TOP[j];
			}
		end = clock();
		//_cprintf("end = %d \n",end);
		/*_cprintf("after chose one \n");
		for(int i=0;i<m ;++i){
			for(int j=0;j<n;++j){
				_cprintf("%d ",temp[i][j]);
			}
			_cprintf(" \n");
		}*/
	}
	int best_choice = BestChild(0,0);
	x = nodes[best_choice].x;
	y = nodes[best_choice].y;
	//_cprintf("best_x=%d best_y=%d \n",x,y);
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
	//_cprintf("judge whether end\n");
	for(int i=0 ;i<n;++i){
		//_cprintf("TEMP[0][%d] = %d\n", i,TEMP[0][i]);
		if(TEMP[0][i]==0 && !(nox==0 && noy==i))return true;
	}
	return false;

}

//扩展该节点
int Expand(int v){
	//_cprintf("Expand \n");
	for(int i=0 ;i<n;++i){
		if(TOP[i]==const_TOP[i]){
			int index_ = nodes[v].right;
			nodes[index_].y = i;
			nodes[index_].x = TOP[i]-1;
			//_cprintf("node x y = %d %d\n",nodes[index_].x,nodes[index_].y);
			//_cprintf("TOP[i] = %d\n", TOP[i]);
			nodes[index_].father = v;
			nodes[index_].isLeaf = true;
			nodes[index_].isUser = true;
			++nodes[v].right;
			--Top[i];
			--TOP[i];
			nodes[v].isLeaf = false;
			return index_;
		}
	}
	return v;
}


//选择出最好的子节点
int BestChild(int v , int c){
	//_cprintf("BestChild \n");
	//_cprintf("father = %d  %d  %d\n",v,nodes[v].left,nodes[v].right);	
	int best = 0;
	double Q = -10000000;
	for(int i= nodes[v].left;i<nodes[v].right;++i){
		double temp_Q = nodes[i].interest/nodes[i].totalRound + c * sqrt(2 * log((long double)nodes[v].totalRound)/nodes[i].totalRound);
		//_cprintf("i= %d; fathertotalRound = %d; childtotalRound = %d;childinterest = %f; temp_Q = %f; \n",i,nodes[v].totalRound,nodes[i].totalRound,nodes[i].interest,temp_Q);
		if(temp_Q > Q){
			Q = temp_Q;
			best = i;
		}
	}
	//_cprintf("BestChild = %d\n",best);
	return best;
}

//搜索树策略
int TreePolicy(int v){
	//_cprintf("Tree Policy!\n");
	while(notEnd()){
		//_cprintf("not END\n");
		if((nodes[v].right - nodes[v].left) < n){			
			return Expand(v);
		}
		else return  BestChild(v,1);
	}
	//_cprintf("v = %d\n", v);
}
//默认策略
//其中v为待扩展的状态节点
double DefaultPolicy(int v){
	//_cprintf("Default Ploicy \n");
	//_cprintf("nodes[%d] x y = %d %d\n",v, nodes[v].x, nodes[v].y);
	temp[nodes[v].x][nodes[v].y] = 2;
	int user = 2;
	//_cprintf("in\n");
	bool isWin = userWin(nodes[v].x, nodes[v].y, m, n, temp)||machineWin(nodes[v].x, nodes[v].y, m,n, temp)||isTie(n, Top);
	//_cprintf("first judge \n");
	int rand_x = 0,rand_y = 0;
	while(!isWin){
		rand_y = rand()%n;
		while(Top[rand_y]== 0)
			rand_y = rand()%n;
		rand_x = Top[rand_y]-1;
		--Top[rand_y];
		user = 3 - user;
		temp[rand_x][rand_y] = user;
		//_cprintf("user = %d\n",user);
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
	else return 0;
}


//回溯修改父辈的估计值和访问次数
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