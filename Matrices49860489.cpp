// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>

// define the screen resolution and keyboard macros
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#define ENEMY_NUM 5
#define ENEMY2_NUM 4
#define ENEMY3_NUM 3
#define ENEMY4_NUM 2
#define ENEMY5_NUM 1

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "winmm")
// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
LPD3DXSPRITE d3dspt;    // the pointer to our Direct3D Sprite interface



						// sprite declarations
LPDIRECT3DTEXTURE9 sprite;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_hero;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_enemy;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_bullet;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_heart;
LPDIRECT3DTEXTURE9 sprite_heart1;
LPDIRECT3DTEXTURE9 sprite_heart2;
LPDIRECT3DTEXTURE9 sprite_gameover;
LPDIRECT3DTEXTURE9 sprite_clear;
LPDIRECT3DTEXTURE9 sprite_enemy2;
LPDIRECT3DTEXTURE9 sprite_enemy3;
LPDIRECT3DTEXTURE9 sprite_enemy4;
LPDIRECT3DTEXTURE9 sprite_enemy5;
LPDIRECT3DTEXTURE9 sprite_round;
LPDIRECT3DTEXTURE9 sprite_round1;
LPDIRECT3DTEXTURE9 sprite_round2;
LPDIRECT3DTEXTURE9 sprite_round3;
LPDIRECT3DTEXTURE9 sprite_round4;
LPDIRECT3DTEXTURE9 sprite_round5;
									 // function prototypes
void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame(void);    // renders a single frame
void cleanD3D(void);		// closes Direct3D and releases memory

void init_game(void);
void do_game_logic(void);
bool sphere_collision_check(float x0, float y0, float size0, float x1, float y1, float size1);


// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

using namespace std;


enum { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };


//기본 클래스 
class entity {

public:
	float x_pos;
	float y_pos;
	int status;
	int HP;
	int score;
};


bool sphere_collision_check(float x0, float y0, float size0, float x1, float y1, float size1)
{

	if ((x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1) < (size0 + size1) * (size0 + size1))
		return true;
	else
		return false;

}



//주인공 클래스 
class Hero :public entity {

public:
	Hero() { HP = 3; }
	void fire() { ; }
	void super_fire() { ; }
	void move(int i);
	void kill();
	void init(float x, float y);
	bool check_collision(float x, float y);

};

void Hero::init(float x, float y)
{

	x_pos = x;
	y_pos = y;

}

bool Hero::check_collision(float x, float y)
{
	if (sphere_collision_check(x_pos, y_pos, 32, x, y, 32) == true)
	{
		return true;

	}
	else {

		return false;
	}
}

void Hero::move(int i)
{
	switch (i)
	{
	case MOVE_UP:
		y_pos -= 3;
		break;

	case MOVE_DOWN:
		y_pos += 3;
		break;


	case MOVE_LEFT:
		x_pos -= 3;
		break;


	case MOVE_RIGHT:
		x_pos += 3;
		break;

	}

}

void Hero::kill()
{
	score++;
}




// 적 클래스 
class Enemy :public entity {
	
public:
	void fire() { ; }
	void init(float x, float y);
	void move(int s);
};

void Enemy::init(float x, float y)
{

	x_pos = x;
	y_pos = y;

}


void Enemy::move(int s)
{
	y_pos += s;

}






// 총알 클래스 
class Bullet :public entity {

public:
	bool bShow;

	void init(float x, float y);
	void move();
	bool show();
	void hide();
	void active();
	bool check_collision(float x, float y);


};


bool Bullet::check_collision(float x, float y)
{

	//충돌 처리 시 
	if (sphere_collision_check(x_pos, y_pos, 32, x, y, 32) == true)
	{
		bShow = false;
		return true;

	}
	else {

		return false;
	}
}




void Bullet::init(float x, float y)
{
	x_pos = x;
	y_pos = y;

}



bool Bullet::show()
{
	return bShow;

}


void Bullet::active()
{
	bShow = true;

}



void Bullet::move()
{
	y_pos -= 8;
}

void Bullet::hide()
{
	bShow = false;

}






//객체 생성 
Hero hero;
Enemy enemy[ENEMY_NUM];
Enemy enemy2[ENEMY2_NUM];
Enemy enemy3[ENEMY3_NUM];
Enemy enemy4[ENEMY4_NUM];
Enemy enemy5[ENEMY5_NUM];
Bullet bullet;



// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	PlaySound(L"BGM.wav", NULL, SND_ASYNC | SND_LOOP);
	
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL, L"WindowClass", L"Our Direct3D Program",
		WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	initD3D(hWnd);


	//게임 오브젝트 초기화 
	init_game();

	// enter the main loop:

	MSG msg;

	while (TRUE)
	{
		DWORD starting_point = GetTickCount();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		do_game_logic();

		render_frame();

		// check the 'escape' key
		if (KEY_DOWN(VK_ESCAPE))
			PostMessage(hWnd, WM_DESTROY, 0, 0);




		while ((GetTickCount() - starting_point) < 25);
	}

	// clean up DirectX and COM
	cleanD3D();

	return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;


	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	D3DXCreateSprite(d3ddev, &d3dspt);    // create the Direct3D Sprite object

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"Panel3.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite);    // load to sprite


	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"hero.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_hero);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"enemy.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_enemy);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"enemy2.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_enemy2);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"enemy3.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_enemy3);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"enemy4.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_enemy4);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"enemy5.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_enemy5);    // load to sprite


	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"bullet.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_bullet);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"heart.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_heart);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"heart1.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_heart1);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"heart2.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_heart2);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"gameover.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_gameover);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"clear.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_clear);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"round.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_round);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"round1.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_round1);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"round2.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_round2);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"round3.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_round3);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"round4.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_round4);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"round5.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_round5);    // load to sprite

	return;
}


void init_game(void)
{
	//객체 초기화 
	hero.init(150, 300);

	//적들 초기화 
	for (int i = 0; i<ENEMY_NUM; i++)
	{
		enemy[i].HP = 2;
		enemy[i].init((float)(rand() % 300), rand() % 200 - 300);
	}
	if (hero.score >= 10) {
		for (int i = 0; i < ENEMY2_NUM; i++)
		{
			enemy2[i].HP = 3;
			enemy2[i].init((float)(rand() % 300), rand() % 200 - 300);
		}
	}
	if (hero.score >= 20) {
		for (int i = 0; i < ENEMY3_NUM; i++)
		{
			enemy3[i].HP = 3;
			enemy3[i].init((float)(rand() % 300), rand() % 200 - 300);
		}
	}
	if (hero.score >= 30) {
		for (int i = 0; i < ENEMY4_NUM; i++)
		{
			enemy4[i].HP = 3;
			enemy4[i].init((float)(rand() % 300), rand() % 200 - 300);
		}
	}
	if (hero.score >= 40) {
		for (int i = 0; i < ENEMY5_NUM; i++)
		{
			enemy5[i].HP = 3;
			enemy5[i].init((float)(rand() % 300), rand() % 200 - 300);
		}
	}

	//총알 초기화 
	bullet.init(hero.x_pos, (hero.y_pos-35.0f));

}


void do_game_logic(void)
{

	//주인공 처리 
	if (KEY_DOWN(VK_UP))
		hero.move(MOVE_UP);

	if (KEY_DOWN(VK_DOWN))
		hero.move(MOVE_DOWN);

	if (KEY_DOWN(VK_LEFT))
		hero.move(MOVE_LEFT);

	if (KEY_DOWN(VK_RIGHT))
		hero.move(MOVE_RIGHT);

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (hero.check_collision(enemy[i].x_pos, enemy[i].y_pos) == true)
		{
			hero.HP--;
			enemy[i].init((float)(rand() % 300), rand() % 200 - 300);
		}
	}
	for (int i = 0; i < ENEMY2_NUM; i++)
	{
		if (hero.check_collision(enemy2[i].x_pos, enemy2[i].y_pos) == true)
		{
			hero.HP--;
			enemy2[i].init((float)(rand() % 300), rand() % 200 - 300);
		}
	}
	for (int i = 0; i < ENEMY3_NUM; i++)
	{
		if (hero.check_collision(enemy3[i].x_pos, enemy3[i].y_pos) == true)
		{
			hero.HP--;
			enemy3[i].init((float)(rand() % 300), rand() % 200 - 300);
		}
	}
	for (int i = 0; i < ENEMY4_NUM; i++)
	{
		if (hero.check_collision(enemy4[i].x_pos, enemy4[i].y_pos) == true)
		{
			hero.HP--;
			enemy4[i].init((float)(rand() % 300), rand() % 200 - 300);
		}
	}
	for (int i = 0; i < ENEMY5_NUM; i++)
	{
		if (hero.check_collision(enemy5[i].x_pos, enemy5[i].y_pos) == true)
		{
			hero.HP--;
			enemy5[i].init((float)(rand() % 300), rand() % 200 - 300);
		}
	}

	//적들 처리 
	for (int i = 0; i<ENEMY_NUM; i++)
	{
		if (enemy[i].y_pos > 500)
			enemy[i].init((float)(rand() % 300), rand() % 200 - 300);
		else
			enemy[i].move(1);
	}
	if (hero.score >= 10) {
		for (int i = 0; i < ENEMY2_NUM; i++)
		{
			if (enemy2[i].y_pos > 500)
				enemy2[i].init((float)(rand() % 300), rand() % 200 - 300);
			else
				enemy2[i].move(2);
		}
	}
	if (hero.score >= 20) {
		for (int i = 0; i < ENEMY3_NUM; i++)
		{
			if (enemy3[i].y_pos > 500)
				enemy3[i].init((float)(rand() % 300), rand() % 200 - 300);
			else
				enemy3[i].move(3);
		}
	}
	if (hero.score >= 30) {
		for (int i = 0; i < ENEMY4_NUM; i++)
		{
			if (enemy4[i].y_pos > 500)
				enemy4[i].init((float)(rand() % 300), rand() % 200 - 300);
			else
				enemy4[i].move(4);
		}
	}
	if (hero.score >= 40) {
		for (int i = 0; i < ENEMY5_NUM; i++)
		{
			if (enemy5[i].y_pos > 500)
				enemy5[i].init((float)(rand() % 300), rand() % 200 - 300);
			else
				enemy5[i].move(5);
		}
	}
	//총알 처리 
	if (bullet.show() == false)
	{
		if (KEY_DOWN(VK_SPACE))
		{
			
			PlaySound(L"Bullet.wav", NULL, SND_ASYNC);
			bullet.active();
			bullet.init(hero.x_pos, (hero.y_pos-35.0f));
		}
	}


	if (bullet.show() == true)
	{
		if (bullet.y_pos < -70)
			bullet.hide();
		else
			bullet.move();


		//충돌 처리 
		for (int i = 0; i<ENEMY_NUM; i++)
		{
			if (bullet.check_collision(enemy[i].x_pos, enemy[i].y_pos) == true)
			{
				
				enemy[i].HP--;
				if (enemy[i].HP <= 0)
				{

					enemy[i].init((float)(rand() % 300), rand() % 200 - 300);
					enemy[i].HP = 2;
					hero.kill();
				}
			}
		}
		for (int i = 0; i<ENEMY2_NUM; i++)
		{
			if (bullet.check_collision(enemy2[i].x_pos, enemy2[i].y_pos) == true)
			{

				enemy2[i].HP--;
				if (enemy2[i].HP <= 0)
				{

					enemy2[i].init((float)(rand() % 300), rand() % 200 - 300);
					enemy2[i].HP = 3;
					hero.kill();
				}
			}
		}
		for (int i = 0; i<ENEMY3_NUM; i++)
		{
			if (bullet.check_collision(enemy3[i].x_pos, enemy3[i].y_pos) == true)
			{

				enemy3[i].HP--;
				if (enemy3[i].HP <= 0)
				{

					enemy3[i].init((float)(rand() % 300), rand() % 200 - 300);
					enemy3[i].HP = 3;
					hero.kill();
				}
			}
		}
		for (int i = 0; i<ENEMY4_NUM; i++)
		{
			if (bullet.check_collision(enemy4[i].x_pos, enemy4[i].y_pos) == true)
			{

				enemy4[i].HP--;
				if (enemy4[i].HP <= 0)
				{

					enemy4[i].init((float)(rand() % 300), rand() % 200 - 300);
					enemy4[i].HP = 3;
					hero.kill();
				}
			}
		}
		for (int i = 0; i<ENEMY5_NUM; i++)
		{
			if (bullet.check_collision(enemy5[i].x_pos, enemy5[i].y_pos) == true)
			{

				enemy5[i].HP--;
				if (enemy5[i].HP <= 0)
				{

					enemy5[i].init((float)(rand() % 300), rand() % 200 - 300);
					enemy5[i].HP = 3;
					hero.kill();
				}
			}
		}
	}

}
// this is the function used to render a single frame
void render_frame(void)
{

	// clear the window to a deep blue
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene();    // begins the 3D scene

	d3dspt->Begin(D3DXSPRITE_ALPHABLEND);    // // begin sprite drawing with transparency

											 //UI 창 렌더링 


											 /*
											 static int frame = 21;    // start the program on the final frame
											 if(KEY_DOWN(VK_SPACE)) frame=0;     // when the space key is pressed, start at frame 0
											 if(frame < 21) frame++;     // if we aren't on the last frame, go to the next frame

											 // calculate the x-position
											 int xpos = frame * 182 + 1;

											 RECT part;
											 SetRect(&part, xpos, 0, xpos + 181, 128);
											 D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
											 D3DXVECTOR3 position(150.0f, 50.0f, 0.0f);    // position at 50, 50 with no depth
											 d3dspt->Draw(sprite, &part, &center, &position, D3DCOLOR_ARGB(127, 255, 255, 255));
											 */

	//주인공 
	RECT part;
	SetRect(&part, 0, 0, 64, 64);
	D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 position(hero.x_pos, hero.y_pos, 0.0f);    // position at 50, 50 with no depth
	d3dspt->Draw(sprite_hero, &part, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));

	////총알 
	if (bullet.bShow == true)
	{
		RECT part1;
		SetRect(&part1, 0, 0, 64, 64);
		D3DXVECTOR3 center1(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position1(bullet.x_pos, bullet.y_pos, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(sprite_bullet, &part1, &center1, &position1, D3DCOLOR_ARGB(255, 255, 255, 255));
	}


	////에네미 
	RECT part2;
	SetRect(&part2, 0, 0, 64, 64);
	D3DXVECTOR3 center2(0.0f, 0.0f, 0.0f);    // center at the upper-left corner

	for (int i = 0; i<ENEMY_NUM; i++)
	{

		D3DXVECTOR3 position2(enemy[i].x_pos, enemy[i].y_pos, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(sprite_enemy, &part2, &center2, &position2, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (hero.score >= 10) {
		RECT part8;
		SetRect(&part8, 0, 0, 64, 64);
		D3DXVECTOR3 center8(0.0f, 0.0f, 0.0f);    // center at the upper-left corner

		for (int i = 0; i < ENEMY2_NUM; i++)
		{

			D3DXVECTOR3 position8(enemy2[i].x_pos, enemy2[i].y_pos, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(sprite_enemy2, &part8, &center8, &position8, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
	if (hero.score >= 20) {
		RECT part8;
		SetRect(&part8, 0, 0, 64, 64);
		D3DXVECTOR3 center8(0.0f, 0.0f, 0.0f);    // center at the upper-left corner

		for (int i = 0; i < ENEMY3_NUM; i++)
		{

			D3DXVECTOR3 position8(enemy3[i].x_pos, enemy3[i].y_pos, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(sprite_enemy3, &part8, &center8, &position8, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
	if (hero.score >= 30) {
		RECT part8;
		SetRect(&part8, 0, 0, 64, 64);
		D3DXVECTOR3 center8(0.0f, 0.0f, 0.0f);    // center at the upper-left corner

		for (int i = 0; i < ENEMY4_NUM; i++)
		{

			D3DXVECTOR3 position8(enemy4[i].x_pos, enemy4[i].y_pos, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(sprite_enemy4, &part8, &center8, &position8, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
	if (hero.score >= 40) {
		RECT part8;
		SetRect(&part8, 0, 0, 64, 64);
		D3DXVECTOR3 center8(0.0f, 0.0f, 0.0f);    // center at the upper-left corner

		for (int i = 0; i < ENEMY5_NUM; i++)
		{

			D3DXVECTOR3 position8(enemy5[i].x_pos, enemy5[i].y_pos, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(sprite_enemy5, &part8, &center8, &position8, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
	RECT part9;
	SetRect(&part9, 0, 0, 250, 64);
	D3DXVECTOR3 center9(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 position9(350.0f, 50.0f, 0.0f);    // position at 50, 50 with no depth
	d3dspt->Draw(sprite_round, &part9, &center9, &position9, D3DCOLOR_ARGB(255, 255, 255, 255));

	//라운드
		if (hero.score >= 0&&hero.score<10)
		{
			RECT part10;
			SetRect(&part10, 0, 0, 64, 64);
			D3DXVECTOR3 center10(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
			D3DXVECTOR3 position10(440.0f, 100.0f, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(sprite_round1, &part10, &center10, &position10, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		if (hero.score >= 10 && hero.score<20)
		{
			RECT part11;
			SetRect(&part11, 0, 0, 64, 64);
			D3DXVECTOR3 center11(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
			D3DXVECTOR3 position11(440.0f, 100.0f, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(sprite_round2, &part11, &center11, &position11, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		if (hero.score >= 20 && hero.score<30)
		{
			RECT part12;
			SetRect(&part12, 0, 0, 64, 64);
			D3DXVECTOR3 center12(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
			D3DXVECTOR3 position12(440.0f, 100.0f, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(sprite_round3, &part12, &center12, &position12, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		if (hero.score >= 30 && hero.score<40)
		{
			RECT part13;
			SetRect(&part13, 0, 0, 64, 64);
			D3DXVECTOR3 center13(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
			D3DXVECTOR3 position13(440.0f, 100.0f, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(sprite_round4, &part13, &center13, &position13, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		if (hero.score >= 40)
		{
			RECT part14;
			SetRect(&part14, 0, 0, 64, 64);
			D3DXVECTOR3 center14(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
			D3DXVECTOR3 position14(440.0f, 100.0f, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(sprite_round5, &part14, &center14, &position14, D3DCOLOR_ARGB(255, 255, 255, 255));
		}


	//라이프
		if (hero.HP >= 3)
		{
			RECT part3;
			SetRect(&part3, 0, 0, 64, 64);
			D3DXVECTOR3 center3(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
			D3DXVECTOR3 position3(500.0f, 200.0f, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(sprite_heart, &part3, &center3, &position3, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		if (hero.HP >= 2)
		{
			RECT part4;
			SetRect(&part4, 0, 0, 64, 64);
			D3DXVECTOR3 center4(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
			D3DXVECTOR3 position4(450.0f, 200.0f, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(sprite_heart1, &part4, &center4, &position4, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		if (hero.HP >= 1) 
		{
			RECT part5;
			SetRect(&part5, 0, 0, 64, 64);
			D3DXVECTOR3 center5(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
			D3DXVECTOR3 position5(400.0f, 200.0f, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(sprite_heart2, &part5, &center5, &position5, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		
		if (hero.HP < 1)
		{
			RECT part6;
			SetRect(&part6, 0, 0, 640, 480);
			D3DXVECTOR3 center6(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
			D3DXVECTOR3 position6(0.0f, 0.0f, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(sprite_gameover, &part6, &center6, &position6, D3DCOLOR_ARGB(255, 255, 255, 255));

		}
		
		if (hero.score >= 50)
		{
			RECT part7;
			SetRect(&part7, 0, 0, 640, 480);
			D3DXVECTOR3 center7(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
			D3DXVECTOR3 position7(0.0f, 0.0f, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(sprite_clear, &part7, &center7, &position7, D3DCOLOR_ARGB(255, 255, 255, 255));

		}


		
	d3dspt->End();    // end sprite drawing

	d3ddev->EndScene();    // ends the 3D scene

	d3ddev->Present(NULL, NULL, NULL, NULL);

	return;
}


// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	sprite->Release();
	d3ddev->Release();
	d3d->Release();

	//객체 해제 
	sprite_hero->Release();
	sprite_enemy->Release();
	sprite_bullet->Release();
	sprite_heart->Release();
	sprite_heart1->Release();
	sprite_heart2->Release();
	sprite_gameover->Release();
	return;
}
