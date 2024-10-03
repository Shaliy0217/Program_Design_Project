#include<stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#define CANVAS 50

int highscore = 0;
char jump = 32;
char fir = 99;

void setup(){
	int cho;
	char change;
	while(1){
		//輸出原本的設定
		puts("\n\t\t\t  *SETTING*");
		printf("\n\t\t  JUMP, START : '%c'", jump);
		printf("\n\t\t\t FIRE : '%c'", fir);
		printf("\n\n\t1 to change jump, 2 to change fire, 3 to back ? ");

		scanf("%d", &cho);
		switch(cho){
			case 1:
				printf("\tplease enter the button you want to use: ");
				getchar();
				scanf("%c", &change);
				if(change == fir || change == 'a'){//用到重複符號
					printf("\n\t\tyou can't choose that button\n");
					Sleep(600);
					system("cls");
				}
				else{ //成功修改按鍵
					printf("\t\tChange complete!");
					jump = change;
					Sleep(600);
					system("cls");
				}
				break;
			case 2:
				printf("\tPlease enter the button you want to use: ");
				getchar();
				scanf("%c", &change);
				if(change == jump || change == 'a'){ //用到重複符號
					printf("\t\tYou can't choose that button\n");
				}
				else{ //成功修改按鍵
					fir = change;
					printf("\t\tChange complete!");
					Sleep(600);
					system("cls");
				}
				break;
			case 3:
				system("cls");
				return ;
			default:
				printf("\t\t Error, please choose again\n");
                Sleep(600);
				system("cls");
		}

	}
}

void display(char floor[],int lowerlimit,int upperlimit){
	int i;
    for(i =  lowerlimit; i < upperlimit ; i++){
    	printf("%c", floor[i]);
	}
}
void shiftLeft(char *floor, char sign){
    int i;
    for(i = 0;i<CANVAS-1;i++)
    {
        floor[i] = floor[i+1];
    }
    floor[CANVAS-1] = sign;
}

void insertBarrier(char *floor,int n){
    int r=rand()%n;
	if(floor[CANVAS-r] != 'S'){
		floor[CANVAS-r]='X';
	}
}
void insertskyBarrier(char *floor, char *ground, int n){
    int r=rand()%n;
	if(ground[CANVAS-r] != 'X'){ //上下不能同時出現障礙物
		floor[CANVAS-r]='X';
	}
}
void insertsuperstar(char *floor){ //無敵星星
    int r=rand()%15;
	if(r == 1){
		floor[CANVAS-1]='S';
	}
    
}
int estar(int curfloor,int playerpos, char floor1[]){
	if(floor1[playerpos] == 'S'){
		return 1;
	}
	return 0;
}
int isDead(int curfloor,int playerpos, char floor1[]){
	if(floor1[playerpos] == 'X'){
		return 1;
	}
	return 0;
}

void dinoart1(){
	printf("\n");
	printf("\t                          _ _                                \n");
	printf("\t             __          | (_)                               \n");
	printf("\t            / _)       __| |_ _ __   ___    _ __ _   _ _ __  \n");
	printf("\t     .-^^^-/ /        / _` | | '_ \\ / _ \\  | '__| | | | '_ \\ \n");
	printf("\t  __/       /        | (_| | | | | | (_) | | |  | |_| | | | |\n");
	printf("    _____<__.|_|-|_|__________\\__,_|_|_|_|_|\\___/__|_|___\\__,_|_|_|_|__\n");
	if(jump == 32){ //ascii的SPACE會變空格
		puts("\n\n\t\t\tPRESS SPACE TO START");
	}
	else{
		printf("\n\n\t\t\t  PRESS %c TO START", jump);
	}
	puts("\n\t\t     OR a TO SET UP THE BUTTON");
}
void dinoart2(){
	printf("\n");
	printf("\t                          _ _                                \n");
	printf("\t             __          | (_)                               \n");
	printf("\t            / _)       __| |_ _ __   ___    _ __ _   _ _ __  \n");
	printf("\t     .-^^^-/ /        / _` | | '_ \\ / _ \\  | '__| | | | '_ \\ \n");
	printf("\t  __/       /        | (_| | | | | | (_) | | |  | |_| | | | |\n");
	printf("    _____<__.-|_||_|__________\\__,_|_|_|_|_|\\___/__|_|___\\__,_|_|_|_|__\n");
	if(jump == 32){ //ascii的SPACE會變空格
		puts("\n\n\t\t\tPRESS SPACE TO START");
	}
	else{
		printf("\n\n\t\t\t  PRESS %c TO START", jump);
	}
	puts("\n\t\t     OR a TO SET UP THE BUTTON");
}
void dinoart3(){
	printf("\n");
	printf("\t                          _ _                                \n");
	printf("\t              __         | (_)                               \n");
	printf("\t             / _)      __| |_ _ __   ___    _ __ _   _ _ __  \n");
	printf("\t      .-^^^-/ /       / _` | | '_ \\ / _ \\  | '__| | | | '_ \\ \n");
	printf("\t   __/       /       | (_| | | | | | (_) | | |  | |_| | | | |\n");
	printf("    ______<__.|_|-|_|_________\\__,_|_|_|_|_|\\___/__|_|___\\__,_|_|_|_|__\n");
	if(jump == 32){ //ascii的SPACE會變空格
		puts("\n\n\t\t\tPRESS SPACE TO START");
	}
	else{
		printf("\n\n\t\t\t  PRESS %c TO START", jump);
	}
	puts("\n\t\t     OR a TO SET UP THE BUTTON");
}

void game(){
    int playerpos = 6;
    char floor1[6][CANVAS];
    int sleeptime = 250;
    int runtime = sleeptime * 2;
    int curfloor = 1;
    int score = 0;
    int lose;
	int eatstar;
	int star = 0;
	int fire = 15 * 600;
	int scope = 0;
    for (int i=0 ; i<CANVAS ; i++){
        floor1[2][i] = '_';
        floor1[1][i] = ' ';
        floor1[0][i] = ' ';
    }
	floor1[2][40] = 'S';
    
	while(1){
    	score++;
		//噴火
		if(fire > 0){ //減少冷卻時間
            fire -= sleeptime;
        }
        else if(fire <= 0 &&(kbhit()&&getch()==fir)){
            scope = 1;
            fire = 15 * 600; //冷卻15秒
        }

        for(int i = 0 ; i < playerpos+1 ; i++){
            if(floor1[2][i] == '^'){ //火焰只在小恐龍前面燃燒
                floor1[2][i] = '_';
            }
        }
        if(scope < 5 && scope != 0){ //火焰範圍慢慢擴大
            scope++;
            for(int i = 0 ; i < scope ; i++){ //增加火焰到陣列
                floor1[2][playerpos+i*3] = '^';
                floor1[2][playerpos+i*3+1] = '^';
                floor1[2][playerpos+i*3+2] = '^';
            }
        }
        else if(scope == 5){
            scope = 0; //把範圍歸零
        }
		
		
		//畫面頂端道具時間
		if(star > 0 && fire > 0){
			star -= sleeptime;
			printf("\n\tFIRE LOADING : %d STAR : %d", fire / 600, star / 600);
		}
		else if(star <= 0 && fire > 0){
			printf("\n\tFIRE LOADING : %d", fire / 600);
		}
		else if(star > 0 && fire <= 0){
			star -= sleeptime;
			printf("\n\tFIRE READY   STAR : %d", star / 600);
		}
		else{
			printf("\n\tFIRE READY");
		}
    	printf("\n\n\t");

		


    	if(curfloor == 2){
			printf("\n\t");
			display(floor1[0], 0, playerpos);//印出天空
			printf("0");
			display(floor1[0], playerpos+1, CANVAS);//印出天空
			printf("\n\t");
			display(floor1[1], 0, CANVAS);
			printf("\n\t");
			display(floor1[2], 0, CANVAS);
			printf("\n\n");
		    printf("\tScore : %d\t\t Highest Score : %d", score, highscore);
			if(star > 0 && floor1[0][playerpos-1] == 'X'){ //無敵時碰到障礙物
				floor1[0][playerpos] = ' ';
			}

			lose = isDead(curfloor,playerpos,floor1[0]);
			if(lose == 1 && star == 0){
				if(score > highscore){
					highscore = score;
					printf("\n\n\t\t\tNEW RECORD!");
				}
				if(jump == 32){
					printf("\n\t\t\t GAME OVER\n\t\tPRESS SPACE TO START AGAIN");
				}
				else{
					printf("\n\t\t\t GAME OVER\n\t\tPRESS %c TO START AGAIN", jump);
				}
				return ;
			}

		    Sleep(sleeptime);
		    system("cls");
		    runtime -= sleeptime;
		    if(runtime <= 0){
		    	curfloor = 1;
		    	runtime = sleeptime * 2;
			}
        }
        else if(curfloor == 1){
			
	        printf("\n\t");
			display(floor1[0], 0, CANVAS);//印出天空
			printf("\n\t");
			display(floor1[1], 0, CANVAS);//印出天空
			printf("\n\t");
			display(floor1[2], 0, playerpos);
			printf("O");
			display(floor1[2], playerpos+1, CANVAS);//印出地板
			printf("\n\n");
			printf("\tScore : %d\t\t Highest Score : %d", score, highscore);

			if(star > 0 && floor1[2][playerpos-1] == 'X'){ //無敵時碰到障礙物
				floor1[0][playerpos] = ' ';
			}

			lose = isDead(curfloor,playerpos,floor1[2]);
			eatstar = estar(curfloor,playerpos,floor1[2]);

			if(lose == 1 && star == 0){
				if(score > highscore){
					highscore = score;
					printf("\n\n\t\t\tNEW RECORD!");
				}
				if(jump == 32){
					printf("\n\t\t\t GAME OVER\n\t\tPRESS SPACE TO START AGAIN");
				}
				else{
					printf("\n\t\t\t GAME OVER\n\t\tPRESS %c TO START AGAIN", jump);
				}
				
				return ;
			}
			if(eatstar == 1){ //無敵狀態撞到的障礙物變成地板
				floor1[2][playerpos] = '_';
				star = 5 * 600; //無敵5秒
			}
		    Sleep(sleeptime);
			
		    system("cls");
		}

		shiftLeft(floor1[2], '_');
		shiftLeft(floor1[0], ' ');
		if(kbhit()&&getch()==jump){
			curfloor = 2;
		}
		
		if(score % 10 == 0){
			insertsuperstar(floor1[2]);
			insertBarrier(floor1[2], 20);
		}
		else if(score %16 == 0){
			insertskyBarrier(floor1[0],floor1[2], 20); //空中障礙物
		}
		if(score % 100 == 0){ //加速
			sleeptime -= 20;
		}
		
	}
}

int main(){
		while(1){
		// 開頭動畫
		dinoart1();
		Sleep(500);
		system("cls");

		dinoart2();
		Sleep(500);
		system("cls");

		dinoart3();
		Sleep(500);
		system("cls");
		if(kbhit()&&getch()==97){
			setup();
		}
		else if(kbhit()&&getch()==jump){//跑完動畫後開始遊戲
    		system("cls");
    		game();
			break;
    	}
	}
	
    while(1){
    	if(kbhit()&&getch()==jump){ //開始遊戲
    		system("cls");
    		game();
    	}
	}
}

/*
ascii art
                         _ _                                
            __          | (_)                               
           / _)       __| |_ _ __   ___    _ __ _   _ _ __  
    .-^^^-/ /        / _` | | '_ \ / _ \  | '__| | | | '_ \ 
 __/       /        | (_| | | | | | (_) | | |  | |_| | | | |
<__.|_|-|_|__________\__,_|_|_|_|_|\___/__|_|___\__,_|_|_|_|__

      _ _                                
     | (_)                               
   __| |_ _ __   ___    _ __ _   _ _ __  
  / _` | | '_ \ / _ \  | '__| | | | '_ \ 
 | (_| | | | | | (_) | | |  | |_| | | | |
  \__,_|_|_| |_|\___/  |_|   \__,_|_| |_| 

*/