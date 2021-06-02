#include <SFML/Graphics.hpp>
#include <string>
#include <ctime>
#include <vector>
#include <algorithm>
using std::string;
using std::array;
using std::vector;

string NamePicker(vector<string> list, string currentvalue, string PatrioticNoun[], string CommieNoun[], string Adjective[], string KillVerb[]) {
	
	string Suggestion = currentvalue;
	bool taken = true;
	while (taken || Suggestion == currentvalue)
	{
		switch (rand() % 4) {
		case 0: //Adjective PatrioticNoun
			Suggestion = Adjective[rand() % 10];
			Suggestion += PatrioticNoun[rand() % 20];
			break;
		case 1: //Adjective CommieNoun KillVerbEr
			Suggestion = Adjective[rand() % 10];
			Suggestion += CommieNoun[rand() % 10];
			Suggestion += KillVerb[rand() % 10];
			Suggestion += "er";
			break;
		case 2: //CommieNoun KillVerbIng PatrioticNoun
			Suggestion = CommieNoun[rand() % 10];
			Suggestion += KillVerb[rand() % 10];
			Suggestion += "ing";
			Suggestion += PatrioticNoun[rand() % 20];
			break;
		case 3: //PatroticNoun####
			Suggestion = PatrioticNoun[rand() % 20];
			Suggestion += std::to_string(rand() % 9999);
			break;
		}

		for (int i = 1; i <= size(list); i++) {
			if (list[i] == Suggestion) {
				taken = true;
				break;
			}
			taken = false;
		}
	}
	return Suggestion;
}

void main() {

	//Open Window
	sf::RenderWindow mywindow(sf::VideoMode(1920, 1080), "Abandon All Hope Ye Who Enter Here");

	//Begin Measuring Time
	sf::Clock clock;

	//Define Window Background Color
	sf::Color sky(20, 20, 40, 255);

	//Seed RNG
	srand(time(NULL));

	//Initialize Text Font
	sf::Font CourierNew;
	CourierNew.loadFromFile("CourierNew.ttf");

	//"Load" list of taken names
	vector<string> namelist = { "CommieCrushingPatriot", "BigIron", "Red-BloodedAmerican", "HugeRoughRider" };
	
	//Set Default Text
	sf::Text DisplayText;
	DisplayText.setString("Press Any Key to Generate a Username Suggestion");
	DisplayText.setPosition(10.0f, 10.0f);
	DisplayText.setFont(CourierNew);
	DisplayText.setCharacterSize(20);
	DisplayText.setFillColor(sf::Color::Red);

	sf::Time delta;
	sf::Event event;

	bool keyheld = false;

	string PatrioticNoun[20] = { "Washington","Lincoln","Jefferson","Teddy","Patriot","American","Minuteman","Cowboy","USGrant","Supercarrier",
									  "Iron","Machine","SaturnV","Mustang","Operator","RoughRider","Pilot","Astronaut","Nationalist","Jackson" };
	string CommieNoun[10] = { "Commie","Pinko","Nazi","Fascist","Maoist","Stalinist","JohnnyReb","Jap","Roosky","Kraut" };
	string Adjective[10] = { "Big","Sexy","Huge","Handsome","Ripped","Strong","God-Fearing","Red-Blooded","Smoking","Supersonic" };
	string KillVerb[10] = { "Kill","Smash","Destroy","Mogg","Own","Crush","Dominat","Slay","Purg","Slapp" };

	//Run Program Until User Closes Window
	while (mywindow.isOpen())
	{
		//Check All New Events
		while (mywindow.pollEvent(event))
		{
			//If an event is type "Closed", close the window
			if (event.type == sf::Event::Closed) {
				mywindow.close();
			}

		}

		//Get Time Since Last Frame
		delta = clock.restart();

		//Reset Window to Blank Background
		mywindow.clear(sky);

		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			keyheld = false;
		}

		//Draw text
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !keyheld)
		{
			DisplayText.setString(NamePicker(namelist, DisplayText.getString(), PatrioticNoun, CommieNoun, Adjective, KillVerb));
			keyheld = true;
		}
		mywindow.draw(DisplayText);

		//Draw Frame to Window
		mywindow.display();

	}
}







