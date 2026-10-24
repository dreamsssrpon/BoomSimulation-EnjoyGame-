#include "RayLibProjects.h"
#include <raylib.h>
#include <string>
#include <vector>

using namespace std;

class iV2 {
public:
	int x;
	int y;
	iV2() {};
	iV2(int x1, int y1) {
		x = x1;
		y = y1;
	}
	iV2 operator-(const iV2& v)const {
		return iV2(x - v.x, y - v.y);
	}
};
class V2 {
public:
	double x;
	double y;
	V2() {};
	V2(double x1,double y1) {
		x = x1;
		y = y1;
	}
	V2 operator-(const V2& v)const {
		return V2(x - v.x, y - v.y);
	}
	V2 operator+(const V2& v)const {
		return V2(x + v.x, y + v.y);
	}
	V2 operator+=(const V2& v) {
		x += v.x; y += v.y;
	}
	V2 operator*(const V2& v)const {
		return V2(x * v.x, y * v.y);
	}
};

Vector2 V2_to_rV2(V2 v) {//V2 to Vector2 
	return Vector2(v.x, v.y);
}

double dot(int x,int x1,int y,int y1) {
	return (x * x1 - y * y1);
}

class Body {
public:
	
	int height, widht;
	double Mass,PL;
	V2 pos,force;

	Body(){}
	Body(int w,int h,V2 p){
		pos = p;
		height = h;
		widht = w;

		PL = Mass = 1;
		force = V2(0, 0);
	}

	void update(double r) {
		pos += force * V2(GetFrameTime(), GetFrameTime());
		force = force * V2(r,r);
	}

	bool is_colaider(int x, int y) {

		Vector2 ps = V2_to_rV2(pos + V2(widht-height,widht-height));// Generate vertex pos
		
	}

	void Colision(int w,int h,int x,int y) {

	}

};

class Timer {
public:
	float life_time;
	float st;

	Timer(){}
	Timer(float x){
		life_time = x;
		start_time();
	}

	void start_time() {
		st = GetTime();
	}
	bool check() {
		if (st - GetTime() <= life_time)
			return true;
		return false;
	}

};
class Truster {
public:
	iV2 pos;
	double force,mass;
	double y_idle;

	int stepx;

	Truster(){}
	Truster(iV2 p2){
		pos = p2;

		force = 0;
		mass = 1.0;
		stepx = 4;

		y_idle = p2.y;
	}

	void update_pos() {

		pos.y += ceil(force)*GetFrameTime();
		force *= 0.9975;

		//cout << "Force " << ceil(force) << endl;
	}

	void update3(double R,Truster lpoint,Truster rpoint) {
		double y = ((rpoint.pos.y) + (lpoint.pos.y))/2;
		//force += (double(((y_idle - pos.y) + (y-pos.y)/2)*R)/mass);
		force += ((double((y-pos.y)+(y_idle - pos.y))*R)/mass);

		//DrawLineEx(Vector2(rpoint.pos.x, rpoint.pos.y+2), Vector2(pos.x, pos.y+2),2,GRAY);
		//DrawLineEx(Vector2(lpoint.pos.x, lpoint.pos.y+2), Vector2(pos.x, pos.y+2),2,GRAY);
	}

	void show() {
		Color c = RED;
		if (mass >= 100) {
			c = BLACK;
		}
		else if (mass >= 10) {
			c = GRAY;
		}
		DrawRectangle(pos.x, pos.y, stepx, stepx,c);
	}
};

class Truster4 {
public:
	float force,f1,y,mass;

	Truster4(){
		force = y = 0;
		mass = 1;
	}
	float update_force(Vector4 v, float speed) {
		float middle = (v.x + v.y + v.z + v.w ) / 4;

		float f = (speed * ((middle)-y) / mass);
		force += f;
		return f;
	}
	void update_force(float middle, float speed) {
		force = (speed * (middle + (-y))) / mass;
	}
	void update_pos() {
		force *= 0.99;
		f1 *= 0.99;
		y += (force * 0.01);
	}
	void setForce(float f) {
		f1 = f;
	}
};

float speed = 0.8;
vector<vector<Truster4>> height_map;

#define SCREEN_SIZE 800
float step_x,step_y;

float get_trusters(int x,int y) {
	if (x < 0 || x >= height_map.size()|| y < 0 || y >= height_map.size()) {
		return 0;
	}
	return height_map[x][y].y;
}
Vector4 GetHeihts(int x,int y) {
	Vector4 v;
	
	v.x = get_trusters(x - 1, y);
	v.y = get_trusters(x, y + 1);
	v.z = get_trusters(x + 1, y);
	v.w = get_trusters(x, y - 1);

	return v;
}
void draw(float y1,float m, Vector2 pos) {
	Color c(BLACK);
	
	float y = abs(y1);

	if (m > 1 && y < 2) {
		c = GRAY;
	}

	/*if (m < 1 && y<2) {
		c = GRAY;
	}
	else {
		c = RED;
	}

	if (m < 30) {
		c = ORANGE;
		c.r *= (y / 30);
		c.g = (c.r * 0.3);
	}
	else if (m < 55) {
		c.r *= (y / 55) * 0.7;
		c.g = (c.r * 0.5);
	}
	else if (m < 100) {
		c = YELLOW;

		c.r *= (y / 100);
		c.g *= (y / 100);
		c.b *= (y / 100);
	}*/
	
	c.r += (y);
	c.g += (y);
	c.b += (y );

	//c = BLUE;
	DrawRectangle(pos.x, pos.y, step_x, step_y, c);
}

void init_grid_trusters(int countx) {
	step_x = step_y = SCREEN_SIZE/countx;

	cout << "stepx " << step_x << " stepy " << step_y << endl;
	height_map = vector<vector<Truster4>>(countx, vector<Truster4>(countx, Truster4()));

	for (uint32_t x = 0;x < height_map.size();++x) {
		for (uint32_t y = 0; y < height_map[0].size();++y) {
			height_map[x][y].mass = (x >= height_map.size()/2 ? 2 : 1);
		}
	}
}

vector<vector<int>> heights(SCREEN_SIZE, vector<int>(SCREEN_SIZE, 0));

void update1(bool p) {
	for (uint32_t x = 0;x < height_map.size();++x) {
		for (uint32_t y = 0; y < height_map[0].size();++y) {
			if (!p) {
				height_map[x][y].update_force(GetHeihts(x, y), speed);
				height_map[x][y].update_pos();
			}
			//float k = heights[x][y];
			//if (abs(height_map[x][y].force) > abs(heights[x][y])) {
			//	k = height_map[x][y].force;
			//	heights[x][y] = k;
			//}

			draw(height_map[x][y].y,height_map[x][y].mass,Vector2((x * step_x), (y * step_y)));
			//draw(height_map[x][y].y, Vector2((x * step_x), (y * step_y)));
		}
	}
}

float boom_force = 20;

bool get_mouse_click_pos(Vector2 &b) {
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		b = GetMousePosition();
		return true;
	}
	return false;
}
void boom(int x,int y) {
	if (x < height_map.size() && x >= 0 &&y < height_map.size() && y >= 0) {
		height_map[x][y].y += boom_force;
		//height_map[x][y].setForce(boom_force);
	}
}
void create_boom() {
	Vector2 pos;
	if (!get_mouse_click_pos(pos)) {
		return;
	}

	pos.x = int(pos.x/step_x)%height_map.size();
	pos.y = int(pos.y/step_y)%height_map.size();

	//for (int i = 0; i < 10;++i) {
	//	boom(pos.x, pos.y-i);
	//}

	boom(pos.x, pos.y);
	//boom(pos.x - 1, pos.y);
	//boom(pos.x, pos.y + 1);
	//boom(pos.x + 1, pos.y);
	//boom(pos.x, pos.y - 1);

}

bool is_baking = false;
vector<vector<float>> force;
void update() {
	for (uint32_t x = 0;x < height_map.size();++x) {
		for (uint32_t y = 0; y < height_map[0].size();++y) {
			force[x][y] += height_map[x][y].update_force(GetHeihts(x, y), speed);
			height_map[x][y].update_pos();
		}
	}
}
void bake() {
	force = vector<vector<float>>(SCREEN_SIZE, vector<float>(SCREEN_SIZE, 0));
	size_t iter = 100,i =0;

	init_grid_trusters(800);

	for (uint32_t x = 0;x < 10;++x) {
		for (uint32_t y = 0; y < 10;++y) {
			cout << force[x][y] << " ";
		}
		cout << endl;
	}

	while (i <= iter) {
		update();
		++i;
	}

	for (uint32_t x = 0;x < 10;++x) {
		for (uint32_t y = 0; y < 10;++y) {
			cout << force[x][y] << " ";
		}
		cout << endl;
	}
}

void draw_bake() {
	for (uint32_t x = 0;x < force.size();++x) {
		for (uint32_t y = 0; y < force[0].size();++y) {
			//draw(force[x][y], Vector2((x * step_x), (y * step_y)));
		}
	}
}

bool pause = false;

class timer1 {
public:
	double st, interval;
	timer1() {}
	timer1(double i){
		interval = i;
		st = 0;
	}
	void start() {
		st = GetTime();
	}
	bool check() {
		double curr = GetTime();
		if (curr-st >= interval) {
			st = curr;
			return true;
		}return false;
	}
};

timer1 t(0.01);

int main()
{
	InitWindow(800, 800, "Truster simulation");
	SetTargetFPS(120);

	init_grid_trusters(200);

	string label = "Boom force: " + to_string(boom_force);

	//bake();

	//height_map[0][0].y += 500;
	height_map[0][0].y += 30000;

	t.start();

	while (!WindowShouldClose()) {
		BeginDrawing();
		
		ClearBackground(WHITE);

		create_boom();
		update1(pause);

		if (IsKeyPressed(KEY_E)&&t.check()) {
			pause = !pause;
		}
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
			init_grid_trusters(200);
		}

		label = "Boom force: " + to_string(boom_force)+(pause?" PAUSED ":" ");
		DrawText(label.data(), 0, 0, 30, WHITE);

		float sc = GetMouseWheelMove();
		boom_force += (sc * 20);

		EndDrawing();
	}

	return 0;
}
