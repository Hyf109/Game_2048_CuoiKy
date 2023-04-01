#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>


using namespace std;

const int SCREEN_WIDTH = 1560;
const int SCREEN_HEIGHT = 720;
const string WINDOW_TITLE = "An Implementation of Code.org Painter";

constexpr int edge = 50;
constexpr int width = 1000;
constexpr int height = 500;

SDL_Window* window;
SDL_Renderer* renderer;

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);

void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal = false);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);

void waitUntilKeyPressed();

SDL_Texture* loadTexture (string path, SDL_Renderer* renderer)
{
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if( loadedSurface == nullptr)
    {
        cout << "Unable to load image " << path << " SDL_Image Error:" << endl;

    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        SDL_FreeSurface( loadedSurface );
    }
    return newTexture;
}

int board[4][4];
int temp[4][4];
int canPlay = 1;


SDL_Rect backgroundBoard[4][4];

SDL_Rect emptyBackgroundBoard;

void banDau()
{
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            backgroundBoard[i][j].w=50;
            backgroundBoard[i][j].h=50;
        }
    }
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            backgroundBoard[i][j].x=90+90*j;
            backgroundBoard[i][j].y=90+90*i;
        }
    }

}

void toMau(SDL_Rect* rect,int n)
{
    string s;
    switch(n)
    {
    case 0:
        s="00.jpg";
        break;
    case 2:
        s="02.jpg";
        break;
    case 4:
        s="04.jpg";
        break;
    case 8:
        s="08.jpg";
        break;
    case 16:
        s="16.jpg";
        break;
    case 32:
        s="32.jpg";
        break;
    case 64:
        s="64.jpg";
        break;
    case 128:
        s="128.jpg";
        break;
    case 256:
        s="256.jpg";
        break;
    case 512:
        s="512.jpg";
        break;
    case 1024:
        s="1024.jpg";
        break;
    case 2048:
        s="2048.jpg";
        break;
    case 4096:
        s="4096.jpg";
        break;
    case 8192:
        s="8192.jpg";
        break;
    case 16384:
        s="16384.jpg";
        break;
    case 32768:
        s="32768.jpg";
        break;
    case 65536:
        s="65536.jpg";
        break;

    }
    SDL_Texture *background = loadTexture(s,renderer);
    SDL_RenderCopy(renderer, background, NULL, rect);
    SDL_RenderPresent(renderer);
}




void copyBoard()
{
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            temp[i][j]==board[i][j];
}



void printUI()
{
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {

            toMau(&backgroundBoard[i][j],board[i][j]);
        }
    }
}





pair<int,int> generateUnoccupiedPosition() //tao ra cap so ngau nhien
{
    int occupied = 1, line, column;
    while (occupied)
    {
        line = rand() % 4;
        column = rand() % 4;
        if(board[line][column]==0)
            occupied = 0;
    }
    return make_pair(line,column);
}


void newGame() //van moi
{
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            board[i][j]=0;

    pair<int,int> pos = generateUnoccupiedPosition();
    board[pos.first][pos.second] = 2;
}

int checkEmpty(int tmp[4][4],int a[4][4]) //ktra con cho trong khong
{
    int fl=1,i,j;
    for(i=0; i<4; i++)
        for(j=0; j<4; j++)
            if(tmp[i][j]!=a[i][j])
            {
                fl=0;
                break;
            }
    return fl;
}

int checkOver(int a[4][4]) // ktra con choi duoc tiep hay khong
{
    int fl=0,gl=0,i,j;
    for(i=0; i<4; i++)
        for(j=0; j<4; j++)
            if(a[i][j]==0)
            {
                fl=1;
                break;
            }

    for(i=0; i<3; i++)
        for(j=0; j<3; j++)
            if(a[i+1][j]==a[i][j] || a[i][j+1]==a[i][j])
            {
                gl=1;
                break;
            }

    if(fl || gl) return 1;
    else return 0;
}

void upMove(int a[4][4]) //di chuyen len
{
    int i,j,li,ri;
    for(j=0; j<4; j++)
    {
        li=0,ri=j;
        for(i=1; i<4; i++)
        {
            if(a[i][j]!=0)
            {
                if(a[i-1][j]==0 || a[i-1][j]==a[i][j])
                {
                    if(a[li][ri]==a[i][j])
                    {
                        a[li][ri]*=2;
                        a[i][j]=0;
                    }
                    else
                    {
                        if(a[li][ri]==0)
                        {
                            a[li][ri]=a[i][j];
                            a[i][j]=0;
                        }
                        else
                        {
                            a[++li][ri]=a[i][j];
                            a[i][j]=0;
                        }
                    }
                }
                else li++;
            }
        }
    }
}

void downMove(int a[4][4]) //di chuyen xuong
{
    int i,j,li,ri;
    for(j=0; j<4; j++)
    {
        li=3,ri=j;
        for(i=2; i>=0; i--)
        {
            if(a[i][j]!=0)
            {
                if(a[i+1][j]==0 || a[i+1][j]==a[i][j])
                {
                    if(a[li][ri]==a[i][j])
                    {
                        a[li][ri]*=2;
                        a[i][j]=0;
                    }
                    else
                    {
                        if(a[li][ri]==0)
                        {
                            a[li][ri]=a[i][j];
                            a[i][j]=0;
                        }
                        else
                        {
                            a[--li][ri]=a[i][j];
                            a[i][j]=0;
                        }
                    }
                }
                else li--;
            }
        }
    }
}

void leftMove(int a[4][4]) //di chuyen trai
{
    int i,j,li,ri;
    for(i=0; i<4; i++)
    {
        li=i,ri=0;
        for(j=1; j<4; j++)
        {
            if(a[i][j]!=0)
            {
                if(a[i][j-1]==0 || a[i][j-1]==a[i][j])
                {
                    if(a[li][ri]==a[i][j])
                    {
                        a[li][ri]*=2;
                        a[i][j]=0;
                    }
                    else
                    {
                        if(a[li][ri]==0)
                        {
                            a[li][ri]=a[i][j];
                            a[i][j]=0;
                        }
                        else
                        {
                            a[li][++ri]=a[i][j];
                            a[i][j]=0;
                        }
                    }
                }
                else ri++;
            }
        }
    }
}

void rightMove(int a[4][4]) //di chuyen phai
{
    int i,j,li,ri;
    for(i=0; i<4; i++)
    {
        li=i,ri=3;
        for(j=2; j>=0; j--)
        {
            if(a[i][j]!=0)
            {
                if(a[i][j+1]==0 || a[i][j+1]==a[i][j])
                {
                    if(a[li][ri]==a[i][j])
                    {
                        a[li][ri]*=2;
                        a[i][j]=0;
                    }
                    else
                    {
                        if(a[li][ri]==0)
                        {
                            a[li][ri]=a[i][j];
                            a[i][j]=0;
                        }
                        else
                        {
                            a[li][--ri]=a[i][j];
                            a[i][j]=0;
                        }
                    }
                }
                else ri--;
            }
        }
    }
}



int main(int argc, char* argv[])
{
    srand(time(0));

    emptyBackgroundBoard.x=50;
    emptyBackgroundBoard.y=50;
    emptyBackgroundBoard.w=400;
    emptyBackgroundBoard.h=400;

    newGame();
    copyBoard();



    initSDL(window, renderer);
// Your drawing code here
// use SDL_RenderPresent(renderer) to show it

    SDL_SetRenderDrawColor(renderer,148,175,159,2); //ve bang
    SDL_RenderFillRect(renderer,&emptyBackgroundBoard);
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);

    banDau();
    printUI();

    SDL_Event e;
    while (true)
    {
        while(!canPlay)
        {
            return 0;
        }
        copyBoard();
        printUI();

        if(SDL_WaitEvent(&e)==0) SDL_Delay(50);
        else if(e.type==SDL_QUIT) break;
        else if(e.type == SDL_KEYDOWN)
        {

            switch(e.key.keysym.sym)
            {
            case SDLK_n:
            {
                newGame();
                break;
            }
            case SDLK_q:
            {
                return 0;
                break;
            }
            case SDLK_w:
            {
                upMove(board);
                if(!checkEmpty(temp,board))
                {
                    pair<int,int> pos = generateUnoccupiedPosition();
                    board[pos.first][pos.second] = 2;
                }
                if(!checkOver(board))
                {
                    canPlay=0;
                    break;
                }
                break;
            }
            case SDLK_s:
            {
                downMove(board);
                if(!checkEmpty(temp,board))
                {
                    pair<int,int> pos = generateUnoccupiedPosition();
                    board[pos.first][pos.second] = 2;
                }
                if(!checkOver(board))
                {

                    canPlay=0;
                    break;
                }
                break;
            }
            case SDLK_a:
            {
                leftMove(board);
                if(!checkEmpty(temp,board))
                {
                    pair<int,int> pos = generateUnoccupiedPosition();
                    board[pos.first][pos.second] = 2;
                }
                if(!checkOver(board))
                {

                    canPlay=0;
                    break;
                }
                break;
            }
            case SDLK_d:
            {
                rightMove(board);
                if(!checkEmpty(temp,board))
                {
                    pair<int,int> pos = generateUnoccupiedPosition();
                    board[pos.first][pos.second] = 2;
                }
                if(!checkOver(board))
                {

                    canPlay=0;
                    break;
                }
                break;
            }

            }

        }

    }


    /*SDL_Texture* background = loadTexure("02.jpg",renderer);
    SDL_RenderCopy(renderer, background, NULL, &box00.rect);
    SDL_RenderPresent(renderer);*/

    waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}

void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal)
    {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);
    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
//window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
    // SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);
//Khi chạy trong môi trường bình thường (không chạy trong máy ảo)
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC);
//Khi chạy ở máy ảo (ví dụ tại máy tính trong phòng thực hành ở trường)
//renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(renderer,219,228,198,2);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}
void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true)
    {
        if ( SDL_WaitEvent(&e) != 0 &&
                (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}

//code
