#include "DirectXModule.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <codecvt>
#include <locale>

#define SAFE_RELEASE(p){if(p){(p)->Release();(p)=NULL;}}
#define PI 3.1415926535

extern HWND hWnd;
namespace dxmod {
	ID2D1RenderTarget* d2dRenderTarget;
	IDXGISwapChain* dxgiSwap;

	IWICImagingFactory* p_WICFac;

	IDWriteFactory* p_DWFac;
	IDWriteFontCollection* p_DWFC;
	IDWriteFontFileLoader* p_DWFFL;
	IDWriteFontFile* p_DWFF;
	IDWriteFontCollectionLoader* p_DWFCL;
	IDWriteFontFileEnumerator* p_DWFFE;

	LPDIRECTSOUND8 DSound;

	ID2D1Layer* p_d2dLayer;
	ID2D1Layer* p_GraphLayer;

	std::vector<ID2D1Bitmap*> p_BitmapVector;
	std::vector<IDWriteTextFormat*> p_FontVector;
	std::vector<LPDIRECTSOUNDBUFFER> p_SoundVector;
	std::vector<ID2D1Brush*> p_BrushVector;
	std::vector<ID2D1StrokeStyle*> p_StrokeStyleVector;
	std::vector<LTimer> TimePointVector;

	uint32_t nowTime;
	uint32_t nextTime;
	static uint32_t OTime;
	static uint32_t NTime;
	static DWORD fontcount;
	int VSync;

	int debugint = 0;

	int CheckBitmapID(int BitmapID) {
		if (p_BitmapVector.size() <= BitmapID)
			return 0;
		return BitmapID;
	}

	int CheckFontID(int FontID) {
		if (p_FontVector.size() <= FontID)
			return 0;
		return FontID;
	}

	int CheckBrushID(int BrushID) {
		if (p_BrushVector.size() <= BrushID)
			return 0;
		return BrushID;
	}

	int CheckSoundID(int SoundID) {
		if (p_SoundVector.size() <= SoundID)
			return 0;
		return SoundID;
	}

	int CheckStrokeStyleID(int StrokeStyleID) {
		if (p_StrokeStyleVector.size() <= StrokeStyleID)
			return 0;
		return StrokeStyleID;
	}

	int CheckTimerID(int TimerID) {
		if (TimePointVector.size() <= TimerID)
			return 0;
		return TimerID;
	}

	void dx_end() {
		all_dxdevice_Release();
		SAFE_RELEASE(d2dRenderTarget);
		SAFE_RELEASE(dxgiSwap);
	}

	void set_VSync() {
		VSync = 1;
	}

	void reset_VSync() {
		VSync = 0;
	}

	int set_time_point() {
		int i;
		bool sizemax;
		sizemax = true;
		for (i = 0; i < TimePointVector.size(); i++) {
			if (TimePointVector[i].set == false) {
				sizemax = false;
				break;
			}
		}
		if (sizemax) {
			i = TimePointVector.size();
			TimePointVector.push_back(LTimer());
		}
		TimePointVector[i].SetTimer = timeGetTime();
		return i;
	}

	unsigned int get_timer(int TimerID) {
		TimerID = CheckTimerID(TimerID);
		TimePointVector[TimerID].AddTimer = timeGetTime();
		if (TimePointVector[TimerID].AddTimer >= TimePointVector[TimerID].SetTimer)
			return TimePointVector[TimerID].AddTimer - TimePointVector[TimerID].SetTimer;
		return TimePointVector[TimerID].AddTimer + (4294967295 - TimePointVector[TimerID].SetTimer);
	}

	void reset_timer(int TimerID) {
		TimePointVector[TimerID].set = false;
	}

	int dx_init() {
		ID3D10Device1* p_d3dDev = nullptr;
		IDXGISurface* p_d3dTex = nullptr;
		IDXGIDevice* p_DGIDev = nullptr;
		IDXGIFactory* p_DGIFac = nullptr;
		IDXGIAdapter* p_DGIAdp = nullptr;

		ID2D1Factory* p_d2dFac = nullptr;

		d2dRenderTarget = nullptr;
		dxgiSwap = nullptr;

		FLOAT dpi;

		//DirectSound8 Init
		if (FAILED(DirectSoundCreate8(NULL, &DSound, NULL))) {
			MessageBox(hWnd, TEXT("Create_DirectSound8_field"), TEXT("Error"), MB_ICONSTOP);
			return 1;
		}
		if (FAILED(DSound->SetCooperativeLevel(hWnd, DSSCL_EXCLUSIVE | DSSCL_PRIORITY))) {
			MessageBox(hWnd, TEXT("Set_cooperative_level_field"), TEXT("Error"), MB_ICONSTOP);
			return 1;
		}
		//WIC Init
		if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&p_WICFac)))) {
			MessageBox(hWnd, TEXT("Create_WICFac_field"), TEXT("Error"), MB_ICONSTOP);
			return 1;
		}
		//D2D Init
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &p_d2dFac))) {
			MessageBox(hWnd, TEXT("Create_D2DFac_field"), TEXT("Error"), MB_ICONSTOP);
			return 1;
		}
		//p_d2dFac->GetDesktopDpi(&dpix, &dpiy);
		dpi = (FLOAT)GetDpiForWindow(hWnd);
		//D3D Init
		if (FAILED(D3D10CreateDevice1(p_DGIAdp, D3D10_DRIVER_TYPE_HARDWARE, NULL, D3D10_CREATE_DEVICE_BGRA_SUPPORT, D3D10_FEATURE_LEVEL_9_3, D3D10_1_SDK_VERSION, &p_d3dDev))) {
			MessageBox(hWnd, TEXT("Create_D3DDev_field"), TEXT("Error"), MB_ICONSTOP);
			return 1;
		}
		if (FAILED(p_d3dDev->QueryInterface(&p_DGIDev))) {
			MessageBox(hWnd, TEXT("DXGIDev_QI_field"), TEXT("Error"), MB_ICONSTOP);
			return 1;
		}
		if (FAILED(p_DGIDev->GetAdapter(&p_DGIAdp))) {
			MessageBox(hWnd, TEXT("DXGI_GetAdapter_field"), TEXT("Error"), MB_ICONSTOP);
			return 1;
		}
		if (FAILED(p_DGIAdp->GetParent(IID_PPV_ARGS(&p_DGIFac)))) {
			MessageBox(hWnd, TEXT("DXGI_GetParent_field"), TEXT("Error"), MB_ICONSTOP);
			return 1;
		}
		DXGI_SWAP_CHAIN_DESC SD;
		::ZeroMemory(&SD, sizeof(SD));
		SD.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		SD.SampleDesc.Count = 1;
		SD.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SD.BufferCount = 1;
		SD.OutputWindow = hWnd;
		SD.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		SD.Windowed = TRUE;
		if (FAILED(p_DGIFac->CreateSwapChain(p_d3dDev, &SD, &dxgiSwap))) {
			MessageBox(hWnd, TEXT("Create_SwapChain_field"), TEXT("Error"), MB_ICONSTOP);
			return 1;
		}
		if (FAILED(dxgiSwap->GetBuffer(0, IID_PPV_ARGS(&p_d3dTex)))) {
			MessageBox(hWnd, TEXT("Get_Backbuffer_field"), TEXT("Error"), MB_ICONSTOP);
			return 1;
		}
		D2D1_RENDER_TARGET_PROPERTIES prop = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpi, dpi);
		RECT rect;
		GetClientRect(hWnd, &rect);
		if (FAILED(p_d2dFac->CreateDxgiSurfaceRenderTarget(p_d3dTex, &prop, &d2dRenderTarget))) {
			MessageBox(hWnd, TEXT("Create_DXGISurfaceRT_field"), TEXT("Error"), MB_ICONSTOP);
			return 1;
		}
		if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&p_DWFac)))) {
			MessageBox(hWnd, TEXT("Create_DWFac_field"), TEXT("Error"), MB_ICONSTOP);
			return 1;
		}
		OTime = timeGetTime();
		fontcount = -1;
		set_VSync();
		d2d_create_font(L"メイリオ", 16.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL);
		d2d_create_brush(1.0f, 1.0f, 1.0f, 1.0f);
		d2dRenderTarget->CreateLayer(NULL, &p_GraphLayer);
		SAFE_RELEASE(p_d3dDev);
		SAFE_RELEASE(p_d3dTex);
		SAFE_RELEASE(p_DGIDev);
		SAFE_RELEASE(p_DGIFac);
		SAFE_RELEASE(p_DGIAdp);
		SAFE_RELEASE(p_d2dFac);
		return 0;
	}

	void d2d_start_draw() {
		d2dRenderTarget->BeginDraw();
		d2d_fill_window(0.0f, 0.0f, 0.0f, 1.0f);
	}

	void d2d_end_draw() {
		d2dRenderTarget->EndDraw();
		dxgiSwap->Present(VSync, 0);
	}

	void d2d_draw_FPSRate(float x, float y) {
		std::wstringstream ss;
		static char Count;
		static float Rate;
		if (!((++Count) % 60)) {
			NTime = timeGetTime();
			Rate = 60000.0f / (NTime - OTime);
			Count = 0;
			OTime = NTime;
		}
		ss.precision(4);
		ss << Rate << L"fps";
		d2d_draw_string(x, y, ss.str());
	}

	void all_dxdevice_Release() {
		SAFE_RELEASE(p_DWFac);
		SAFE_RELEASE(p_DWFC);
		SAFE_RELEASE(p_DWFFL);
		SAFE_RELEASE(p_DWFF);
		SAFE_RELEASE(p_DWFCL);
		SAFE_RELEASE(p_DWFFE);
		SAFE_RELEASE(p_WICFac);
		SAFE_RELEASE(p_GraphLayer);
		SAFE_RELEASE(p_d2dLayer);
		for (unsigned int i = 0; i < p_BitmapVector.size(); i++)
			SAFE_RELEASE(p_BitmapVector[i]);
		p_BitmapVector.clear();
		for (unsigned int i = 0; i < p_FontVector.size(); i++)
			SAFE_RELEASE(p_FontVector[i]);
		p_FontVector.clear();
		for (unsigned int i = 0; i < p_SoundVector.size(); i++)
			SAFE_RELEASE(p_SoundVector[i]);
		p_SoundVector.clear();
		for (unsigned int i = 0; i < p_BrushVector.size(); i++)
			SAFE_RELEASE(p_BrushVector[i]);
		p_BrushVector.clear();
	}

	void d2d_fill_window(float R, float G, float B, float alpha) {
		d2dRenderTarget->Clear(D2D1::ColorF(R, G, B, alpha));
	}

	void d2d_full_screen(bool FullScreen) {
		dxgiSwap->SetFullscreenState(FullScreen, NULL);
	}

	int d2d_load_png_from_resource(LPCSTR ResourceName) {
		int i;
		bool sizemax;
		sizemax = true;
		for (i = 0; i < p_BitmapVector.size(); i++) {
			if (p_BitmapVector[i] == nullptr) {
				sizemax = false;
				break;
			}
		}
		if (sizemax) {
			i = p_BitmapVector.size();
			p_BitmapVector.push_back(nullptr);
		}
		IWICFormatConverter* p_WICCon = nullptr;
		IWICBitmapDecoder* p_WICBDec = nullptr;
		IWICBitmapFrameDecode* p_WICBFDec = nullptr;
		IWICStream* p_WICStr = nullptr;
		IWICBitmapScaler* p_WICBSca = nullptr;
		HMODULE hmodule = ::GetModuleHandle(NULL);
		HRSRC hrs = FindResource(hmodule, ResourceName, "PNG");
		HANDLE hHn = LoadResource(hmodule, hrs);
		LPBYTE lpByte = (LPBYTE)LockResource(hHn);
		DWORD size = SizeofResource(hmodule, hrs);
		if (hrs == NULL) {
			MessageBox(hWnd, TEXT("Load_From_Resource_failed"), TEXT("Error"), MB_ICONSTOP);
			return -1;
		}
		if (FAILED(p_WICFac->CreateStream(&p_WICStr))) {
			MessageBox(hWnd, TEXT("Create_WICStream_field"), TEXT("Error"), MB_ICONSTOP);
			return -1;
		}
		p_WICStr->InitializeFromMemory(lpByte, size);
		if (FAILED(p_WICFac->CreateDecoderFromStream(p_WICStr, NULL, WICDecodeMetadataCacheOnLoad, &p_WICBDec))) {
			MessageBox(hWnd, TEXT("Create_WICDec_field"), TEXT("Error"), MB_ICONSTOP);
			return -1;
		}
		p_WICBDec->GetFrame(0, &p_WICBFDec);
		if (FAILED(p_WICFac->CreateFormatConverter(&p_WICCon))) {
			MessageBox(hWnd, TEXT("Create_D2DFormatCon_field"), TEXT("Error"), MB_ICONSTOP);
			return -1;
		}
		p_WICFac->CreateFormatConverter(&p_WICCon);
		p_WICCon->Initialize(p_WICBFDec, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
		d2dRenderTarget->CreateBitmapFromWicBitmap(p_WICCon, NULL, &p_BitmapVector[i]);
		SAFE_RELEASE(p_WICCon);
		SAFE_RELEASE(p_WICBDec);
		SAFE_RELEASE(p_WICBFDec);
		SAFE_RELEASE(p_WICStr);
		SAFE_RELEASE(p_WICBSca);
		return i;
	}

	int d2d_load_png_from_file(LPCWSTR FileName) {
		int i;
		bool sizemax;
		sizemax = true;
		for (i = 0; i < p_BitmapVector.size(); i++) {
			if (p_BitmapVector[i] == nullptr) {
				sizemax = false;
				break;
			}
		}
		if (sizemax) {
			i = p_BitmapVector.size();
			p_BitmapVector.push_back(nullptr);
		}
		IWICFormatConverter* p_WICCon = nullptr;
		IWICBitmapDecoder* p_WICBD = nullptr;
		IWICBitmapFrameDecode* p_WICBFD = nullptr;
		if (FAILED(p_WICFac->CreateDecoderFromFilename(FileName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &p_WICBD))) {
			std::stringstream ss;
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			ss << "file_cant_open:" << converter.to_bytes(FileName);
			MessageBox(hWnd, TEXT(ss.str().c_str()), TEXT("Error"), MB_ICONSTOP);
			return -1;
		}
		if (FAILED(p_WICBD->GetFrame(0, &p_WICBFD))) {
			MessageBox(hWnd, TEXT("2"), TEXT("Error"), MB_ICONSTOP);
			return -1;
		}
		if (FAILED(p_WICFac->CreateFormatConverter(&p_WICCon))) {
			MessageBox(hWnd, TEXT("3"), TEXT("Error"), MB_ICONSTOP);
			return -1;
		}
		if (FAILED(p_WICCon->Initialize(p_WICBFD, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 1.0f, WICBitmapPaletteTypeMedianCut))) {
			MessageBox(hWnd, TEXT("4"), TEXT("Error"), MB_ICONSTOP);
			return -1;
		}
		if (FAILED(d2dRenderTarget->CreateBitmapFromWicBitmap(p_WICCon, NULL, &p_BitmapVector[i]))) {
			MessageBox(hWnd, TEXT("5"), TEXT("Error"), MB_ICONSTOP);
			return -1;
		}
		SAFE_RELEASE(p_WICCon);
		SAFE_RELEASE(p_WICBD);
		SAFE_RELEASE(p_WICBFD);
		return i;
	}

	int d2d_create_brush(float Red, float Green, float Blue, float Alpha) {
		int i;
		bool sizemax;
		sizemax = true;
		for (i = 0; i < p_BrushVector.size(); i++) {
			if (p_BrushVector[i] == nullptr) {
				sizemax = false;
				break;
			}
		}
		if (sizemax) {
			i = p_BrushVector.size();
			p_BrushVector.push_back(nullptr);
		}
		ID2D1SolidColorBrush* Brushtemp;
		d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(Red, Green, Blue, Alpha), &Brushtemp);
		p_BrushVector[i] = Brushtemp;
		return i;
	}

	int d2d_create_rg_brush(float Red1, float Green1, float Blue1, float Alpha1, float Red2, float Green2, float Blue2, float Alpha2, float StartX, float StartY, float EndX, float EndY) {
		int i;
		bool sizemax;
		sizemax = true;
		for (i = 0; i < p_BrushVector.size(); i++) {
			if (p_BrushVector[i] == nullptr) {
				sizemax = false;
				break;
			}
		}
		if (sizemax) {
			i = p_BrushVector.size();
			p_BrushVector.push_back(nullptr);
		}
		ID2D1RadialGradientBrush* Brushtemp;
		ID2D1GradientStopCollection* p_d2dGSPC = nullptr;
		D2D1_GRADIENT_STOP Gradientstop[2];
		Gradientstop[0].color = D2D1::ColorF(Red1, Green1, Blue1, Alpha1);
		Gradientstop[0].position = 0.0f;
		Gradientstop[1].color = D2D1::ColorF(Red2, Green2, Blue2, Alpha2);
		Gradientstop[1].position = 1.0f;
		if (FAILED(d2dRenderTarget->CreateGradientStopCollection(Gradientstop, 2, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &p_d2dGSPC))) {
			return -1;
		}
		if (FAILED(d2dRenderTarget->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(StartX, StartY), D2D1::Point2F(EndX, EndY), 100, 100), p_d2dGSPC, &Brushtemp))) {
			return -1;
		}
		p_BrushVector[i] = Brushtemp;
		return i;
	}

	int d2d_create_lg_brush(float Red1, float Green1, float Blue1, float Alpha1, float Red2, float Green2, float Blue2, float Alpha2, float StartX, float StartY, float EndX, float EndY) {
		int i;
		bool sizemax;
		sizemax = true;
		for (i = 0; i < p_BrushVector.size(); i++) {
			if (p_BrushVector[i] == nullptr) {
				sizemax = false;
				break;
			}
		}
		if (sizemax) {
			i = p_BrushVector.size();
			p_BrushVector.push_back(nullptr);
		}
		ID2D1LinearGradientBrush* Brushtemp;
		ID2D1GradientStopCollection* p_d2dGSPC = nullptr;
		D2D1_GRADIENT_STOP Gradientstop[2];
		Gradientstop[0].color = D2D1::ColorF(Red1, Green1, Blue1, Alpha1);
		Gradientstop[0].position = 0.0f;
		Gradientstop[1].color = D2D1::ColorF(Red2, Green2, Blue2, Alpha2);
		Gradientstop[1].position = 1.0f;
		if (FAILED(d2dRenderTarget->CreateGradientStopCollection(Gradientstop, 2, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &p_d2dGSPC))) {
			return -1;
		}
		if (FAILED(d2dRenderTarget->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(D2D1::Point2F(StartX, StartY), D2D1::Point2F(EndX, EndY)), p_d2dGSPC, &Brushtemp))) {
			return -1;
		}
		p_BrushVector[i] = Brushtemp;
		return i;
	}

	int d2d_create_bitmap_brush(int BitmapID) {
		int i;
		bool sizemax;
		sizemax = true;
		for (i = 0; i < p_BrushVector.size(); i++) {
			if (p_BrushVector[i] == nullptr) {
				sizemax = false;
				break;
			}
		}
		if (sizemax) {
			i = p_BrushVector.size();
			p_BrushVector.push_back(nullptr);
		}
		ID2D1BitmapBrush* Brushtemp;
		d2dRenderTarget->CreateBitmapBrush(p_BitmapVector[BitmapID], &Brushtemp);
		p_BrushVector[i] = Brushtemp;
		return i;
	}

	int d2d_create_font(LPCWSTR FontName, float FontSize, DWRITE_FONT_WEIGHT FontWeight, DWRITE_FONT_STYLE FontStyle, DWRITE_FONT_STRETCH FontStretch) {
		int i;
		bool sizemax;
		sizemax = true;
		for (i = 0; i < p_FontVector.size(); i++) {
			if (p_FontVector[i] == nullptr) {
				sizemax = false;
				break;
			}
		}
		if (sizemax) {
			i = p_FontVector.size();
			p_FontVector.push_back(nullptr);
		}
		p_DWFac->CreateTextFormat(FontName, NULL, FontWeight, FontStyle, FontStretch, FontSize, L"ja-jp", &p_FontVector[i]);
		return i;
	}

	int d2d_set_layer(float left, float top, float right, float bottom) {
		d2dRenderTarget->PushLayer(D2D1::LayerParameters(D2D1::RectF(left, top, right, bottom)), p_d2dLayer);
		return 0;
	}

	void d2d_pop_layer() {
		d2dRenderTarget->PopLayer();
	}

	void d2d_set_translation(float x, float y) {
		d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(x, y));
	}

	void d2d_set_rotate(float centerx, float centery, float Rotate) {
		d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(Rotate, D2D1::Point2F(centerx, centery)));
	}

	void d2d_set_scale(float centerx, float centery, float width, float height) {
		d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(D2D1::SizeF(width, height), D2D1::Point2F(centerx, centery)));
	}

	void d2d_set_skew(float centerx, float centery, float anglex, float angley) {
		d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Skew(anglex, angley, D2D1::Point2F(centerx, centery)));
	}

	void d2d_set_reversex(float centerx) {
		D2D1::Matrix3x2F mat;
		mat._11 = -1.0;
		mat._12 = 0;
		mat._21 = 0;
		mat._22 = 1;
		mat._31 = centerx * 2.f;
		mat._32 = 0;
		d2dRenderTarget->SetTransform(mat);
	}

	void d2d_reset_transform() {
		d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	}

	void d2d_draw_bitmapUL(int BitmapID, float leftx, float topy) {
		BitmapID = CheckBitmapID(BitmapID);
		D2D1_SIZE_F BitmapSize = p_BitmapVector[BitmapID]->GetSize();
		d2dRenderTarget->DrawBitmap(p_BitmapVector[BitmapID], D2D1::RectF(leftx, topy, leftx + BitmapSize.width, topy + BitmapSize.height));
	}

	void d2d_draw_bitmap(int BitmapID, float x, float y) {
		BitmapID = CheckBitmapID(BitmapID);
		D2D1_SIZE_F BitmapSize = p_BitmapVector[BitmapID]->GetSize();
		x -= (double)(BitmapSize.width / 2.0);
		y -= (double)(BitmapSize.height / 2.0);
		d2dRenderTarget->DrawBitmap(p_BitmapVector[BitmapID], D2D1::RectF(x, y, x + BitmapSize.width, y + BitmapSize.height));
	}

	void d2d_draw_rotate_bitmap(int BitmapID, float x, float y, float Rotate) {
		BitmapID = CheckBitmapID(BitmapID);
		D2D1_SIZE_F BitmapSize = p_BitmapVector[BitmapID]->GetSize();
		d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(Rotate, D2D1::Point2F(x, y)));
		x -= (BitmapSize.width / 2.0);
		y -= (BitmapSize.height / 2.0);
		d2dRenderTarget->DrawBitmap(p_BitmapVector[BitmapID], D2D1::RectF(x, y, x + BitmapSize.width, y + BitmapSize.height));
		d2d_reset_transform();
	}

	void d2d_draw_xreversed_bitmap(int BitmapID, float x, float y) {
		BitmapID = CheckBitmapID(BitmapID);
		D2D1_SIZE_F BitmapSize = p_BitmapVector[BitmapID]->GetSize();
		d2d_set_reversex(x);
		x -= (BitmapSize.width / 2.0);
		y -= (BitmapSize.height / 2.0);
		d2dRenderTarget->DrawBitmap(p_BitmapVector[BitmapID], D2D1::RectF(x, y, x + BitmapSize.width, y + BitmapSize.height));
		d2d_reset_transform();
	}

	void d2d_draw_part_of_bitmapUL(int BitmapID, float x, float y, float Bitmapx, float Bitmapy, float BitmapWidth, float BitmapHeight) {
		BitmapID = CheckBitmapID(BitmapID);
		D2D1_SIZE_F BitmapSize = p_BitmapVector[BitmapID]->GetSize();
		d2dRenderTarget->PushLayer(D2D1::LayerParameters(D2D1::RectF(x, y, x + BitmapWidth, y + BitmapHeight)), p_GraphLayer);
		d2d_draw_bitmapUL(BitmapID, x - Bitmapx, y - Bitmapy);
		d2dRenderTarget->PopLayer();
	}

	void d2d_draw_part_of_bitmap_NumUL(int BitmapID, float x, float y, float BitmapWidth, float BitmapHeight, int xNum, int yNum) {
		BitmapID = CheckBitmapID(BitmapID);
		D2D1_SIZE_F BitmapSize = p_BitmapVector[BitmapID]->GetSize();
		d2dRenderTarget->PushLayer(D2D1::LayerParameters(D2D1::RectF(x, y, x + BitmapWidth, y + BitmapHeight)), p_GraphLayer);
		d2d_draw_bitmapUL(BitmapID, x - (BitmapWidth * xNum), y - (BitmapHeight * yNum));
		d2dRenderTarget->PopLayer();
	}

	void d2d_draw_part_of_xreversed_bitmapUL(int BitmapID, float x, float y, float Bitmapx, float Bitmapy, float BitmapWidth, float BitmapHeight) {
		d2d_set_reversex(x + BitmapWidth/2.f);
		d2d_draw_part_of_bitmapUL(BitmapID, x, y, Bitmapx, Bitmapy, BitmapWidth, BitmapHeight);
		d2d_reset_transform();
	}

	void d2d_draw_part_of_xreversed_bitmap_NumUL(int BitmapID, float x, float y, float BitmapWidth, float BitmapHeight, int xNum, int yNum) {
		d2d_set_reversex(x + BitmapWidth/2.f);
		d2d_draw_part_of_bitmap_NumUL(BitmapID, x, y, BitmapWidth, BitmapHeight, xNum, yNum);
		d2d_reset_transform();
	}

	void d2d_draw_string(float x, float y, std::wstring DrawStr) {
		//wstring wstr = DrawStr;
		D2D1_RECT_F rect;
		rect.left = x;
		rect.top = y;
		rect.right = x + 1000;
		rect.bottom = y + 1000;
		d2dRenderTarget->DrawText(DrawStr.c_str(), DrawStr.size(), p_FontVector[0], rect, p_BrushVector[0]);
	}

	void d2d_draw_custom_text_string(int FontID, int BrushID, float x, float y, std::wstring DrawStr) {
		FontID = CheckFontID(FontID);
		BrushID = CheckBrushID(BrushID);
		D2D1_RECT_F rect;
		rect.left = x;
		rect.top = y;
		rect.right = x + 1920;
		rect.bottom = y + 1080;
		d2dRenderTarget->DrawText(DrawStr.c_str(), DrawStr.size(), p_FontVector[FontID], rect, p_BrushVector[BrushID]);
	}
	void d2d_draw_line(int BrushID, float startx, float starty, float endx, float endy, float StrokeWidth) {
		BrushID = CheckBrushID(BrushID);
		d2dRenderTarget->DrawLine(D2D1::Point2F(startx, starty), D2D1::Point2F(endx, endy), p_BrushVector[BrushID], StrokeWidth, NULL);
	}

	void d2d_draw_circle(int BrushID, float Centerx, float Centery, float Width, float Height, float StrokeWidth, bool fill) {
		BrushID = CheckBrushID(BrushID);
		D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(Centerx, Centery), Width / 2.0, Height / 2.0);
		if (fill)
			d2dRenderTarget->FillEllipse(&ellipse, p_BrushVector[BrushID]);
		else
			d2dRenderTarget->DrawEllipse(&ellipse, p_BrushVector[BrushID], StrokeWidth, NULL);
	}

	void d2d_draw_rect(int BrushID, int left, int top, int right, int bottom, float StrokeWidth, bool fill) {
		BrushID = CheckBrushID(BrushID);
		D2D1_RECT_F rect = D2D1::RectF(left, top, right, bottom);
		if (fill)
			d2dRenderTarget->FillRectangle(&rect, p_BrushVector[BrushID]);
		else
			d2dRenderTarget->DrawRectangle(&rect, p_BrushVector[BrushID], StrokeWidth);
	}

	int d2d_draw_rect_with_dynamicscb(float Red, float Green, float Blue, float Alpha, float right, float top, float left, float bottom) {
		ID2D1SolidColorBrush* p_SolidColorBrush;
		d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(Red, Green, Blue, Alpha), &p_SolidColorBrush);
		D2D_RECT_F rect = D2D1::RectF(left, top, right, bottom);
		d2dRenderTarget->FillRectangle(&rect, p_SolidColorBrush);
		SAFE_RELEASE(p_SolidColorBrush);
		return 0;
	}

	int d2d_draw_rect_with_dynamiclb(float Red1, float Green1, float Blue1, float Alpha1, float Red2, float Green2, float Blue2, float Alpha2, float StartX, float StartY, float EndX, float EndY, float right, float top, float left, float bottom) {
		ID2D1LinearGradientBrush* p_LinerBrush;
		ID2D1GradientStopCollection* p_d2dGSPC = nullptr;
		D2D1_GRADIENT_STOP Gradientstop[2];
		Gradientstop[0].color = D2D1::ColorF(Red1, Green1, Blue1, Alpha1);
		Gradientstop[0].position = 0.0f;
		Gradientstop[1].color = D2D1::ColorF(Red2, Green2, Blue2, Alpha2);
		Gradientstop[1].position = 1.0f;
		if (FAILED(d2dRenderTarget->CreateGradientStopCollection(Gradientstop, 2, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &p_d2dGSPC))) {
			return -1;
		}
		if (FAILED(d2dRenderTarget->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(D2D1::Point2F(StartX, StartY), D2D1::Point2F(EndX, EndY)), p_d2dGSPC, &p_LinerBrush))) {
			return -1;
		}
		D2D_RECT_F rect = D2D1::RectF(left, top, right, bottom);
		d2dRenderTarget->FillRectangle(&rect, p_LinerBrush);
		SAFE_RELEASE(p_LinerBrush);
		return 0;
	}

	void d2d_release_bitmap(int BitmapID) {
		SAFE_RELEASE(p_BitmapVector[BitmapID]);
	}

	void d2d_release_font(int FontID) {
		SAFE_RELEASE(p_FontVector[FontID]);
	}

	void d2d_release_brush(int BrushID) {
		SAFE_RELEASE(p_BrushVector[BrushID]);
	}

	int ds_load_wave_from_file(LPSTR FileName) {
		int i;
		bool sizemax;
		sizemax = true;
		for (i = 0; i < p_SoundVector.size(); i++) {
			if (p_SoundVector[i] == nullptr) {
				sizemax = false;
				break;
			}
		}
		if (sizemax) {
			i = p_SoundVector.size();
			p_SoundVector.push_back(nullptr);
		}
		int HeaderSize;
		MMCKINFO WaveFile, WaveFmt, WaveData;
		HMMIO src;
		LPWAVEFORMATEX lpWF;
		if (!(src = mmioOpenA(FileName, NULL, MMIO_ALLOCBUF | MMIO_READ | MMIO_COMPAT))) {
			MessageBox(hWnd, TEXT("WaveFileを開けません"), TEXT("Error"), MB_ICONSTOP);
			return -1;
		}
		ZeroMemory(&WaveFile, sizeof(WaveFile));
		if (FAILED(mmioDescend(src, &WaveFile, NULL, MMIO_FINDRIFF))) {
			MessageBox(hWnd, TEXT("(wave)mmioDescendFailed"), TEXT("Error"), MB_ICONSTOP);
			return -1;
		}

		if (WaveFile.fccType != mmioFOURCC('W', 'A', 'V', 'E')) {
			MessageBox(hWnd, TEXT("Waveチャンクエラー"), TEXT("Error"), MB_ICONSTOP);
			mmioClose(src, 0);
			return -1;
		}
		ZeroMemory(&WaveFmt, sizeof(WaveFmt));
		if (FAILED(mmioDescend(src, &WaveFmt, &WaveFile, MMIO_FINDCHUNK))) {
			MessageBox(hWnd, TEXT("(fmt)mmioDescendFailed"), TEXT("Error"), MB_ICONSTOP);
			return -1;
		}
		if (WaveFmt.ckid != mmioFOURCC('f', 'm', 't', ' ')) {
			MessageBox(hWnd, TEXT("fmtチャンクエラー"), TEXT("Error"), MB_ICONSTOP);
			mmioClose(src, 0);
			return -1;
		}
		HeaderSize = WaveFmt.cksize;
		if (HeaderSize < sizeof(WAVEFORMATEX))
			HeaderSize = sizeof(WAVEFORMATEX);
		if (!(lpWF = (LPWAVEFORMATEX)malloc(HeaderSize))) {
			MessageBox(hWnd, TEXT("メモリ確保エラー"), TEXT("Error"), MB_ICONSTOP);
			mmioClose(src, 0);
			return -1;
		}
		ZeroMemory(lpWF, HeaderSize);
		if (FAILED(mmioRead(src, (char*)lpWF, WaveFmt.cksize))) {
			MessageBox(hWnd, TEXT("Waveフォーマットロードエラー"), TEXT("Error"), MB_ICONSTOP);
			mmioClose(src, 0);
			return -1;
		}
		mmioAscend(src, &WaveFmt, 0);
		while (1) {
			if (FAILED(mmioDescend(src, &WaveData, &WaveFile, 0))) {
				MessageBox(hWnd, TEXT("dataチャンクが見つかりません"), TEXT("Error"), MB_ICONSTOP);
				free(lpWF);
				mmioClose(src, 0);
				return -1;
			}
			if (WaveData.ckid == mmioStringToFOURCCA("data", 0))
				break;
			mmioAscend(src, &WaveData, 0);
		}
		DSBUFFERDESC dsbd;
		ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
		dsbd.dwSize = sizeof(DSBUFFERDESC);
		dsbd.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STATIC | DSBCAPS_LOCDEFER | DSBCAPS_CTRLVOLUME;
		dsbd.dwBufferBytes = WaveData.cksize;
		dsbd.lpwfxFormat = lpWF;
		dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
		if (FAILED(DSound->CreateSoundBuffer(&dsbd, &p_SoundVector[i], NULL))) {
			MessageBox(hWnd, TEXT("サウンドバッファが作れません"), TEXT("Error"), MB_ICONSTOP);
			free(lpWF);
			mmioClose(src, 0);
			return -1;
		}
		LPVOID pMem1, pMem2;
		DWORD dwSize1, dwSize2;
		if (FAILED((p_SoundVector[i])->Lock(0, WaveData.cksize, &pMem1, &dwSize1, &pMem2, &dwSize2, 0))) {
			MessageBox(hWnd, TEXT("ロックできません"), TEXT("Error"), MB_ICONSTOP);
			free(lpWF);
			mmioClose(src, 0);
			return -1;
		}
		mmioRead(src, (char*)pMem1, dwSize1);
		mmioRead(src, (char*)pMem2, dwSize2);
		(p_SoundVector[i])->Unlock(pMem1, dwSize1, pMem2, dwSize2);
		free(lpWF);
		mmioClose(src, 0);
		return i;
	}

	void ds_sound_play(int SoundID) {
		SoundID = CheckSoundID(SoundID);
		if (FAILED(p_SoundVector[SoundID]->Play(0, SoundID, 0))) {
			MessageBox(hWnd, TEXT("BGMを再生できません"), TEXT("Error"), MB_ICONSTOP);
		}
	}

	void ds_set_volume(int SoundID, long SoundVol) {
		SoundID = CheckSoundID(SoundID);
		p_SoundVector[SoundID]->SetVolume(SoundVol);
	}

	int ds_get_current_cursor(int SoundID) {
		DWORD playPosition;
		p_SoundVector[SoundID]->GetCurrentPosition(&playPosition, NULL);
		return playPosition;
	}

	void ds_set_freqency(int SoundID, int SoundFreq) {
		SoundID = CheckSoundID(SoundID);
		p_SoundVector[SoundID]->SetFrequency(SoundFreq);
	}

	void ds_sound_stop(int SoundID) {
		SoundID = CheckSoundID(SoundID);
		p_SoundVector[SoundID]->Stop();
	}

	void ds_sound_set_pos(int SoundID, int StartPosition) {
		SoundID = CheckSoundID(SoundID);
		p_SoundVector[SoundID]->SetCurrentPosition(StartPosition);
	}

	void ds_release_wave(int SoundID) {
		SAFE_RELEASE(p_SoundVector[SoundID]);
	}
}