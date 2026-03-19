//#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
//#include <iostream>
//
//using namespace std; 
//using namespace sf;
//
//int main()
//{
//    RenderWindow window(VideoMode(800, 600), "Audio Toggle Example");
//
//    // Load audio file (replace with your audio file path)
//    Music music;
//    if (!music.openFromFile("C:\\Users\\User\\Desktop\\Battleship\\657643__josefpres__8-bit-game-loop-010-only-bass-long-120-bpm.ogg"))
//    {
//        std::cerr << "Error loading music!" << std::endl;
//        return -1;
//    }
//
//    // Load icon images
//    Texture audioOnTexture, audioOffTexture;
//    if (!audioOnTexture.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\Sound-on-icon.png") || !audioOffTexture.loadFromFile("C:\\Users\\User\\Desktop\\Battleship\\Sound-off-icon.png"))
//    {
//        std::cerr << "Error loading icon images!" << std::endl;
//        return -1;
//    }
//
//    Sprite audioOnSprite(audioOnTexture);
//    Sprite audioOffSprite(audioOffTexture);
//
//    // Set the position of the audio icon
//    audioOnSprite.setPosition(700, 20);  // Position the icon in the top-right corner
//    audioOffSprite.setPosition(700, 20); // Same position
//
//    bool audioOn = true; // Track audio state (true = on, false = off)
//
//    // Start playing the music
//    music.setLoop(true);
//    music.play();
//
//    while (window.isOpen())
//    {
//        Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == Event::Closed)
//                window.close();
//
//            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
//            {
//                // Check if the user clicked on the audio icon
//                FloatRect audioBounds = audioOn ? audioOnSprite.getGlobalBounds() : audioOffSprite.getGlobalBounds();
//                if (audioBounds.contains(Vector2f(event.mouseButton.x, event.mouseButton.y)))
//                {
//                    // Toggle the audio
//                    if (audioOn)
//                    {
//                        music.pause(); // Pause the music (turn off)
//                        audioOn = false;
//                    }
//                    else 
//                    {
//                        music.play();  // Play the music (turn on)
//                        audioOn = true;
//                    }
//                }
//            }
//        }
//
//        window.clear();
//
//        // Draw the appropriate icon based on the audio state
//        if (audioOn)
//        {
//            window.draw(audioOnSprite);
//        }
//        else
//        {
//            window.draw(audioOffSprite);
//        }
//
//        window.display();
//    }
//
//    return 0;
//}
