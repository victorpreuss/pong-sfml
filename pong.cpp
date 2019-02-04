#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#pragma GCC diagnostic ignored "-Wswitch"

#include <cstdlib>
#include <cstdio>
#include <cmath>
const float pi = 4.f * std::atan(1.f);

// game size
unsigned int width = 500, height = 600;

// ball variables
float ballRadius = 8.f;
float ballSpeed = 3.f;
float ballAngle = 45.f; // starts going up (0 is a a vector pointing to the right)
float ballX, ballY;

// paddle variables
float paddleSpeed = 10.f;
float paddleLength = 80.f, paddleThickness = 10.f;
float paddle1X, paddle1Y;
float paddle2X, paddle2Y;

void restartBallPosition (void) {
    ballX = static_cast<float>(std::rand() % width);
    ballY = static_cast<float>(std::rand() % height);
    ballY = (ballY < 150.f) ? 150.f : (ballY > 450.f) ? 450.f : ballY;
    ballAngle = (ballY > height / 2.f) ? 45.f : -45.f;
}

int main() {

    // the window is larger than the game for the gui elements
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong dos campeoes", sf::Style::Default);
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);

    // collors
    sf::Color bgColor = sf::Color::Black;
    sf::Color objColor = sf::Color::White;
    float color1[3] = { 0.f, 0.f, 0.f };
    float color2[3] = { 0.f, 0.f, 0.f };

    // slider to control the paddle position
    int sliderPos = 0;

    // game options
    char p1Name[100] = "Citor";
    char p2Name[100] = "Tormena";

    int p1Score = 0, p2Score = 0;
    
    // create player paddle
    sf::RectangleShape paddle1(sf::Vector2f(paddleLength, paddleThickness));

    // set paddle's initial position
    paddle1X = (width - paddleLength) / 2.f;
    paddle1Y = height - paddleThickness;
    paddle1.setPosition(sf::Vector2f(paddle1X, paddle1Y));

    // create player paddle
    sf::RectangleShape paddle2(sf::Vector2f(paddleLength, paddleThickness));

    // set paddle's initial position
    paddle2X = (width - paddleLength) / 2.f;
    paddle2Y = 0.f;
    paddle2.setPosition(sf::Vector2f(paddle2X, paddle2Y));

    // create ball
    sf::CircleShape ball(ballRadius);
    restartBallPosition();

    // create visual boundary
    sf::RectangleShape leftLine(sf::Vector2f(3, height));
    sf::RectangleShape rightLine(sf::Vector2f(3, height));
    rightLine.setPosition(width-3, 0.f);

    // main loop
    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                break;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            paddle1X -= paddleSpeed;
            if (paddle1X < 0.f) paddle1X = 0.f;
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            paddle1X += paddleSpeed;
            if (paddle1X > (width - paddleLength)) paddle1X = width - paddleLength;
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            paddle2X -= paddleSpeed;
            if (paddle2X < 0.f) paddle2X = 0.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            paddle2X += paddleSpeed;
            if (paddle2X > (width - paddleLength)) paddle2X = width - paddleLength;
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        // create gui here
        ImGui::Begin("Scoreboard");

        ImGui::Text("%s: %d", p1Name, p1Score);
        ImGui::Text("%s: %d", p2Name, p2Score);

        ImGui::End();

        ImGui::Begin("Configurations");

        if (ImGui::Button("Restart Game")) {
            restartBallPosition();
            p1Score = p2Score = 0;
        }

        ImGui::InputText("Player 1", p1Name, 100);
        ImGui::InputText("Player 2", p2Name, 100);

        sliderPos = static_cast<int>(paddle1X);
        if (ImGui::SliderInt("Player 1 control", &sliderPos, 0, width-paddleLength)) {
            paddle1X = static_cast<float>(sliderPos);
        }

        ImGui::SliderFloat("Ball speed", &ballSpeed, 0.f, 10.f);

        ImGui::Text("Window width: %d", window.getSize().x);
        ImGui::Text("Window height: %d", window.getSize().y);
        ImGui::Text("Paddle length: %.2f", paddleLength);
        ImGui::Text("Paddle thickness: %.2f", paddleThickness);
        ImGui::Text("Ball X coordinate: %.2f", ballX);
        ImGui::Text("Ball Y coordinate: %.2f", ballY);

        if (ImGui::ColorEdit3("Background color", color1)) {
            bgColor.r = static_cast<sf::Uint8>(color1[0] * 255.f);
            bgColor.g = static_cast<sf::Uint8>(color1[1] * 255.f);
            bgColor.b = static_cast<sf::Uint8>(color1[2] * 255.f);
        }

        if (ImGui::ColorEdit3("Objects color", color2)) {
            objColor.r = static_cast<sf::Uint8>(color2[0] * 255.f);
            objColor.g = static_cast<sf::Uint8>(color2[1] * 255.f);
            objColor.b = static_cast<sf::Uint8>(color2[2] * 255.f);
        }

        ImGui::End();

        // update game here
        paddle1.setPosition(sf::Vector2f(paddle1X, paddle1Y));
        paddle1.setFillColor(objColor);

        paddle2.setPosition(sf::Vector2f(paddle2X, paddle2Y));
        paddle2.setFillColor(objColor);

        // calculate ball movement
        ballX += std::cos(ballAngle * pi / 180.f) * ballSpeed;
        ballY -= std::sin(ballAngle * pi / 180.f) * ballSpeed;

        // check boundaries
        if (ballX <= 0.f) {
            ballX = 0.f;
            if (ballAngle >= 180.f) {
                float theta = 270.f - ballAngle;
                ballAngle += 2 * theta;
            } else {
                float theta = ballAngle - 90.f;
                ballAngle -= 2 * theta;
            }
        } else if (ballX >= width - 2 * ballRadius) {
            ballX = width - 2 * ballRadius;
            if (ballAngle >= 0.f) {
                float theta = 90.f - ballAngle;
                ballAngle += 2 * theta;
            } else {
                float theta = ballAngle - 270.f;
                ballAngle -= 2 * theta;
            }
        } else if (ballY <= paddleThickness) {
            if ((ballX >= paddle2X - ballRadius) && (ballX <= paddle2X + paddleLength + ballRadius)) {
                ballY = paddleThickness;
                if (ballAngle >= 90.f) {
                    float theta = 180.f - ballAngle;
                    ballAngle += 2 * theta;
                } else {
                    float theta = ballAngle;
                    ballAngle -= 2 * theta;
                }
            } else { // player 2 lost
                restartBallPosition();
                p1Score++;
            }
        } else if (ballY >= height - paddleThickness - 2 * ballRadius) {
            if ((ballX >= paddle1X - ballRadius) && (ballX <= paddle1X + paddleLength + ballRadius)) {
                ballY = height - paddleThickness - 2 * ballRadius;
                if (ballAngle >= 270.f) {
                    float theta = 360.f - ballAngle;
                    ballAngle += 2 * theta;
                } else {
                    float theta = ballAngle - 180.f;
                    ballAngle -= 2 * theta;
                    
                }
            } else { // player 1 lost
                restartBallPosition();
                p2Score++;
            }
        }

        if (ballAngle >= 360.f) ballAngle -= 360.f;
        if (ballAngle < 0.f) ballAngle += 360.f;

        ball.setPosition(sf::Vector2f(ballX, ballY));
        ball.setFillColor(objColor);
        
        // draw here
        window.clear(bgColor); // fill background with color

        window.draw(paddle1);
        window.draw(paddle2);
        window.draw(ball);
        window.draw(leftLine);
        window.draw(rightLine);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}