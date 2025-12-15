//Full names: Toma Spirov
//Moodle username: tls1crt (tls1crt@bolton.ac.uk)
//Student numbers: 2416915

#include <iostream>
#include <memory>
#include <fstream>
#include <vector>

// comment
// NOTE: SFML is our platform layer: window, input, textures, shapes, fonts.
// This is our "OS abstraction" for now.
#include <SFML/Graphics.hpp>


// ImGui gives us an editor/debug UI layer on top of the game.
// Here we use the SFML backend instead of OpenGL.
#include "imgui.h"
#include "imgui-SFML.h"

//wrapper struct to hold shape data - i use to manage and update data and apply changes to SFML shape objects
struct ShapeData {
	//warning message required some variables to be inintialized with default values 
	enum Type { CIRCLE, RECTANGLE } type = CIRCLE;
	std::string _Name = "";
	float _X = 0.f, _Y = 0.f;//position
	float _SX = 0.f, _SY = 0.f;//velosity
	uint8_t _R = 0, _G = 0, _B = 0;//color RGB 0-255

	// Circle specific
	float _Radius = 0.f;//radius

	// Rectangle specific
	float _W = 0.f, _H = 0.f;//width, height

	//shapes SFML objects
	sf::CircleShape circle;
	sf::RectangleShape rect;

	bool drawShape = true;//flag to draw/not draw shape

};

//function to animate all shapes in the vector
void static animateShapes(std::vector<ShapeData>& shapes, uint32_t  windowdWidth, uint32_t windowHeight)
{
	//iterator through all shapes and move them
	for (auto& currShape : shapes)
	{
		//check if shape flag to draw is false skip to next iteration -  no need to animate it
		//if (!currShape.drawShape) continue; //this is good to be used as we skip unnecesary calculations for shapes that are not drawn
		//but on the A1_Solution_Win64 it was not requested so I left it commented

		if (currShape.type == ShapeData::CIRCLE)
		{
			currShape.circle.setRadius(currShape._Radius);
			currShape.circle.setFillColor(sf::Color(currShape._R, currShape._G, currShape._B));

			sf::Vector2f pos = currShape.circle.getPosition();

			if (pos.x <0 || pos.x + 2 * currShape._Radius > windowdWidth) currShape._SX *= -1;//changing direction
			if (pos.y <0 || pos.y + 2 * currShape._Radius > windowHeight) currShape._SY *= -1;//changing direction

			pos.x += currShape._SX;
			pos.y += currShape._SY;

			currShape.circle.setPosition(pos);
		}

		if (currShape.type == ShapeData::RECTANGLE)
		{
			currShape.rect.setSize({ currShape._W, currShape._H });
			currShape.rect.setFillColor(sf::Color(currShape._R, currShape._G, currShape._B));

			sf::Vector2f pos = currShape.rect.getPosition();

			if (pos.x <0 || pos.x + currShape._W > windowdWidth) currShape._SX *= -1;//changing direction
			if (pos.y <0 || pos.y + currShape._H > windowHeight) currShape._SY *= -1;//changing direction

			pos.x += currShape._SX;
			pos.y += currShape._SY;

			currShape.rect.setPosition(pos);
		}
	}

}

//function to animate all texts in the vector regarding the related shapes(each index is related to both vectors elements on this index position)
void static animateText(std::vector<sf::Text>& textVec, std::vector<ShapeData>& shapesVec)
{
	int textWCenter = 0;//for curr text width center
	std::size_t textHCenter = textVec[0].getCharacterSize() / 2;//get text height = font size from first text object(as the font size remain the same )

	for (std::size_t i = 0; i < shapesVec.size(); i++)
	{
		//check if shape flag to draw is false skip to next iteration -  no need to animate it
		if (!shapesVec[i].drawShape) continue;//this is good to be used as we skip unnecesary calculations for text that are not drawn
		//but on the A1_Solution_Win64 it was not requested so I left it commented

		//curr text width center
		textWCenter = (textVec[i].getString().getSize() * textHCenter) / 2;
		//std::cout << "Text Width: " << textWCenter << ", Text Height: " << textHCenter << std::endl;

		if (shapesVec[i].type == ShapeData::CIRCLE)
		{
			sf::Vector2f circlePos = shapesVec[i].circle.getPosition();
			float radius = shapesVec[i]._Radius;
			textVec[i].setPosition(sf::Vector2f((circlePos.x + radius - textWCenter), (circlePos.y + radius - textHCenter)));//center text for circle shape
		}
		else
		{
			sf::Vector2f rectPos = shapesVec[i].rect.getPosition();
			float width = shapesVec[i]._W;
			float height = shapesVec[i]._H;
			textVec[i].setPosition(sf::Vector2f((rectPos.x + (width / 2.0f) - textWCenter), (rectPos.y + (height / 2.0f) - textHCenter)));//center text for rectangle shape

		}
	}
}

//global variables

std::vector<ShapeData> shapesVector;
std::vector<sf::Text> shapesTextVector;
int selectedShapeIndex = -1;

int main(int argc, char* argv[])
{
	float c[3] = { 1.0f, 1.0f, 1.0f };//used for color editor only

	uint32_t  wW = 0, wH = 0;//window width and height variables

	//font variable
	sf::Font myFont;//font object
	std::string fontFilePath;//string no spaces
	int fontSize;
	int fontColor_R, fontColor_G, fontColor_B;//font color RGB

	//loading data from file
	std::ifstream istr;
	istr.open("config.txt");

	//checks while opening the file if not being opened
	if (!istr.is_open())
	{
		std::cerr << "Couldn't open the file! ERRROOOORRRR!!!!";
	}

	//fethch the data from file
	std::string type;
	while (istr >> type)
	{
		//Window W H
		if (type == "Window")
		{
			istr >> wW >> wH;
			//std::cout << " Window -> w:" << w << ", h:" << h << std::endl;
		}
		//Font F S R G B
		if (type == "Font")
		{
			istr >> fontFilePath >> fontSize >> fontColor_R >> fontColor_G >> fontColor_B;

			// attempt to load the font from a file
			if (!myFont.openFromFile(fontFilePath))
			{
				// if we can't load the font, print an error to the error console and exit
				std::cout << "Could not load font!\n";
				std::exit(-1);
			}

			//std::cout << " Font -> path:" << fontFilePath << ", size:" << fontSize << ", r:" << fontColor_R << ", g:" << fontColor_G << ", b:" << fontColor_B << std::endl;

		}
		//Rectangle N X Y SX SY R G B W H
		if (type == "Rectangle")
		{
			ShapeData rectObj;//wrapper object for rectangle shape data

			std::string shapeName;
			float X, Y;//position
			float SX, SY;//velosity
			int R, G, B;//color RGB
			float W, H;//width, height
			istr >> shapeName >> X >> Y >> SX >> SY >> R >> G >> B >> W >> H;
			//std::cout << " Rectangle -> x:" << X << ", y:" << Y << ", sx:" << SX << ", sy:" << SY << ", r:" << R << ", g:" << G << ", b:" << B << ", width:" << W << ", height:" << H << std::endl;

			//inintialize variables
			rectObj._Name = shapeName;
			rectObj._X = X;
			rectObj._Y = Y;
			rectObj._SX = SX;
			rectObj._SY = SY;
			rectObj._R = R;
			rectObj._G = G;
			rectObj._B = B;
			rectObj._W = W;
			rectObj._H = H;

			//create rectangle shape object and inintialize it
			rectObj.type = ShapeData::RECTANGLE;
			rectObj.rect = sf::RectangleShape(sf::Vector2(W, H));
			rectObj.rect.setPosition(sf::Vector2f(X, Y));
			//rectObj.rect.setFillColor(sf::Color(R, G, B));
			//std::cout << "Rectangel wiht name " << shapeName << "color (" << R << "," << G << "," << B << ")";

			sf::Text text(myFont, shapeName, fontSize);
			text.setFillColor(sf::Color::White);

			shapesTextVector.push_back(text);
			shapesVector.push_back(rectObj);

		}
		//Circle N X Y SX SY R G B R
		if (type == "Circle")
		{
			ShapeData circObj;//wrapper object for circle shape data

			std::string shapeName;
			float X, Y;//position
			float SX, SY;//velosity
			int R, G, B;//color RGB
			float Radius;//radius
			istr >> shapeName >> X >> Y >> SX >> SY >> R >> G >> B >> Radius;
			//std::cout << " Circle -> x:" << X << ", y:" << Y << ", sx:" << SX << ", sy:" << SY << ", r:" << R << ", g:" << G << ", b:" << B << ", radius:" << Radius << std::endl;

			//inintialize variables
			circObj._Name = shapeName;
			circObj._X = X;
			circObj._Y = Y;
			circObj._SX = SX;
			circObj._SY = SY;
			circObj._R = R;
			circObj._G = G;
			circObj._B = B;
			circObj._Radius = Radius;

			//create circle shape object and inintialize it
			circObj.type = ShapeData::CIRCLE;
			circObj.circle = sf::CircleShape(Radius);
			circObj.circle.setPosition(sf::Vector2f(X, Y));
			//circObj.circle.setRadius(Radius);
			//circObj.circle.setFillColor(sf::Color(R, G, B));


			sf::Text text(myFont, shapeName, fontSize);
			text.setFillColor(sf::Color::White);
			shapesTextVector.push_back(text);
			shapesVector.push_back(circObj);
		}

	}

	istr.close();//end the file open 

	// create a new window of size w * h pixels
	// top-left of the window is (0,0) and bottom-right is (w, h)
	// you will have to read these from the config file (later in engine construction)
	const uint32_t wWidth = static_cast<const uint32_t>(wW);//get width from txt file
	const uint32_t wHeight = static_cast<const uint32_t>(wH);//get width from txt file

	//sf::RenderWindow window(sf::VideoMode({wW, wH}), "SFML works!");
	sf::RenderWindow window(sf::VideoMode({ wWidth, wHeight }), "SFML works!");
	window.setFramerateLimit(60); // limit frame rate to 60 fps

	// initialize imgui and create a clock used for its internal timing
	if (!ImGui::SFML::Init(window))
	{
		//std::print("Could not initialize window\n");
		std::cout << "Could not initialize window\n";
		std::exit(-1);
	}

	// -------------------------------------------------------------
	// Engine state: these variables represent editable "game state."
	// In Week 1 of engine construction, teaching state management
	// is more important than any features. Everything you do is
	// Input → State Update → Rendering.
	// -------------------------------------------------------------

	// SFML clock for delta time
	sf::Clock deltaClock;

	// scale the imgui ui and text size by 2
	ImGui::GetStyle().ScaleAllSizes(1.0f);
	ImGui::GetIO().FontGlobalScale = 1.0f;


	// -------------------------------------------------------------
	// create the sfml circle shape based on our parameters
	// SFML uses a convex polygon to approximate a circle
	// -------------------------------------------------------------

	// Input text buffer for ImGui
	char displayString[255] = "Sample Text";

	// ==============================================================
	// Main Engine Loop (Game Loop)
	// ==============================================================
	//
	// Classic structure:
	//     1. Poll input events
	//     2. Update game state (movement, animation, logic)
	//     3. Render world
	//     4. Render UI
	//
	// This is the same structure used by every engine (Unity, Unreal,
	// Godot), only with different architecture layers.
	// ==============================================================

	// MAIN LOOP - continues for each frame while window is open
	while (window.isOpen())
	{
		// event handling
		while (auto event = window.pollEvent())
		{
			// pass the event to imgui to be parsed
			ImGui::SFML::ProcessEvent(window, *event);

			// this event triggers when the window is closed
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}

			// this event is triggered when a key is pressed
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				//std::print("Key pressed with code = {}\n", int(keyPressed->scancode));

				std::cout << "Key pressed with code = " << int(keyPressed->scancode) << "\n";

				// simple game logic: pressing X reverses horizontal direction
				if (keyPressed->scancode == sf::Keyboard::Scancode::X)
				{
					//circleSpeedX *= -1.0f;//reverse direction
					for (auto& currShape : shapesVector)
					{
						currShape._SX *= -1.0f;//reverse direction for all shapes
					}
				}
			}
		}

		// update imgui for this frame with the time that the last frame took
		ImGui::SFML::Update(window, deltaClock.restart());

		// ====================
		// Draw the UI 
		// ====================
		ImGui::Begin("Shape Properties");

		//create COMBO box filled with shapes data from vector
		const char* comboLabel = (selectedShapeIndex >= 0 && selectedShapeIndex < shapesVector.size() ? shapesVector[selectedShapeIndex]._Name.c_str() : "Select Shape");

		if (ImGui::BeginCombo("Select Shape", comboLabel))
		{
			for (int i = 0; i < shapesVector.size(); i++)
			{
				bool isSelectsed = (selectedShapeIndex == i);
				if (ImGui::Selectable(shapesVector[i]._Name.c_str(), isSelectsed))
				{
					selectedShapeIndex = i;//update selectedShapeIndex with curr index from combo box

					//update color editor with selected shape color
					c[0] = shapesVector[i]._R / 255.0f;
					c[1] = shapesVector[i]._G / 255.0f;
					c[2] = shapesVector[i]._B / 255.0f;

				}
				if (isSelectsed) ImGui::SetItemDefaultFocus();//mark selected item
			}
			ImGui::EndCombo();
		}

		//display selected shape properties

		if (selectedShapeIndex >= 0) //make sure a shape is selected
		{
			//Display UI for selected shape
			ShapeData& selectedShape = shapesVector[selectedShapeIndex];

			//draw current circle bool
			ImGui::Checkbox("Draw Shape", &selectedShape.drawShape);//TODO change to draw selected shape only


			if (selectedShape.type == ShapeData::CIRCLE)
			{
				//radius slider for circle
				ImGui::SliderFloat("Radius", &selectedShape._Radius, 0.0f, 300.0f);
			}
			else if (selectedShape.type == ShapeData::RECTANGLE)
			{
				//size sliders for rectangle
				ImGui::SliderFloat("Width", &selectedShape._W, 0.0f, 300.0f);
				ImGui::SliderFloat("Height", &selectedShape._H, 0.0f, 300.0f);
			}


			ImGui::SliderFloat("Velocity X ", &selectedShape._SX, 1.0f, 5.0f);
			ImGui::SliderFloat("Velocity Y ", &selectedShape._SY, 1.0f, 5.0f);
			//ImGui::SameLine();

			//color editor update the selected shape color
			if (ImGui::ColorEdit3("Color", c))
			{
				//update selected shape color values from color editor
				selectedShape._R = static_cast<uint8_t>(c[0] * 255);//convert from 0-1 float to 0-255 uint8_t
				selectedShape._G = static_cast<uint8_t>(c[1] * 255);
				selectedShape._B = static_cast<uint8_t>(c[2] * 255);
			}


			//add selected shape name to displayString char
			strcpy_s(displayString, selectedShape._Name.c_str());

			ImGui::InputText("Name", displayString, 255);

		}

		ImGui::End();

		// ==============================================================
		// Apply UI-modified values to actual SFML drawable objects
		// ==============================================================


		animateShapes(shapesVector, wWidth, wHeight);
		animateText(shapesTextVector, shapesVector);

		// RENDERING -  basic rendering function calls
		window.clear(); // clear the window of anything previously drawn

		//render shapes from vector
		for (auto& currShapeData : shapesVector)
		{
			if (!currShapeData.drawShape) continue; //if flag is false not drawing shape, move to next iteration

			if (currShapeData.type == ShapeData::CIRCLE)
			{
				window.draw(currShapeData.circle);
			}

			if (currShapeData.type == ShapeData::RECTANGLE)
			{
				window.draw(currShapeData.rect);
			}
		}


		//render texts from vector
		for (std::size_t i = 0; i < shapesTextVector.size(); i++)
		{
			if (!shapesVector[i].drawShape) continue; //if shape flag is false not drawing text, move to next iteration

			window.draw(shapesTextVector[i]);
		}


		ImGui::SFML::Render(window); // draw the ui last so its on top
		window.display(); // call the window display function
	}

	ImGui::SFML::Shutdown();

	return 0;
}