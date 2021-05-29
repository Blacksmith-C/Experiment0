#include <SFML/Graphics.hpp>
using std::vector;

//Declare 3D Vector Format
struct vec3
{
	float x, y, z;
};

//Declare 3D Polygon Format
struct tri3
{
	vec3 p[3];
};

//Declare 3D Mesh Format
struct mesh
{
	vector<tri3> polys;
};

//Declare 4x4 Matrix Format
struct mat4
{
	float m[4][4] = { 0 };
};





//Function for Multiplying Vectors by Matrix
void MatxVec(vec3 &i, vec3 &o, mat4 &m)
{
	o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
	o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
	o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
	float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

	if (w != 0.0f)
	{
		o.x /= w;
		o.y /= w;
		o.z /= w;
	}
}





//Do this before every frame
void update(sf::Time elapsedtime, mesh obj, mat4 Projection, sf::RenderWindow& window, float& Theta)
{
	mat4 RotZ, RotX;
	Theta += 1.0f * elapsedtime.asSeconds();

	//Define Camera Rotation Matrices
	RotZ.m[0][0] = cosf(Theta);
	RotZ.m[0][1] = sinf(Theta);
	RotZ.m[1][0] = -sinf(Theta);
	RotZ.m[1][1] = cosf(Theta);
	RotZ.m[2][2] = 1;
	RotZ.m[3][3] = 1;

	RotX.m[0][0] = 1;
	RotX.m[1][1] = cosf(Theta*0.1f);
	RotX.m[1][2] = sinf(Theta*0.1f);
	RotX.m[2][1] = -sinf(Theta*0.1f);
	RotX.m[2][2] = cosf(Theta*0.1f);
	RotX.m[3][3] = 1;
	
	// Draw Mesh
	for(auto tri : obj.polys)
		{
		tri3 Projected, Translated, RotatedZ, RotatedZX;

		//Rotate Triangles Based on Camera Orientation
		MatxVec(tri.p[0], RotatedZ.p[0], RotZ);
		MatxVec(tri.p[1], RotatedZ.p[1], RotZ);
		MatxVec(tri.p[2], RotatedZ.p[2], RotZ);

		MatxVec(RotatedZ.p[0], RotatedZX.p[0], RotX);
		MatxVec(RotatedZ.p[1], RotatedZX.p[1], RotX);
		MatxVec(RotatedZ.p[2], RotatedZX.p[2], RotX);
		
		//Move Triangles Away from Camera
		Translated = RotatedZX;
		Translated.p[0].z = RotatedZX.p[0].z + 1.5f;
		Translated.p[1].z = RotatedZX.p[1].z + 1.5f;
		Translated.p[2].z = RotatedZX.p[2].z + 1.5f;

		//Project Triangles onto 2D
		MatxVec(Translated.p[0], Projected.p[0], Projection);
		MatxVec(Translated.p[1], Projected.p[1], Projection);
		MatxVec(Translated.p[2], Projected.p[2], Projection);

		//Scale Triangles to Screen Size
		Projected.p[0].x += 1.0f;
		Projected.p[0].y += 1.0f;
		Projected.p[1].x += 1.0f;
		Projected.p[1].y += 1.0f;
		Projected.p[2].x += 1.0f;
		Projected.p[2].y += 1.0f;

		Projected.p[0].x *= 0.5f * 1920.0f;
		Projected.p[0].y *= 0.5f * 1080.0f;
		Projected.p[1].x *= 0.5f * 1920.0f;
		Projected.p[1].y *= 0.5f * 1080.0f;
		Projected.p[2].x *= 0.5f * 1920.0f;
		Projected.p[2].y *= 0.5f * 1080.0f;

		//Draw Triangles to Window
		sf::VertexArray poly(sf::LineStrip, 4);

		poly[0].position = sf::Vector2f(Projected.p[0].x, Projected.p[0].y);
		poly[1].position = sf::Vector2f(Projected.p[1].x, Projected.p[1].y);
		poly[2].position = sf::Vector2f(Projected.p[2].x, Projected.p[2].y);
		poly[3].position = sf::Vector2f(Projected.p[0].x, Projected.p[0].y);

		poly[0].color = sf::Color(200, 100, 100, 255);
		poly[1].color = sf::Color(100, 200, 100, 255);
		poly[2].color = sf::Color(100, 100, 200, 255);
		poly[3].color = sf::Color(200, 100, 100, 255);

		window.draw(poly);

		//sf::ConvexShape Poly;
		//Poly.setPointCount(3);
		//Poly.setPoint(0, sf::Vector2f(Projected.p[0].x, Projected.p[0].y));
		//Poly.setPoint(1, sf::Vector2f(Projected.p[1].x, Projected.p[1].y));
		//Poly.setPoint(2, sf::Vector2f(Projected.p[2].x, Projected.p[2].y));
		//Poly.setOutlineThickness(1.0f);
		//Poly.setOutlineColor(sf::Color(100, 100, 100, 255));
		//Poly.setFillColor(sf::Color(0, 0, 0, 0));

		//window.draw(Poly);
	}
}





int main()
{
	//Open Window
	sf::RenderWindow mywindow(sf::VideoMode(1920, 1080), "Abandon All Hope Ye Who Enter Here");
	
	//Begin Measuring Time
	sf::Clock clock;
	
	//Define Window Background Color
	sf::Color sky(20, 20, 40, 255);

	//Define Mesh to Render
	mesh cubemesh;
	cubemesh.polys = {
		// South Face
		{-0.5f, -0.5f, -0.5f,  -0.5, 0.5f, -0.5f,  0.5f, 0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f,  0.5f, 0.5f, -0.5f,  0.5f, -0.5f, -0.5f},
		
		//East Face
		{0.5f, -0.5f, -0.5f,  0.5f, 0.5f, -0.5f,  0.5f, 0.5f, 0.5f},
		{0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,  0.5f, -0.5f, 0.5f},

		//North Face
		{0.5f, -0.5f, 0.5f,  0.5f, 0.5f, 0.5f,  -0.5f, 0.5f, 0.5f},
		{0.5f, -0.5f, 0.5f,  -0.5f, 0.5f, 0.5f,  -0.5f, -0.5f, 0.5f},

		// West Face
		{-0.5f, -0.5f, 0.5f,  -0.5f, 0.5f, 0.5f,  -0.5f, 0.5f, -0.5f},
		{-0.5f, -0.5f, 0.5f,  -0.5f, 0.5f, -0.5f,  -0.5f, -0.5f, -0.5f},

		//Top Face
		{-0.5f, 0.5f, -0.5f,  -0.5f, 0.5f, 0.5f,  0.5f, 0.5f, 0.5f},
		{-0.5f, 0.5f, -0.5f,  0.5f, 0.5f, 0.5f,  0.5f, 0.5f, -0.5f},

		//Bottom Face
		{0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f},
		{0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f},
	};

	//Define Projection Matrix
	float CameraDistance = 0.1f;
	float RenderDistance = 1000.0f;
	float FOV = 105.0f;
	float Aspect = 1080.0f / 1920.0f;
	float fFOV = 1.0f / tanf(FOV * 0.5f / 180.0f *3.14159f);
	
	mat4 Projection;

	Projection.m[0][0] = Aspect * fFOV;
	Projection.m[1][1] = fFOV;
	Projection.m[2][2] = RenderDistance / (RenderDistance - CameraDistance);
	Projection.m[3][2] = (-RenderDistance * CameraDistance) / (RenderDistance - CameraDistance);
	Projection.m[2][3] = 1.0f;
	Projection.m[3][3] = 0.0f;

	//Initialize Rotation Angle
	float Theta = 0.1f;

	//Run Program Until User Closes Window
	while (mywindow.isOpen())
	{
		//Check All New Events
		sf::Event event;
		while (mywindow.pollEvent(event))
		{
			//If an event is type "Closed", close the window
			if (event.type == sf::Event::Closed)
				mywindow.close();
		}

		//Get Time Since Last Frame
		sf::Time delta = clock.restart();

		//Reset Window to Blank Background
		mywindow.clear(sky);
		
		//Update Game Logic
		update(delta,cubemesh,Projection,mywindow,Theta);

		//Draw Frame to Window
		mywindow.display();
	}
	return 0;
}