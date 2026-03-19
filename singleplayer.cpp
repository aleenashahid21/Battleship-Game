//
//#include <iostream>
//#include <cstdlib> // for random function
//#include <ctime> // for seed time
//#include <algorithm> // for min max
//#include <string>
//#include <SFML/Graphics.hpp>
//#include <SFML/System.hpp>
//#include <SFML/Window.hpp>
//#include <SFML/Audio.hpp>
//#include <SFML/Network.hpp>
//
//using namespace std;
//using namespace sf;
//
//const int gridsize = 10;
//const int cellsize = 35;  // Reduced size for better fit
//const int margin = 50;
//const int EMPTY = 0;
//const int SHIP = 1;
//const int HIT = 3;
//const int MISS = 4;
//int playerScore = 100;  
//int aiScore = 100;   
//
//
//int playergrid[10][10];
//int opponentgrid[10][10];
//
//int previousattacks[10][10] = { 0 };
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
//
//    if (!missMusic.openFromFile("C:\\Users\\User\\Desktop\\Battleship\\479070__nox_sound__foley_impact_stone_water_02.ogg"))
//    {
//        std::cerr << "Error loading hit sound!" << std::endl;
//        return false; 
//    }
//
//    if (!Gameovermusic.openFromFile("C:\\Users\\User\\Desktop\\Battleship\\664606__mt4000__game-over-low.ogg"))
//    {
//        std::cerr << "Error loading music!" << std::endl;
//    }
//
//    return true;  
//}
//
//void initializegrid(int grid[10][10])
//{
//    for (int i = 0; i < 10; i++)
//    {
//        for (int k = 0; k < 10; k++)
//        {
//            grid[i][k] = 0;
//        }
//    }
//}
//
//void placeship(int grid[10][10], RenderWindow& window)
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
//void randomshipplacement(int grid[10][10])
//{
//    int sizearr[5] = { 5, 4, 3, 3, 2 };
//
//    for (int i = 0; i < 5; i++) 
//    {
//        int size = sizearr[i];
//        bool placed = false;
//
//        while (!placed) 
//        {
//            int rowindex = rand() % 10;
//            int col = rand() % 10;
//            char direction;
//            if (rand() % 2 == 0)
//            {
//                direction = 'h';
//            }
//            else
//            {
//                direction = 'v';
//            }
//
//            bool canPlace = true;
//
//            if (direction == 'h')
//            {
//                if (col + size <= 10)
//                {
//                    for (int x = 0; x < size; x++)
//                    {
//                        if (grid[rowindex][col + x] != 0) 
//                        {
//                            canPlace = false;
//                            break;
//                        }
//                    }
//                }
//                else 
//                {
//                    canPlace = false;
//                }
//            }
//            else
//            {
//                if (rowindex + size <= 10) 
//                {
//                    for (int x = 0; x < size; x++)
//                    {
//                        if (grid[rowindex + x][col] != 0) 
//                        {
//                            canPlace = false;
//                            break;
//                        }
//                    }
//                }
//                else 
//                {
//                    canPlace = false;
//                }
//            }
//
//            if (canPlace)
//            {
//                if (direction == 'h')
//                {
//                    for (int x = 0; x < size; x++)
//                    {
//                        grid[rowindex][col + x] = 1;
//                    }
//                }
//                else
//                {
//                    for (int x = 0; x < size; x++)
//                    {
//                        grid[rowindex + x][col] = 1;
//                    }
//                }
//                placed = true;
//            }
//        }
//    }
//}
//
//void printgrid(int grid[10][10])
//{
//    cout << "  ";
//    for (int i = 0; i < 10; i++)
//    {
//        cout << i << " ";
//    }
//    cout << endl;
//    for (int i = 0; i < 10; i++)
//    {
//        cout << char('A' + i) << " ";
//        for (int k = 0; k < 10; k++)
//        {
//            cout << grid[i][k] << " ";
//        }
//        cout << endl;
//    }
//}
//
//void drawgrid(RenderWindow& window,
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
//
//            // Set cell color based on its value in the grid
//            switch (grid[i][j])
//            {
//            case 1: // Ship (concealed or revealed)
//                if (!concealShips)
//                {
//                    cell.setFillColor(Color(139, 69, 19, 200)); // Brown for revealed ships 
//                }
//                else
//                {
//                    cell.setFillColor(Color(0, 0, 0, 0)); //  for concealed ships  
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
//            
//            window.draw(cell);
//        }
//    }
//}
//
//bool isValidCell(int row, int col)
//{
//    return row >= 0 && row < 10 && col >= 0 && col < 10;
//}
//
//void playershoot(RenderWindow& window, int enemyGrid[10][10], float offsetX, float offsetY, float cellSize, bool& isPlayerTurn)
//{
//    static bool clickProcessed = false;
//
//    if (Mouse::isButtonPressed(Mouse::Left) && !clickProcessed) // detection  
//    {
//        clickProcessed = true;
//
//        Vector2i mousePos = Mouse::getPosition(window);
//
//        //  mouse position to grid 
//        int col = (mousePos.x - offsetX) / cellSize;
//        int row = (mousePos.y - offsetY) / cellSize;
//
//        // click is in a valid cell or not
//        if (col >= 0 && col < 10 && row >= 0 && row < 10)
//        {
//            //  if cell has already been shot at
//            if (enemyGrid[row][col] == HIT || enemyGrid[row][col] == MISS)
//            {
//                cout << "This position has already been shot at. Try again." << endl;
//                return;
//            }
//
//            // If the player hits a ship (1), mark it as a hit (3)
//            if (enemyGrid[row][col] == SHIP)
//            {
//                enemyGrid[row][col] = HIT; // Mark the hit
//                cout << "You hit a ship at (" << row << ", " << col << ")!" << endl;
//                if (hitMusic.getStatus() != sf::Music::Playing)
//                {
//                    hitMusic.play();
//                }
//            }
//            // If player misses
//            else
//            {
//                enemyGrid[row][col] = MISS; // Mark the miss
//                cout << "You missed at (" << row << ", " << col << ")." << endl;
//                if (missMusic.getStatus() != sf::Music::Playing)
//                {
//                    missMusic.play();
//                }
//            }
//            playerScore--;
//            isPlayerTurn = false;
//        }
//    }
//    else if (!Mouse::isButtonPressed(Mouse::Left))
//    {
//        clickProcessed = false; // Reset click 
//    }
//}
//
//
//
//bool trydirection(int r, int c, int grid[10][10], int dir)
//{
//    int direction[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };
//    int newrow = r + direction[dir - 1][0];
//    int newcol = c + direction[dir - 1][1];
//
//    if (newrow >= 0 && newrow < 10 && newcol >= 0 && newcol < 10 && previousattacks[newrow][newcol] == 0)
//    {
//        if (grid[newrow][newcol] == 1) 
//        {
//            cout << "AI hit your ship" << endl;
//            grid[newrow][newcol] = 3;
//            previousattacks[newrow][newcol] = 1;
//            return true;
//        }
//        else if (grid[newrow][newcol] == 0) 
//        {
//            cout << "AI Missed" << endl;
//            previousattacks[newrow][newcol] = 1;
//            return true;
//        }
//    }
//    return false;
//}
//
//void AIshoot(int playerGrid[10][10])
//{
//    static bool hitOnce = false;
//    static int rowOfLastHit = -1, colOfLastHit = -1;
//
//    // If AI has already hit a ship, search around the last hit
//    if (hitOnce) 
//    {
//        // Search in the surrounding cells
//        bool found = false;
//        for (int dir = 1; dir <= 4; ++dir)
//        {
//            if (trydirection(rowOfLastHit, colOfLastHit, playerGrid, dir))
//            {
//                found = true;
//                break;
//            }
//        }
//
//        // If no adjacent hit was found, pick a random shot
//        if (!found) 
//        {
//            hitOnce = false;
//            AIshoot(playerGrid); // Recursively call to try again with a random shot
//        }
//    }
//    else
//    {
//        // Make a random shot
//        int rindex = rand() % 10;
//        int cindex = rand() % 10;
//
//        while (previousattacks[rindex][cindex] !=0) 
//        {
//            rindex = rand() % 10;
//            cindex = rand() % 10;
//        }
//
//        if (playerGrid[rindex][cindex] == 1) 
//        {
//            cout << "AI hit your ship at (" << rindex << ", " << cindex << ")!" << endl;
//            playerGrid[rindex][cindex] = HIT;  
//            previousattacks[rindex][cindex] = 1;
//            hitOnce = true; // The AI now remembers the hit
//            rowOfLastHit = rindex;
//            colOfLastHit = cindex;
//        }
//        else
//        {
//            cout << "AI missed at (" << rindex << ", " << cindex << ")." << endl;
//            playerGrid[rindex][cindex] = MISS;   
//            previousattacks[rindex][cindex] = 1;
//        }
//    }
//    aiScore--;        
//}
//
//
//bool terminate(int grid[10][10])
//{
//    for (int i = 0; i < 10; i++) 
//    {
//        for (int j = 0; j < 10; j++) 
//        {
//            if (grid[i][j] == 1)
//            {
//                return false;
//            }
//        }
//    }
//    return true;
//}
//
//void playerturn(RenderWindow& window, int grid[10][10], float offsetX, float offsetY, float cellSize, bool& isPlayerTurn) 
//{
//    playershoot(window, grid, offsetX, offsetY, cellSize, isPlayerTurn);
//    if (!isPlayerTurn)  // If the player has taken their turn 
//    {
//        isPlayerTurn = false; // Hand over to AI
//    }
//}
//
//void displayGameOver(RenderWindow& window, const string& resultMessage)
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
//    Text playerFinalScore("Your Score: " + std::to_string(playerScore), font1, 30); 
//    playerFinalScore.setFillColor(Color::Black);
//    playerFinalScore.setPosition(window.getSize().x / 2.0f - playerFinalScore.getLocalBounds().width / 2.0f,
//        window.getSize().y / 2.0f + 50);
//
//    Text aiFinalScore("AI's Score: " + std::to_string(aiScore), font1, 30);
//    aiFinalScore.setFillColor(Color::Black);
//    aiFinalScore.setPosition(window.getSize().x / 2.0f - aiFinalScore.getLocalBounds().width / 2.0f,window.getSize().y / 2.0f + 100);
//
//    // Draw the scores
//    //window.draw(gameOverText);
//    window.draw(playerFinalScore);
//    window.draw(aiFinalScore);
//    window.draw(gameOverText);    
//
//   
//
//     // Draw the text
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
//      Gameovermusic.stop();  
//      // Close the window
//    window.close();
//}
//
//void gameloop(RenderWindow& window)
//{
//    float offsetX = 50.f; // Horizontal offset for the grid 
//    float offsetY = 100.f; // Vertical offset for the grid 
//    float cellSize = 35.f;
//
//    bool isPlayerTurn = true;
//    bool Gameover = false;
//
//    Font font1; 
//    if (!font1.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\CENTAUR.TTF")) 
//    {
//        throw std::runtime_error("Font file not found."); 
//    }
//
//    while (window.isOpen())
//    {
//        Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == Event::Closed)
//                window.close();
//        }
//
//        // Clear the window
//        window.clear(Color::White);
//
//        drawgrid(window, playergrid, margin, margin, true, cellsize, "Player's Grid");
//        drawgrid(window, opponentgrid, margin + 500, margin, false, cellsize, "Opponent's Grid");
//
//        if (!Gameover)
//        {
//
//            if (isPlayerTurn)
//            {
//                // Player's turn
//                playerturn(window, opponentgrid, offsetX + 500, offsetY, cellSize, isPlayerTurn);  // Player makes a shot      
//
//                // Check if the player won
//                if (terminate(opponentgrid))
//                {
//                    displayGameOver(window, "You win!");
//                    Gameover = true;
//                    break;
//                }
//                sf::sleep(sf::seconds(4));
//            }
//            else
//            {
//                // AI's turn
//                AIshoot(playergrid); // AI makes a shot 
//
//                // Check if the AI won
//                if (terminate(playergrid))
//                {
//                    displayGameOver(window, "AI wins!");
//                    Gameover = true;
//                }
//
//                isPlayerTurn = true;
//            }
//        }
//            // Display scores
//            Text playerScoreText("Player Score: " + std::to_string(playerScore), font1, 24);   
//            playerScoreText.setFillColor(Color::Black);
//            playerScoreText.setPosition(offsetX, offsetY - 60); // Position above the player's grid
//
//            Text aiScoreText("AI Score: " + std::to_string(aiScore), font1, 24); 
//            aiScoreText.setFillColor(Color::Black);
//            aiScoreText.setPosition(offsetX + 500, offsetY - 60); // Position above the AI's grid
//
//            window.draw(playerScoreText);
//            window.draw(aiScoreText);
//
//            // Display the updated window
//            window.display();
//            sf::sleep(sf::seconds(2));  
//        }
//    }
//
//
//
//
//int main()
//{
//    // Initialize the window for the Battleship game
//    RenderWindow window(VideoMode(950, 600), "Battleship Game"); 
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
//    // Initialize the random seed for ship placements
//    srand(static_cast<unsigned>(time(0))); 
//
//    // Initialize player and opponent grids
//    int playerGrid[10][10];   // Player's grid 
//    int opponentGrid[10][10]; // Opponent's grid    
//
//    initializegrid(playerGrid);
//    placeship(playerGrid, window);  // Player places their ships 
//
//    initializegrid(opponentGrid);
//    randomshipplacement(opponentGrid);  // AI places its ships randomly
//
//    if (!loadMusic())
//    {
//        return -1; // If music files fail to load, exit the game
//    }
//
//    bool concealShips = true; 
//    bool isPlayerTurn = true;
//    bool gameover = false;
//    float cellSize = 40.0f; // Size of each cell in the grid
//
//    while (window.isOpen()&& !gameover)
//    {
//
//        Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == Event::Closed)
//                window.close();
//        }
//
//        // Clear the window
//        window.clear(); 
//        window.draw(sprite3);  
//
//        // Draw the player's grid with ships visible
//        drawgrid(window, playerGrid, 50, 100, false, cellSize, "Player Grid");   
//        drawgrid(window, opponentGrid, 500, 100, true, cellSize, "Opponent Grid"); // Changed from 450 to 500
//
//        
//
//        // Game logic 
//        if (isPlayerTurn) 
//        {
//            playerturn(window, opponentGrid, 500, 100, cellSize, isPlayerTurn);  
//
//            if (terminate(opponentGrid))   
//            { 
//                displayGameOver(window, "You Win!"); 
//                gameover = true;  
//                continue; 
//            }
//            
//        }
//        else 
//        {
//            AIshoot(playerGrid); 
//
//            if (terminate(playerGrid)) 
//            {
//                displayGameOver(window, "AI Wins!"); 
//                gameover = true; 
//                continue;
//            }
//            isPlayerTurn = true; 
//        }
//        
//        // Display the updated window
//        window.display();  
//    }
//
//    return 0;
//}