/////////////////////////
//SIMPLE PING PONG GAME//
/////////////////////////

#include<iostream>
#include<windows.h>
#include<conio.h>
#include<cstdlib>
#include<time.h>

using namespace std;
//color defination
enum color//color count start from black is 0
{	
	BLACK		 ,
	BLUE_DARK	 ,
	GREEN_DARK	 ,
	CYAN_DARK	 ,
	RED_DARK	 ,
	PURPLE_DARK	 ,
	YELLOW_DARK	 ,
	WHITE	 	 ,
	GRAY	 	 ,
	BLUE_BRIGHT	 ,
	GREEN_BRIGHT ,
	CYAN_BRIGHT	 ,
	RED_BRIGHT	 ,
	PURPLE_BRIGHT,
	YELLOW_BRIGHT
};
enum direction
{
	LEFT	  ,
	RIGHT	  ,
	UP		  ,
	DOWN	  ,
	UP_LEFT	  ,
	UP_RIGHT  ,
	DOWN_LEFT ,
	DOWN_RIGHT
};
struct player {
	int x;
	int y;
};
struct ball
{
	int x;
	int y;
	int dir;
};
/////////////console Fix funtion
void textColor(int x);

void gotoxy(int Xpos, int Ypos);

void clear();

//////////////////////////////
////game loop function
void catchEvent(bool &gameEnd,int &bar1Xpos,int &bar2Xpos,bool &gameStart);

void Update(struct  player &  bar1,int & bar1Xpos,struct  player & bar2,int & bar2Xpos,int **map,struct ball& ballTmp,bool& gameStart,int &score1,int &score2 );

void Draw(int **map,const int & score1,const int & score2);

void setMap(int **map);
//////////////
/////////////player funtion
void setPlayer(struct player& playerTmp,int Xindex,int Yindex,int **map);

void updatePlayer(struct player& playerTmp,int &newX,int **map);
/////////////////////////////
/////ball funtion
void updateBall(struct ball &ballTmp,bool&gameStart,int & score1,int &score2,int **map);
////////////////////////////
#define screenWidth   60
#define screenHeight  28
#define barWidth 4

int main(){
	srand(time(0));
	//the condition to end game
	bool gameEnd=false;
	bool gameStart=false;
	//Initialize the map define
	int **map=(int**)malloc(screenHeight*sizeof(int*));
	for(int i=0;i<screenHeight;i++)
	{
		map[i]=(int*)calloc(screenWidth,sizeof(int));
	}
	
	//set map
	setMap(map);
	//player define
	player player1,player2;
	//set the player first position
	setPlayer(player1,screenHeight/2-2,1,map);
	setPlayer(player2,screenHeight/2-2,screenWidth-2,map);
	//position of two player at the  first place 
	int bar1Xpos=player1.x;
	int bar2Xpos=player2.x;
	//set player score
	int score1=0;
	int score2=0;
	
	//ball define
	ball pinPong;
	int Xball=pinPong.x=screenHeight/2;
	int Yball=pinPong.y=screenWidth/2;
	pinPong.dir=(rand()%2==1 ? UP:DOWN);
	//the game Loop
	while(!gameEnd)
	{
		catchEvent(gameEnd,bar1Xpos,bar2Xpos,gameStart);
		Update(player1,bar1Xpos,player2,bar2Xpos,map,pinPong,gameStart,score1,score2);
		Draw(map,score1,score2);
	}
	_getch();
	return 0;
}
void textColor(int x)
{
	HANDLE mau;
	mau = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau, x);
}

void gotoxy(int x, int y)
{
	HANDLE hConsoleOutput;
	COORD Cursor_an_Pos = { x - 1,y - 1 };
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

void clear()
{
	HANDLE hOut;
	COORD Position;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Position.X = 29;
	Position.Y = 2;
	SetConsoleCursorPosition(hOut, Position);
}

void catchEvent(bool &gameEnd,int &bar1Xpos,int &bar2Xpos,bool &gameStart)
{
	if(GetAsyncKeyState(VK_ESCAPE))
	{
		//game end condition
		gameEnd=true;
	}
	//game start condition
	if(GetAsyncKeyState(VK_SPACE)&&!gameStart)
	{
		gameStart =true;
	}
	//controls function of player 2
	if(GetAsyncKeyState(VK_UP))
	{
		bar2Xpos--;
		//limit the X posittion
		if(bar2Xpos==0)
			bar2Xpos=1;
	}
	else if(GetAsyncKeyState(VK_DOWN))
	{
		bar2Xpos++;
		//limit the X posittion
		if(bar2Xpos>=screenHeight-barWidth)
			bar2Xpos=screenHeight-barWidth-1;
	}
	//control function of player 1
	if(GetAsyncKeyState(0x57))//0x57 is virtual key for W
	{
		bar1Xpos--;
		//limit the X posittion
		if(bar1Xpos==0)
			bar1Xpos=1;
	}
	else if(GetAsyncKeyState(0x53))//0x53 is virtual key for S
	{
		bar1Xpos++;
		//limit the X posittion
		if(bar1Xpos==screenHeight-barWidth)
			bar1Xpos=screenHeight-barWidth-1;
	}
		
	return ;
}

void Update(struct  player &  bar1,int & bar1Xpos,struct  player & bar2,int & bar2Xpos,int **map,struct ball& ballTmp,bool& gameStart,int &score1,int &score2 )
{
	updatePlayer(bar1,bar1Xpos,map);
	updatePlayer(bar2,bar2Xpos,map);
	updateBall(ballTmp,gameStart,score1,score2,map);
	return ;
}

void Draw(int **map,const int & score1,const int & score2)
{	
	//wipeout the console screen 
	clear();
	//Draw the instruction 
	gotoxy(5,10);
	cout<<"PRESS 'W' TO MOVE UP";
	gotoxy(5,15);
	cout<<"PRESS 'S' TO MOVE DOWN";
	gotoxy(5,25);
	cout<<"PRESS 'Esc' TO ESCAPE";
	
	gotoxy(90,10);
	cout<<"PRESS 'Arrow_Up' TO MOVE UP";
	gotoxy(90,15);
	cout<<"PRESS 'Arrow_Down' TO MOVE DOWN";
	gotoxy(90,25);
	cout<<"PRESS 'Space' TO START GAME";
	//draw score 
	gotoxy(10,5);
	cout<<"SCORE:"<<score1;
	gotoxy(100,5);
	cout<<"SCORE:"<<score2;
	//draw map
	for(int i=0;i<screenHeight;i++)
	{
		//move to the center of the screen 
		gotoxy(29,2+i);
		for(int j=0;j<screenWidth;j++)
		{
			if(map[i][j]==1)
			{
				//wall
				textColor(CYAN_BRIGHT);
				cout<<(char)219;//(char)219 is alt code
				textColor(WHITE);
			}
			else if(map[i][j]==2)
			{
				//player Bar
				textColor(RED_BRIGHT);
				cout<<(char)178;//(char)178 is alt code
				textColor(WHITE);
			}
			else if(map[i][j]==3)
			{
				//ball
				textColor(YELLOW_BRIGHT);
				cout<<(char)233;//(char)233 is alt code
			}
			else
			{
				cout<<" ";
			}
		}
		cout<<endl;
	}
	return ;
}

void setMap(int **map)
{
	for(int i=0;i<screenHeight;i++)
	{
		for(int j=0;j<screenWidth;j++)
		{
			if(i==0||i==screenHeight-1||j==0||j==screenWidth-1)
			{
				//set the wall
				map[i][j]=1;
			}
			if(i==screenHeight/2&&j==screenWidth/2)
			{
				//set the ball
				map[i][j]=3;
			}
		}
	}
}

void setPlayer(struct player& playerTmp,int Xindex,int Yindex,int **map)
{
	
	playerTmp.x=Xindex;
	playerTmp.y=Yindex;
	for(int i=0;i<barWidth;i++)
	{
		map[playerTmp.x+i][playerTmp.y]=2;
	}
	return ;
}

void updatePlayer(struct player& playerTmp,int &newX,int **map)
{
	if(newX<playerTmp.x)
	{
		//change the map value 
		map[playerTmp.x-1][playerTmp.y]=2;
		map[playerTmp.x+barWidth-1][playerTmp.y]=0;
		playerTmp.x=newX;
	}
	else if(newX>playerTmp.x)
	{
		//change the map value 
		map[playerTmp.x+barWidth][playerTmp.y]=2;
		map[playerTmp.x][playerTmp.y]=0;
		playerTmp.x=newX;
	}
	return ;
}

void updateBall(struct ball &ballTmp,bool &gameStart,int & score1,int &score2,int**map)
{
	if(gameStart)
	{
		switch(ballTmp.dir)
		{
			case UP:
				if(ballTmp.x>1)
				{
					map[ballTmp.x][ballTmp.y]=0;
					ballTmp.x-=1;
					map[ballTmp.x][ballTmp.y]=3;
				}
				else
				{
					ballTmp.dir=(rand()%2==1 ? DOWN_RIGHT:DOWN_LEFT);;
				}
				break;
			case DOWN:
				if(ballTmp.x<screenHeight-2)
				{
					map[ballTmp.x][ballTmp.y]=0;
					ballTmp.x+=1;
					map[ballTmp.x][ballTmp.y]=3;
				}
				else
				{
					ballTmp.dir=(rand()%2==1 ? UP_RIGHT:UP_LEFT);
				}
				break;
			case LEFT:
				if(ballTmp.y>2)
				{
					map[ballTmp.x][ballTmp.y]=0;
					ballTmp.y-=1;
					map[ballTmp.x][ballTmp.y]=3;
				}
				else
				{
					if(map[ballTmp.x][ballTmp.y-1]==0)
					{
						score2++;
						map[ballTmp.x][ballTmp.y]=0;
						ballTmp.x=screenHeight/2;
						ballTmp.y=screenWidth/2;
						map[ballTmp.x][ballTmp.y]=3;
						gameStart=false;
					}
					else if(map[ballTmp.x][ballTmp.y-1]==2)
					{
						ballTmp.dir= (rand()%2==1 ? UP_RIGHT:DOWN_RIGHT);
					}
				}
				break;
			case RIGHT:
				if(ballTmp.y<screenWidth-3)
				{
					map[ballTmp.x][ballTmp.y]=0;
					ballTmp.y+=1;
					map[ballTmp.x][ballTmp.y]=3;
				}
				else
				{
					if(map[ballTmp.x][ballTmp.y+1]==0)
					{
						score1++;
						map[ballTmp.x][ballTmp.y]=0;
						ballTmp.x=screenHeight/2;
						ballTmp.y=screenWidth/2;
						map[ballTmp.x][ballTmp.y]=3;
						gameStart=false;
					}
					else if(map[ballTmp.x][ballTmp.y+1]==2)
					{
						ballTmp.dir= (rand()%2==1 ? UP_LEFT:DOWN_LEFT);
					}
				}
				break;
			case UP_LEFT:
				if(ballTmp.x>1&&ballTmp.y>2)
				{
					map[ballTmp.x][ballTmp.y]=0;
					ballTmp.x-=1;
					ballTmp.y-=1;
					map[ballTmp.x][ballTmp.y]=3;
				}
				else
				{
					if(ballTmp.x==1)
					{
						ballTmp.dir=DOWN_LEFT;
					}
					else if(map[ballTmp.x-1][ballTmp.y-1]==0)
					{
						score2++;
						map[ballTmp.x][ballTmp.y]=0;
						ballTmp.x=screenHeight/2;
						ballTmp.y=screenWidth/2;
						map[ballTmp.x][ballTmp.y]=3;
						gameStart=false;
					}
					else if(map[ballTmp.x-1][ballTmp.y-1]==2)
					{
						ballTmp.dir= (rand()%2==1 ? UP_RIGHT:DOWN_RIGHT);
					}
				}
				break;
			case UP_RIGHT:
				if(ballTmp.x>1&&ballTmp.y<screenWidth-3)
				{
					map[ballTmp.x][ballTmp.y]=0;
					ballTmp.x-=1;
					ballTmp.y+=1;
					map[ballTmp.x][ballTmp.y]=3;
				}
				else
				{
					if(ballTmp.x==1)
					{
						ballTmp.dir=DOWN_RIGHT;
					}
					else if(map[ballTmp.x-1][ballTmp.y+1]==0)
					{
						score1++;
						map[ballTmp.x][ballTmp.y]=0;
						ballTmp.x=screenHeight/2;
						ballTmp.y=screenWidth/2;
						map[ballTmp.x][ballTmp.y]=3;
						gameStart=false;
					}
					else if(map[ballTmp.x-1][ballTmp.y+1]==2)
					{
						ballTmp.dir= (rand()%2==1 ? UP_LEFT:DOWN_LEFT);
					}
				}
				break;
			case DOWN_LEFT:
				if(ballTmp.x<screenHeight-2&&ballTmp.y>2)
				{
					map[ballTmp.x][ballTmp.y]=0;
					ballTmp.x+=1;
					ballTmp.y-=1;
					map[ballTmp.x][ballTmp.y]=3;
				}
				else
				{
					if(ballTmp.x==screenHeight-2)
					{
						ballTmp.dir=UP_LEFT;
					}
					else if(map[ballTmp.x+1][ballTmp.y-1]==0)
					{
						score2++;
						map[ballTmp.x][ballTmp.y]=0;
						ballTmp.x=screenHeight/2;
						ballTmp.y=screenWidth/2;
						map[ballTmp.x][ballTmp.y]=3;
						gameStart=false;
					}
					else if(map[ballTmp.x+1][ballTmp.y-1]==2)
					{
						ballTmp.dir= (rand()%2==1 ? UP_RIGHT:DOWN_RIGHT);
					}
				}
				break;
			case DOWN_RIGHT:
				if(ballTmp.x<screenHeight-2&&ballTmp.y<screenWidth-3)
				{
					map[ballTmp.x][ballTmp.y]=0;
					ballTmp.x+=1;
					ballTmp.y+=1;
					map[ballTmp.x][ballTmp.y]=3;
				}
				else
				{
					if(ballTmp.x==screenHeight-2)
					{
						ballTmp.dir=UP_RIGHT;
					}
					else if(map[ballTmp.x+1][ballTmp.y+1]==0)
					{
						score1++;
						map[ballTmp.x][ballTmp.y]=0;
						ballTmp.x=screenHeight/2;
						ballTmp.y=screenWidth/2;
						map[ballTmp.x][ballTmp.y]=3;
						gameStart=false;
					}
					else if(map[ballTmp.x+1][ballTmp.y+1]==2)
					{
						ballTmp.dir= (rand()%2==0 ? UP_LEFT:DOWN_LEFT);
					}
				}
				break;
			}
	}
	return ;
}

