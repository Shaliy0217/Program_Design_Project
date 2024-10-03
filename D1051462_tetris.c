#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#define HEIGHT 14
#define WIDTH 22
#define block_h 4
#define block_w 4
//改變方塊，每加50分加速
//加速.改變方塊已完成

HANDLE hand;
int cur_x,cur_y;
int block_cur[block_h][block_w]={0};
int screen[HEIGHT][WIDTH]={0};
int blockO[block_h][block_w]={
	{0,0,0,0},
	{0,1,1,0},
	{0,1,1,0},
	{0,0,0,0}
};
int blockL[block_h][block_w]={
	{0,2,0,0},
	{0,2,0,0},
	{0,2,2,0},
	{0,0,0,0}
};
int blockJ[block_h][block_w]={
	{0,0,3,0},
	{0,0,3,0},
	{0,3,3,0},
	{0,0,0,0}
};
int blockI[block_h][block_w]={
	{0,4,0,0},
	{0,4,0,0},
	{0,4,0,0},
	{0,4,0,0}
};
int blockZ[block_h][block_w]={
	{0,0,0,0},
	{0,0,5,5},
	{0,5,5,0},
	{0,0,0,0}
};
int blockS[block_h][block_w]={
	{0,0,0,0},
	{6,6,0,0},
	{0,6,6,0},
	{0,0,0,0}
};
int blockT[block_h][block_w]={
	{0,0,0,0},
	{7,7,7,0},
	{0,7,0,0},
	{0,0,0,0}
};
char ascii[27][40] = {
	" ,---------------------===----.\n",
	" | Tetris                     |\n",
	" | ,-----------------------.  |\n",
	" | |                        | |\n",
	" | |                        | |\n",
	" | |                        | |\n",
	" | |                        | |\n",
	" | |  press space to start  | |\n",
	" | |     or s to set up     | |\n",
	" | |          _ _           | |\n",
	" | |         |_|_|          | |\n",
	" | |         |_|_|          | |\n",
	" | |                        | |\n",
	" | |                        | |\n",
	" | |                        | |\n",
	" | |                     _  | |\n",
	" | |                   _|_| | |\n",
	" | |   _              |_|_| | |\n",
	" | | _|_|_           _|_|_| | |\n",
	" | ||_|_|_|         |_|_|_| | |\n",
	" | |........................| |\n",
	" | |     _            ___   | |\n",
	" | |   _|_|_         /   \\  | |\n",
	" | |  |_|_|_|        \\___/  | |\n",
	" | |                        | |\n",
	" | '------...______...------' |\n",
	" ,------------====-----------.\n"
};
char mode[27][40] = {
	" ,---------------------===----.",
	" | Tetris                     |",
	" | ,-----------------------.  |",
	" | |                        | |",
	" | |                        | |",
	" | |                        | |",
	" | |                        | |",
	" | |   1       2        3   | |",
	" | |  wasd tradition goback | |",
	" | |                        | |",
	" | |                        | |",
	" | |                        | |",
	" | |                        | |",
	" | |                        | |",
	" | |                        | |",
	" | |                     _  | |",
	" | |                   _|_| | |",
	" | |   _              |_|_| | |",
	" | | _|_|_           _|_|_| | |",
	" | ||_|_|_|         |_|_|_| | |",
	" | |........................| |",
	" | |     _            ___   | |",
	" | |   _|_|_         /   \\  | |",
	" | |  |_|_|_|        \\___/  | |",
	" | |                        | |",
	" | '------...______...------' |",
	" ,------------====-----------."
};
//shadow

enum block_type{
	O,J,L,I,Z,S,T
};
char up = 72, down = 80,left = 75, right = 77; //按鍵預設72 80 75 77
int block_type_num = 7;
int score = 0;
int scoretemp = 0;
int tm = 800; //間隔時間
int change = 5; //可以換的次數
int notchange = 0; //用來避免連換
int curtype = 7; //紀錄方塊編號
UINT_PTR timerId;

void color(int x) //自定義函根據參數改變顏色 
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x );    //只有一個參數，改變字體顏色
}

void choosecolor(int num){
	if(num == 1){
		color(224);
	}
	else if(num == 2){
		color(96);
	}
	else if(num == 3){
		color(176);
	}
	else if(num == 4){
		color(192);
	}
	else if(num == 5){
		color(112);
	}
	else if(num == 6){
		color(48);
	}
	else if(num == 7){
		color(208);
	}
	else if(num == 0){
		return ;
	}
	//color(15);
}

/*游標控制&&timer設置*/ 
void setCursorVisible(int v){ //if V是1就會顯示游標 
 	CONSOLE_CURSOR_INFO info={1, v};//size1-100  v bool
 	SetConsoleCursorInfo(hand, &info);
}
void gotoxy(int x, int y){//設定游標的位置 
 	COORD pos={.X=x, .Y=y};
 	SetConsoleCursorPosition(hand, pos);
}
static void set_timer(int t){
	KillTimer(NULL, timerId); //清除Timer
	timerId = SetTimer(NULL,0,t,NULL);
}
static void close_timer(){
	KillTimer(NULL, timerId);
}

void setup(){
	system("cls");
	if(up == 119){
		for(int i = 0 ; i < 27 ; i++){
			for(int j = 0 ;j < 31 ; j++){
				if(i == 7 && j == 7){
					color(11);
					printf("1");
					color(15);
				}
				else{
					printf("%c", mode[i][j]);
				}
			}
			printf("\n");
		}
	}
	else{
		for(int i = 0 ; i < 27 ; i++){
			for(int j = 0 ;j < 31 ; j++){
				if(i == 7 && j == 15){
					color(11);
					printf("2");
					color(15);
				}
				else{
					printf("%c", mode[i][j]);
				}
			}
			printf("\n");
		}
	}
	
	
	char now = up;
	char choice = 40;
	while(1){
		if(kbhit()!=0){
			choice=getch();
			if(choice == 49){
				up = 119;
				down = 115;
				left = 97;
				right = 100;
				system("cls");
				for(int i = 0 ; i < 27 ; i++){
					for(int j = 0 ;j < 31 ; j++){
						if(i == 7 && j == 7){
							color(11);
							printf("1");
							color(15);
						}
						else{
							printf("%c", mode[i][j]);
						}
					}
					printf("\n");
				}
			}
			else if(choice == 50){
				up = 72;
				down = 80;
				left = 75;
				right = 77;
				system("cls");
				for(int i = 0 ; i < 27 ; i++){
					for(int j = 0 ;j < 31 ; j++){
						if(i == 7 && j == 15){
							color(11);
							printf("2");
							color(15);
						}
						else{
							printf("%c", mode[i][j]);
						}
					}
					printf("\n");
				}

			}
			else if(choice == 51){
				return ;
			}
		}
		
	}
}

/*印出內容*/
void printxy(char* c, int x, int y){//在XY的位置上印出字元
	gotoxy(x, y); 
	printf("%s", c);
}
void printScreen(){ 
	int i,j;
	color(385);
	for(i=0;i<WIDTH;i++){
		printxy(" ",i+5,3);//上
		printxy(" ",i+5,4+HEIGHT);//下
	}
	for(j=-1;j<HEIGHT+1;j++){
		printxy(" ",4,j+4);	//左
		printxy(" ",5+WIDTH,j+4);//右
	}
	color(1);
	for(i=0;i<WIDTH;i++){
		for(j=0;j<HEIGHT;j++){
			if(screen[j][i]==0){
				color(15);
				printxy(" ",i+5,j+4);	
			} 
			else{
				choosecolor(screen[j][i]);
				printxy(" ",i+5,j+4);
				
			}
		}
	}
	color(15);
	
	gotoxy(0,19);
	
	printf(" | |  score:%2d changetime:%d ",score, change);
	
}
void printBlock(int block[][block_w],int x,int y){//在1印出方塊
	int i,j;
	
	for(i=0;i<block_h;i++){
		for(j=0;j<block_w;j++){
			if(block[i][j] != 0){
				choosecolor(block[i][j]);
				printxy(" ",x+5+j,y+4+i);
			} 
		}
	}
	color(15);
}
void eraseBlock(int block[][block_w],int x,int y){//全部都填空白格 
	int i,j;
	for(i=0;i<block_w;i++){
		for(j=0;j<block_h;j++){
			if(block[i][j] != 0){
				printxy(" ",x+5+j,y+4+i);
			} 
		}
	}
}

/*邊界&&碰撞判斷*/ 
int get_block_y_fill(int row){//判斷每個橫向有沒有方塊 
	int ans=0;
	for(int col=0;col<block_w;col++){
		if(block_cur[row][col] != 0){
			ans=1;	
		 }
	}
	return ans;
}
int get_block_x_fill(int col){//判斷每個縱向有沒有方塊 
	int ans=0;
	 for(int row=0;row<block_w;row++){
	 	if(block_cur[row][col] != 0){
	 		ans=1;	
		}
	 }
	 return ans;
}
void get_block_left_right_margin(int *left_margin ,int *right_margin){//那個方向的白邊數量 

	for(int col=0;col<2;col++){//判斷左邊 
		if(get_block_x_fill(col)==0){
			*left_margin=*left_margin+1;
		}
	}
	for(int col=2;col<4;col++){//判斷右邊 
		if(get_block_x_fill(col)==0){
			*right_margin=*right_margin+1;
		}
	}
	return ;
}
void get_block_top_button_margin(int *top_margin ,int *button_margin){//那個方向的白邊數量 
	for(int row=0;row<2;row++){//判斷上面
		if(get_block_y_fill(row)==0){
			*top_margin=*top_margin+1;
		}
	}
	for(int row=2;row<4;row++){//判斷下面 
		if(get_block_y_fill(row)==0){
			*button_margin=*button_margin+1;
		}
	}
}

int isCollision(int x , int y){//x,y block現在位置 ，如果碰撞 return 1 ,如果沒碰撞 return 0 
	int top_margin=0,button_margin=0,left_margin=0,right_margin=0;
	
	get_block_top_button_margin(&top_margin,&button_margin);
	get_block_left_right_margin(&left_margin,&right_margin);
	
	if(x<0-left_margin || x>WIDTH-block_w+right_margin || y > HEIGHT-block_h+button_margin){
		return 1;
	}
	else if(y<0-top_margin){
		return 1;
	}
	//加上判斷有沒有碰到其他方塊(已經落下的方塊) screen!=0裡面有方塊了
	int w,h;
	for(w=0;w<block_w;w++){
		for(h=0;h<block_h;h++){
			if(block_cur[h][w]!= 0){
				if(screen[y+h][x+w] != 0){
					return 1;
				}
			}
		} 
	}
	return 0;
} 

/*成行判斷及消去*/ 
int is_line_fill(int h){
    int w;
    for(w=0;w<WIDTH;w++){
        if(screen[h][w]==0){
            return 0;
        }        
    }
    return 1;
}

void clear_one_line(int h){//要清除的行數 
	int w,row;	
	for(row = h;row>0;row--){
		for(w=0;w<WIDTH;w++){//把每個row往下複製
			choosecolor(screen[row-1][w]);
			screen[row][w] = screen[row-1][w];
		}
	}
	color(15);
	//第0 row 
	for(w=0;w<WIDTH;w++){
		screen[0][w] = 0;
	}
	
}

void check_line(){
    int total_line = 0; //計算消除行數
    int h;
    //判斷是否連線，從cur_y開始判斷。
    for(h=cur_y;h<HEIGHT;h++)
    {
        if(is_line_fill(h)==1)
        {	
            clear_one_line(h);
			total_line++;
        }
    }
	for(int i = 1 ; i < total_line+1 ; i++){ //一次消去中第一條加10分，第二條20
		score += i*10;
		scoretemp += i*10;
	}
	while(scoretemp >= 100){ //每增加100分下降時間變快50
		tm -= 50;
		scoretemp -= 100;
	}
    printScreen();
}

void land_block(){
	notchange = 0; //有碰到地了就可以再換一次方塊
    int w,h;
	for(w=0;w<block_w;w++){
		for(h=0;h<block_h;h++){
			if(block_cur[h][w] != 0){
				//把surface對應的位置設成!= 0的數
				screen[cur_y+h][cur_x+w]=block_cur[h][w];
			}
		}
	}
}

/*block控制(複製、生成、旋轉、移動*/ 
void copy_block(int block_s[][block_w],int block_d[][block_w]){//block_s代表來源， block_d代表被複製的目的地 
    int w,h;
    for(w=0;w<block_w;w++){
        for(h=0;h<block_h;h++){
            block_d[h][w] = block_s[h][w];
        }
    }
}

void rotate_block(){//順時針旋轉90度 
    int temp[block_h][block_w] = {0};
    copy_block(block_cur,temp);//做出兩個block，來做對應(cur<-temp) 
    int w,h;
    for(w=0;w<block_w;w++){
    	for(h=0;h<block_h;h++){
    		block_cur[h][w] = temp[w][block_h-1-h];
		}
	}
}

void make_new_block(){//生成一個新的block 
    //隨機選擇一種方塊O,J,L,I,Z,S,T
    enum block_type type = (int)(rand()%7);
    //將block_cur變成隨機產生的方塊(產生方塊)
	while(type == curtype){ //如果還是同一個，就重選
		type = (int)(rand()%7);
	}
	curtype = type;
    switch(type){
        case O:
            copy_block(blockO,block_cur);
            break;
        case J:
            copy_block(blockJ,block_cur);
            break;
		case L:
			copy_block(blockL,block_cur);
            break;
		case I:
			copy_block(blockI,block_cur);
            break;
		case Z:
			copy_block(blockZ,block_cur);
            break;
		case S:
			copy_block(blockS,block_cur);
            break;
		case T:
			copy_block(blockT,block_cur);
            break;
        default:
            break;
	}
    //產生的位子
    cur_x = (WIDTH-block_w)/2;
    cur_y = 0;
    printBlock(block_cur,cur_x,cur_y);
}

void move_to_land(){
	while(isCollision(cur_x,cur_y+1)!=1){
		cur_y++;
	}
	land_block();//落地 
	check_line();
	make_new_block();//生成新的block
	printBlock(block_cur,cur_x,cur_y);
	if(isCollision(cur_x,cur_y)==1){ //結束點
		close_timer();
	}
}

void move_block_down(){//把現在的方塊向下移動，並且做無法移動的處理
    if(isCollision(cur_x,cur_y+1)==0){ 
		eraseBlock(block_cur,cur_x,cur_y);
		cur_y++;
		printBlock(block_cur,cur_x,cur_y);
	}
	else{
		land_block();//落地 
		check_line();
		make_new_block();//生成新的block
		printBlock(block_cur,cur_x,cur_y);
		if(isCollision(cur_x,cur_y)==1){ //結束點
			close_timer();
		}
	}
}

/*按鍵控制 */ 
void key_control(){
	MSG msg;
	while(1){
		if(PeekMessage(&msg,NULL,WM_TIMER,WM_TIMER,PM_REMOVE)!=0){//如果timer的倒數結束，會發訊息到這裡 
		    move_block_down();
		}
		if(kbhit()!=0){
			char ch=getch();
			color(1);
			if(ch == up){
				eraseBlock(block_cur,cur_x,cur_y);
				rotate_block();
				//判斷有沒有發生碰撞，如果有再轉三次把它轉回來 
				if(isCollision(cur_x,cur_y)==1)//發生碰撞 
				{
					rotate_block();
					rotate_block();
					rotate_block();
				}
				printBlock(block_cur,cur_x,cur_y);
			}
			else if(ch == down){
				if(isCollision(cur_x,cur_y+1)==0){
					eraseBlock(block_cur,cur_x,cur_y);
					cur_y++;
					printBlock(block_cur,cur_x,cur_y);
				}
				
			}
			else if(ch == left){
				if(isCollision(cur_x-1,cur_y)==0){
					eraseBlock(block_cur,cur_x,cur_y);
					cur_x--;
					printBlock(block_cur,cur_x,cur_y);
				} 
			}
			else if(ch == right){
				if(isCollision(cur_x+1,cur_y)==0){
					eraseBlock(block_cur,cur_x,cur_y);
					cur_x++;
					printBlock(block_cur,cur_x,cur_y);
				} 
			}
			else if(ch == 32){
				if(isCollision(cur_x,cur_y)==0){
					move_to_land();
				}
			}	
			else if(ch == 'c'){
				if(change > 0 && notchange == 0){
					change -= 1; //次數-1
					notchange = 1;
					eraseBlock(block_cur,cur_x,cur_y);
					make_new_block();
				}
			}
		} 
	}
} 

void game(){
	hand=GetStdHandle(STD_OUTPUT_HANDLE);
 	srand(time(NULL));//隨機!
 	setCursorVisible(0);
	set_timer(tm);
	//把screen初始化，不然有可能出現殘餘值 
 	int i,j;
 	for(i=0;i<WIDTH;i++){
		for(j=0;j<HEIGHT;j++){
			screen[j][i]=0;
		}
	}
 	printScreen();
 	make_new_block();
	key_control();
	system("cls");
	return ;
}

int main(){
	system("cls");
	for(int i = 0 ; i < 27 ; i++){
		printf("%s", ascii[i]);
	}
	while(1){
		if(kbhit() != 0){
			char ch = getch();
			switch(ch){
				case 32:
					game();
					break;
				case 's':
					setup();
					system("cls");
					for(int i = 0 ; i < 27 ; i++){ //把畫面重印
						
						printxy(ascii[i], 0,i);
						
					}
					break;
			}
		}
		
	}
 	return 0;
 }