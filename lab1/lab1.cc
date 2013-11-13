#include <cmath>
#include <iostream>

#include "Point.hh"

//compute area of triangle with Heron's formula
double computeArea(Point &a, Point &b, Point &c)
{
	double area = 0.0;
	
	double distance_ab = a.distanceTo(b);
	double distance_ac = a.distanceTo(c);	
	double distance_bc = b.distanceTo(c);	

	double s = (distance_ab + distance_ac + distance_bc) / 2.0;

	area = s*(s - distance_ab)*(s - distance_ac)*(s - distance_bc);	
	area = sqrt(area);

	return area;
}

int main()
{
	double x,y,z;
	int i;

	Point arr[3];

	for(i=0 ; i<3 ; ++i)
	{
		std::cout << "enter cordinate x" << i << " : ";
		std::cin  >> x;

		std::cout << "enter cordinate y" << i << " : ";
		std::cin  >> y;

		std::cout << "enter cordinate z" << i << " : ";
		std::cin  >> z;

		arr[i] = Point(x,y,z);
	}

	std::cout << "the area is : " << computeArea(arr[0], arr[1], arr[2]) << std::endl;

	return 0;
}


