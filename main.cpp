#include<iostream>
#include<math.h>
#include <stdlib.h>
#include<time.h>
#include<vector>

using namespace std;

struct Coordinate{
	float x;
	float y;
};

struct Circle {
	Coordinate center; 
	float radius;
};

struct Rectangle{
	Coordinate top;
	Coordinate bottom;
};

//Randomly generate co-ordinates
Coordinate gen_random_coordinates() {
	Coordinate random_coordinate = {float(rand() % 20), float(rand() % 20)};
	return random_coordinate;
}

//Calculate distance between 2 co-ordinates
float calc_distance(float x1, float y1, float x2, float y2) {
	return sqrt(pow(x2-x1,2) + pow(y2-y1,2));
}

//Generate the unique circle from 3 given points
Circle gen_circle_three_points(float x1, float y1, float x2, float y2, float x3, float y3) {
	Circle gen_circle;
	float m = ((x1*x1) + (y1*y1)) - ((x2*x2) + (y2*y2));
	float p = ((x1*x1) + (y1*y1)) - ((x3*x3) + (y3*y3));
	float b = 2*(x1-x2);
	float c = 2*(y1-y2);
	float q = 2*(x1 - x3);
	float r = 2*(y1 - y3);

	float radius = 0.0;
	float h = 0.0;
	float k = 0.0;

	if(c/b == r/q) {
		float max_dis = 0;
		float dis1 = calc_distance(x1,y1,x2,y2);
		float dis2 = calc_distance(x1,y1,x3,y3);
		float dis3 = calc_distance(x2,y2,x3,y3);
		if(dis1 > dis2 && dis1 > dis3) {
			max_dis = dis1;
			gen_circle.center.x = (x2+x1)/2;
			gen_circle.center.y = (y2+y1)/2;
			gen_circle.radius = max_dis/2;
		}
		else if (dis2 > dis3 && dis2 > dis1) {
			max_dis = dis2;
			gen_circle.center.x = (x3+x1)/2;
			gen_circle.center.y = (y3+y1)/2;
			gen_circle.radius = max_dis/2;
		}
		else {
			max_dis = dis3;
			gen_circle.center.x = (x3+x2)/2;
			gen_circle.center.y = (y3+y2)/2;
			gen_circle.radius = max_dis/2;
		}
	}
	else {
		float det = b*r - c*q;
		if(det != 0) {
			h = (m*r - c*p)/det;
			k = (b*p - m*q)/det;
			radius = sqrt((x1-h)*(x1-h) + (y1-k)*(y1-k));
		}
		gen_circle.center.x = h;
		gen_circle.center.y = k;
		gen_circle.radius = radius;
	}
	return gen_circle;
}

bool check_circle_validity(Circle circle, Coordinate point_coordinates[], int size) {
	for(int i=0; i<size; i++) {
		float distance = sqrt(pow(point_coordinates[i].x - circle.center.x,2) + pow(point_coordinates[i].y - circle.center.y,2));
		if(distance > circle.radius)
			return false;
	}
	return true;
}

Circle min_area_circle_enclosing_rects(Rectangle rect[], int size) {
	Circle enclosing_circle;
	int no_of_coordinates = size*4;
	Coordinate coordinates[no_of_coordinates];
	float min_radius = 0.0;

	int index = 0;

	for (int i = 0; i < size; ++i)
	{	
		//(x1,y1)
		coordinates[index].x = rect[i].top.x;
		coordinates[index++].y = rect[i].top.y;		
		//(x2,y2)
		coordinates[index].x = rect[i].bottom.x;
		coordinates[index++].y = rect[i].bottom.y;
		//(x2,y1)
		coordinates[index].x = rect[i].bottom.x;
		coordinates[index++].y = rect[i].top.y;
		//(x1,y2)
		coordinates[index].x = rect[i].top.x;
		coordinates[index++].y = rect[i].bottom.y;
	}

	
	for (int i = 0; i < no_of_coordinates; ++i)
	{
		for (int j = i+1; j < no_of_coordinates; ++j)
		{	
			for (int k = j+1; k < no_of_coordinates; ++k)
			{
				Circle gen_circle = gen_circle_three_points(coordinates[i].x,coordinates[i].y,coordinates[j].x,coordinates[j].y,coordinates[k].x,coordinates[k].y);
				float area = 3.14 * gen_circle.radius * gen_circle.radius;

				if((gen_circle.radius != 0) && (gen_circle.radius < min_radius || min_radius == 0.0)) {
					bool is_circle_enclosing = check_circle_validity(gen_circle, coordinates, no_of_coordinates);
					
					if(is_circle_enclosing) {
						min_radius = gen_circle.radius;
						if(gen_circle.center.x == -0){
							gen_circle.center.x = 0;
						}
						if(gen_circle.center.y == -0){
							gen_circle.center.y = 0;
						}
						enclosing_circle.center.x = gen_circle.center.x;
						enclosing_circle.center.y = gen_circle.center.y;
						enclosing_circle.radius = gen_circle.radius;
					}
				}	
			}
		}
	}
	return enclosing_circle;
}

Rectangle min_area_rect_enclosing_rects(Rectangle rect[], int size) {
	Rectangle enclosing_rect;
	float max_top_x, max_top_y, min_bottom_x, min_bottom_y;
	max_top_x = rect[0].top.x;
	max_top_y = rect[0].top.y;
	min_bottom_x = rect[0].bottom.x;
	min_bottom_y = rect[0].bottom.y;

	for (int i = 0; i < size; ++i)
	{
		//Deciding the max_top_x
		if(rect[i].top.x > max_top_x)
			max_top_x = rect[i].top.x;
		if(rect[i].bottom.x > max_top_x)
			max_top_x = rect[i].bottom.x;

		//Deciding the max_top_y
		if(rect[i].top.y > max_top_y)
			max_top_y = rect[i].top.y;
		if(rect[i].bottom.y > max_top_y)
			max_top_y = rect[i].bottom.y;
		
		//Deciding the min_bottom_x
		if(rect[i].top.x < min_bottom_x)
			min_bottom_x = rect[i].top.x;
		if(rect[i].bottom.x < min_bottom_x)
			min_bottom_x = rect[i].bottom.x;
		
		//Deciding the min_bottom_x		
		if(rect[i].top.y < min_bottom_y)
			min_bottom_y = rect[i].top.y;
		if(rect[i].bottom.y < min_bottom_y)
			min_bottom_y = rect[i].bottom.y;							
	}
	
	enclosing_rect.top.x = max_top_x;
	enclosing_rect.top.y = max_top_y;
	enclosing_rect.bottom.x = min_bottom_x;
	enclosing_rect.bottom.y = min_bottom_y;
	return enclosing_rect;
}

Rectangle min_area_rect_enclosing_circles(Circle circle[], int size) {
	Rectangle enclosing_rect;
	float max_top_x, max_top_y, min_bottom_x, min_bottom_y;

	max_top_x = circle[0].center.x + circle[0].radius;
	max_top_y = circle[0].center.y + circle[0].radius;
	min_bottom_x = circle[0].center.x - circle[0].radius;
	min_bottom_y = circle[0].center.y - circle[0].radius;
	float center_x, center_y, radius;

	for (int i = 0; i < size; ++i)
	{
		center_x = circle[i].center.x;
		center_y = circle[i].center.y;
		radius = circle[i].radius;

		//Deciding the max_top_x
		if(max_top_x < center_x + radius)
			max_top_x = center_x + radius;
		
		//Deciding the max_top_y
		if(max_top_y < center_y + radius)
			max_top_y = center_y + radius;
		
		//Deciding the min_bottom_x
		if(min_bottom_x > center_x - radius)
			min_bottom_x = center_x - radius;
		
		//Deciding the min_bottom_x
		if(min_bottom_y > center_y - radius)
			min_bottom_y = center_y - radius;
	}

	enclosing_rect.top.x = max_top_x;
	enclosing_rect.top.y = max_top_y;
	enclosing_rect.bottom.x = min_bottom_x;
	enclosing_rect.bottom.y = min_bottom_y;
	return enclosing_rect;
}

int main() {
	int no_of_rectangles,no_of_circles;
	
	cout<<"Enter the number of rectangles to be generated randomly:";
	cin>>no_of_rectangles;

	cout<<"Enter the number of circles to be generated randomly:";
	cin>>no_of_circles;
	cout<<endl;

	srand(time(0));

	Rectangle rect[no_of_rectangles];
	for (int i = 0; i < no_of_rectangles; ++i)
	{
		rect[i].top = gen_random_coordinates();
		rect[i].bottom = gen_random_coordinates();
		cout<<"Co-ordinates for ("<<i+1<<") rectangle is: \n ("<<rect[i].top.x<<","<<rect[i].top.y<<") \n ("<<rect[i].bottom.x<<","<<rect[i].bottom.y<<")"<<endl<<endl;
	}

	cout<<endl;

	Circle circle[no_of_circles];
	for (int i = 0; i < no_of_circles; ++i)
	{
		circle[i].center = gen_random_coordinates();
		circle[i].radius = float(rand() % 20);
		cout<<"Co-ordinates for ("<<i+1<<") circle is: \n ("<<circle[i].center.x<<","<<circle[i].center.y<<") \n Readius = "<<circle[i].radius<<endl<<endl;
	}

	cout<<endl;

	Circle res_circle_enclosing_rect = min_area_circle_enclosing_rects(rect, no_of_rectangles);
	Rectangle res_rect_enclosing_rect = min_area_rect_enclosing_rects(rect, no_of_rectangles);
	Rectangle res_rect_enclosing_circle = min_area_rect_enclosing_circles(circle, no_of_circles);

	cout<<"Co-ordinates for min-area rectangle enclosing generated rectangles:"<<endl;
	cout<<"  Co-ordinates for top: ("<<res_rect_enclosing_rect.top.x<<","<<res_rect_enclosing_rect.top.y<<")"<<endl;	
	cout<<"  Co-ordinates for bottom: ("<<res_rect_enclosing_rect.bottom.x<<","<<res_rect_enclosing_rect.bottom.y<<")"<<endl<<endl;	

	cout<<"Co-ordinates for min-area rectangle enclosing generated circles:"<<endl;
	cout<<"  Co-ordinates for top: ("<<res_rect_enclosing_circle.top.x<<","<<res_rect_enclosing_circle.top.y<<")"<<endl;	
	cout<<"  Co-ordinates for bottom: ("<<res_rect_enclosing_circle.bottom.x<<","<<res_rect_enclosing_circle.bottom.y<<")"<<endl<<endl;

	cout<<"Co-ordinates for min-area cirlce enclosing generated rectangles:"<<endl;
	cout<<"  Co-ordinates for center: ("<<res_circle_enclosing_rect.center.x<<","<<res_circle_enclosing_rect.center.y<<")"<<endl;
	cout<<"  Radius of the enclosing circle = "<<res_circle_enclosing_rect.radius<<endl;	
	return 1;
}