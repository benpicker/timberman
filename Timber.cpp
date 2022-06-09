// This is where our game starts from
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <string>
#include <iostream>

using namespace sf;



const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

// where is the player/branch? 
// left or right 
enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

// function declaration
void updateBranches(int seed)
{
	// move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}

	// Spawn a new branch at position 0
	// LEFT, RIGHT, or NONE
	srand((int)time(0) + seed);
	int r = (rand() % 5);

	switch (r) {
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;

	}
}




int main()
{

	// create a video mode object
	VideoMode vm(1920, 1080);

	// Create and open a window for the game
	RenderWindow window(vm, "timber!!!", Style::Fullscreen);

	// create a texture to hold up a graphic on the GPU
	Texture textureBackground;

	// load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");

	// create a sprite
	Sprite spriteBackground;

	// attach the texture to the sprite 
	spriteBackground.setTexture(textureBackground);

	// set the sprite background to cover the screen
	spriteBackground.setPosition(0, 0);

	// make a tree sprite -- main tree
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	// additional background trees
	Sprite spriteTree2;
	spriteTree2.setTexture(textureTree);
	spriteTree2.setPosition(400, -100);
	spriteTree2.setScale(0.4, 1);

	Sprite spriteTree3;
	spriteTree3.setTexture(textureTree);
	spriteTree3.setPosition(1300, -100);
	spriteTree3.setScale(0.4, 1);

	Sprite spriteTree4;
	spriteTree4.setTexture(textureTree);
	spriteTree4.setPosition(50, 0);
	spriteTree4.setScale(0.6, 1.2);
	
	Sprite spriteTree5;
	spriteTree5.setTexture(textureTree);
	spriteTree5.setPosition(1500, 0);
	spriteTree5.setScale(0.6, 1.2);

	// Prepare the bee 
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);

	// is the bee currently moving?
	bool beeActive = false;

	// how fast can the bee fly
	float beeSpeed = 0.0f;

	// make 3 cloud sprites from 1 texture
	Texture textureCloud;

	// load 1 new texture
	textureCloud.loadFromFile("graphics/cloud.png");

	// 3 new sprites with same texture
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);

	// position the clouds on the left side of the screen 
	// at different heights
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setPosition(0, 500);

	// are the clouds currently on screen? 
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;

	// how fast is each cloud? 
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	// variables to control time itself 
	Clock clock;

	// time bar 
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// track whether game is paused 
	bool paused = true;

	// draw some text
	int score = 0;
	int highScore = 0;

	Text messageText;
	Text scoreText;
	Text highScoreText;

	// choose a font 
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	// set the font to our message 
	messageText.setFont(font);
	scoreText.setFont(font);
	highScoreText.setFont(font);


	// assign the actual message
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");
	highScoreText.setString("High = 0");

	// make it really big (that's what she said) 
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);
	highScoreText.setCharacterSize(100);

	// choose a color 
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);
	highScoreText.setFillColor(Color::White);

	// Position the text
	FloatRect textRect = messageText.getLocalBounds();

	messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top +
		textRect.height / 2.0f);

	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

	scoreText.setPosition(20, 20);
	highScoreText.setPosition(20, 950);


	// prepare 6 branches 
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	// set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);

		// set the sprite's origin to dead center
		// we can then spin it around without changing its position
		branches[i].setOrigin(220, 20);
	}

	// prepare the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	// the player starts on the left 
	side playerSide = side::LEFT;

	// prepare the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	// prepare the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	// line up the axe with the tree 
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	// prepare the flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	// some other useful log related variables 
	bool logActive = false;
	float  logSpeedX = 1000;
	float logSpeedY = 100;

	// control the player input
	bool acceptInput = false;

	// Prepare Yogi 
	Texture textureYogi;
	textureYogi.loadFromFile("graphics/Yogi.png");
	Sprite spriteYogi;
	spriteYogi.setTexture(textureYogi);
	spriteYogi.setPosition(1600, 200);
	spriteYogi.setScale(0.2, 0.2);

	// prepare the sounds

	// the player chopping sound 
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	// the player dies 
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	// out of time
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	// prepare the music 
	// music 
	sf::Music music;
	if (!music.openFromFile("music/theme_music.wav"))
		return -1; // error
	music.play();
	music.setLoop(true);
	music.play();



	while (window.isOpen())
	{

		
		/*
		------------------------------
		Handle the player input
		------------------------------
		*/
		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				// listen for key presses again
				acceptInput = true;

				// hide the axe
				spriteAxe.setPosition(2000,
					spriteAxe.getPosition().y);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// start game
		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			paused = false;

			// reset the time and score
			score = 0;
			timeRemaining = 6;

			// make all the branches disappear
			// starting in the second position 
			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			// make sure the gravestone is hidden
			spriteRIP.setPosition(675, 2000);
			// move player into position 
			spritePlayer.setPosition(580, 720);

			acceptInput = true;
		}

		// wrap the player controls to 
		// make sure we are accepting input
		if (acceptInput == true)
		{
			// pressing the right cursor key 
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				// make sure the player is on the left 
				playerSide = side::RIGHT;

				score++;

				// add to the amount of time remaining 
				timeRemaining += (2 / score) + 0.15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT,
					spriteAxe.getPosition().y);

				spritePlayer.setPosition(1200, 720);

				// update the branches
				updateBranches(score);

				// set the log flying to the left 
				spriteLog.setPosition(810, 720);
				logSpeedX = -500;
				logActive = true;

				acceptInput = false;

				// play a chop sound
				chop.play();
			}

			// pressing left cursor key 
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				// make sure the player is on the left 
				playerSide = side::LEFT;

				score++;

				// add to the amount of time remaining 
				timeRemaining += (2 / score) + 0.15;

				spriteAxe.setPosition(AXE_POSITION_LEFT,
					spriteAxe.getPosition().y);

				spritePlayer.setPosition(580, 720);

				// update the branches
				updateBranches(score);

				// set the log flying to the left 
				spriteLog.setPosition(810, 720);
				logSpeedX = 500;
				logActive = true;

				acceptInput = false;
			}
		}
		/*
		------------------------------
		Update the scene
		------------------------------
		*/
		if (!paused)
		{



			// Measure time 
			Time dt = clock.restart();

			// subtract from the amount of time remaining 
			timeRemaining -= dt.asSeconds();

			// size up the bar 
			timeBar.setSize(Vector2f(timeBarWidthPerSecond *
				timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f)
			{
				// Pause the game 
				paused = true;

				// change the message shown to the player 
				messageText.setString("Out of time!!");

				// Reposition the text based on its new size 
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top +
					textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
				
				// play the out of time sound
				outOfTime.play();
			}

			messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);



			if (!beeActive)
			{
				// how fast is the be
				srand((int)time(0));
				beeSpeed = (rand() % 200) + 200;

				// how high is the bee
				srand((int)time(0));
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else
			{
				spriteBee.setPosition(
					spriteBee.getPosition().x -
					(beeSpeed * dt.asSeconds()),
					spriteBee.getPosition().y);
				// has the bee reached the left-hand edge of the screen? 
				if (spriteBee.getPosition().x < -100)
				{
					// set it up ready to be a whole new bee in the next frame
					beeActive = false;
				}
			}

			// manage the clouds
			// cloud 1
			if (!cloud1Active)
			{
				// how fast is the cloud 
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				// how high is the cloud
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			else
			{
				spriteCloud1.setPosition(
					spriteCloud1.getPosition().x +
					(cloud1Speed * dt.asSeconds()),
					spriteCloud1.getPosition().y);;

				// has cloud reached the right hand edge of the screen
				if (spriteCloud1.getPosition().x > 1920)
				{
					// set it up ready to be a whole new cloud next frame 
					cloud1Active = false;
				}
			}

			// manage the clouds
			// cloud 2
			if (!cloud2Active)
			{
				// how fast is the cloud 
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				// how high is the cloud
				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			else
			{
				spriteCloud2.setPosition(
					spriteCloud2.getPosition().x +
					(cloud2Speed * dt.asSeconds()),
					spriteCloud2.getPosition().y);;

				// has cloud reached the right hand edge of the screen
				if (spriteCloud2.getPosition().x > 1920)
				{
					// set it up ready to be a whole new cloud next frame 
					cloud2Active = false;
				}
			}

			// manage the clouds
			// cloud 1
			if (!cloud3Active)
			{
				// how fast is the cloud 
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				// how high is the cloud
				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			else
			{
				spriteCloud3.setPosition(
					spriteCloud3.getPosition().x +
					(cloud3Speed * dt.asSeconds()),
					spriteCloud3.getPosition().y);;

				// has cloud reached the right hand edge of the screen
				if (spriteCloud3.getPosition().x > 1920)
				{
					// set it up ready to be a whole new cloud next frame 
					cloud3Active = false;
				}
			}

			// update the score text 
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			// choose message for 
			srand((int)time(0) + score);
			int k = (rand() % 3);
			std::string deathText;
			if (score > highScore)
			{
				deathText = "HIGH SCORE!!";
			}
			else if (k == 0) {
				deathText = "OH NO!!";
			}
			else if (k == 1)
			{
				deathText = "OUCH!!";;
			}
			else
			{
				deathText = "SQUISHED!!";
			}



			// update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT)
				{
					// move the sprite to the left side 
					branches[i].setPosition(610, height);

					// flip the sprite around the other way
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					// move the sprite to the right side 
					branches[i].setPosition(1330, height);
					// set the sprite rotation to normal 
					branches[i].setRotation(0);
				}
				else
				{
					// hide the branch
					branches[i].setPosition(3000, height);
				}

				// Handle a flying log 
				if (logActive)
				{
					spriteLog.setPosition(
						spriteLog.getPosition().x
						+ (logSpeedX * dt.asSeconds()),
						spriteLog.getPosition().y +
						(logSpeedY * dt.asSeconds()));

					// has the log reached the right hand edge? 
					if (spriteLog.getPosition().x < -100 ||
						spriteLog.getPosition().y > 2000)
					{
						// set it up ready to be a whole new log next frame 
						logActive = false;
						spriteLog.setPosition(810, 720);
					}

				}
				// has the player been squished by a branch? 
				if (branchPositions[5] == playerSide)
				{
					// hide the axe
					spriteAxe.setPosition(2000,
						spriteAxe.getPosition().y);

					// death
					paused = true;
					acceptInput = false;
					if (playerSide == side::LEFT)
					{
						// draw the gravestone on the left 
						spriteRIP.setPosition(525, 760);
					}
					else
					{
						// draw the grave stone on the right
						spriteRIP.setPosition(1200, 760);
					}

					// hide the player
					spritePlayer.setPosition(2000, 660);

					// change the text of the message 
					messageText.setString(deathText);
					// center it on the screen
					FloatRect textRect = messageText.getLocalBounds();

					messageText.setOrigin(textRect.left +
						textRect.width / 2.0f,
						textRect.top + textRect.height / 2.0f);

					messageText.setPosition(1920 / 2.0f,
						1080 / 2.0f);
					if (score > highScore)
					{
						// update the score text 
						std::stringstream highss;
						highScore = score;
						highss << "High = " << highScore;
						highScoreText.setString(highss.str());

					}

					// play death sound
					death.play();

				}

			}
		} // End if(!paused)


		/*
		------------------------------
		Draw the scene
		------------------------------
		*/


		// clear everything from the last frame 
		window.clear();

		// draw our game scene here
		window.draw(spriteBackground);

		// draw the clouds	 
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		// draw background trees
		window.draw(spriteTree2);
		window.draw(spriteTree3);
		window.draw(spriteTree4);
		window.draw(spriteTree5);

		// draw the branches
		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}

		// draw the tree
		window.draw(spriteTree);

		// draw the tree


		// draw the axe
		window.draw(spriteAxe);

		// draw the player 
		window.draw(spritePlayer);

		// draw the flying log
		window.draw(spriteLog);

		// draw the gravestone
		window.draw(spriteRIP);

		// draw the bee
		window.draw(spriteBee);

		// draw Yogi
		window.draw(spriteYogi);

		// draw the score
		window.draw(scoreText);

		// draw the high score
		window.draw(highScoreText);

		// draw the timebar
		window.draw(timeBar);

		if (paused)
		{
			// draw our message
			window.draw(messageText);
		}

		// show everything we just drew 
		window.display();

	}

	return 0;
}