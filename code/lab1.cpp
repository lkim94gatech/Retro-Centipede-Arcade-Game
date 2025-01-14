/**
 * Author: Leandro Alan Kim
 * Class: ECE4122 Section A
 * Turn in Date : 30 Sep 2024
 * 
 * Description and Purpose: This project is the recreation of the Retro Centipede
 * Arcade Game by applying the principles of 2D graphics and class creation in C++.
 * Using Cmake and SFML was the key concept of this project in reproducing the game 
 * play that supported the first level of Centipede Arcade Game.
 */
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <cmath>
#include <random>
#include <iostream>

using namespace sf;

// dividing the top info area, main game area, and mushroom free area
const int WIDTH = 800;
const int HEIGHT = 600;
const int TOP_AREA_HEIGHT = 50;
const int BOTTOM_AREA_HEIGHT = 50;
const int MAIN_AREA_HEIGHT = HEIGHT - TOP_AREA_HEIGHT - BOTTOM_AREA_HEIGHT;

/**
 * mushroom struct
 */
struct Mushroom
{
    Sprite sprite;
    int health;
};

/**
 * ECE_LaserBlast class derived from the SFML::Sprite.
 * initializes the textture, positoin, and speed.
 * moves the laser upwards
 * collision check returns true if there is a collision
 */
class ECE_LaserBlast : public Sprite
{
public:
    ECE_LaserBlast(const Texture& texture, const Vector2f& position, float speed)
        : speed(speed)
    {
        setTexture(texture);
        setPosition(position);
    }

    void update(float deltaTime)
    {
        move(0.0f, -speed * deltaTime); // laser going up
    }

    // returns true if collision is found
    bool checkCollision(const Sprite& other)
    {
        return getGlobalBounds().intersects(other.getGlobalBounds());
    }

private:
    float speed;
};

// centipede struct
struct CentipedeParticle
{
    Sprite sprite;
};

// function to normalize a vector
Vector2f normalize(Vector2f v)
{
    float mag = std::sqrt(v.x * v.x + v.y * v.y);
    if (mag == 0) return Vector2f(0, 0);
    return Vector2f(v.x / mag, v.y / mag);
}

/**
 * constuctor initializes head and body and positions them accordingly.
 * update method moves the head and updates on collisions
 * body particles foolows the head
 */
class ECE_Centipede
{
public:
    // constructor
    ECE_Centipede(const Texture& headTexture, const Texture& bodyTexture, int numParticles, Vector2f startPosition, float speed)
        : headTexture(headTexture), bodyTexture(bodyTexture), speed(speed), alive(true)
    {
        direction = Vector2f(-1.0f, 0.0f); // starts out moving to the left

        // head particle
        CentipedeParticle headParticle;
        headParticle.sprite.setTexture(headTexture);
        headParticle.sprite.setPosition(startPosition);
        particles.push_back(headParticle);

        // body particles is following the head
        for (int i = 1; i < numParticles; ++i)
        {
            CentipedeParticle bodyParticle;
            bodyParticle.sprite.setTexture(bodyTexture);
            bodyParticle.sprite.setPosition(startPosition.x + i * bodyTexture.getSize().x, startPosition.y);
            particles.push_back(bodyParticle);
        }
    }

    // Update method
    void update(float deltaTime, const std::list<Mushroom>& mushrooms)
    {
        if (!alive) return;

        Vector2f movement = direction * speed * deltaTime;
        particles[0].sprite.move(movement); // move head

        // check head collision
        Sprite& head = particles[0].sprite;

        float leftBound = 0.0f;
        float rightBound = WIDTH - head.getTexture()->getSize().x;

        bool changeDirection = false;

        // Check screen edge collision
        if (head.getPosition().x <= leftBound || head.getPosition().x >= rightBound)
        {
            changeDirection = true;
        }

        // Check collision with mushroom
        for (const Mushroom& mushroom : mushrooms)
        {
            if (head.getGlobalBounds().intersects(mushroom.sprite.getGlobalBounds()))
            {
                changeDirection = true;
                break;
            }
        }

        if (changeDirection)
        {
            direction.x = -direction.x; // change directions
            head.move(0.0f, head.getTexture()->getSize().y); // move head down
        }

        // body following the head
        for (size_t i = 1; i < particles.size(); ++i)
        {
            Vector2f dir = (particles[i - 1].sprite.getPosition()) - (particles[i].sprite.getPosition());
            float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

            if (dist > bodyTexture.getSize().x)
            {
                dir = normalize(dir);
                particles[i].sprite.move(dir * speed * deltaTime);
            }
        }
    }

    // draw method
    void draw(RenderWindow& window) const
    {
        if (!alive) return;

        for (const auto& particle : particles)
        {
            window.draw(particle.sprite);
        }
    }

    // check if centipede is alive
    bool isAlive() const 
    {
        return alive;
    }

private:
    std::vector<CentipedeParticle> particles;
    Vector2f direction;
    float speed;
    bool alive;
    const Texture& headTexture;
    const Texture& bodyTexture;
};

int main() {
    VideoMode vm(WIDTH, HEIGHT);
    RenderWindow window(vm, "Centipede Game");

    // start screen texture
    Texture textureBackground;
    textureBackground.loadFromFile("graphics/Background.png");

    // mushroom0 texture
    Texture mushroom0Texture;
    mushroom0Texture.loadFromFile("graphics/Mushroom0.png");
    
    // mushroom1 texture
    Texture mushroom1Texture;
    mushroom1Texture.loadFromFile("graphics/Mushroom1.png");

    // starship texture
    Texture starshipTexture;
    starshipTexture.loadFromFile("graphics/StarShip.png");

    // spider texture
    Texture spiderTexture;
    spiderTexture.loadFromFile("graphics/Spider.png");

    // centipede head texture
    Texture cHeadTexture;
    cHeadTexture.loadFromFile("graphics/CentipedeHead.png");

    // centipede body texture
    Texture cBodyTexture;
    cBodyTexture.loadFromFile("graphics/CentipedeBody.png");

    // centipede instance created
    std::vector<ECE_Centipede> centipedes;

    int centipedeLength = 12; // 1 head and 11 body

    // set starting position at the right corner of top info area
    Vector2f centipedeStartPosition(
        WIDTH - cHeadTexture.getSize().x,
        TOP_AREA_HEIGHT
    );

    float centipedeSpeed = 100.0f; // speed of centipede

    ECE_Centipede initialCentipede(
        cHeadTexture,
        cBodyTexture,
        centipedeLength,
        centipedeStartPosition,
        centipedeSpeed
    );

    centipedes.push_back(initialCentipede);

    // red laser texture
    Texture laserTexture;
    Image laser;
    laser.create(2, 15, Color::Red);
    laserTexture.loadFromImage(laser);

    int mushroomWidth = mushroom0Texture.getSize().x;
    int mushroomHeight = mushroom0Texture.getSize().y;

    // start screen sprite
    Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0, 0);

    // top information area
    RectangleShape topArea(Vector2f(WIDTH, TOP_AREA_HEIGHT));
    topArea.setFillColor(Color::Black);

    // mushroom free area
    RectangleShape bottomArea(Vector2f(WIDTH, BOTTOM_AREA_HEIGHT));
    bottomArea.setPosition(0, HEIGHT - BOTTOM_AREA_HEIGHT);
    bottomArea.setFillColor(Color::Black);

    // main game area
    RectangleShape mainArea(Vector2f(WIDTH, MAIN_AREA_HEIGHT));
    mainArea.setPosition(0, TOP_AREA_HEIGHT);
    mainArea.setFillColor(Color::Black);

    bool gameStarted = false;

    // random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    //mushroom placement boundaries are set in the main game area
    float mushroomBottomLimit = HEIGHT - BOTTOM_AREA_HEIGHT - 2 * mushroomHeight;
    std::uniform_int_distribution<> disX(0, WIDTH - mushroomWidth);
    std::uniform_int_distribution<> disY(TOP_AREA_HEIGHT, static_cast<int>(mushroomBottomLimit - mushroomHeight));

    // 30 random mushrooms
    std::list<Mushroom> mushrooms;
    for (int i = 0; i < 30; ++i)
    {
        Mushroom m;
        m.sprite.setTexture(mushroom0Texture);
        m.health = 2; // mushroom0 and mushroom1
        int x = disX(gen);
        int y = disY(gen);
        m.sprite.setPosition(x, y);
        mushrooms.push_back(m);
    }

    // starship sprite
    Sprite starshipSprite;
    starshipSprite.setTexture(starshipTexture);

    // starting position middle bottom 
    float starshipX = (WIDTH - starshipTexture.getSize().x) / 2.0f;
    float starshipY = HEIGHT - BOTTOM_AREA_HEIGHT - starshipTexture.getSize().y;
    starshipSprite.setPosition(starshipX, starshipY);
    const float starshipSpeed = 300.0f;

    const float laserSpeed = 500.0f;
    std::list<ECE_LaserBlast> shootLaser;

    // spider sprite
    Sprite spiderSprite;
    spiderSprite.setTexture(spiderTexture);

    // starting position
    float spiderX = WIDTH / 2.0f;
    float spiderY = TOP_AREA_HEIGHT + MAIN_AREA_HEIGHT / 2.0f;
    spiderSprite.setPosition(spiderX, spiderY);

    // spider movement
    float spiderSpeed = 250.0f;
    Vector2f spiderDirection(1.0f, 0.0f);
    float spiderChangeDirectionTime = 0.0f;
    float spiderDirectionChangeInterval = 1.0f;
    bool spiderActive = true;
    float spiderRespawnTimer = 0.0f;
    const float spiderRespawnInterval = 5.0f; // respawn

    // starship life
    int lives = 3;
    Vector2f starshipStartPosition = starshipSprite.getPosition();
    std::vector<Sprite> life;

    // font
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    // player score
    int score = 0;
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(36);
    scoreText.setFillColor(Color::White);
    scoreText.setString(std::to_string(score));

    // score at the middle of info area
    FloatRect scoreTextRect = scoreText.getLocalBounds();
    scoreText.setOrigin(scoreTextRect.left + scoreTextRect.width / 2.0f,
                        scoreTextRect.top + scoreTextRect.height / 2.0f);
    scoreText.setPosition(WIDTH / 2.0f, TOP_AREA_HEIGHT / 2.0f);

    // life icons on the right side of info area
    auto updateLifeIcons = [&]()
    {
        life.clear();
        float lifeIconWidth = lives * (starshipTexture.getSize().x * 0.5f + 5) - 5;
        float livesStartX = WIDTH - lifeIconWidth - 10.0f;
        float lifeIconY = (TOP_AREA_HEIGHT - starshipTexture.getSize().y * 0.5f) / 2.0f;

        for (int i = 0; i < lives; ++i)
        {
            Sprite lifeIcon;
            lifeIcon.setTexture(starshipTexture);
            lifeIcon.setScale(0.5f, 0.5f);
            lifeIcon.setPosition(livesStartX + i * (starshipTexture.getSize().x * 0.5f + 5), lifeIconY);
            life.push_back(lifeIcon);
        }
    };
    updateLifeIcons();

    Clock clock;

    // main loop
    while (window.isOpen())
    {
        // delta time for smooth movement
        float deltaTime = clock.restart().asSeconds();

        // events handlers
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
            {
                window.close(); // close on escape
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)
            {
                gameStarted = true; // start game on enter
            }
            // shoot laser on space button. position is at the tip of the startship
            if (gameStarted && event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
            {
                Vector2f starshipPosition = starshipSprite.getPosition();
                float starshipWidth = starshipTexture.getSize().x;
                Vector2f laserPosition(
                    starshipPosition.x + starshipWidth / 2.0f - laserTexture.getSize().x / 2.0f,
                    starshipPosition.y - laserTexture.getSize().y
                );
                // new laser blast
                ECE_LaserBlast laser(laserTexture, laserPosition, laserSpeed);
                // add to list
                shootLaser.push_back(laser);
            }
        }

        if (gameStarted) {
            // starship movement with left, right, up, down key
            float moveX = 0.0f;
            float moveY = 0.0f;
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                moveX -= starshipSpeed * deltaTime;
            }
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                moveX += starshipSpeed * deltaTime;
            }
            if (Keyboard::isKeyPressed(Keyboard::Up))
            {
                moveY -= starshipSpeed * deltaTime;
            }
            if (Keyboard::isKeyPressed(Keyboard::Down))
            {
                moveY += starshipSpeed * deltaTime;
            }

            Vector2f position = starshipSprite.getPosition();

            // boundaries for starship
            float leftBound = 0.0f;
            float rightBound = WIDTH - starshipTexture.getSize().x;
            float lowerBound = HEIGHT - BOTTOM_AREA_HEIGHT - starshipTexture.getSize().y;
            float upperBound = lowerBound - (MAIN_AREA_HEIGHT * 0.25f);

            // collision on x
            Vector2f newPosition = position;
            newPosition.x += moveX;
            FloatRect starshipBounds = starshipSprite.getGlobalBounds();
            starshipBounds.left = newPosition.x;

            bool collisionX = false;
            for (const auto& mushroom : mushrooms)
            {
                if (starshipBounds.intersects(mushroom.sprite.getGlobalBounds()))
                {
                    collisionX = true;
                    break;
                }
            }

            if (!collisionX) {
                position.x = newPosition.x;
            }

            // collision on y
            newPosition = position;
            newPosition.y += moveY;
            starshipBounds.top = newPosition.y;

            bool collisionY = false;
            for (const auto& mushroom : mushrooms)
            {
                if (starshipBounds.intersects(mushroom.sprite.getGlobalBounds()))
                {
                    collisionY = true;
                    break;
                }
            }

            if (!collisionY)
            {
                position.y = newPosition.y;
            }

            // boundaries set at .25 from the bottom
            if (position.x < leftBound)
            {
                position.x = leftBound;
            }
            if (position.x > rightBound)
            {
                position.x = rightBound;
            }
            if (position.y < upperBound)
            {
                position.y = upperBound;
            }
            if (position.y > lowerBound)
            {
                position.y = lowerBound;
            }
            starshipSprite.setPosition(position);

            // update laser blasts when outof bounds
            for (auto it = shootLaser.begin(); it != shootLaser.end();)
            {
                it->update(deltaTime);
                if (it->getPosition().y + it->getGlobalBounds().height < 0)
                {
                    it = shootLaser.erase(it);
                } else
                {
                    ++it;
                }
            }

            // laser hitting mushroom and spider
            for (auto laserIt = shootLaser.begin(); laserIt != shootLaser.end();)
            {
                bool laserRemoved = false;

                // mushroom collision
                for (auto mushroomIt = mushrooms.begin(); mushroomIt != mushrooms.end();)
                {
                    if (laserIt->checkCollision(mushroomIt->sprite))
                    {
                        mushroomIt->health--; // to mushroom1

                        if (mushroomIt->health == 1)
                        {
                            mushroomIt->sprite.setTexture(mushroom1Texture); // change texture to mushrrom1
                        }

                        // if mushroom1 is hit again then it is gone
                        if (mushroomIt->health <= 0)
                        {
                            score += 4; // killing mushroom is 4 points

                            // score update
                            scoreText.setString(std::to_string(score));
                            FloatRect scoreTextRect = scoreText.getLocalBounds();
                            scoreText.setOrigin(scoreTextRect.left + scoreTextRect.width / 2.0f,
                                                scoreTextRect.top + scoreTextRect.height / 2.0f);
                            scoreText.setPosition(WIDTH / 2.0f, TOP_AREA_HEIGHT / 2.0f);

                            mushroomIt = mushrooms.erase(mushroomIt);
                        } else
                        {
                            ++mushroomIt;
                        }

                        // erase laser after hitting
                        laserIt = shootLaser.erase(laserIt);
                        laserRemoved = true;
                        break;
                    } else 
                    {
                        ++mushroomIt;
                    }
                }

                if (!laserRemoved)
                {
                    // spider collision
                    if (spiderActive && laserIt->checkCollision(spiderSprite))
                    {
                        spiderActive = false; // remove spider
                        spiderRespawnTimer = 0.0f;
                        score += 500; // killing the spider is 500 points

                        // score update
                        scoreText.setString(std::to_string(score));
                        FloatRect scoreTextRect = scoreText.getLocalBounds();
                        scoreText.setOrigin(scoreTextRect.left + scoreTextRect.width / 2.0f,
                                            scoreTextRect.top + scoreTextRect.height / 2.0f);
                        scoreText.setPosition(WIDTH / 2.0f, TOP_AREA_HEIGHT / 2.0f);

                        // erase laser after hitting
                        laserIt = shootLaser.erase(laserIt);
                        laserRemoved = true;
                    } else
                    {
                        ++laserIt;
                    }
                }
            }

            // spider update
            if (spiderActive)
            {
                spiderChangeDirectionTime += deltaTime;

                if (spiderChangeDirectionTime >= spiderDirectionChangeInterval)
                {
                    spiderChangeDirectionTime = 0.0f;
                    // random direction
                    std::uniform_real_distribution<float> disDir(-1.0f, 1.0f);
                    float dirX = disDir(gen);
                    float dirY = disDir(gen);
                    // normalize vector
                    float magnitude = std::sqrt(dirX * dirX + dirY * dirY);
                    if (magnitude != 0) {
                        spiderDirection.x = dirX / magnitude;
                        spiderDirection.y = dirY / magnitude;
                    }
                }
                spiderSprite.move(spiderDirection.x * spiderSpeed * deltaTime,
                                    spiderDirection.y * spiderSpeed * deltaTime); // spider movement

                // spider boundaries
                Vector2f spiderPosition = spiderSprite.getPosition();
                float spiderLeftBound = 0.0f;
                float spiderRightBound = WIDTH - spiderTexture.getSize().x;
                float spiderUpperBound = TOP_AREA_HEIGHT;
                float spiderLowerBound = TOP_AREA_HEIGHT + MAIN_AREA_HEIGHT - spiderTexture.getSize().y;

                if (spiderPosition.x < spiderLeftBound)
                {
                    spiderPosition.x = spiderLeftBound;
                    spiderDirection.x = -spiderDirection.x;
                }
                if (spiderPosition.x > spiderRightBound)
                {
                    spiderPosition.x = spiderRightBound;
                    spiderDirection.x = -spiderDirection.x;
                }
                if (spiderPosition.y < spiderUpperBound)
                {
                    spiderPosition.y = spiderUpperBound;
                    spiderDirection.y = -spiderDirection.y;
                }
                if (spiderPosition.y > spiderLowerBound)
                {
                    spiderPosition.y = spiderLowerBound;
                    spiderDirection.y = -spiderDirection.y;
                }
                spiderSprite.setPosition(spiderPosition);

                // when spider hits mushroom
                for (auto mushroomIt = mushrooms.begin(); mushroomIt != mushrooms.end(); )
                {
                    // mushroom is removed
                    if (spiderSprite.getGlobalBounds().intersects(mushroomIt->sprite.getGlobalBounds()))
                    {
                        mushroomIt = mushrooms.erase(mushroomIt);
                    } else 
                    {
                        ++mushroomIt;
                    }
                }

                // when spider hits starship
                if (spiderSprite.getGlobalBounds().intersects(starshipSprite.getGlobalBounds()))
                {
                    // starship starts again at the starting position
                    starshipSprite.setPosition(starshipStartPosition);
                    lives--; // lose one life

                    // when all 3 lives are used reset
                    if (lives == 0)
                    {
                        gameStarted = false;
                        lives = 3; // reset life
                        starshipSprite.setPosition(starshipStartPosition); // starship back to starting position
                        spiderSprite.setPosition(spiderX, spiderY); // spider back to starting position
                        spiderActive = true;
                        spiderRespawnTimer = 0.0f;

                        // reset score
                        score = 0;
                        scoreText.setString(std::to_string(score));
                        FloatRect scoreTextRect = scoreText.getLocalBounds();
                        scoreText.setOrigin(scoreTextRect.left + scoreTextRect.width / 2.0f,
                                            scoreTextRect.top + scoreTextRect.height / 2.0f);
                        scoreText.setPosition(WIDTH / 2.0f, TOP_AREA_HEIGHT / 2.0f);

                        // reset 30 mushrooms
                        mushrooms.clear();
                        for (int i = 0; i < 30; ++i)
                        {
                            Mushroom m;
                            m.sprite.setTexture(mushroom0Texture);
                            m.health = 2; // mushroom0 and mushroom1
                            int x = disX(gen);
                            int y = disY(gen);
                            m.sprite.setPosition(x, y);
                            mushrooms.push_back(m);
                        }

                        // reset laser
                        shootLaser.clear();

                        // reset centipede
                        centipedes.clear();
                        ECE_Centipede newCentipede(
                            cHeadTexture,
                            cBodyTexture,
                            centipedeLength,
                            centipedeStartPosition,
                            centipedeSpeed
                        );
                        centipedes.push_back(newCentipede);
                    }
                    // reset life
                    updateLifeIcons();
                }
            } else
            {
                // reset spider at random position
                spiderRespawnTimer += deltaTime;
                if (spiderRespawnTimer >= spiderRespawnInterval)
                {
                    spiderActive = true; // respawn
                    spiderRespawnTimer = 0.0f;
                    std::uniform_int_distribution<> disSpiderX(0, WIDTH - spiderTexture.getSize().x);
                    std::uniform_int_distribution<> disSpiderY(TOP_AREA_HEIGHT, TOP_AREA_HEIGHT + MAIN_AREA_HEIGHT - spiderTexture.getSize().y);
                    float newSpiderX = disSpiderX(gen);
                    float newSpiderY = disSpiderY(gen);
                    spiderSprite.setPosition(newSpiderX, newSpiderY);
                }
            }
            // update centipedes
            for (auto& centipede : centipedes)
            {
                centipede.update(deltaTime, mushrooms);
            }
        }

        // rendering
        window.clear();
        window.draw(spriteBackground);

        if (!gameStarted)
        {
            window.draw(spriteBackground); // start screen
        } else
        {
            window.draw(mainArea);
            window.draw(topArea);
            window.draw(bottomArea);

            // draw mushroom
            for (const auto& mushroom : mushrooms)
            {
                window.draw(mushroom.sprite);
            }

            // draw centipedes
            for (const auto& centipede : centipedes)
            {
                centipede.draw(window);
            }

            // draw laser
            for (const auto& laser : shootLaser)
            {
                window.draw(laser);
            }

            // draw spider
            if (spiderActive)
            {
                window.draw(spiderSprite);
            }

            // draw starship
            window.draw(starshipSprite);

            // draw score
            window.draw(scoreText);

            // draw life icon
            for (const auto& icon : life)
            {
                window.draw(icon);
            }
        }

        window.display();
    }

    return 0;
}
