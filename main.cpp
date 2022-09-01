#include <iostream>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Coord{
public:
	int x;
	int y;

	Coord() : x(0), y(0) {};
	Coord(int _x, int _y) : x(_x), y(_y) {};
	
};

class Rect {
public:
	int x;
	int y;
	int width;
	int height;

public:
	Rect():x(0), y(0), width(0), height(0){};
	Rect(int _x, int _y, int w, int h) : x(_x), y(_y), width(w), height(h) { };

	bool contains(Coord p) {
		return (p.x > this->x - this->width && p.x < this->x + 
			this->width && p.y > this->y - this->width && p.y < this->y + this->width);
	}
};


class QuadTree {
public:
	Rect boundary;
	std::vector<Coord> points;
	int capacity;
	QuadTree* se, *sw, *ne, *nw;
	bool notDivided;
	QuadTree() {
		boundary = Rect(0, 0, 0, 0);
		capacity = 0;
		notDivided = true;
		se = nullptr;
		sw = nullptr;
		ne = nullptr;
		nw = nullptr;
	}
	QuadTree(Rect _boundary, int cap) {
		boundary = _boundary;
		capacity = cap;
		notDivided = true;
		se = nullptr;
		sw = nullptr;
		ne = nullptr;
		nw = nullptr;
	}

	void divide() {
		Rect noe(boundary.x + boundary.width / 2, boundary.y + boundary.height / 2, boundary.width / 2, boundary.height / 2);
		Rect now(boundary.x - boundary.width / 2, boundary.y + boundary.height / 2, boundary.width / 2, boundary.height / 2);
		Rect soe(boundary.x + boundary.width / 2, boundary.y - boundary.height / 2, boundary.width / 2, boundary.height / 2);
		Rect sow(boundary.x - boundary.width / 2, boundary.y - boundary.height / 2, boundary.width / 2, boundary.height / 2);

		se = new QuadTree(soe, capacity);
		sw = new QuadTree(sow, capacity);
		ne = new QuadTree(noe, capacity);
		nw = new QuadTree(now, capacity);

		notDivided = false;

	}
	void insert(Coord p) {

		if (!this->boundary.contains(p))
			return;
		
		if (points.size() < capacity) {
			points.push_back(p);
			return;

		}
			
		else
		{
			if (notDivided) {
				divide();	
			}
			else {
				se->insert(p);
				sw->insert(p);
				ne->insert(p);
				nw->insert(p);
			}

		}

	}

};

class Application : public olc::PixelGameEngine {


public:
	QuadTree qtree;
	Application() {
		
		sAppName = "QuadTree";
	}
	~Application() {

	}

public:
	bool OnUserCreate() override {
		qtree = QuadTree(Rect(200, 200, 200, 200), 4);
		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override {

		Clear(olc::DARK_BLUE);

		if (GetMouse(0).bPressed) {
			qtree.insert(Coord(GetMouseX(), GetMouseY()));
		}

		draw(qtree);
		drawPoints(qtree);
		return true;
	}

	void draw(QuadTree quadtree) {
		DrawRect(quadtree.boundary.x - quadtree.boundary.width, quadtree.boundary.y - 
			quadtree.boundary.height, quadtree.boundary.width*2, quadtree.boundary.height*2);

		if (quadtree.notDivided) return;
		draw(*quadtree.ne);
		draw(*quadtree.nw);
		draw(*quadtree.se);
		draw(*quadtree.sw);

	}
	
	void drawPoints(QuadTree quadtree) {
		for (auto x : quadtree.points)
			DrawCircle(x.x, x.y, 2);
		if (quadtree.notDivided) return;
		drawPoints(*quadtree.ne);
		drawPoints(*quadtree.nw);
		drawPoints(*quadtree.se);
		drawPoints(*quadtree.sw);

	}
};


int main(void) {
	Application app;
	if (app.Construct(400, 400, 1, 1));
		app.Start();

	return 0;
}