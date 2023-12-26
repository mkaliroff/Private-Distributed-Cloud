#include <iostream>
#include "my_assert.hpp"
#include "shapes.hpp"
#include "factory.hpp"/*header*/

using namespace pdc;

void ShapeTest(void);
void AddSetTest(void);
std::shared_ptr<Shape> CreateLine(int id);
std::shared_ptr<Shape> CreateRectangle(int id);
std::shared_ptr<Shape> CreateSquare(int id);
std::shared_ptr<Shape> CreateCircle(int id);

int main()
{
	ShapeTest();
	AddSetTest();
	return 0;
}


void ShapeTest(void)
{
	Factory::Factory<Shape, std::string, int> &factory = 
	Singleton<Factory::Factory<Shape, std::string, int>>::Get();

	factory.Add("Line", CreateLine);
	factory.Add("Rectangle", CreateRectangle);
	factory.Set("Square", CreateSquare);
	factory.Set("Circle", CreateCircle);

	std::shared_ptr<Shape> l1 = factory.Create("Line" ,1);
	std::shared_ptr<Shape> l2 = factory.Create("Line" ,2);
	std::shared_ptr<Shape> r1 = factory.Create("Rectangle" ,3);
	std::shared_ptr<Shape> s1 = factory.Create("Square" ,4);
	std::shared_ptr<Shape> c1 = factory.Create("Circle" ,5);

	l1->Draw();
	l2->Draw();
	r1->Draw();
	s1->Draw();
	c1->Draw();
}

void AddSetTest(void)
{
	Factory::Factory<Shape, std::string, int> &factory = 
	Singleton<Factory::Factory<Shape, std::string, int>>::Get();

	factory.Add("Test", CreateLine);
	std::shared_ptr<Shape> shape1 = factory.Create("Test", 1);
	try
	{
		factory.Add("Test", CreateRectangle);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	std::shared_ptr<Shape> shape2 = factory.Create("Test", 2);
	factory.Set("Test", CreateRectangle);
	std::shared_ptr<Shape> shape3 = factory.Create("Test", 3);

	shape1->Draw();
	shape2->Draw();
	shape3->Draw();
}

std::shared_ptr<Shape> CreateLine(int id)
{
	std::cout << "creating line, id: " << id <<'\n';
    return std::shared_ptr<Line>(new Line);
}

std::shared_ptr<Shape> CreateRectangle(int id)
{
	std::cout << "creating Rectangle, id: " << id <<'\n';
    return std::shared_ptr<Rectangle>(new Rectangle);
}

std::shared_ptr<Shape> CreateSquare(int id)
{
	std::cout << "creating Square, id: " << id <<'\n';
    return std::shared_ptr<Square>(new Square);
}

std::shared_ptr<Shape> CreateCircle(int id)
{
	std::cout << "creating Circle, id: " << id <<'\n';
    return std::shared_ptr<Circle>(new Circle);
}
