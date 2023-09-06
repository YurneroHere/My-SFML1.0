
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////


#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
class Button {
public:
    Button(std::string btnText, sf::Vector2f buttonSize, int charSize, sf::Color bgColor, sf::Color textColor) {
        button.setSize(buttonSize);
        button.setFillColor(bgColor);

        // Get these for later use:
        btnWidth = buttonSize.x;
        btnHeight = buttonSize.y;

    }

    // Pass font by reference:
    void setFont(sf::Font& fonts) {
        text.setFont(fonts);
    }

    void setBackColor(sf::Color color) {
        button.setFillColor(color);
    }



    void setPosition(sf::Vector2f point) {
        button.setPosition(point);

        // Center text on button:
        float xPos = (point.x + btnWidth / 2) - (text.getLocalBounds().width / 2);
        float yPos = (point.y + btnHeight / 2.2) - (text.getLocalBounds().height / 2);
        text.setPosition(xPos, yPos);
    }

    void drawTo(sf::RenderWindow& window) {
        window.draw(button);
        window.draw(text);
    }

    // Check if the mouse is within the bounds of the button:
    bool isMouseOver(sf::RenderWindow& window) {
        int mouseX = sf::Mouse::getPosition(window).x;
        int mouseY = sf::Mouse::getPosition(window).y;

        int btnPosX = button.getPosition().x;
        int btnPosY = button.getPosition().y;

        int btnxPosWidth = button.getPosition().x + btnWidth;
        int btnyPosHeight = button.getPosition().y + btnHeight;

        if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
            return true;
        }
        return false;
    }
private:
    sf::RectangleShape button;
    sf::Text text;

    int btnWidth;
    int btnHeight;
};
int main()
{
    //std::srand(static_cast<unsigned int>(std::time(NULL)));

    // Define some constants
    const float pi = 3.14159f;
    const int gameWidth = 1200;
    const int gameHeight = 720;
    sf::Vector2f paddleSize(25, 100);
    float ballRadius = 10.f;

    // Create the window of the application
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "SFML Pong",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    // Load the sounds used in the game
    sf::SoundBuffer ballSoundBuffer;
    if (!ballSoundBuffer.loadFromFile("resources/ball.wav"))
        return EXIT_FAILURE;
    sf::Sound ballSound(ballSoundBuffer);

    sf::SoundBuffer btnSoundBuffer;
    if (!btnSoundBuffer.loadFromFile("resources/button-10.wav"))
        return EXIT_FAILURE;
    sf::Sound btnSound(btnSoundBuffer);

    sf::Music bgMusic;
    if (!bgMusic.openFromFile("resources/MainMusic.ogg"))
        return EXIT_FAILURE;

    sf::Texture texture;
    if (!texture.loadFromFile("resources/image3.jpg"))
        return EXIT_FAILURE;
    sf::Sprite sprite(texture);
    sprite.scale(sf::Vector2f(2.f, 1.5));

    // Create the left paddle
    sf::RectangleShape leftPaddle;
    leftPaddle.setSize(paddleSize - sf::Vector2f(3, 3));
    leftPaddle.setOutlineThickness(3);
    leftPaddle.setOutlineColor(sf::Color::Black);
    leftPaddle.setFillColor(sf::Color(100, 100, 200));
    leftPaddle.setOrigin(paddleSize / 2.f);
    int player1Score = 0;


    // Create the right paddle
    sf::RectangleShape rightPaddle;
    rightPaddle.setSize(paddleSize - sf::Vector2f(3, 3));
    rightPaddle.setOutlineThickness(3);
    rightPaddle.setOutlineColor(sf::Color::Black);
    rightPaddle.setFillColor(sf::Color(200, 100, 100));
    rightPaddle.setOrigin(paddleSize / 2.f);
    int player2Score = 0;

    // Create the ball
    sf::CircleShape ball;
    ball.setRadius(ballRadius - 3);
    ball.setOutlineThickness(3);
    ball.setOutlineColor(sf::Color::Black);
    ball.setFillColor(sf::Color::Red);
    ball.setOrigin(ballRadius / 2, ballRadius / 2);

    // Load the text font
    sf::Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;

    // Initialize the pause message
    sf::Text pauseMessage;
    pauseMessage.setFont(font);
    pauseMessage.setCharacterSize(40);
    pauseMessage.setPosition(5.f, gameHeight/2.f);
    pauseMessage.setFillColor(sf::Color::Yellow);
    pauseMessage.setString("Welcome to Pong! \nClick start to begin the game");
    // button Text
    sf::Text btn1Text;
    btn1Text.setFont(font);
    btn1Text.setCharacterSize(40);
    btn1Text.setPosition(50.f, 100.f);
    btn1Text.setFillColor(sf::Color::Black);
    btn1Text.setString("START");


    sf::Text btn2Text;
    btn2Text.setFont(font);
    btn2Text.setCharacterSize(40);
    btn2Text.setPosition(gameWidth - 150, 100.f);
    btn2Text.setFillColor(sf::Color::Black);
    btn2Text.setString("QUIT");

 

    // Define the paddles properties
    const float paddleSpeed = 400.f;
    const float ballSpeed   = 400.f;
    float ballAngle         = 0.f; // to be changed later

    Button btn1("Start", { 200, 100 }, 30, sf::Color::Red, sf::Color::Black);
    btn1.setFont(font);
    btn1.setPosition({ 5.f, 5.f });

    Button btn2("Quit", { 200, 100 }, 30, sf::Color::Red, sf::Color::Black);
    btn2.setFont(font);
    btn2.setPosition({ gameWidth - 201.f, 5.f });

    sf::Clock clock;
    bool isPlaying = false;
    bgMusic.play();
    bgMusic.setLoop(true);
    
    while (window.isOpen())
    {

        // Handle events
        sf::Event event;
        
        while (window.pollEvent(event))
        {
            // Window closed or escape key pressed: exit
                if ((event.type == sf::Event::Closed) ||
                    (event.type == sf::Event::MouseButtonPressed && btn2.isMouseOver(window)))
                {

                    btnSound.play();
                    isPlaying = false;
                    for (int i = 0; i < 1000000000/2; i++);
                    window.close();
                    break;
                }
           

                // Space key pressed: play
                if (event.type == sf::Event::MouseButtonPressed && btn1.isMouseOver(window))
                {
                    btnSound.play();
                    if (!isPlaying)
                    {
                        // (re)start the game
                        isPlaying = true;
                        clock.restart();

                        // Reset the position of the paddles and ball
                        leftPaddle.setPosition(10 + paddleSize.x / 2, gameHeight / 2);
                        rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
                        ball.setPosition(gameWidth / 2, gameHeight / 2);

                        // Reset the ball angle
                        do
                        {
                            // Make sure the ball initial angle is not too much vertical
                            ballAngle = (std::rand() % 360) * 2 * pi / 360;
                        } while (std::abs(std::cos(ballAngle)) < 0.7f);


                    }
                }
                
        }

        if (isPlaying)
        {
           
            float deltaTime = clock.restart().asSeconds();

            // Move the player 1 paddle
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
               (leftPaddle.getPosition().y - paddleSize.y / 2 > 5.f))
            {
                leftPaddle.move(0.f, -paddleSpeed * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
               (leftPaddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f))
            {
                leftPaddle.move(0.f, paddleSpeed * deltaTime);
            }
            // Move the player 2 paddle
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) &&
                (rightPaddle.getPosition().y - paddleSize.y / 2 < gameWidth - 5.f))
            {
                rightPaddle.move(0.f, -paddleSpeed * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
                (rightPaddle.getPosition().y + paddleSize.y / 2 > 5.f))
            {
                rightPaddle.move(0.f, paddleSpeed * deltaTime);
            }

            // Move the ball
            float factor = ballSpeed * deltaTime;
            ball.move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);

            // Check collisions between the ball and the screen
            if (ball.getPosition().x - ballRadius < 0.f)
            {
                isPlaying = false;
                player1Score++;
          
                pauseMessage.setString("P2 won this round!  \n\n Scores  \n P1:  " + std::to_string(player1Score) + "  -  " + std::to_string(player2Score) +"  :P2\n\nClick start to continue or\n click quit to exit");
            }
            if (ball.getPosition().x + ballRadius > gameWidth)
            {
                isPlaying = false;
                player2Score++;
                pauseMessage.setString("P1 won this round!  \n\n Scores  \n P1:  " + std::to_string(player1Score) + "  -  " + std::to_string(player2Score) +"  :P2\n\nPress start to continue or\n click quit to exit");
            }
            if (ball.getPosition().y - ballRadius < 0.f)
            {
                ballSound.play();
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, ballRadius + 0.1f);
            }
            if (ball.getPosition().y + ballRadius > gameHeight)
            {
                ballSound.play();
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, gameHeight - ballRadius - 0.1f);
            }

            // Check the collisions between the ball and the paddles
            // Left Paddle
            if (ball.getPosition().x - ballRadius < leftPaddle.getPosition().x + paddleSize.x / 2 &&
                ball.getPosition().x - ballRadius > leftPaddle.getPosition().x &&
                ball.getPosition().y + ballRadius >= leftPaddle.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRadius <= leftPaddle.getPosition().y + paddleSize.y / 2)
            {
                if (ball.getPosition().y > leftPaddle.getPosition().y)
                    ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

                ballSound.play();
                ball.setPosition(leftPaddle.getPosition().x + ballRadius + paddleSize.x / 2 + 0.1f, ball.getPosition().y);
            }

            // Right Paddle
            if (ball.getPosition().x + ballRadius > rightPaddle.getPosition().x - paddleSize.x / 2 &&
                ball.getPosition().x + ballRadius < rightPaddle.getPosition().x &&
                ball.getPosition().y + ballRadius >= rightPaddle.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRadius <= rightPaddle.getPosition().y + paddleSize.y / 2)
            {
                if (ball.getPosition().y > rightPaddle.getPosition().y)
                    ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

                ballSound.play();
                ball.setPosition(rightPaddle.getPosition().x - ballRadius - paddleSize.x / 2 - 0.1f, ball.getPosition().y);
            }
        }

        // Clear the window
        window.clear(sf::Color(200, 200, 200));
        if (player1Score == 5 || player2Score == 5) {
            if (player1Score == 5)
                pauseMessage.setString("GAMEOVER \n P1 is winner\n Click quit button to exit or Start button for new game");
            else
                pauseMessage.setString("GAMEOVER \n P2 is winner\n Click quit button to exit or Start button for new game");
            player1Score = 0;
            player2Score = 0;
        }
        window.draw(sprite);

        if (isPlaying)
        {
            // Draw the paddles and the ball
            window.draw(leftPaddle);
            window.draw(rightPaddle);
            window.draw(ball);
        }
        else
        {
            // Draw the pause message
            window.draw(pauseMessage);
            window.draw(btn1Text);
            window.draw(btn2Text);
            btn1.drawTo(window);
            btn2.drawTo(window);
            
        }

        // Display things on screen
        
        window.display();
    }

    return EXIT_SUCCESS;
}
