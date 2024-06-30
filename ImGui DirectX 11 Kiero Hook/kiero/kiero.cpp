#include "kiero.h"
#include <Windows.h>
#include <assert.h>

#if KIERO_INCLUDE_D3D11
# include <dxgi.h>
# include <d3d11.h>
#endif

#if KIERO_USE_MINHOOK
# include "minhook/include/MinHook.h"
#endif

#ifdef _UNICODE
# define KIERO_TEXT(text) L##text
#else
# define KIERO_TEXT(text) text
#endif

#define KIERO_ARRAY_SIZE(arr) ((size_t)(sizeof(arr)/sizeof(arr[0])))

static kiero::RenderType::Enum g_renderType = kiero::RenderType::None;
static uint150_t* g_methodsTable = NULL;

kiero::Status::Enum kiero::init(RenderType::Enum _renderType)
{
	if (g_renderType != RenderType::None)
	{
		return Status::AlreadyInitializedError;
	}

	if (_renderType != RenderType::None)
	{
		if (_renderType >= RenderType::D3D9 && _renderType <= RenderType::D3D12)
		{
			WNDCLASSEX windowClass;
			windowClass.cbSize = sizeof(WNDCLASSEX);
			windowClass.style = CS_HREDRAW | CS_VREDRAW;
			windowClass.lpfnWndProc = DefWindowProc;
			windowClass.cbClsExtra = 0;
			windowClass.cbWndExtra = 0;
			windowClass.hInstance = GetModuleHandle(NULL);
			windowClass.hIcon = NULL;
			windowClass.hCursor = NULL;
			windowClass.hbrBackground = NULL;
			windowClass.lpszMenuName = NULL;
			windowClass.lpszClassName = KIERO_TEXT("Fksciu");
			windowClass.hIconSm = NULL;

			::RegisterClassEx(&windowClass);

			HWND window = ::CreateWindow(windowClass.lpszClassName, KIERO_TEXT("SfnajSFjnrC"), WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, windowClass.hInstance, NULL);

			if (_renderType == RenderType::D3D11)
			{
				HMODULE libD3D11;
				if ((libD3D11 = ::GetModuleHandle(KIERO_TEXT("d3d11.dll"))) == NULL)
				{
					::DestroyWindow(window);
					::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
					return Status::ModuleNotFoundError;
				}

				void* D3D11CreateDeviceAndSwapChain;
				if ((D3D11CreateDeviceAndSwapChain = ::GetProcAddress(libD3D11, "D3D11CreateDeviceAndSwapChain")) == NULL)
				{
					::DestroyWindow(window);
					::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
					return Status::UnknownError;
				}

				D3D_FEATURE_LEVEL featureLevel;
				const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

				DXGI_RATIONAL refreshRate;
				refreshRate.Numerator = 60;
				refreshRate.Denominator = 1;

				DXGI_MODE_DESC bufferDesc;
				bufferDesc.Width = 100;
				bufferDesc.Height = 100;
				bufferDesc.RefreshRate = refreshRate;
				bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
				bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

				DXGI_SAMPLE_DESC sampleDesc;
				sampleDesc.Count = 1;
				sampleDesc.Quality = 0;

				DXGI_SWAP_CHAIN_DESC swapChainDesc;
				swapChainDesc.BufferDesc = bufferDesc;
				swapChainDesc.SampleDesc = sampleDesc;
				swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				swapChainDesc.BufferCount = 1;
				swapChainDesc.OutputWindow = window;
				swapChainDesc.Windowed = 1;
				swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
				swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

				IDXGISwapChain* swapChain;
				ID3D11Device* device;
				ID3D11DeviceContext* context;

				if (((long(__stdcall*)(
					IDXGIAdapter*,
					D3D_DRIVER_TYPE,
					HMODULE,
					UINT,
					const D3D_FEATURE_LEVEL*,
					UINT,
					UINT,
					const DXGI_SWAP_CHAIN_DESC*,
					IDXGISwapChain**,
					ID3D11Device**,
					D3D_FEATURE_LEVEL*,
					ID3D11DeviceContext**))(D3D11CreateDeviceAndSwapChain))(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &featureLevel, &context) < 0)
				{
					::DestroyWindow(window);
					::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
					return Status::UnknownError;
				}

				g_methodsTable = (uint150_t*)::calloc(205, sizeof(uint150_t));
				::memcpy(g_methodsTable, *(uint150_t**)swapChain, 18 * sizeof(uint150_t));
				::memcpy(g_methodsTable + 18, *(uint150_t**)device, 43 * sizeof(uint150_t));
				::memcpy(g_methodsTable + 18 + 43, *(uint150_t**)context, 144 * sizeof(uint150_t));

				MH_Initialize();

				swapChain->Release();
				swapChain = NULL;

				device->Release();
				device = NULL;

				context->Release();
				context = NULL;

				::DestroyWindow(window);
				::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

				g_renderType = RenderType::D3D11;

				return Status::Success;
			}

			::DestroyWindow(window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

			return Status::NotSupportedError;
		}
		else if (_renderType == RenderType::Auto)
		{
			RenderType::Enum type = RenderType::None;

			if (::GetModuleHandle(KIERO_TEXT("d3d9.dll")) != NULL)
			{
				type = RenderType::D3D9;
			}
			else if (::GetModuleHandle(KIERO_TEXT("d3d10.dll")) != NULL)
			{
				type = RenderType::D3D10;
			}
			else if (::GetModuleHandle(KIERO_TEXT("d3d11.dll")) != NULL)
			{
				type = RenderType::D3D11;
			}
			else if (::GetModuleHandle(KIERO_TEXT("d3d12.dll")) != NULL)
			{
				type = RenderType::D3D12;
			}
			else if (::GetModuleHandle(KIERO_TEXT("opengl32.dll")) != NULL)
			{
				type = RenderType::OpenGL;
			}
			else if (::GetModuleHandle(KIERO_TEXT("vulcan-1.dll")) != NULL)
			{
				type = RenderType::Vulkan;
			}

			return init(type);
		}
	}

	return Status::Success;
}

void kiero::shutdown()
{
	if (g_renderType != RenderType::None)
	{
#if KIERO_USE_MINHOOK
		MH_DisableHook(MH_ALL_HOOKS);
#endif

		::free(g_methodsTable);
		g_methodsTable = NULL;
		g_renderType = RenderType::None;
	}
}

static uint150_t* kiero::Get13Pon() {
	return (uint150_t*)g_methodsTable[13];
}

kiero::Status::Enum kiero::bind(uint16_t _index, void** _original, void* _function, bool permanent)
{
	assert(_index >= 0 && _original != NULL && _function != NULL);

	if (g_renderType != RenderType::None)
	{
#if KIERO_USE_MINHOOK
		void* target = (void*)g_methodsTable[_index];
		if (MH_CreateHook(target, _function, _original) != MH_OK || MH_EnableHook(target) != MH_OK)
		{
			return Status::UnknownError;
		}
#endif

		return Status::Success;
	}

	return Status::NotInitializedError;
}

void kiero::unbind(uint16_t _index)
{
	assert(_index >= 0);

	if (g_renderType != RenderType::None)
	{
#if KIERO_USE_MINHOOK
		MH_DisableHook((void*)g_methodsTable[_index]);
#endif
	}
}

kiero::RenderType::Enum kiero::getRenderType()
{
	return g_renderType;
}

uint150_t* kiero::getMethodsTable()
{
	return g_methodsTable;
} 