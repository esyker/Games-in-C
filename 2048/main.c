/* Data :11/04/2017
Este trabalho foi realizado pelo aluno do IST Diogo Guilherme de Castro Soares Moura, nÃºmero 86976.
Pretende desenvolver o conhecido jogo 2048.
De referir, que  optei por nÃ£o desenvolver nenhuma das funcionalidades avanÃ§adas, dado que estava com constragimentos de tempo
e entendi que o desenvolvimento da mesma poderia ter implicaÃ§Ãµes no correto funcionamento das restantes funcionalidades
do projeto.
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>


#define STRING_SIZE 100       // max size for some strings
#define TABLE_SIZE 650        // main game space size
#define LEFT_BAR_SIZE 150     // left white bar size
#define WINDOW_POSX 200       // initial position of the window: x
#define WINDOW_POSY 200       // initial position of the window: y
#define SQUARE_SEPARATOR 8    // square separator in px
#define BOARD_SIZE_PER 0.7f   // board size in % wrt to table size
#define MAX_BOARD_POS 11      // maximum size of the board
#define MAX_LEVELS 23
#define MARGIN 5
#define MAX_ARRAY 100000


// declaration of the functions related to graphical issues
void InitEverything(int, int, TTF_Font **, SDL_Surface **, SDL_Window **, SDL_Renderer ** );
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int, int );
SDL_Renderer* CreateRenderer(int, int, SDL_Window *);
int RenderText(int, int, const char *, TTF_Font *, SDL_Color *, SDL_Renderer *);
int RenderLogo(int, int, SDL_Surface *, SDL_Renderer *);
void RenderTable(int, int *, int *, TTF_Font *, SDL_Surface **, SDL_Renderer *);
void RenderBoard(int [][MAX_BOARD_POS], SDL_Surface **, int, int, int, SDL_Renderer *);
void RenderStats( SDL_Renderer *_renderer, TTF_Font * _font, int score, int time, int win, int tentativas, int gameover);
void LoadValues(SDL_Surface **);
void UnLoadValues(SDL_Surface **);

// Declaration of the new functions created by student
void getgameover(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos,int *gameover);
void mergeup(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos, int*score);
void mergedown(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos, int *score);
void mergeleft(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos,int *score);
void mergeright(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos, int *score);
void storerand(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos);
void moveup(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos );
void movedown(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos );
void moveleft(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos);
void moveright(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos );
void newgame(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos);
void save_board(int board[MAX_BOARD_POS][MAX_BOARD_POS],int board_undo[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos);
void getboard_size(int *board_pos);
void getinitgame (char initgame[]);
void getdifficulty(int *difficulty);
void getwin(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos, int difficulty, int *win);
void getmaxtile (int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos, int *maxtile);
void getcurrenttime(int initialtime, int *currenttime);
void getname (char name[]);
int board_difference(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_undo[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos);
void save_stats(FILE *stats, char name[MAX_ARRAY], int maxtile, int currenttime, int score);

// definition of some strings: they cannot be changed when the program is executed !
const char myName[] = "Diogo Moura";
const char myNumber[] = "IST86976";
FILE *stats;

/**
 * main function: entry point of the program
 * only to invoke other functions !
 */
int main( int argc, char* args[] )
{



    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL;
    SDL_Surface *array_of_numbers[MAX_LEVELS], *imgs[2];
    SDL_Event event;
    int delay = 300;
    int quit = 0;
    int width = (TABLE_SIZE + LEFT_BAR_SIZE);
    int height = TABLE_SIZE;
    int square_size_px, board_size_px, board_pos = 5;
    int board[MAX_BOARD_POS][MAX_BOARD_POS] = {{0}};
    // declaration of the new variables and arrays used
    int board_undo[MAX_BOARD_POS][MAX_BOARD_POS]; // array used to save previews board
    char name[MAX_ARRAY]; // string used to store player name
    int difficulty=0; // variable used to store the game difficulty . if player gets 2^â½(difficulty) wins the game
    int score=0; // variable used to store score
    int currenttime=0;//variable used to store time of gameplay
    int tentativas=0;// variable used to store number of attempts
    int maxtile=0;// variable used to store maxtile
    int gameover=0, win=0;// variables used to see if game is won or over
    char initgame[MAX_ARRAY]= {0};// variable to see if user pressed n to initialize game
    FILE *stats;

    srand(time(NULL)); //  seed rand used to get random board number and position using rand()

    getname(name);// get the player name
    getboard_size(&board_pos);// get the board dimensions
    getdifficulty(&difficulty);// get the game difficulty( if user gets a tile with pow(2, difficulty) wins the game
    getinitgame(initgame);//see if user presses n to begin game
    int initialtime=time(NULL); //variable that sets the initial game after user presses n to play



    storerand(board, board_pos);//store  random number at random positions
    storerand(board, board_pos);//store  random number at random positions


    // initialize graphics
    InitEverything(width, height, &serif, imgs, &window, &renderer);
    // loads numbers as images
    LoadValues(array_of_numbers);




    while( quit == 0 )
    {
        getmaxtile (board,board_pos, &maxtile);// this functions stores the max tile gotten in int maxtile
        getwin(board, board_pos, difficulty, &win);// this functions stores 1 in win if game is won
        getgameover(board, board_pos, &gameover);// this function stores 1 in gameover if game is lost
        if (win==0 && gameover==0) // time stops if game is either over or  won
        {
            getcurrenttime( initialtime, &currenttime);//this function stores play time in variable currenttime
        }


        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {
                save_stats(stats,name, maxtile, currenttime, score);//save the stats if user quits the game
                quit=1;
            }
            else if ( event.type == SDL_KEYDOWN  )
            {
                if (event.key.keysym.sym==SDLK_q)
                {
                    save_stats(stats,name, maxtile, currenttime, score);//save the stats if user quits the game
                    quit=1;

                }

                switch ( event.key.keysym.sym )
                {

                case SDLK_UP:

                    if (win==0)
                    {
                        save_board(board, board_undo, board_pos);//save board_undo before movement and merging
                        moveup(board, board_pos);
                        mergeup(board, board_pos, &score);
                        moveup(board, board_pos);
                        if (board_difference(board,board_undo, board_pos)==1) //only if the movement is valid stores rand and increases attempts
                        {
                            storerand(board, board_pos);
                            tentativas++;
                        }


                    }

                    break;

                case SDLK_DOWN:
                    if (win==0)
                    {
                        save_board(board, board_undo, board_pos);//save board_undo before movement and merging
                        movedown(board, board_pos);
                        mergedown(board, board_pos, &score);
                        movedown(board, board_pos);
                        if (board_difference(board,board_undo, board_pos)==1) //only if the movement is valid stores rand and increases attempts
                        {

                            storerand(board, board_pos);
                            tentativas++;
                        }

                    }
                    break;




                case SDLK_LEFT:
                    if (win==0)
                    {
                        save_board(board, board_undo, board_pos);//save board_undo before movement and merging
                        moveleft(board, board_pos);
                        mergeleft(board, board_pos, &score);
                        moveleft(board, board_pos);
                        if (board_difference(board,board_undo, board_pos)==1) //only if the movement is valid stores rand and increases attempts
                        {
                            storerand(board, board_pos);
                            tentativas++;
                        }


                    }
                    break;




                case SDLK_RIGHT:
                    if (win==0)
                    {
                        save_board(board, board_undo, board_pos);//save board_undo before movement and merging
                        moveright(board, board_pos);
                        mergeright(board, board_pos, &score);
                        moveright(board, board_pos);
                        if (board_difference(board,board_undo, board_pos)==1) //only if the movement is valid stores rand and increases attempts
                        {
                            storerand(board, board_pos);
                            tentativas++;
                        }

                    }

                    break;


                case SDLK_n:
                    newgame(board, board_pos);// clears the board and stores 2 random numbers
                    win=0;// reset win
                    gameover=0;//reset gameover
                    score=0; //resets score
                    break;

                case SDLK_u:
                    win=0;// if game is won reset won
                    gameover=0;//if game is over reset game over
                    save_board(board_undo, board, board_pos);//save board_undo in board

                    if (board_difference(board, board_undo, board_pos)==1)
                    {
                        // only reduces attempts if the undo isnt already done
                        tentativas--;
                    }
                    break;


                default:
                    break;

                }
            }
        }
        // render game table
        RenderTable( board_pos, &board_size_px, &square_size_px, serif, imgs, renderer);
        // render board
        RenderBoard(board, array_of_numbers, board_pos, board_size_px, square_size_px, renderer);
        RenderStats( renderer, serif, score, currenttime, win, tentativas, gameover );

        // render in the screen all changes above
        SDL_RenderPresent(renderer);


        // add a delay
        SDL_Delay( delay );

    }

    // free memory allocated for images and textures and closes everything including fonts
    UnLoadValues(array_of_numbers);
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}




/**
 * LoadCards: Loads all images with the values
 * \param _array_of_numbers vector with all loaded numbers
 */
void LoadValues(SDL_Surface **_array_of_numbers)
{
    int i;
    char filename[STRING_SIZE];

    // loads all images to an array
    for ( i = 0 ; i < MAX_LEVELS; i++ )
    {
        // create the filename !
        sprintf(filename, ".//numbers//value_%02d.jpg", i+1);
        // loads the image !
        _array_of_numbers[i] = IMG_Load(filename);
        // check for errors: deleted files ?
        if (_array_of_numbers[i] == NULL)
        {
            printf("Unable to load image: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }
}


/**
 * UnLoadCards: unloads all values images of the memory
 * \param _array_of_numbers vector with all loaded image numbers
 */
void UnLoadValues(SDL_Surface **_array_of_numbers)
{
    // unload all cards of the memory: +1 for the card back
    for ( int i = 0 ; i < MAX_LEVELS; i++ )
    {
        SDL_FreeSurface(_array_of_numbers[i]);
    }
}

/**
 * RenderBoard: renders the board
 * \param _board multidimensional array with the board numbers
 * \param _array_of_numbers vector with all loaded images
 * \param _board_pos number of positions in the board
 * \param _board_size_px size of the board in pixels
 * \param _square_size_px size of each square
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderBoard(int _board[][MAX_BOARD_POS], SDL_Surface **_array_of_numbers, int _board_pos,
                 int _board_size_px, int _square_size_px, SDL_Renderer *_renderer )
{
    int x_corner, y_corner;
    SDL_Rect boardPos;
    SDL_Texture *number_text;

    // corner of the board
    x_corner = (TABLE_SIZE - _board_size_px) >> 1;
    y_corner = (TABLE_SIZE - _board_size_px - 15);

    // renders the squares where the numbers will appear
    for ( int i = 0; i < _board_pos; i++ )
    {
        for ( int j = 0; j < _board_pos; j++ )
        {
            // basic check
            if ( _board[i][j] > 23 || _board[i][j] < 0 )
            {
                printf("Invalid board value: RenderBoard\n");
                exit(EXIT_FAILURE);
            }
            // only draws a number if it corresponds to a non-empty space
            if ( _board[i][j] != 0 )
            {
                // define the size and copy the image to display
                boardPos.x = x_corner + (i+1)*SQUARE_SEPARATOR + i*_square_size_px;
                boardPos.y = y_corner + (j+1)*SQUARE_SEPARATOR + j*_square_size_px;
                boardPos.w = _square_size_px;
                boardPos.h = _square_size_px;
                number_text = SDL_CreateTextureFromSurface( _renderer, _array_of_numbers[ _board[i][j] - 1] );
                SDL_RenderCopy( _renderer, number_text, NULL, &boardPos );
                // destroy texture
                SDL_DestroyTexture(number_text);
            }
        }
    }
}

/*
 * Shows some information about the game:
 * - Level to achieve (difficulty)
 * - Score of the game
 * - Time spent in seconds
 * \param _renderer renderer to handle all rendering in a window
 * \param _level current level
 * \param _score actual score
 * \param _time time to display in seconds
 */
void RenderStats( SDL_Renderer *_renderer, TTF_Font * _font, int score, int time, int win, int tentativas, int gameover)//, int _level, int _score, int _time )
{
    char time_render[MAX_ARRAY]= {0};
    char score_render[MAX_ARRAY]= {0};
    char tentativa_render[MAX_ARRAY]= {0};
    char scoredisplay[6]= {'S', 'c','o', 'r', 'e','\0'};
    char timedisplay[5]= {'T', 'i', 'm', 'e','\0'};
    char tentativadisplay [8]= {'A','t','t','e','m','p','t','\0'};
    char windisplay[9]= {'Y','o','u',' ','w','i','n','!', '\0'};
    char gameoverdisplay[11]= {'G','a','m','e',' ','O','v','e','r','!','\0'};


    SDL_Color words = {101, 199, 255};// define color used for words
    SDL_Color wincolor= {24,68,21};// define color used for words  when you lose or win
    sprintf(score_render, "%d", score);
    sprintf (time_render, "%d", time);
    sprintf(tentativa_render, "%d", tentativas);
    RenderText(720, 220, score_render, _font, &words, _renderer);//render the number of your score
    RenderText (720, 240, time_render, _font, &words, _renderer);//render the number of seconds
    RenderText(740, 260, tentativa_render, _font, &words, _renderer);//render the number of attempts
    RenderText(660, 220, scoredisplay, _font, &words, _renderer);//render word "score"
    RenderText(660, 240, timedisplay,_font, &words, _renderer);//render word "time"
    RenderText(660, 260,tentativadisplay,_font, &words, _renderer ); //render word "attempt"



    SDL_Rect rect_level;//define rectangle


    rect_level.x=250;//set x position
    rect_level.y=275;// set y position
    rect_level.w=200;// set weight
    rect_level.h=100;// set height
    if (win==1)
    {
        SDL_SetRenderDrawColor(_renderer, 148, 255, 155, 255);//set color used in rectangle
        SDL_RenderFillRect(_renderer, &rect_level);// render rectangle
        RenderText(260, 275, windisplay, _font, &wincolor, _renderer);// render "you win"
    };
    if (gameover==1)
    {
        SDL_SetRenderDrawColor(_renderer, 148, 255, 155, 255);//set color used in rectangle
        SDL_RenderFillRect(_renderer, &rect_level);// render rectangle
        RenderText(260, 275, gameoverdisplay, _font, &wincolor, _renderer);//render "you lose"
    }
}


/*
 * RenderTable: Draws the table where the game will be played, namely:
 * -  some texture for the background
 * -  the right part with the IST logo and the student name and number
 * -  the grid for game board with squares and seperator lines
 * \param _board_pos number of squares in the board
 * \param _font font used to render the text
 * \param _img surfaces with the table background and IST logo (already loaded)
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderTable( int _board_pos, int *_board_size_px, int *_square_size_px, TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer )
{
    SDL_Color black = { 0, 0, 0 }; // black
    SDL_Color light = { 205, 193, 181 };
    SDL_Color dark = { 120, 110, 102 };
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest, board, board_square;
    int height, board_size_px, square_size_px;

    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;
    tableDest.w = TABLE_SIZE;
    tableDest.h = TABLE_SIZE;

    // draws the table texture
    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);

    // render the IST Logo
    height = RenderLogo(TABLE_SIZE, 0, _img[1], _renderer);

    // render the student name
    height += RenderText(TABLE_SIZE+3*MARGIN, height, myName, _font, &black, _renderer);

    // this renders the student number
    RenderText(TABLE_SIZE+3*MARGIN, height, myNumber, _font, &black, _renderer);

    // compute and adjust the size of the table and squares
    board_size_px = (int)(BOARD_SIZE_PER*TABLE_SIZE);
    square_size_px = (board_size_px - (_board_pos+1)*SQUARE_SEPARATOR) / _board_pos;
    board_size_px -= board_size_px % (_board_pos*(square_size_px+SQUARE_SEPARATOR));
    board_size_px += SQUARE_SEPARATOR;

    // renders the entire board background
    SDL_SetRenderDrawColor(_renderer, dark.r, dark.g, dark.b, dark.a );
    board.x = (TABLE_SIZE - board_size_px) >> 1;
    board.y = (TABLE_SIZE - board_size_px - 15);
    board.w = board_size_px;
    board.h = board_size_px;
    SDL_RenderFillRect(_renderer, &board);

    // renders the squares where the numbers will appear
    SDL_SetRenderDrawColor(_renderer, light.r, light.g, light.b, light.a );

    // iterate over all squares
    for ( int i = 0; i < _board_pos; i++ )
    {
        for ( int j = 0; j < _board_pos; j++ )
        {
            board_square.x = board.x + (i+1)*SQUARE_SEPARATOR + i*square_size_px;
            board_square.y = board.y + (j+1)*SQUARE_SEPARATOR + j*square_size_px;
            board_square.w = square_size_px;
            board_square.h = square_size_px;
            SDL_RenderFillRect(_renderer, &board_square);
        }
    }

    // destroy everything
    SDL_DestroyTexture(table_texture);
    // store some variables for later use
    *_board_size_px = board_size_px;
    *_square_size_px = square_size_px;
}

/**
 * RenderLogo function: Renders the IST logo on the app window
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _logoIST surface with the IST logo image to render
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
    SDL_Texture *text_IST;
    SDL_Rect boardPos;

    // space occupied by the logo
    boardPos.x = x;
    boardPos.y = y;
    boardPos.w = _logoIST->w;
    boardPos.h = _logoIST->h;

    // render it
    text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
    SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
    SDL_DestroyTexture(text_IST);
    return _logoIST->h;
}

/**
 * RenderText function: Renders some text on a position inside the app window
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string with the text to be written
 * \param _font TTF font used to render the text
 * \param _color color of the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect solidRect;

    solidRect.x = x;
    solidRect.y = y;
    // create a surface from the string text with a predefined font
    text_surface = TTF_RenderText_Blended(_font,text,*_color);
    if(!text_surface)
    {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    // create texture
    text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
    SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
    SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);
    // clear memory
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    return solidRect.h;
}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _font font that will be used to render the text
 * \param _img surface to be created with the table background and IST logo
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    _img[0] = IMG_Load("table_texture.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load IST logo
    _img[1] = SDL_LoadBMP("ist_logo.bmp");
    if (_img[1] == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 20);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
    // Init font library
    if(TTF_Init()==-1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
    window = SDL_CreateWindow( "1024", WINDOW_POSX, WINDOW_POSY, width, height, 0 );
    // check for error !
    if ( window == NULL )
    {
        printf("Failed to create window : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
    renderer = SDL_CreateRenderer( _window, -1, 0 );

    if ( renderer == NULL )
    {
        printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // set size of renderer to the same as window
    SDL_RenderSetLogicalSize( renderer, width, height );

    return renderer;
}

void getgameover(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos,int *gameover)
{
    //see if game is over and store 1 in variable game over  if it is
    int c=0, l=0;
    int zero=0;
    int vertical=0, horizontal=0;


    for (l=0; l<board_pos; l++)// see if there is any empty tile
    {
        for(c=0; c<board_pos; c++)
            if (board[c][l]==0)
            {
                zero=1;
                break;

            }

    }



    // check if there is a tile equal to another wich are in contiguous horizontal positions
    for (l=0; l<board_pos; l++ )
    {
        for (c=0; c<board_pos-1; c++)
        {


            if (board[c+1][l]==board [c][l] && board [c][l]!=0)
            {
                horizontal=1;// if horizontal=1 there are contiguous horizontal tiles that can be merged
                break;
            }
        }
    }      // so game isnt over

    //check if there is a tile equal to another in contiguous vertical position
    for (c=0; c<board_pos; c++ )
    {
        for (l=0; l<board_pos-1; l++)

            if (board[c][l+1]==board [c][l] && board [c][l]!=0)
            {
                vertical=1;//if vertical=1 there are contiguous vertical tiles that
                break;//can be merged so game isnt over

            }
    }

    if(horizontal ==0 && vertical ==0 &&zero==0 )// if there isnt a tile equal to 0 and tiles cant be merged horizontly or vertically
    {
        *gameover= 1;   //game is over
    }
    else
    {
        *gameover= 0;   // game isnt over
    }


}


void mergeup(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos, int*score)
{
    int c=0,l=0;

    for (l=0; l<board_pos; l++)
    {
        for (c=0; c<board_pos; c++)
        {
            if (board[c][l+1]==board[c][l]&& board[c][l+1]!=0)// see if there is a tile, upwards, equal to tile
            {
                *score=*score+pow(2,board[c][l]+1);// increment score with the value of the new tile created
                board[c][l]=(board[c][l]+1);// merge tiles
                board [c][l+1]=0;//reset tile that was used to merge
            }
        }
    }
}


void mergedown(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos, int *score)
{
    int c=0,l=0;

    for (c=0; c<board_pos; c++)
    {
        for (l=board_pos-1; l>0; l--)
        {
            if (board[c][l]==board[c][l-1]&& board[c][l]!=0)// see if there is a tile, downwards, equal to tile
            {
                *score=*score+pow(2,board[c][l]+1);// increment score with the value of the new tile created
                board[c][l]=(board[c][l]+1);//merge tiles
                board [c][l-1]=0;//reset tile that was used to merge

            }
        }
    }
}


void mergeleft(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos,int *score)
{
    int c=0,l=0;

    for (l=0; l<board_pos; l++)
    {
        for (c=0; c<board_pos; c++)
        {
            if (board[c+1][l]==board[c][l]&& board[c][l]!=0)// see if there is a tile, to the left, equal to tile
            {
                *score=*score+pow(2,board[c][l]+1);// increment score with the value of the new tile created
                board[c][l]=(board[c][l]+1);//merge tiles
                board [c+1][l]=0;//reset tile that was used to merge
            }
        }
    }
}



void mergeright(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos, int *score)
{
    int c=0,l=0;

    for (l=0; l<board_pos; l++)
    {
        for (c=board_pos; c>0; c--)
        {
            if (board[c][l]==board[c-1][l]&& board[c][l]!=0)// see if there is a tile, to the right, equal to tile
            {
                *score=*score+pow(2,board[c][l]+1);// increment score with the value of the new tile created
                board[c][l]=(board[c][l]+1);//merge tiles
                board [c-1][l]=0;//reset tile that was used to merge

            }
        }
    }
}






void storerand(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos)
{
    // this function is used to store 1 random number at random position in array board
    int randomnumb1=0, randomnumb2=0, randomnumb3=0;
    int numbboard=0;
    do
    {
        randomnumb1=(rand()%2)+1;// see if the number stored is 1 (stores 2) or 2 (stores 4)
        randomnumb2=rand()%(board_pos);// generate a random position in board
        randomnumb3=rand ()%(board_pos);// generate a random position in board

        numbboard=board[randomnumb2][randomnumb3];// numbboard stores the number in that board position

        if (numbboard==0)
        {
            board[randomnumb2][randomnumb3]=randomnumb1;
        }
    }
    while(numbboard!=0);//this function continues while numbboard is different from 0, this is, until a random number is stored


}




void moveup(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos )
{
    int c=0,l=0, k=0;

    for (c=0; c<board_pos; c++)
    {
        for (l=0; l<board_pos; l++)
            if (board[c][l]==0)// see if there is a tile equal to 0
            {
                k=l+1;
                for(k=l+1; k<board_pos; k++) // see if below that tile there is a tile different from 0
                {
                    if (board[c][k]!=0)
                    {
                        board[c][l]=board[c][k];// if there is change positions
                        board[c][k]=0;
                        break;
                    }
                }
            }
    }
}



void movedown(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos )
{
    int c=0,l=0, k=0;

    for (c=0; c<board_pos; c++)
    {
        for (l=board_pos-1; l>=0; l--)
        {
            if (board[c][l]==0)// see if there is a tile equal to 0
            {
                k=l-1;
                for(k=l-1; k>=0; k--)// see if above that tie there is a tile different from 0
                {
                    if (board[c][k]!=0)
                    {
                        board[c][l]=board[c][k];// if there is, change position between tiles
                        board[c][k]=0;
                        break;

                    }


                }
            }
        }
    }
}

void moveleft(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos)
{
    int c=0,l=0, k=0;

    for (l=0; l<board_pos; l++)
    {
        for (c=0; c<board_pos; c++)
            if (board[c][l]==0)// see if there is a tile equal to 0
            {
                k=c+1;
                for(k=c+1; k<board_pos; k++)// see if to the right of that tile there is a tile diferent from 0
                {
                    if (board[k][l]!=0)
                    {
                        board[c][l]=board[k][l];//if there is change position between tiles
                        board[k][l]=0;
                        break;
                    }
                }
            }
    }
}

void moveright(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos )
{
    int c=0,l=0, k=0;

    for (l=0; l<board_pos; l++)
    {
        for (c=board_pos-1; c>=0; c--)
            if (board[c][l]==0)// see if there is a tile equal to 0
            {
                k=c-1;
                for(k=c-1; k>=0; k--)// see if, to the left of that tile there is any tile different from o
                {
                    if (board[k][l]!=0)
                    {
                        board[c][l]=board[k][l];// if there is a tile different form 0 change positions
                        board[k][l]=0;
                        break;
                    }
                }
            }
    }
}


void newgame(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos)
{
    // this function is used to reset board when user wants to play a new game
    int c=0, l=0;


    for (l=0; l<board_pos; l++)
        for (c=0; c<board_pos; c++)
        {
            board[c][l]=0; // reset all the tiles to 0
        }


    storerand(board, board_pos);
    storerand(board, board_pos);// store 2 random numbers at random positions



}



void save_board(int board[MAX_BOARD_POS][MAX_BOARD_POS],int board_undo[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos)
{
    // this function is used to save board in board_undo ( used to return to previous board)
    int c=0, l=0;
    for (l=0; l<board_pos; l++)
    {
        for (c=0; c<board_pos; c++)
        {
            board_undo[c][l]=board[c][l];

        }
    }


}



void getboard_size(int *board_pos)
{
    // get the board dimensions
    printf("\nEnter board size(bigger then 3 less or equal then 11):");
    scanf("%d", &*board_pos);
    if (*board_pos<4||*board_pos>11)
    {
        do
        {
            printf("\nInvalid board size\n Enter Board size:");
            scanf("%d",&*board_pos );

        }
        while (*board_pos<4||*board_pos>11);// the dimensions only are accepted when they are bigger then 3 and smaller then 12
    }

}
void getinitgame (char initgame[])
{
    // see if user enter 'n' to begin game
    char compare[MAX_ARRAY];
    compare [0]='n';

    getchar();// to clear the last \n entered
    do
    {

        printf (" \nPrima n para jogar::");
        gets(initgame);

    }
    while(strcmp(compare, initgame)!=0);// This loop ends when strings initgame and compare are equal, this is when user presses n and \n after it

}


void getdifficulty(int *difficulty)
{
    // This function is used to get game difficulty
    printf("\nEnter difficulty:");
    scanf("%d", &*difficulty);
}

void getwin(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos, int difficulty, int *win)
{
    // this function sees if there is a tile equal to pow(2, difficulty), If there is, user has won the game: If not game continues
    int c=0, l=0;
    for (l=0; l<board_pos; l++)
    {
        for (c=0; c<board_pos; c++)
        {
            if (board[c][l]==difficulty)
            {
                *win=1;
                break;
            }

        }
    }

}




void getmaxtile (int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos, int *maxtile)//This function returns the maximum number in the tiles
{
    *maxtile=0;
    int c=0, l=0;
    int maxvalue=0;//this variable is used to compare the value in the board with max. value already read
    for (c=0; c<board_pos; c++)
    {
        for(l=0; l<board_pos; l++)
        {
            if (board[c][l]>maxvalue)
            {
                *maxtile=pow (2, board[c][l]);
                maxvalue=board[c][l];
            }
        }

    }

}

void getcurrenttime(int initialtime, int *currenttime)
{
    // This function returns the time of gameplay ( subtratcts initial game time to present time)
    *currenttime = time(NULL)- initialtime;

}


void getname (char name[])
{
    //This function gets the gamer username but only if it has 8 or less then 8 characters
    do
    {
        if (strlen(name)>8)
        {
            printf("\nNome InvÃ¡lido");
        }
        printf ("\nIntroduza o seu nome(Max. 8 caracteres):");
        fgets(name, MAX_ARRAY, stdin);

    }
    while(strlen(name)>8);

}

int board_difference(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_undo[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos)
{
    // see if there is a difference between present board and previous board (Board_undo)
    // if boards are different the number of attempts is increased
    int c=0, l=0;
    for(c=0; c<board_pos; c++)
    {
        for(l=0; l<board_pos; l++)
            if(board[c][l]!=board_undo[c][l])
            {
                return 1;
                break;
            }
    }
    return 0;

}


void save_stats(FILE *stats, char name[MAX_ARRAY], int maxtile, int currenttime, int score)
{
    // this function is used to save the stats of all the games ever played(this function is called only when user quits game)
    stats=fopen("stats.txt", "a");
    fprintf(stats, " \n Name: %s \n Time: %d \n Score:%d \n Max.Tile:%d \n", name,currenttime, score, maxtile );
    fclose(stats);

}


