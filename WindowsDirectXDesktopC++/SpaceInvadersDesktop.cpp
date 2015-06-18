// include the basic windows header files and the Direct3D header files
#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include "Sprite.h"
#include <algorithm>

using namespace SpaceInvaders;
using namespace DirectX;
using namespace Windows::Foundation;

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

// define the screen resolution
#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

// global declarations
IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
ID3D11Device *dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
ID3D11RenderTargetView *backbuffer;    // the pointer to our back buffer
ID3D11InputLayout *pLayout;            // the pointer to the input layout
ID3D11VertexShader *pVS;               // the pointer to the vertex shader
ID3D11PixelShader *pPS;                // the pointer to the pixel shader
ID3D11Buffer *pVBuffer;                // the pointer to the vertex buffer

std::unique_ptr<DirectX::CommonStates> states;
std::shared_ptr<DirectX::SpriteBatch> spriteBatch;
float time = 0;
const float timeDelta = 1.0f / 300.0f;
float lastTimeShot;
std::shared_ptr<Sprite> background;
std::shared_ptr<Sprite> ship;
float shipPosition = 0;
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> missileTexture;
std::vector<std::shared_ptr<Sprite>> activeMissiles;
std::vector<std::shared_ptr<Sprite>> enemies;
float enemiesPosition = 0;
bool enemiesMovingRight = true;

// a struct to define a single vertex
struct VERTEX{ FLOAT X, Y, Z; D3DXCOLOR Color; };

// function prototypes
void InitD3D(HWND hWnd);    // sets up and initializes Direct3D
void RenderFrame(void);     // renders a single frame
void CleanD3D(void);        // closes Direct3D and releases memory
void InitGraphics(void);    // creates the shape to render
void InitPipeline(void);    // loads and prepares the shaders
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL,
		L"WindowClass",
		L"Our First Direct3D Program",
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	InitD3D(hWnd);

	// enter the main loop:

	MSG msg;

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}

		RenderFrame();
	}

	// clean up DirectX and COM
	CleanD3D();

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			shipPosition -= timeDelta * 30;
			break;

		case VK_RIGHT:
			shipPosition += timeDelta * 30;
			break;
		}
		break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void CreateBackground()
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> backgroundTexture;
	CreateWICTextureFromFile(dev, L"Background.png", nullptr, backgroundTexture.ReleaseAndGetAddressOf());
	background = std::make_shared<Sprite>(backgroundTexture, 0.0f, 0.0f, 2.0f, 1.0f);
}

void CreateShip()
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shipTexture;
	CreateWICTextureFromFile(dev, L"Ship.png", nullptr, shipTexture.ReleaseAndGetAddressOf());
	ship = std::make_shared<Sprite>(shipTexture, 0.0f, -0.8f, 0.095f, 0.095f);
	CreateWICTextureFromFile(dev, L"Missile.png", nullptr, missileTexture.ReleaseAndGetAddressOf());
}

void CreateEnemies()
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> enemyTexture;
	CreateWICTextureFromFile(dev, L"Enemy.png", nullptr, enemyTexture.ReleaseAndGetAddressOf());
	for (float y = 0.7f; y > 0.3f; y -= 0.3f)
		for (float x = -0.8f; x <= 0.8f; x += 0.2f)
			enemies.push_back(std::make_shared<Sprite>(enemyTexture, x, y, 0.1f, 0.1f));
}

void InitD3D(HWND hWnd)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                   // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
	scd.BufferDesc.Width = SCREEN_WIDTH;                   // set the back buffer width
	scd.BufferDesc.Height = SCREEN_HEIGHT;                 // set the back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
	scd.OutputWindow = hWnd;                               // the window to be used
	scd.SampleDesc.Count = 4;                              // how many multisamples
	scd.Windowed = TRUE;                                   // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon);


	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	devcon->OMSetRenderTargets(1, &backbuffer, NULL);

	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;

	devcon->RSSetViewports(1, &viewport);

	InitPipeline();
	InitGraphics();
	states.reset(new CommonStates(dev));
	spriteBatch.reset(new SpriteBatch(devcon));
	CreateBackground();
	CreateShip();
	CreateEnemies();
}

void FireMissile()
{
	lastTimeShot = time;
	activeMissiles.push_back(std::make_shared<Sprite>(missileTexture, shipPosition, -0.67f, 0.02f, 0.02f));
}

bool CollidingWithEnemyAndKilledIt(std::shared_ptr<Sprite> missile)
{
	for (auto enemy : enemies)
		if (enemy->DistanceTo(missile, enemiesPosition) < 0.05f)
		{
			enemy->IncreaseY(1.0f); // Move out of screen
			return true;
		}
	return false;
}

void HandleMissiles()
{
	activeMissiles.erase(std::remove_if(activeMissiles.begin(), activeMissiles.end(),
		[=](std::shared_ptr<Sprite> missile)
	{
		return missile->IncreaseY(timeDelta * 3) || CollidingWithEnemyAndKilledIt(missile);
	}), activeMissiles.end());
}

void ControlShip()
{
	if (lastTimeShot < time - 0.33f)
		FireMissile();
	HandleMissiles();
}

void MoveEnemies()
{
	enemiesPosition += (enemiesMovingRight ? 0.2f : -0.2f) * timeDelta;
	if (enemiesPosition < -0.1f || enemiesPosition > 0.1f)
		enemiesMovingRight = !enemiesMovingRight;
}

void Update()
{
	time += timeDelta;
	ControlShip();
	MoveEnemies();
}

void RenderFrame(void)
{
	Update();

	// clear the back buffer to a deep blue
	devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied());
	background->Draw(spriteBatch, SCREEN_WIDTH, SCREEN_HEIGHT);
	for (auto missile : activeMissiles)
		missile->Draw(spriteBatch, SCREEN_WIDTH, SCREEN_HEIGHT);
	ship->Draw(spriteBatch, SCREEN_WIDTH, SCREEN_HEIGHT, shipPosition);
	for (auto enemy : enemies)
		enemy->Draw(spriteBatch, SCREEN_WIDTH, SCREEN_HEIGHT, enemiesPosition);
	spriteBatch->End();

	// switch the back buffer and the front buffer
	swapchain->Present(0, 0);
}

void CleanD3D(void)
{
	swapchain->SetFullscreenState(FALSE, NULL);
	spriteBatch.reset();
	// close and release all existing COM objects
	pLayout->Release();
	pVS->Release();
	pPS->Release();
	pVBuffer->Release();
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
}

void InitGraphics()
{
	// create a triangle using the VERTEX struct
	VERTEX OurVertices[] =
	{
		{ 0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) },
		{ 0.45f, -0.5, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) },
		{ -0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) }
	};


	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(VERTEX) * 3;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	dev->CreateBuffer(&bd, NULL, &pVBuffer);       // create the buffer


	// copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
	devcon->Unmap(pVBuffer, NULL);                                      // unmap the buffer
}

void InitPipeline()
{
	// load and compile the two shaders
	ID3D10Blob *VS, *PS;
	D3DX11CompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

	// encapsulate both shaders into shader objects
	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	// set the shader objects
	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	devcon->IASetInputLayout(pLayout);
}