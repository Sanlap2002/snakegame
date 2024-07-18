#include <bits/stdc++.h> 
#include<conio.h> 
#include<windows.h>

using namespace std;
#define MAX_LENGTH 1000
#define height 50
#define width 20
int consoleWidth, consoleHeight;

const char DIR_UP = 'U';
const char DIR_DOWN = 'D';
const char DIR_LEFT = 'L';
const char DIR_RIGHT = 'R';

struct Point{
    int xcoord;
    int ycoord;
    Point(){
    }
    Point(int x, int y)
    {
        xcoord = x;
        ycoord = y;
    }
};


void initScreen(){
    HANDLE hconsole=GetStdHandle(STD_OUTPUT_HANDLE); //hconsole is a handle to interfere with the console screen buffer
    CONSOLE_SCREEN_BUFFER_INFO csbi; // This variable stores the console screen buffer info
    GetConsoleScreenBufferInfo(hconsole,&csbi); 
    consoleHeight=csbi.srWindow.Bottom-csbi.srWindow.Top+1;
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    }

class Snake{
     
     int length;
     char direction;

   public:
      Point body[ MAX_LENGTH];  
      Snake(int x,int y){
       length = 1;
       body[0] = Point(x,y);
       direction = DIR_RIGHT;
      }
       void changeDirection(char newDirection){
        if(newDirection == DIR_UP && direction != DIR_DOWN)
        {
            direction = newDirection;
        }
        else if(newDirection == DIR_DOWN && direction != DIR_UP)
        {
            direction = newDirection;
        }
        else if(newDirection == DIR_LEFT && direction != DIR_RIGHT)
        {
            direction = newDirection;
        }
        else if(newDirection == DIR_RIGHT && direction != DIR_LEFT)
        {
            direction = newDirection;
        }
    }
    bool move(Point food){
        for(int i= length-1;i>0;i--)  
        {
            body[i] = body[i-1];
        }

         switch(direction)
        {
            int val;   //In the console going up means decrease in the y coordinate
            case DIR_UP:
                val = body[0].ycoord;
                body[0].ycoord = val-1;
                break;
            case DIR_DOWN:
                val = body[0].ycoord;
                body[0].ycoord = val+1;
                break;
            case DIR_RIGHT:
                val = body[0].xcoord;
                body[0].xcoord = val+1;
                break;
            case DIR_LEFT:
                val = body[0].xcoord;
                body[0].xcoord = val-1;
                break;

        }
         //snake bites itself
        for(int i=1;i<length;i++)
        {
            if(body[0].xcoord == body[i].xcoord && body[0].ycoord == body[i].ycoord)
            {
                return false;
            }
        }
        
        //If snake eats the food
        if(food.xcoord == body[0].xcoord && food.ycoord == body[0].ycoord)
        {
            body[length] = Point(body[length-1].xcoord, body[length-1].ycoord);
            length++;
        }

        return true;



    }
    int getlength(){
        return length;
    }




};

class Board{
     Snake *snake;
     const char SNAKE_BODY='0';
     Point food;
     const char FOOD='o';
     int score;
  public:
     Board(){
        spawnFood();
        snake=new Snake(10,10);
        score=0;
     }   

     ~Board(){
        delete snake;
     }
      void spawnFood(){
        int x = rand() % consoleWidth;
        int y = rand() % consoleHeight;
        food = Point(x, y);
    }

     bool update(){
        bool isalive=snake->move(food);
        if(isalive==false) return false;
        //If the snake consumes the food then increment the score by 1 and respawn the food
        if(food.xcoord==snake->body[0].xcoord && food.ycoord==snake->body[0].ycoord){
            score=score+1;
            spawnFood();
        }
        return true;
     }
     void getInput(){
        if(kbhit()){
            int key=getch();
            if(key == 'w' || key == 'W')
            {
                snake->changeDirection(DIR_UP);
            }
            else if(key == 'a' || key == 'A')
            {
                snake->changeDirection(DIR_LEFT);
            }
            else if(key == 's' || key == 'S')
            {
                snake->changeDirection(DIR_DOWN);
            }
            else if(key == 'd' || key == 'D')
            {
                snake->changeDirection(DIR_RIGHT);
            }
        }
     }
void displayCurrentScore(){
        gotoxyonconsole(consoleWidth/2,0);
        cout<<"Current Score : "<< score;
    }

     void gotoxyonconsole(int x,int y){
             COORD coord;
            coord.X = x;
            coord.Y = y;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
     }
     int getScore(){
        return score;}

     void draw(){
        system("cls"); //Clears the console screen
        for(int i=0;i<snake->getlength();i++){
            //Go to the coordinates of the snakes body and add '0'
            gotoxyonconsole(snake->body[i].xcoord,snake->body[i].ycoord);
            cout<<SNAKE_BODY;
        }
        gotoxyonconsole(food.xcoord,food.ycoord);
        cout<<FOOD;
        displayCurrentScore();
     }
     
};



int main(){
    srand(time(NULL)); //Seeds the rand() function such that everytime we execute this function the rand() function doesn't produce the same sequence of random numbers
    initScreen(); //This function captures the consoleheight and consolewidth 
    Board *board=new Board(); // Initializes the board by determining generating food,setting the coordinates of the snake's head and setting the score to 0.
    while(board->update())   //update function of board is checking the snake is alive or not
    {
        board->getInput();  //It's only function is to take the user input for directions
        board->draw();      
        Sleep(100);
    }

    cout<<"Game over"<<endl;
    cout<<"Final score is :"<<board->getScore();
    return 0;
}