#include<iostream> 
#include<fstream>
#include<string>
#include <cstdlib> // for random function
#include <ctime> // for seed time
#include <algorithm> // for min max
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

using namespace std;
using namespace sf;

const int gridsize = 10;
const int cellsize = 35; 
const int margin = 50;
const int EMPTY = 0;
const int SHIP = 1;
const int HIT = 3;
const int MISS = 4;
//FOR SINGLE PLAYER
int playerScore = 100; 
int aiScore = 100;
//FOR MULTIPLAYER
int player1Score = 100; 
int player2Score = 100; 

//FOR SINGLE PLAYER
int playergrid[10][10];
int opponentgrid[10][10];

//FOR MULTI PLAYER
int player1grid[10][10];
int P2grid[10][10];

int previousattacks[10][10] = { 0 };

Music hitMusic;
Music missMusic;
Music Gameovermusic;

bool loadMusic()
{
    if (!hitMusic.openFromFile("C:\\Users\\User\\Desktop\\Battleship\\220078__nicocadiou__hitmouv_short02.ogg"))
    {
        std::cerr << "Error loading HIT sound!" << std::endl;
        return false;
    }

    if (!missMusic.openFromFile("C:\\Users\\User\\Desktop\\Battleship\\479070__nox_sound__foley_impact_stone_water_02.ogg"))
    {
        std::cerr << "Error loading MISS sound!" << std::endl;
        return false;
    }

    if (!Gameovermusic.openFromFile("C:\\Users\\User\\Desktop\\Battleship\\664606__mt4000__game-over-low.ogg"))
    {
        std::cerr << "Error loading GAME OVER music!" << std::endl;
    }

    return true;
}

void ShowLoading()
{
    RenderWindow LoadingWindow(VideoMode(500, 600), "Loading");

    Texture texture1;
    if (!texture1.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\loading.jpg"))
    {
        return;
    }

    Vector2u texture1Size = texture1.getSize();
    float scaleX = 500.0f / texture1Size.x;
    float scaleY = 600.0f / texture1Size.y; 
    float scale = std::min(scaleX, scaleY); //RATIO MAINTAIN HO PIC KA WINDOW PAR

    Sprite sprite1(texture1);
    sprite1.setScale(scaleX, scaleY); 

    RectangleShape progressbar(Vector2f(400, 30));
    progressbar.setPosition(50, 270);
    progressbar.setFillColor(Color::White);

    RectangleShape progressBar(Vector2f(0, 30));  
    progressBar.setPosition(50, 270);  
    progressBar.setFillColor(Color::Black);

    Font font;
    if (!font.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\BOD_PSTC.TTF"))
    {
        return;
    }

    Text loadingText("Loading", font, 40);  
    loadingText.setFillColor(Color::White);  
    loadingText.setPosition(50 + (400 - loadingText.getLocalBounds().width) / 2, 200);

    Clock clock;
    const float loadingDuration = 3.0f;

    while (LoadingWindow.isOpen())
    {
        Event LoadingEvent;
        while (LoadingWindow.pollEvent(LoadingEvent))
        {
            if (LoadingEvent.type == Event::Closed)
            {
                LoadingWindow.close();
            }
        }
        LoadingWindow.clear();
        float time = clock.getElapsedTime().asSeconds(); // time for progress bar
        if (time > loadingDuration)
        {
            LoadingWindow.close();
        }
        float progress = (time / loadingDuration) * 400;
        progressbar.setSize(Vector2f(progress, 30));


        LoadingWindow.draw(sprite1);  //  background image 
        LoadingWindow.draw(progressbar);  
        LoadingWindow.draw(loadingText);
        LoadingWindow.display();


    }
}

void createButton(RectangleShape& button, Vector2f size, Color color, Vector2f position)
{
    button.setSize(size);
    button.setFillColor(color);
    button.setPosition(position);

}

void initializegrid(int grid[10][10])
{
    for (int i = 0; i < 10; i++)
    {
        for (int k = 0; k < 10; k++)
        {
            grid[i][k] = 0;
        }
    }
}

void placeship(int grid[10][10], RenderWindow& SGamewindow) // for single player
{
    const int square = 40; 
    const int distleft = 50;
    RectangleShape tile(Vector2f(square - 2, square - 2));
    tile.setOutlineThickness(1);
    tile.setOutlineColor(Color::Black);

    RectangleShape shipTile(Vector2f(square - 4, square - 4)); 
    shipTile.setFillColor(Color::Green);

    int sizearr[5] = { 5, 4, 3, 3, 2 };
    int currentShipIndex = 0;
    bool isHorizontal = true;
    static bool spacePressed = false; 
    bool updateScreen = true;

    while (currentShipIndex < 5 && SGamewindow.isOpen())
    {
        Event event;
        while (SGamewindow.pollEvent(event))
        {
            if (event.type == Event::Closed)
                SGamewindow.close();

            // press spacebar to change the direction ( h or v)
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
            {
                if (!spacePressed)
                {
                    isHorizontal = !isHorizontal;
                    spacePressed = true;
                    updateScreen = true;
                }
            }
            else if (event.type == Event::KeyReleased && event.key.code == Keyboard::Space)
            {
                spacePressed = false;
            }

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                Vector2i mousePos = Mouse::getPosition(SGamewindow);
                int row = (mousePos.y - distleft) / square;
                int col = (mousePos.x - distleft) / square;

                if (row >= 0 && row < 10 && col >= 0 && col < 10)
                {
                    bool canPlace = true;
                    // checking if space is valid
                    for (int i = 0; i < sizearr[currentShipIndex]; ++i)
                    {
                        int r;
                        int c;

                        if (isHorizontal)
                        {
                            r = row;
                            c = col + i;
                        }
                        else
                        {
                            r = row + i;
                            c = col;
                        }

                        if (r < 0 || r >= 10 || c < 0 || c >= 10 || grid[r][c] == SHIP)
                        {
                            canPlace = false;
                            break;
                        }
                    }
                    // Place  if  valid
                    if (canPlace)
                    {
                        for (int i = 0; i < sizearr[currentShipIndex]; ++i)
                        {
                            int r;
                            int c;

                            if (isHorizontal)
                            {
                                r = row;
                                c = col + i;
                            }
                            else
                            {
                                r = row + i;
                                c = col;
                            }

                            grid[r][c] = SHIP; 
                        }
                        currentShipIndex++; // 1 ship down!!
                        updateScreen = true;
                    }
                }
            }
        }

        // Update screen after evrry placement of ships
        if (updateScreen)
        {
            SGamewindow.clear(Color::White);

            // Drawinf grid
            for (int i = 0; i < 10; ++i)
            {
                for (int j = 0; j < 10; ++j)
                {
                    tile.setPosition(distleft + j * square, distleft + i * square);
                    if (grid[i][j] == SHIP)
                    {
                        tile.setFillColor(Color::Blue);
                    }
                    else
                    {
                        tile.setFillColor(Color::White);
                    }
                    SGamewindow.draw(tile);
                }
            }

            // shows valid places
            Vector2i mousePos = Mouse::getPosition(SGamewindow);
            int row = (mousePos.y - distleft) / square;
            int col = (mousePos.x - distleft) / square;

            if (row >= 0 && row < 10 && col >= 0 && col < 10)
            {
                bool canPlace = true;
                // check for green
                for (int i = 0; i < sizearr[currentShipIndex]; ++i)
                {
                    int r;
                    int c;

                    if (isHorizontal)
                    {
                        r = row;
                        c = col + i;
                    }
                    else
                    {
                        r = row + i;
                        c = col;
                    }

                    if (r < 0 || r >= 10 || c < 0 || c >= 10 || grid[r][c] == SHIP)
                    {
                        canPlace = false;
                        break;
                    }
                }

                // show the place where we want to place the ship
                for (int i = 0; i < sizearr[currentShipIndex]; ++i)
                {
                    int r;
                    int c;

                    if (isHorizontal)
                    {
                        r = row;
                        c = col + i;
                    }
                    else
                    {
                        r = row + i;
                        c = col;
                    }

                    if (r >= 0 && r < 10 && c >= 0 && c < 10)
                    {
                        shipTile.setPosition(distleft + c * square + 2, distleft + r * square + 2);

                        // decide clur acc to placement 
                        if (canPlace)
                        {
                            shipTile.setFillColor(Color::Green); // valid
                        }
                        else
                        {
                            shipTile.setFillColor(Color::Red); // invalid
                        }

                        SGamewindow.draw(shipTile);
                    }
                }
            }

            SGamewindow.display();
            updateScreen = false;
        }

        sleep(milliseconds(16)); // to let the cpu calm down
    }
}


void randomshipplacement(int grid[10][10])
{
    int sizearr[5] = { 5, 4, 3, 3, 2 };

    for (int i = 0; i < 5; i++)
    {
        int size = sizearr[i];
        bool placed = false;

        while (!placed)
        {
            int rowindex = rand() % 10;
            int col = rand() % 10;
            char direction;
            if (rand() % 2 == 0)
            {
                direction = 'h';
            }
            else
            {
                direction = 'v';
            }

            bool canPlace = true;

            if (direction == 'h')
            {
                if (col + size <= 10)
                {
                    for (int x = 0; x < size; x++)
                    {
                        if (grid[rowindex][col + x] != 0)
                        {
                            canPlace = false;
                            break;
                        }
                    }
                }
                else
                {
                    canPlace = false;
                }
            }
            else
            {
                if (rowindex + size <= 10)
                {
                    for (int x = 0; x < size; x++)
                    {
                        if (grid[rowindex + x][col] != 0)
                        {
                            canPlace = false;
                            break;
                        }
                    }
                }
                else
                {
                    canPlace = false;
                }
            }

            if (canPlace)
            {
                if (direction == 'h')
                {
                    for (int x = 0; x < size; x++)
                    {
                        grid[rowindex][col + x] = 1;
                    }
                }
                else
                {
                    for (int x = 0; x < size; x++)
                    {
                        grid[rowindex + x][col] = 1;
                    }
                }
                placed = true;
            }
        }
    }
}

void drawgrid(RenderWindow& SGamewindow,
    int grid[10][10],
    float offsetX,
    float offsetY,
    bool concealShips,
    float cellSize,
    const std::string& title)
{

    Font font1;
    if (!font1.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\CENTAUR.TTF"))
    {
        throw std::runtime_error("Font file not found.");
    }


    // title 
    Text titleText;
    titleText.setFont(font1);
    titleText.setString(title);
    titleText.setCharacterSize(24); 
    titleText.setFillColor(Color::White); 
    titleText.setPosition(offsetX, offsetY - 40); // above the grid

    SGamewindow.draw(titleText);

    Texture waterTexture;
    waterTexture.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\water.jpg");

    // draw the grid
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            float x = offsetX + j * cellSize;
            float y = offsetY + i * cellSize;

            Sprite sprite;
            sprite.setTexture(waterTexture);
            sprite.setPosition(x, y);
            sprite.setScale(cellSize / waterTexture.getSize().x, cellSize / waterTexture.getSize().y);
            SGamewindow.draw(sprite);

            RectangleShape cell(Vector2f(cellSize, cellSize));
            cell.setPosition(x, y);
            // decdide colouer
            switch (grid[i][j])
            {
            case 1: // hide or show
                if (!concealShips)
                {
                    cell.setFillColor(Color(139, 69, 19, 200)); // brown to show 
                }
                else
                {
                    cell.setFillColor(Color(0, 0, 0, 0)); //  hidw
                }
                break;
            case 3: // sucess
                cell.setFillColor(Color(0, 255, 0, 100)); // green
                break;
            case 4: // fail
                cell.setFillColor(Color(255, 0, 0, 100)); // red 
                break;
            default:
                cell.setFillColor(Color(0, 0, 0, 0));
                break;
            }
            cell.setOutlineThickness(1);
            cell.setOutlineColor(Color::Black);

            SGamewindow.draw(cell);
        }
    }
}

void MPplaceship(int grid[10][10], RenderWindow& MPwindow)
{
    const int square = 40;
    const int distfleft = 50;
    RectangleShape tile(Vector2f(square - 2, square - 2));
    tile.setOutlineThickness(1);
    tile.setOutlineColor(Color::Black);

    RectangleShape shipTile(Vector2f(square - 4, square - 4));
    shipTile.setFillColor(Color::Green);

    // Define ship sizes for each ship
    int sizearr[5] = { 5, 4, 3, 3, 2 };
    int currentShipIndex = 0;
    bool isHorizontal = true;
    static bool spacePressed = false;
    bool updateScreen = true;

    while (currentShipIndex < 5 && MPwindow.isOpen())
    {
        Event event;
        while (MPwindow.pollEvent(event))
        {
            if (event.type == Event::Closed)
                MPwindow.close();

           // space bar press to change h or v
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
            {
                if (!spacePressed)
                {
                    isHorizontal = !isHorizontal;
                    spacePressed = true;
                    updateScreen = true;
                }
            }
            else if (event.type == Event::KeyReleased && event.key.code == Keyboard::Space)
            {
                spacePressed = false;
            }
           
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                Vector2i mousePos = Mouse::getPosition(MPwindow);
                int row = (mousePos.y - distfleft) / square;
                int col = (mousePos.x - distfleft) / square;

                if (row >= 0 && row < 10 && col >= 0 && col < 10)
                {
                    bool canPlace = true;
                    // check if place is valid
                    for (int i = 0; i < sizearr[currentShipIndex]; ++i)
                    {
                        int r;
                        int c;

                        if (isHorizontal)
                        {
                            r = row;
                            c = col + i;
                        }
                        else
                        {
                            r = row + i;
                            c = col;
                        }

                        if (r < 0 || r >= 10 || c < 0 || c >= 10 || grid[r][c] == SHIP)
                        {
                            canPlace = false;
                            break;
                        }
                    }
                    // place if valid
                    if (canPlace)
                    {
                        for (int i = 0; i < sizearr[currentShipIndex]; ++i)
                        {
                            int r;
                            int c;

                            if (isHorizontal)
                            {
                                r = row;
                                c = col + i;
                            }
                            else
                            {
                                r = row + i;
                                c = col;
                            }

                            grid[r][c] = SHIP; 
                        }
                        currentShipIndex++; 
                        updateScreen = true;
                    }
                }
            }
        }

        
        if (updateScreen)
        {
            MPwindow.clear(Color::White);

            // drawing grid
            for (int i = 0; i < 10; ++i)
            {
                for (int j = 0; j < 10; ++j)
                {
                    tile.setPosition(distfleft + j * square, distfleft + i * square);
                    if (grid[i][j] == SHIP)
                    {
                        tile.setFillColor(Color::Blue);
                    }
                    else
                    {
                        tile.setFillColor(Color::White);
                    }
                    MPwindow.draw(tile);
                }
            }

            // show if ship can be placed ya nahi
            Vector2i mousePos = Mouse::getPosition(MPwindow);
            int row = (mousePos.y - distfleft) / square;
            int col = (mousePos.x - distfleft) / square;

            if (row >= 0 && row < 10 && col >= 0 && col < 10)
            {
                bool canPlace = true;
                // check if fit
                for (int i = 0; i < sizearr[currentShipIndex]; ++i)
                {
                    int r;
                    int c;

                    if (isHorizontal)
                    {
                        r = row;
                        c = col + i;
                    }
                    else
                    {
                        r = row + i;
                        c = col;
                    }

                    if (r < 0 || r >= 10 || c < 0 || c >= 10 || grid[r][c] == SHIP)
                    {
                        canPlace = false;
                        break;
                    }
                }

                // see ship at current position
                for (int i = 0; i < sizearr[currentShipIndex]; ++i)
                {
                    int r;
                    int c;

                    if (isHorizontal)
                    {
                        r = row;
                        c = col + i;
                    }
                    else
                    {
                        r = row + i;
                        c = col;
                    }

                    if (r >= 0 && r < 10 && c >= 0 && c < 10)
                    {
                        shipTile.setPosition(distfleft + c * square + 2, distfleft + r * square + 2);

                        
                        if (canPlace)
                        {
                            shipTile.setFillColor(Color::Green); // valid 
                        }
                        else
                        {
                            shipTile.setFillColor(Color::Red); // invalid
                        }

                        MPwindow.draw(shipTile); 
                    }
                }
            }

            MPwindow.display(); 
            updateScreen = false;
        }

        sleep(milliseconds(16)); 
    }
}


void MPdrawgrid(RenderWindow& MPwindow,
    int grid[10][10],
    float offsetX,
    float offsetY,
    bool concealShips,
    float cellSize,
    const std::string& title)
{

    Font font1;
    if (!font1.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\CENTAUR.TTF"))
    {
        throw std::runtime_error("Font file not found.");
    }

    Text titleText;
    titleText.setFont(font1);
    titleText.setString(title);
    titleText.setCharacterSize(24); 
    titleText.setFillColor(Color::White); 
    titleText.setPosition(offsetX, offsetY - 40); 

    MPwindow.draw(titleText);

    Texture waterTexture;
    waterTexture.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\water.jpg");

    
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            float x = offsetX + j * cellSize;
            float y = offsetY + i * cellSize;

            Sprite sprite;
            sprite.setTexture(waterTexture);
            sprite.setPosition(x, y);
            sprite.setScale(cellSize / waterTexture.getSize().x, cellSize / waterTexture.getSize().y);
            MPwindow.draw(sprite);

            RectangleShape cell(Vector2f(cellSize, cellSize));
            cell.setPosition(x, y);

           
            switch (grid[i][j])
            {
            case 1: 
                if (!concealShips)
                {
                    cell.setFillColor(Color(0, 0, 0, 0)); // hav to be concealed for both players
                }
                else
                {
                    cell.setFillColor(Color(0, 0, 0, 0));   
                }
                break;
            case 3: 
                cell.setFillColor(Color(0, 255, 0, 100)); 
                break;
            case 4: 
                cell.setFillColor(Color(255, 0, 0, 100)); 
                break;
            default:
                cell.setFillColor(Color(0, 0, 0, 0));
                break;
            }

            cell.setOutlineThickness(1);
            cell.setOutlineColor(Color::Black);

            MPwindow.draw(cell);
        }
    }
}

bool isValidCell(int row, int col)
{
    return row >= 0 && row < 10 && col >= 0 && col < 10;
}

void playershoot(RenderWindow& SGamewindow, int enemyGrid[10][10], float offsetX, float offsetY, float cellSize, bool& isPlayerTurn)
{
    static bool clickProcessed = false;

    if (Mouse::isButtonPressed(Mouse::Left) && !clickProcessed) // detection  
    {
        clickProcessed = true;

        Vector2i mousePos = Mouse::getPosition(SGamewindow);

        //  mouse position to grid 
        int col = (mousePos.x - offsetX) / cellSize;
        int row = (mousePos.y - offsetY) / cellSize;

        // click is in a valid cell or not
        if (col >= 0 && col < 10 && row >= 0 && row < 10)
        {
            //  if cell has already been shot at
            if (enemyGrid[row][col] == HIT || enemyGrid[row][col] == MISS)
            {
                cout << "This position has already been shot at. Try again." << endl;
                return;
            }

            // If the player hits a ship, then hit 
            if (enemyGrid[row][col] == SHIP)
            {
                enemyGrid[row][col] = HIT; 
                cout << "You hit a ship at (" << row << ", " << col << ")!" << endl;
                if (hitMusic.getStatus() != sf::Music::Playing)
                {
                    hitMusic.play();
                }
            }
            
            else
            {
                enemyGrid[row][col] = MISS; 
                cout << "You missed at (" << row << ", " << col << ")." << endl;
                if (missMusic.getStatus() != sf::Music::Playing) 
                {
                    missMusic.play();
                }
            }
            playerScore--;
            isPlayerTurn = false;
        }
    }
    else if (!Mouse::isButtonPressed(Mouse::Left))
    {
        clickProcessed = false; // dobara shuru se  
    }
}

void MPplayershoot(RenderWindow& MPwindow, int enemyGrid[10][10], float offsetX, float offsetY, float cellSize, bool& isPlayerTurn)
{
    static bool clickProcessed = false;

    if (Mouse::isButtonPressed(Mouse::Left) && !clickProcessed)
    {
        clickProcessed = true;

        Vector2i mousePos = Mouse::getPosition(MPwindow);
        int col = (mousePos.x - offsetX) / cellSize;
        int row = (mousePos.y - offsetY) / cellSize;

        if (row >= 0 && row < 10 && col >= 0 && col < 10)   //  valid cell or not
        {
            if (enemyGrid[row][col] != HIT && enemyGrid[row][col] != MISS)  // not already clicked
            {
                if (enemyGrid[row][col] == SHIP)
                {
                    enemyGrid[row][col] = HIT;
                    if (hitMusic.getStatus() != Music::Playing) hitMusic.play();


                    if (isPlayerTurn)
                    {
                        player2Score -= 1;  // Player 1 hits Player 2 ship
                    }
                    else
                    {
                        player1Score -= 1;  // Player 2 hits Player 1 ship
                    }
                }
                else
                {
                    enemyGrid[row][col] = MISS;
                    if (missMusic.getStatus() != Music::Playing) missMusic.play();

                    // Decrease the current player's score when they miss
                    if (isPlayerTurn)
                    {
                        player1Score -= 1;  // Player 1 miss
                    }
                    else
                    {
                        player2Score -= 1;  // Player 2 miss
                    }
                }

                isPlayerTurn = !isPlayerTurn; // switch turns 
                  
            }
        }
    }
    else if (!Mouse::isButtonPressed(Mouse::Left))
    {
        clickProcessed = false;
    }
    
}


bool AllShipsSunk(int grid[10][10]) // use in MP
{
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            if (grid[i][j] == SHIP)
            {
                return false; // A ship is still present
            }
        }
    }
    return true; // no ship left
}



bool trydirection(int r, int c, int grid[10][10], int dir)
{
    int direction[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };
    int newrow = r + direction[dir - 1][0];
    int newcol = c + direction[dir - 1][1];

    if (newrow >= 0 && newrow < 10 && newcol >= 0 && newcol < 10 && previousattacks[newrow][newcol] == 0)
    {
        if (grid[newrow][newcol] == 1)
        {
            cout << "AI hit your ship" << endl;
            grid[newrow][newcol] = 3;
            previousattacks[newrow][newcol] = 1;
            return true;
        }
        else if (grid[newrow][newcol] == 0)
        {
            cout << "AI Missed" << endl;
            previousattacks[newrow][newcol] = 1;
            return true;
        }
    }
    return false;
}

void AIshoot(int playerGrid[10][10])
{
    static bool hitOnce = false;
    static int rowOfLastHit = -1, colOfLastHit = -1;
    // if hit search uske pas
    if (hitOnce)
    {
        // search around it
        bool found = false;
        for (int dir = 1; dir <= 4; ++dir)
        {
            if (trydirection(rowOfLastHit, colOfLastHit, playerGrid, dir))
            {
                found = true;
                break;
            }
        }

        // random again
        if (!found)
        {
            hitOnce = false;
            AIshoot(playerGrid); 
        }
    }
    else
    {
        // random
        int rindex = rand() % 10;
        int cindex = rand() % 10;

        while (previousattacks[rindex][cindex] != 0)
        {
            rindex = rand() % 10;
            cindex = rand() % 10;
        }

        if (playerGrid[rindex][cindex] == 1)
        {
            cout << "AI hit your ship at (" << rindex << ", " << cindex << ")!" << endl;
            playerGrid[rindex][cindex] = HIT;
            previousattacks[rindex][cindex] = 1;
            hitOnce = true; // rmrbr the hit
            rowOfLastHit = rindex;
            colOfLastHit = cindex;
        }
        else
        {
            cout << "AI missed at (" << rindex << ", " << cindex << ")." << endl;
            playerGrid[rindex][cindex] = MISS;
            previousattacks[rindex][cindex] = 1;
        }
    }
    aiScore--;
}


bool terminate(int grid[10][10])
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (grid[i][j] == 1)
            {
                return false;
            }
        }
    }
    return true;
}

void playerturn(RenderWindow& SGamewindow, int grid[10][10], float offsetX, float offsetY, float cellSize, bool& isPlayerTurn)
{
    playershoot(SGamewindow, grid, offsetX, offsetY, cellSize, isPlayerTurn);
    if (!isPlayerTurn) 
    {
        isPlayerTurn = false; // ai ki turn
    }
}

void MPplayerturn(RenderWindow& MPwindow, int grid[10][10], float offsetX, float offsetY, float cellSize, bool& isPlayer1Turn)
{
    playershoot(MPwindow, grid, offsetX, offsetY, cellSize, isPlayer1Turn);
    if (!isPlayer1Turn)  // 1st player
    {
        isPlayer1Turn = false; // next player's turn
    }
}

void displayGameOver(RenderWindow& SGamewindow, const string& resultMessage)
{
    if (!SGamewindow.isOpen())
    {
        cout << "Window is not open!" << endl;
        return;
    }

    SGamewindow.clear(Color::White);

    Font font1;
    if (!font1.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\CENTAUR.TTF"))
    {
        throw std::runtime_error("Font file not found.");
    }


    Text gameOverText(resultMessage, font1, 50);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition(SGamewindow.getSize().x / 2.0f - gameOverText.getLocalBounds().width / 2.0f, SGamewindow.getSize().y / 2.0f - gameOverText.getLocalBounds().height / 2.0f);

   
    Text playerFinalScore("Your Score: " + std::to_string(playerScore), font1, 30);
    playerFinalScore.setFillColor(Color::Black);
    playerFinalScore.setPosition(SGamewindow.getSize().x / 2.0f - playerFinalScore.getLocalBounds().width / 2.0f,
        SGamewindow.getSize().y / 2.0f + 50);

    Text aiFinalScore("AI's Score: " + std::to_string(aiScore), font1, 30);
    aiFinalScore.setFillColor(Color::Black);
    aiFinalScore.setPosition(SGamewindow.getSize().x / 2.0f - aiFinalScore.getLocalBounds().width / 2.0f, SGamewindow.getSize().y / 2.0f + 100);

  
    SGamewindow.draw(playerFinalScore);
    SGamewindow.draw(aiFinalScore);
    SGamewindow.draw(gameOverText);


    Gameovermusic.play();
    SGamewindow.display();

    // Wait for a short period
    sf::Clock clock;
    while (clock.getElapsedTime().asSeconds() < 3)
    {
        // Pause for 3 seconds
    }
    Gameovermusic.stop();
    
    SGamewindow.close();
}

void MPdisplayGameOver(RenderWindow& MPwindow, const string& resultMessage)
{
    if (!MPwindow.isOpen())
    {
        cout << "Window is not open!" << endl;
        return;
    }

    
    MPwindow.clear(Color::White);

    Font font1;
    if (!font1.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\CENTAUR.TTF"))
    {
        throw std::runtime_error("Font file not found.");
    }

    Text gameOverText(resultMessage, font1, 50);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition(MPwindow.getSize().x / 2.0f - gameOverText.getLocalBounds().width / 2.0f, MPwindow.getSize().y / 2.0f - gameOverText.getLocalBounds().height / 2.0f);

    
    Text player1FinalScore("Your Score: " + std::to_string(player1Score), font1, 30);
    player1FinalScore.setFillColor(Color::Black);
    player1FinalScore.setPosition(MPwindow.getSize().x / 2.0f - player1FinalScore.getLocalBounds().width / 2.0f,
        MPwindow.getSize().y / 2.0f + 50);

    Text player2FinalScore("AI's Score: " + std::to_string(player2Score), font1, 30);
    player2FinalScore.setFillColor(Color::Black);
    player2FinalScore.setPosition(MPwindow.getSize().x / 2.0f - player2FinalScore.getLocalBounds().width / 2.0f, MPwindow.getSize().y / 2.0f + 100);



    MPwindow.draw(player1FinalScore);
    MPwindow.draw(player2FinalScore);
    MPwindow.draw(gameOverText);


    Gameovermusic.play();
    MPwindow.display();

    sf::Clock clock;
    while (clock.getElapsedTime().asSeconds() < 3)
    {
        // Pause for 3 seconds
    }
    Gameovermusic.stop();
    MPwindow.close();
}


void gameloop(RenderWindow& SGamewindow)
{
    float offsetX = 50.f; 
    float offsetY = 100.f;  
    float cellSize = 35.f;

    bool isPlayerTurn = true;
    bool Gameover = false;

    Font font1;
    if (!font1.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\CENTAUR.TTF"))
    {
        throw std::runtime_error("Font file not found.");
    }

    while (SGamewindow.isOpen())
    {
        Event event;
        while (SGamewindow.pollEvent(event))
        {
            if (event.type == Event::Closed)
                SGamewindow.close();
        }

        SGamewindow.clear(Color::White);

        drawgrid(SGamewindow, playergrid, margin, margin, true, cellsize, "Player's Grid");
        drawgrid(SGamewindow, opponentgrid, margin + 500, margin, false, cellsize, "Opponent's Grid");

        if (!Gameover)
        {

            if (isPlayerTurn)
            {
                
                playerturn(SGamewindow, opponentgrid, offsetX + 500, offsetY, cellSize, isPlayerTurn);      

               
                if (terminate(opponentgrid))
                {
                    displayGameOver(SGamewindow, "You win!");
                    Gameover = true;
                    break;
                }
                sf::sleep(sf::seconds(4));
            }
            else
            {
                
                AIshoot(playergrid); 

                if (terminate(playergrid))
                {
                    displayGameOver(SGamewindow, "AI wins!");
                    Gameover = true;
                }

                isPlayerTurn = true;
            }
        }
        
        Text playerScoreText("Player Score: " + std::to_string(playerScore), font1, 24);
        playerScoreText.setFillColor(Color::Black);
        playerScoreText.setPosition(offsetX, offsetY - 60); 

        Text aiScoreText("AI Score: " + std::to_string(aiScore), font1, 24);
        aiScoreText.setFillColor(Color::Black);
        aiScoreText.setPosition(offsetX + 500, offsetY - 60); 

        SGamewindow.draw(playerScoreText);
        SGamewindow.draw(aiScoreText);

        
        SGamewindow.display();
        sf::sleep(sf::seconds(2));
    }
}

void SGstartGame(sf::RenderWindow& SGamewindow)
{
    
    srand(static_cast<unsigned>(time(0)));

    
    int playerGrid[10][10];    
    int opponentGrid[10][10];    

    initializegrid(playerGrid);
    placeship(playerGrid, SGamewindow);  

    initializegrid(opponentGrid);
    randomshipplacement(opponentGrid);  

    
    if (!loadMusic()) 
    {
        return; 
    }

    bool concealShips = true;
    bool isPlayerTurn = true;
    bool gameover = false;
    float cellSize = 40.0f; 

    
    while (SGamewindow.isOpen() && !gameover)
    {
        sf::Event event;
        while (SGamewindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) 
            {
                SGamewindow.close();
            }
        }

        SGamewindow.clear();

        
        sf::Texture texture3;
        if (!texture3.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\loading.jpg")) 
        {
            return;
        }

        sf::Vector2u texture3Size = texture3.getSize();
        float scaleX3 = 950.0f / texture3Size.x;
        float scaleY3 = 600.0f / texture3Size.y;
        float scale3 = std::min(scaleX3, scaleY3);

        sf::Sprite sprite3(texture3);
        sprite3.setScale(scaleX3, scaleY3);
        SGamewindow.draw(sprite3);

        
        drawgrid(SGamewindow, playerGrid, 50, 100, false, cellSize, "Player Grid");
        drawgrid(SGamewindow, opponentGrid, 500, 100, true, cellSize, "Opponent Grid");

       
        if (isPlayerTurn) 
        {
            playerturn(SGamewindow, opponentGrid, 500, 100, cellSize, isPlayerTurn);

            if (terminate(opponentGrid))
            {
                displayGameOver(SGamewindow, "You Win!");
                gameover = true;
                continue;
            }
        }
        else
        {
            AIshoot(playerGrid);

            if (terminate(playerGrid))
            {
                displayGameOver(SGamewindow, "AI Wins!");
                gameover = true;
                continue;
            }
            isPlayerTurn = true;
        }

        SGamewindow.display();
    }
}

void MPstartgame(RenderWindow& MPwindow) 
{
    Texture texture3;
    if (!texture3.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\loading.jpg"))
    {
        throw std::runtime_error("Texture file not found.");
    }

    Vector2u texture3Size = texture3.getSize();
    float scaleX3 = 950.0f / texture3Size.x;
    float scaleY3 = 600.0f / texture3Size.y;
    float scale3 = std::min(scaleX3, scaleY3);

    Sprite sprite3(texture3);
    sprite3.setScale(scaleX3, scaleY3);

    if (!loadMusic()) 
    {
        throw std::runtime_error("Music not loaded.");
    }

    int player1grid[10][10], P2grid[10][10];
    initializegrid(player1grid);
    initializegrid(P2grid);

    MPplaceship(player1grid, MPwindow);
    MPplaceship(P2grid, MPwindow);

    Font font2;
    if (!font2.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\CENTAUR.TTF"))
    {
        throw std::runtime_error("Font file not found.");
    }

    Text turnText;
    turnText.setFont(font2);
    turnText.setCharacterSize(24);
    turnText.setFillColor(Color::White);
    turnText.setPosition(315.f, 560.f);

   
    bool isPlayer1Turn = true;
    float cellsize = 40.f; 

    while (MPwindow.isOpen())
    {
        Event event;
        while (MPwindow.pollEvent(event)) 
        {
            if (event.type == Event::Closed) 
            {
                MPwindow.close();
            }
        }

        
        if (AllShipsSunk(P2grid))
        {
            MPdisplayGameOver(MPwindow, "Player 1 Wins!");
            break;
        }

        
        if (AllShipsSunk(player1grid))
        {
            MPdisplayGameOver(MPwindow, "Player 2 Wins!");
            break;
        }

        MPwindow.clear();
        MPwindow.draw(sprite3);
        
        MPdrawgrid(MPwindow, player1grid, 50.f, 50.f, !isPlayer1Turn, cellsize, "Player 1's Grid");
        MPdrawgrid(MPwindow, P2grid, 500.f, 50.f, isPlayer1Turn, cellsize, "Player 2's Grid");

        
        if (isPlayer1Turn) 
        {
            MPplayershoot(MPwindow, P2grid, 500.f, 50.f, cellsize, isPlayer1Turn); // Player 1 guesses Player 2
        }
        else 
        {
            MPplayershoot(MPwindow, player1grid, 50.f, 50.f, cellsize, isPlayer1Turn); // Player 2 guesses Player 1  
        }

        MPwindow.display();
        isPlayer1Turn = !isPlayer1Turn;    
    }
}


int main()
{
    RenderWindow window(VideoMode(500, 600), "Battleship Game");

    Texture texture;
    if (!texture.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\screen-4.jpg"))
    {
        return -1;
    }

    Vector2u textureSize = texture.getSize();
    float scaleX = 500.0f / textureSize.x;
    float scaleY = 600.0f / textureSize.y;
    float scale = std::min(scaleX, scaleY);

    Sprite sprite(texture);
    sprite.setScale(scaleX, scaleY);

    RectangleShape startButton;
    Vector2f size(150, 85);
    Color buttonColor(128, 0, 128); 
    Vector2f position((500 - size.x) / 2, (600 - size.y) / 2);
    createButton(startButton, size,buttonColor, position);   

    Font font; 
    if (!font.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\BOD_PSTC.TTF")) 
    {
        return -1;
    }

    
    Text startText("START", font, 46);  
    startText.setFillColor(Color::White); 
    startText.setPosition(startButton.getPosition().x + (startButton.getSize().x - startText.getLocalBounds().width) / 2, startButton.getPosition().y + (startButton.getSize().y - startText.getLocalBounds().height) / 2);  // Center the text inside the button

    FloatRect clickableRegion(startButton.getPosition().x, startButton.getPosition().y, startButton.getSize().x, startButton.getSize().y);

    Music clickmusic;
    if (!clickmusic.openFromFile("C:\\Users\\User\\Desktop\\Battleship\\223445__six-ways__mouse_click.ogg"))
    {
        std::cerr << "Error loading music!" << std::endl;
        return -1;
    }

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(window);

                    if (clickableRegion.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                    {
                        clickmusic.play();  
                        ShowLoading();
                        RenderWindow Menuwindow(VideoMode(500, 600), "Menu");

                        Texture texture2;
                        if (!texture2.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\loading.jpg"))
                        {
                            return -1;
                        }

                        Sprite sprite(texture2);

                        Vector2u texture2Size = texture2.getSize();
                        float scaleX = 500.0f / texture2Size.x;
                        float scaleY = 600.0f / texture2Size.y;
                        float scale = std::min(scaleX, scaleY);
                        sprite.setScale(scaleX, scaleY);

                        // Start Playing button  
                        RectangleShape StartPlaying;
                        Vector2f buttonsize1(200, 80); 
                        Color buttonColor1(106, 90, 205);                      
                        Vector2f  buttonposition1((500 - buttonsize1.x) / 2, (600 - buttonsize1.y) / 2);
                        createButton(StartPlaying, buttonsize1, buttonColor1, buttonposition1);

                        // "Start playing" text
                        Text StartPlayingText("Start Playing", font, 42);  
                        StartPlayingText.setFillColor(Color::White);   
                        float textX = StartPlaying.getPosition().x + (StartPlaying.getSize().x - StartPlayingText.getLocalBounds().width) / 2;
                        float textY = StartPlaying.getPosition().y + (StartPlaying.getSize().y - StartPlayingText.getLocalBounds().height) / 2;
                        StartPlayingText.setPosition(textX, textY);

                        FloatRect clickableRegion1(StartPlaying.getPosition().x, StartPlaying.getPosition().y, StartPlaying.getSize().x, StartPlaying.getSize().y);

                        // Leaderboard button 
                        RectangleShape Leaderboard(Vector2f(200, 80));   
                        Vector2f  buttonsize2(200, 80);
                        Color buttonColor2(106, 90, 205); 
                        Vector2f  buttonposition2((500 - buttonsize2.x) / 2, (600 - buttonsize2.y) / 3);
                        createButton(Leaderboard, buttonsize2, buttonColor2, buttonposition2);

                        // "Leaderboard" text
                        Text LeaderboardText("Leaderboard", font, 42);  
                        LeaderboardText.setFillColor(Color::White);  
                        float textX1 = Leaderboard.getPosition().x + (Leaderboard.getSize().x - LeaderboardText.getLocalBounds().width) / 2;
                        float textY1 = Leaderboard.getPosition().y + (Leaderboard.getSize().y - LeaderboardText.getLocalBounds().height) / 2;
                        LeaderboardText.setPosition(textX1, textY1);

                        FloatRect clickableRegion2(Leaderboard.getPosition().x, Leaderboard.getPosition().y, Leaderboard.getSize().x, Leaderboard.getSize().y);


                        // Instructions button  
                        RectangleShape Instructions(Vector2f(200, 80));
                        Vector2f  buttonsize3(200, 80);
                        Color buttonColor3(106, 90, 205); 
                        Vector2f  buttonposition3((500 - buttonsize3.x) / 2, (600 - buttonsize3.y) * 2 / 3);
                        createButton(Instructions, buttonsize3, buttonColor3, buttonposition3);
                        
                        // "Instructions" text
                        Text InstructionsText("Instructions", font, 42);  
                        InstructionsText.setFillColor(Color::White);      
                        float textX2 = Instructions.getPosition().x + (Instructions.getSize().x - InstructionsText.getLocalBounds().width) / 2;
                        float textY2 = Instructions.getPosition().y + (Instructions.getSize().y - InstructionsText.getLocalBounds().height) / 2;
                        InstructionsText.setPosition(textX2, textY2);

                        
                        FloatRect clickableRegion3(Instructions.getPosition().x, Instructions.getPosition().y, Instructions.getSize().x, Instructions.getSize().y);

                        // Mouse click sound
                        Music music;
                        if (!music.openFromFile("C:\\Users\\User\\Desktop\\Battleship\\657643__josefpres__8-bit-game-loop-010-only-bass-long-120-bpm.ogg"))
                        {
                            std::cerr << "Error loading music!" << std::endl;
                            return -1;
                        }

                        // i con of sound
                        Texture audioOnTexture, audioOffTexture;
                        if (!audioOnTexture.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\Sound-on-icon.png") || !audioOffTexture.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\Sound-off-icon.png"))
                        {
                            std::cerr << "Error loading icon images!" << std::endl;
                            return -1;
                        }

                        Sprite audioOnSprite(audioOnTexture);
                        Sprite audioOffSprite(audioOffTexture);

                        
                        audioOnSprite.setPosition(window.getSize().x - audioOnSprite.getLocalBounds().width - 10, 10); 
                        audioOffSprite.setPosition(window.getSize().x - audioOffSprite.getLocalBounds().width - 10, 10); 

                        bool audioOn = true; 

                        music.setLoop(true);
                        music.play();

                        while (Menuwindow.isOpen())
                        {
                            sf::Event event;
                            while (Menuwindow.pollEvent(event))
                            {
                                if (event.type == sf::Event::Closed)
                                    Menuwindow.close();

                                if (event.type == sf::Event::MouseButtonPressed)
                                {
                                    if (event.mouseButton.button == sf::Mouse::Left)
                                    {
                                        Vector2i mousePos = Mouse::getPosition(window);

                                        FloatRect audioBounds; 
                                        if (audioOn) 
                                        {
                                            audioBounds = audioOnSprite.getGlobalBounds(); 
                                        }
                                        else 
                                        {
                                            audioBounds = audioOffSprite.getGlobalBounds(); 
                                        }
                                         
                                        if (audioBounds.contains(Vector2f(event.mouseButton.x, event.mouseButton.y)))  
                                        {
                                            
                                            if (audioOn) 
                                            {
                                                music.pause(); 
                                                audioOn = false; 
                                            }
                                            else
                                            {
                                                music.play();  
                                                audioOn = true; 
                                            }
                                        }
                                        
                                        if (clickableRegion1.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                                        {
                                            clickmusic.play();  
                                            ShowLoading();
                                            RenderWindow Modewindow(VideoMode(500, 600), "Mode");

                                            RectangleShape SinglePlayer;
                                            Vector2f  buttonsize4(200, 80);
                                            Color buttonColor4(106, 90, 205);
                                            Vector2f  buttonposition4((500 - buttonsize4.x) / 2, (600 - buttonsize4.y) / 3);
                                            createButton(SinglePlayer, buttonsize4, buttonColor4, buttonposition4);

                                            Text SinglePlayerText("Single Player", font, 42);
                                            SinglePlayerText.setFillColor(Color::White);
                                            float textX1 = SinglePlayer.getPosition().x + (SinglePlayer.getSize().x - SinglePlayerText.getLocalBounds().width) / 2;
                                            float textY1 = SinglePlayer.getPosition().y + (SinglePlayer.getSize().y - SinglePlayerText.getLocalBounds().height) / 2;
                                            SinglePlayerText.setPosition(textX1, textY1);

                                            FloatRect clickableRegionS(SinglePlayer.getPosition().x, SinglePlayer.getPosition().y, SinglePlayer.getSize().x, SinglePlayer.getSize().y);

                                            // Multiplayer Button
                                            RectangleShape Multiplayer;
                                            Vector2f  buttonsize5(200, 80);
                                            Color buttonColor5(106, 90, 205);
                                            Vector2f  buttonposition5((500 - buttonsize5.x) / 2, (600 - buttonsize5.y) / 2);
                                            createButton(Multiplayer, buttonsize5, buttonColor5, buttonposition5);

                                            Text MultiplayerText("Multiplayer", font, 42);
                                            MultiplayerText.setFillColor(Color::White);
                                            float textX2 = Multiplayer.getPosition().x + (Multiplayer.getSize().x - MultiplayerText.getLocalBounds().width) / 2;
                                            float textY2 = Multiplayer.getPosition().y + (Multiplayer.getSize().y - MultiplayerText.getLocalBounds().height) / 2;
                                            MultiplayerText.setPosition(textX2, textY2);

                                            FloatRect clickableRegionM(Multiplayer.getPosition().x, Multiplayer.getPosition().y, Multiplayer.getSize().x, Multiplayer.getSize().y);

                                            // Back to menu button  
                                            RectangleShape BacktoMenu;
                                            Vector2f  buttonsize6(200, 80);
                                            Color buttonColor6(106, 90, 205);
                                            Vector2f  buttonposition6((500 - buttonsize6.x) / 2, (600 - buttonsize6.y) * 2 / 3);
                                            createButton(BacktoMenu, buttonsize6, buttonColor6, buttonposition6);

                                            // "Back to menu" text
                                            Text BacktoMenuText("Back To Menu", font, 42);  // Font size 42
                                            BacktoMenuText.setFillColor(Color::White);  // White color for the text    
                                            float textX3 = BacktoMenu.getPosition().x + (BacktoMenu.getSize().x - BacktoMenuText.getLocalBounds().width) / 2;
                                            float textY3 = BacktoMenu.getPosition().y + (BacktoMenu.getSize().y - BacktoMenuText.getLocalBounds().height) / 2;
                                            BacktoMenuText.setPosition(textX3, textY3);

                                            FloatRect clickableRegionB(BacktoMenu.getPosition().x, BacktoMenu.getPosition().y, BacktoMenu.getSize().x, BacktoMenu.getSize().y);

                                            while (Modewindow.isOpen())
                                            {
                                                sf::Event event1;
                                                while (Modewindow.pollEvent(event1))
                                                {
                                                    if (event1.type == sf::Event::Closed)
                                                        Modewindow.close();

                                                    if (event1.type == sf::Event::MouseButtonPressed)
                                                    {
                                                        if (event1.mouseButton.button == sf::Mouse::Left)
                                                        {
                                                            Vector2i mousePos = Mouse::getPosition(Modewindow);

                                                            if (clickableRegionB.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                                                            {
                                                                clickmusic.play();   
                                                                Modewindow.close();
                                                            }
                                                            else if (clickableRegionS.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                                                            {
                                                                clickmusic.play();   
                                                                RenderWindow Namewindow(VideoMode(500, 600), "Name");

                                                                Text EnterName("Enter your name", font, 60);  // Font size 60 
                                                                EnterName.setFillColor(Color::White);  // White color for the text 
                                                                EnterName.setStyle(Text::Underlined);
                                                                FloatRect NtextBounds = EnterName.getLocalBounds();
                                                                float NtextX = (window.getSize().x - NtextBounds.width) / 2;
                                                                float NtextY = 20;
                                                                EnterName.setPosition(NtextX, NtextY);

                                                                Vector2f textboxSize(400, 50);
                                                                RectangleShape textbox(textboxSize); // Width: 400px, Height: 50px   
                                                                textbox.setPosition(50, 80); // Position the textbox 
                                                                textbox.setFillColor(sf::Color(255, 255, 255, 200)); // Light white background 
                                                                textbox.setOutlineThickness(2);
                                                                textbox.setOutlineColor(sf::Color::Black);

                                                                sf::Vector2u windowSize = Namewindow.getSize();
                                                                textbox.setPosition((windowSize.x - textboxSize.x) / 2, (windowSize.y - textboxSize.y) / 2);

                                                                sf::Text inputText("", font, 24);
                                                                inputText.setPosition(textbox.getPosition().x + 10, textbox.getPosition().y + 10);
                                                                inputText.setFillColor(sf::Color::Black);

                                                                
                                                                char name[51] = ""; 
                                                                int nameLength = 0;
                                                                bool nameEntered = false;

                                                                while (Namewindow.isOpen())
                                                                {
                                                                    sf::Event event;
                                                                    while (Namewindow.pollEvent(event))
                                                                    {
                                                                        if (event.type == sf::Event::Closed)
                                                                        {
                                                                            Namewindow.close();
                                                                        }

                                                                        if (!nameEntered && event.type == sf::Event::TextEntered)
                                                                        {
                                                                            char c = static_cast<char>(event.text.unicode);

                                                                            if (c == '\b' && nameLength > 0)  // backspaceee
                                                                            {
                                                                                name[--nameLength] = '\0';
                                                                            }
                                                                            else if (c >= 32 && c < 127 && nameLength < 50)  // normally
                                                                            {
                                                                                name[nameLength++] = c;
                                                                                name[nameLength] = '\0';
                                                                            }
                                                                            else if (c == '\r' && nameLength > 0)  // enter
                                                                            {
                                                                                nameEntered = true;
                                                                                ofstream f1("Leaderboard.txt", ios::app);
                                                                                if (f1.is_open())
                                                                                {
                                                                                    f1 << name << std::endl;
                                                                                    f1.close();
                                                                                }
                                                                                name[0] = '\0'; // dobara shuru jab agli dafa krein 
                                                                                nameLength = 0;

                                                                                RenderWindow SGamewindow(VideoMode(950, 600), "Single Player Battleship Game");
                                                                                SGstartGame(SGamewindow);
                                                                            }
                                                                        }
                                                                    
                                                                                inputText.setString(name);

                                                                        Namewindow.clear(sf::Color::White);
                                                                        Namewindow.draw(sprite);
                                                                        Namewindow.draw(textbox);
                                                                        Namewindow.draw(EnterName);
                                                                        Namewindow.draw(inputText);
                                                                        Namewindow.display();
                                                                    }
                                                                }

                                                            }
                                                            else if (clickableRegionM.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                                                            {
                                                                RenderWindow MPwindow(VideoMode(950, 600), "Multiplayer Battleship Game");  
                                                                MPstartgame(MPwindow); 
                                                            }
                                                        }
                                                    }
                                                }
                                                Modewindow.clear();
                                                Modewindow.draw(sprite);
                                                Modewindow.draw(SinglePlayer);
                                                Modewindow.draw(SinglePlayerText);
                                                Modewindow.draw(Multiplayer);
                                                Modewindow.draw(MultiplayerText);
                                                Modewindow.draw(BacktoMenu);
                                                Modewindow.draw(BacktoMenuText);
                                                Modewindow.display();
                                            }


                                        }
                                        else if (clickableRegion2.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                                        {
                                            clickmusic.play();   
                                            ShowLoading();
                                            RenderWindow LBwindow(VideoMode(500, 600), "Leaderboard");

                                            Text LB("LEADERBOARD", font, 60);  
                                            LB.setFillColor(Color::White);   
                                            LB.setStyle(Text::Underlined);

                                            FloatRect LBBounds = LB.getLocalBounds();
                                            float LBtextX = (window.getSize().x - LBBounds.width) / 2;
                                            float LBtextY = 20;
                                            LB.setPosition(LBtextX, LBtextY);

                                            char names[10][20];  // 10 players with name upto 20 chars
                                            int count = 0;      

                                            ifstream f1("Leaderboard.txt");
                                            if (f1.is_open())
                                            {
                                                while (f1 >> names[count] && count < 10)
                                                {
                                                   
                                                    LB.setString(names[count]);
                                             
                                                    cout << names[count] << endl;    
                                                    count++;
                                                }
                                                f1.close();
                                            }
                                            Text entries[10];  
                                            for (int i = 0; i < count; ++i)
                                            {
                                                char entry[50]; 
                                                entries[i].setString(entry);
                                                entries[i].setFont(font);
                                                entries[i].setCharacterSize(30);
                                                entries[i].setFillColor(Color::White);  
                                                entries[i].setPosition(50, 50);
                                            }

                                            while (LBwindow.isOpen())
                                            {
                                                Event leaderboardEvent;
                                                while (LBwindow.pollEvent(leaderboardEvent))
                                                {
                                                    if (leaderboardEvent.type == Event::Closed)
                                                    {
                                                        LBwindow.close();
                                                    }

                                                    ifstream f1("Leaderboard.txt");
                                                    cout << "LEADERBOARD" << endl;
                                                    int i = 0;
                                                    while (f1.getline(names[i], 20))
                                                    {
                                                        cout << names[i] << "  " << endl;
                                                        i++;

                                                    }
                                                    f1.close();
                                                }

                                                LBwindow.clear();
                                                LBwindow.draw(sprite);
                                                LBwindow.draw(LB);

                                                for (int i = 0; i < count; ++i)
                                                {
                                                    LBwindow.draw(entries[i]);
                                                    cout << endl;  
                                                }
                                                LBwindow.display();
                                            }
                                        
                                        }
                                        else if (clickableRegion3.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) 
                                        {
                                            clickmusic.play();     
                                            ShowLoading(); 
                                            RenderWindow Instructionswindow(VideoMode(500, 600), "Instructions"); 

                                            // rules of game heading
                                            Text Rulestext("HOW TO PLAY", font, 60); 
                                            Rulestext.setFillColor(Color::White);  
                                            Rulestext.setStyle(Text::Underlined);  
                                            FloatRect textBounds = Rulestext.getLocalBounds(); 
                                            float RtextX = (window.getSize().x - textBounds.width) / 2;  
                                            float RtextY = 20; 
                                            Rulestext.setPosition(RtextX, RtextY);  

                                            //rules of game
                                            Text RulesListtext("Sink all of your opponent's ships before they sink yours.\nGuess the location of opponent's ship.\n Box changes to red if hit otherwise green if miss.\nThe player to find all ships of opponent wins.", font, 36);  // Font size 40  
                                            RulesListtext.setFillColor(Color::White);  
                                            FloatRect RulesListtextBounds = RulesListtext.getLocalBounds(); 
                                            float RLtextX = (window.getSize().x - RulesListtextBounds.width) / 2;  
                                            float RLtextY = 100; 
                                            RulesListtext.setPosition(RLtextX, RLtextY);  

                                            // Back to menu button  
                                            RectangleShape BacktoMenu(Vector2f(200, 80)); 
                                            Vector2f  buttonsize7(200, 80);
                                            Color buttonColor7(106, 90, 205);
                                            Vector2f  buttonposition7((500 - buttonsize7.x) / 2, (600 - buttonsize7.y));
                                            createButton(BacktoMenu, buttonsize7, buttonColor7, buttonposition7);
                                            

                                            // "Back to menu" text
                                            Text BacktoMenuText("Back To Menu", font, 42); 
                                            BacktoMenuText.setFillColor(Color::White);   
                                            float textX4 = BacktoMenu.getPosition().x + (BacktoMenu.getSize().x - BacktoMenuText.getLocalBounds().width) / 2;
                                            float textY4 = BacktoMenu.getPosition().y + (BacktoMenu.getSize().y - BacktoMenuText.getLocalBounds().height) / 2;
                                            BacktoMenuText.setPosition(textX4, textY4);
 
                                            FloatRect clickableRegion4(BacktoMenu.getPosition().x, BacktoMenu.getPosition().y, BacktoMenu.getSize().x, BacktoMenu.getSize().y);

                                            while (Instructionswindow.isOpen())
                                            {
                                                sf::Event event1;
                                                while (Instructionswindow.pollEvent(event1))
                                                {
                                                    if (event1.type == sf::Event::Closed)
                                                        Instructionswindow.close();

                                                    if (event1.type == sf::Event::MouseButtonPressed)
                                                    {
                                                        if (event1.mouseButton.button == sf::Mouse::Left)
                                                        {
                                                            Vector2i mousePos = Mouse::getPosition(Instructionswindow);

                                                            if (clickableRegion4.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                                                            {
                                                                clickmusic.play();    
                                                                Instructionswindow.close();
                                                            }

                                                        }
                                                    }
                                                }
                                                Instructionswindow.clear();
                                                Instructionswindow.draw(sprite);
                                                Instructionswindow.draw(Rulestext);  
                                                Instructionswindow.draw(RulesListtext); 
                                                Instructionswindow.draw(BacktoMenu); 
                                                Instructionswindow.draw(BacktoMenuText);
                                                Instructionswindow.display();
                                            }
                                        }
                                    }

                                }

                                Menuwindow.clear();
                                Menuwindow.draw(sprite); 
                                if (audioOn) 
                                {
                                    Menuwindow.draw(audioOnSprite); 
                                }
                                else
                                {
                                    Menuwindow.draw(audioOffSprite); 
                                }
                                Menuwindow.draw(StartPlaying); 
                                Menuwindow.draw(StartPlayingText); 
                                Menuwindow.draw(Leaderboard);  
                                Menuwindow.draw(LeaderboardText);  
                                Menuwindow.draw(Instructions);  
                                Menuwindow.draw(InstructionsText);   
                                Menuwindow.display(); 
                            }

                        }  
                    }   
                }
            }
        }
        window.clear();
        window.draw(sprite);
        window.draw(startButton);
        window.draw(startText);
        window.display();
    }

    return 0;
}