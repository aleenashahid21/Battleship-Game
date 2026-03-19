//#include <iostream>
//#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
//#include <SFML/Window.hpp>
//#include <SFML/System.hpp>
//#include <SFML/Network.hpp>
//#include <cstdlib> // for random function
//#include <ctime> // for seed time
//#include <algorithm> // for min max
//#include <string>
//
//using namespace std;
//using namespace sf;
//
//const int gridsize = 10;
//const int cellsize = 35;
//const int EMPTY = 0;
//const int SHIP = 1;
//const int HIT = 3;
//const int MISS = 4;
//int player1Score = 100;
//int player2Score = 100;
// 
//int player1grid[10][10];  
//int P2grid[10][10];
//
//Music hitMusic;
//Music missMusic;
//Music Gameovermusic;
//
//bool loadMusic()
//{
//    if (!hitMusic.openFromFile("C:\\Users\\User\\Desktop\\Battleship\\220078__nicocadiou__hitmouv_short02.ogg")) 
//    {
//        std::cerr << "Error loading hit sound!" << std::endl;
//        return false;
//    }
//    if (!missMusic.openFromFile("C:\\Users\\User\\Desktop\\Battleship\\479070__nox_sound__foley_impact_stone_water_02.ogg"))
//    {
//        std::cerr << "Error loading miss sound!" << std::endl;
//        return false;
//    }
//    if (!Gameovermusic.openFromFile("C:\\Users\\User\\Desktop\\Battleship\\664606__mt4000__game-over-low.ogg"))
//    {
//        std::cerr << "Error loading game over music!" << std::endl;
//    }
//    return true;
//}
//
//void initializegrid(int grid[10][10])
//{
//    for (int i = 0; i < 10; i++) {
//        for (int k = 0; k < 10; k++) 
//        {
//            grid[i][k] = EMPTY;
//        }
//    }
//}
//
//void MPplaceship(int grid[10][10], RenderWindow& window)
//{
//    const int tileSize = 40;
//    const int gridOffset = 50;
//    RectangleShape tile(Vector2f(tileSize - 2, tileSize - 2));
//    tile.setOutlineThickness(1);
//    tile.setOutlineColor(Color::Black);
//
//    RectangleShape shipTile(Vector2f(tileSize - 4, tileSize - 4));
//    shipTile.setFillColor(Color::Green);
//
//    // Define ship sizes for each ship
//    int sizearr[5] = { 5, 4, 3, 3, 2 };
//    int currentShipIndex = 0;
//    bool isHorizontal = true;
//    static bool spacePressed = false;
//    bool updateScreen = true;
//
//    while (currentShipIndex < 5 && window.isOpen())
//    {
//        Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == Event::Closed)
//                window.close();
//
//            // Toggle ship orientation when space is pressed
//            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
//            {
//                if (!spacePressed)
//                {
//                    isHorizontal = !isHorizontal;
//                    spacePressed = true;
//                    updateScreen = true;
//                }
//            }
//            else if (event.type == Event::KeyReleased && event.key.code == Keyboard::Space)
//            {
//                spacePressed = false;
//            }
//
//            // Handle mouse click to place ship
//            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
//            {
//                Vector2i mousePos = Mouse::getPosition(window);
//                int row = (mousePos.y - gridOffset) / tileSize;
//                int col = (mousePos.x - gridOffset) / tileSize;
//
//                if (row >= 0 && row < 10 && col >= 0 && col < 10)
//                {
//                    bool canPlace = true;
//                    // Check if the ship can fit at the selected position
//                    for (int i = 0; i < sizearr[currentShipIndex]; ++i)
//                    {
//                        int r;
//                        int c;
//
//                        if (isHorizontal)
//                        {
//                            r = row;
//                            c = col + i;
//                        }
//                        else
//                        {
//                            r = row + i;
//                            c = col;
//                        }
//
//                        if (r < 0 || r >= 10 || c < 0 || c >= 10 || grid[r][c] == SHIP)
//                        {
//                            canPlace = false;
//                            break;
//                        }
//                    }
//                    // Place the ship if it's valid
//                    if (canPlace)
//                    {
//                        for (int i = 0; i < sizearr[currentShipIndex]; ++i)
//                        {
//                            int r;
//                            int c;
//
//                            if (isHorizontal)
//                            {
//                                r = row;
//                                c = col + i;
//                            }
//                            else
//                            {
//                                r = row + i;
//                                c = col;
//                            }
//
//                            grid[r][c] = SHIP; // Mark the ship's tiles on the grid
//                        }
//                        currentShipIndex++; // Move to the next ship
//                        updateScreen = true;
//                    }
//                }
//            }
//        }
//
//        // Update screen to display current ship placement
//        if (updateScreen)
//        {
//            window.clear(Color::White);
//
//            // Draw the grid
//            for (int i = 0; i < 10; ++i)
//            {
//                for (int j = 0; j < 10; ++j)
//                {
//                    tile.setPosition(gridOffset + j * tileSize, gridOffset + i * tileSize);
//                    if (grid[i][j] == SHIP)
//                    {
//                        tile.setFillColor(Color::Blue);
//                    }
//                    else
//                    {
//                        tile.setFillColor(Color::White);
//                    }
//                    window.draw(tile);
//                }
//            }
//
//            // Show where the player can place the current ship
//            Vector2i mousePos = Mouse::getPosition(window);
//            int row = (mousePos.y - gridOffset) / tileSize;
//            int col = (mousePos.x - gridOffset) / tileSize;
//
//            if (row >= 0 && row < 10 && col >= 0 && col < 10)
//            {
//                bool canPlace = true;
//                // Check if the ship can fit at the selected position
//                for (int i = 0; i < sizearr[currentShipIndex]; ++i)
//                {
//                    int r;
//                    int c;
//
//                    if (isHorizontal)
//                    {
//                        r = row;
//                        c = col + i;
//                    }
//                    else
//                    {
//                        r = row + i;
//                        c = col;
//                    }
//
//                    if (r < 0 || r >= 10 || c < 0 || c >= 10 || grid[r][c] == SHIP)
//                    {
//                        canPlace = false;
//                        break;
//                    }
//                }
//
//                // Visualize the ship at the current mouse position
//                for (int i = 0; i < sizearr[currentShipIndex]; ++i)
//                {
//                    int r;
//                    int c;
//
//                    if (isHorizontal)
//                    {
//                        r = row;
//                        c = col + i;
//                    }
//                    else
//                    {
//                        r = row + i;
//                        c = col;
//                    }
//
//                    if (r >= 0 && r < 10 && c >= 0 && c < 10)
//                    {
//                        shipTile.setPosition(gridOffset + c * tileSize + 2, gridOffset + r * tileSize + 2);
//
//                        // Set color based on canPlace flag
//                        if (canPlace)
//                        {
//                            shipTile.setFillColor(Color::Green); // Valid placement
//                        }
//                        else
//                        {
//                            shipTile.setFillColor(Color::Red); // Invalid placement
//                        }
//
//                        window.draw(shipTile);
//                    }
//                }
//            }
//
//            window.display();
//            updateScreen = false;
//        }
//
//        sleep(milliseconds(16)); // Frame limiter
//    }
//}
//
//
//
//
//void MPdrawgrid(RenderWindow& window,
//    int grid[10][10],
//    float offsetX,
//    float offsetY,
//    bool concealShips,
//    float cellSize,
//    const std::string& title)
//{
//
//    Font font1;
//    if (!font1.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\CENTAUR.TTF"))
//    {
//        throw std::runtime_error("Font file not found.");
//    }
//
//    // Setup the title text
//    Text titleText;
//    titleText.setFont(font1);
//    titleText.setString(title);
//    titleText.setCharacterSize(24); // Font size
//    titleText.setFillColor(Color::White); // Font color
//    titleText.setPosition(offsetX, offsetY - 40); // Position above the grid
//
//    // Draw the title above the grid
//    window.draw(titleText);
//
//    Texture waterTexture;
//    waterTexture.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\water.jpg");
//
//    // Draw the grid
//    for (int i = 0; i < 10; i++)
//    {
//        for (int j = 0; j < 10; j++)
//        {
//            float x = offsetX + j * cellSize;
//            float y = offsetY + i * cellSize;
//
//            Sprite sprite;
//            sprite.setTexture(waterTexture);
//            sprite.setPosition(x, y);
//            sprite.setScale(cellSize / waterTexture.getSize().x, cellSize / waterTexture.getSize().y);
//            window.draw(sprite);
//
//            RectangleShape cell(Vector2f(cellSize, cellSize));
//            cell.setPosition(x, y);
//
//            // Set cell color based on its value in the grid
//            switch (grid[i][j])
//            {
//            case 1: // Ship (concealed or revealed)
//                if (!concealShips)
//                {
//                    cell.setFillColor(Color(0, 0, 0, 0)); // Brown for revealed ships 
//                }
//                else
//                {
//                    cell.setFillColor(Color(0, 0, 0, 0)); // For concealed ships  
//                }
//                break;
//            case 3: // Hit
//                cell.setFillColor(Color(0, 255, 0, 100)); // Green
//                break;
//            case 4: // Miss
//                cell.setFillColor(Color(255, 0, 0, 100)); // Red 
//                break;
//            default:
//                cell.setFillColor(Color(0, 0, 0, 0));
//                break;
//            }
//
//            // Add border outline
//            cell.setOutlineThickness(1);
//            cell.setOutlineColor(Color::Black);
//
//            // Draw the cell
//            window.draw(cell);
//        }
//    }
//}
//
//void MPplayershoot(RenderWindow& window, int enemyGrid[10][10], float offsetX, float offsetY, float cellSize, bool& isPlayerTurn)
//{
//    static bool clickProcessed = false;
//
//    if (Mouse::isButtonPressed(Mouse::Left) && !clickProcessed)
//    {
//        clickProcessed = true;
//
//        Vector2i mousePos = Mouse::getPosition(window);
//        int col = (mousePos.x - offsetX) / cellSize;
//        int row = (mousePos.y - offsetY) / cellSize;
//
//        if (row >= 0 && row < 10 && col >= 0 && col < 10)   //  valid cell or not
//        {
//            if (enemyGrid[row][col] != HIT && enemyGrid[row][col] != MISS)  // not already clicked
//            {
//                if (enemyGrid[row][col] == SHIP)
//                {
//                    enemyGrid[row][col] = HIT;
//                    if (hitMusic.getStatus() != Music::Playing) hitMusic.play();
//
//                    
//                    if (isPlayerTurn) 
//                    {
//                        player2Score -= 1;  // Player 1 hits Player 2 ship
//                    }
//                    else
//                    {
//                        player1Score -= 1;  // Player 2 hits Player 1 ship
//                    }
//                }
//                else
//                {
//                    enemyGrid[row][col] = MISS;
//                    if (missMusic.getStatus() != Music::Playing) missMusic.play();
//
//                    // Decrease the current player's score when they miss
//                    if (isPlayerTurn)
//                    {
//                        player1Score -= 1;  // Player 1 miss
//                    }
//                    else 
//                    {
//                        player2Score -= 1;  // Player 2 miss
//                    }
//                }
//
//                isPlayerTurn = !isPlayerTurn; // Switch turns 
//            }
//        }
//    }
//    else if (!Mouse::isButtonPressed(Mouse::Left)) 
//    {
//        clickProcessed = false;
//    }
//}
//
//
//bool AllShipsSunk(int grid[10][10]) 
//{
//    for (int i = 0; i < 10; ++i) 
//    {
//        for (int j = 0; j < 10; ++j)
//        {
//            if (grid[i][j] == SHIP)
//            {
//                return false; // A ship is still present
//            }
//        }
//    }
//    return true; // no ship left
//}
//
//
//void MPdisplayGameOver(RenderWindow& window, const string& resultMessage)
//{
//    if (!window.isOpen())
//    {
//        cout << "Window is not open!" << endl;
//        return;
//    }
//
//    // Clear the window
//    window.clear(Color::White);
//
//    Font font1;
//    if (!font1.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\CENTAUR.TTF"))
//    {
//        throw std::runtime_error("Font file not found.");
//    }
//
//
//    //  text to display the result
//    Text gameOverText(resultMessage, font1, 50); 
//    gameOverText.setFillColor(Color::Red); 
//    gameOverText.setPosition(window.getSize().x / 2.0f - gameOverText.getLocalBounds().width / 2.0f, window.getSize().y / 2.0f - gameOverText.getLocalBounds().height / 2.0f); 
//
//    //  text for the final scores
//    Text player1FinalScore("Your Score: " + std::to_string(player1Score), font1, 30); 
//    player1FinalScore.setFillColor(Color::Black);
//    player1FinalScore.setPosition(window.getSize().x / 2.0f - player1FinalScore.getLocalBounds().width / 2.0f,
//        window.getSize().y / 2.0f + 50);
//
//    Text player2FinalScore("AI's Score: " + std::to_string(player2Score), font1, 30); 
//    player2FinalScore.setFillColor(Color::Black);
//    player2FinalScore.setPosition(window.getSize().x / 2.0f - player2FinalScore.getLocalBounds().width / 2.0f, window.getSize().y / 2.0f + 100);
//
//    
//    
//    window.draw(player1FinalScore); 
//    window.draw(player2FinalScore); 
//    window.draw(gameOverText); 
//
//
//    Gameovermusic.play();
//    window.display();
//
//    // Wait for a short period
//    sf::Clock clock;
//    while (clock.getElapsedTime().asSeconds() < 3)
//    {
//        // Pause for 3 seconds
//    }
//    Gameovermusic.stop();
//    // Close the window
//    window.close();
//}
//
//int main()
//{
//    RenderWindow window(VideoMode(950, 600), "Battleship");
//
//    Texture texture3;
//    if (!texture3.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\loading.jpg"))
//    {
//        return -1;
//    }
//
//    Vector2u texture3Size = texture3.getSize();
//    float scaleX3 = 950.0f / texture3Size.x;
//    float scaleY3 = 600.0f / texture3Size.y;
//    float scale3 = std::min(scaleX3, scaleY3);
//
//    Sprite sprite3(texture3);
//    sprite3.setScale(scaleX3, scaleY3);
//
//    
//    if (!loadMusic())
//    {
//        return -1;
//    }
//
//    // Initializing of grids
//    initializegrid(player1grid);
//    initializegrid(P2grid);
//
//    // Placing of ships
//    MPplaceship(player1grid, window); 
//    MPplaceship(P2grid, window); 
//
//    
//    Font font;
//    if (!font.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\CENTAUR.TTF")) 
//    {
//        throw std::runtime_error("Font file not found.");
//    }
//
//    Text turnText;
//    turnText.setFont(font);
//    turnText.setCharacterSize(24); 
//    turnText.setFillColor(Color::White); 
//    turnText.setPosition(315.f, 560.f); 
//
//    // Game loop
//    bool isPlayerTurn = true;
//    while (window.isOpen()) 
//    {
//        Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == Event::Closed) 
//            {
//                window.close();
//            }
//        }
//
//        // check if Player 1  won
//        if (AllShipsSunk(P2grid))
//        {
//            MPdisplayGameOver(window, "Player 1 Wins!");
//            break;
//        }
//
//        // Check if Player 2  won
//        if (AllShipsSunk(player1grid)) 
//        {
//            MPdisplayGameOver(window, "Player 2 Wins!");
//            break;
//        }
//
//        // Update turn text 
//        if (isPlayerTurn) 
//        {
//            turnText.setString("Player 1's Turn");
//        }
//        else {
//            turnText.setString("Player 2's Turn");
//        }
//
//        window.clear();
//        window.draw(sprite3);
//        window.draw(turnText);
//
//        // Draw both grids
//        MPdrawgrid(window, player1grid, 50.f, 50.f, !isPlayerTurn, cellsize, "Player 1's Grid");
//        MPdrawgrid(window, P2grid, 500.f, 50.f, isPlayerTurn, cellsize, "Player 2's Grid");
//
//        //  player shoot based on which player has turn
//        if (isPlayerTurn) {
//            MPplayershoot(window, P2grid, 500.f, 50.f, cellsize, isPlayerTurn); // Player 1 guess player 2
//        }
//        else {
//            MPplayershoot(window, player1grid, 50.f, 50.f, cellsize, isPlayerTurn); // Player 2 guess p1
//        }
//
//        window.display();
//    }
//
//    return 0;
//}
