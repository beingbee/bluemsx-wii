/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Common core implementation header
*/


#ifndef HGE_IMPL_H
#define HGE_IMPL_H

#include "..\..\..\include\hge\hge.h"
#include <stdio.h>
#include <d3d8.h>
#ifndef UNDER_CE
#include <d3dx8.h>
#endif

#define D3DFVF_HGEVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define VERTEX_BUFFER_SIZE 4000


struct CRenderTargetList
{
	int					width;
	int					height;
	IDirect3DTexture8*	pTex;
	IDirect3DSurface8*	pDepth;
	CRenderTargetList*	next;
};

struct CTextureList
{
	HTEXTURE			tex;
	int					width;
	int					height;
	CTextureList*		next;
};

struct CInputEventList
{
	hgeInputEvent		event;
	CInputEventList*	next;
};


/*
** HGE Interface implementation
*/
class HGE_Impl : public HGE
{
public:
	virtual	void		CALL	Release();

	virtual bool		CALL	System_Initiate();
	virtual void		CALL	System_Shutdown();
	virtual bool		CALL	System_Start();
	virtual wchar_t*		CALL	System_MakePath      (const wchar_t *filename);
	virtual void		CALL	System_SetStateBool  (hgeBoolState   state, bool        value);
	virtual void		CALL	System_SetStateFunc  (hgeFuncState   state, hgeCallback value);
	virtual void		CALL	System_SetStateHwnd  (hgeHwndState   state, HWND        value);
	virtual void		CALL	System_SetStateInt   (hgeIntState    state, int         value);
	virtual void		CALL	System_SetStateString(hgeStringState state, const wchar_t *value);
	virtual bool		CALL	System_GetStateBool  (hgeBoolState  );
	virtual hgeCallback	CALL	System_GetStateFunc  (hgeFuncState  );
	virtual HWND		CALL	System_GetStateHwnd  (hgeHwndState  );
	virtual int			CALL	System_GetStateInt   (hgeIntState   );
	virtual const wchar_t*	CALL	System_GetStateString(hgeStringState);
	virtual char*		CALL	System_GetErrorMessage();
	virtual	void		CALL	System_Log(const char *format, ...);
#ifdef HGE_FULL
	virtual bool		CALL	System_Launch(const wchar_t *url);
#endif

	virtual float		CALL	Timer_GetTime();
	virtual float		CALL	Timer_GetDelta();
	virtual int			CALL	Timer_GetFPS();

	virtual void		CALL	Input_GetMousePos(float *x, float *y);
	virtual void		CALL	Input_SetMousePos(float x, float y);
	virtual int			CALL	Input_GetMouseWheel();
	virtual bool		CALL	Input_IsMouseOver();
	virtual bool		CALL	Input_KeyDown(int key);
	virtual bool		CALL	Input_KeyUp(int key);
	virtual bool		CALL	Input_GetKeyState(int key);
	virtual char*		CALL	Input_GetKeyName(int key);
	virtual int			CALL	Input_GetKey();
	virtual int			CALL	Input_GetChar();
	virtual bool		CALL	Input_GetEvent(hgeInputEvent *event);

	virtual bool		CALL	Gfx_BeginScene(HTARGET target=0);
	virtual void		CALL	Gfx_EndScene();
	virtual void		CALL	Gfx_Clear(DWORD color);
	virtual void		CALL	Gfx_RenderQuad(const hgeQuad *quad);
	virtual void		CALL	Gfx_SetClipping(int x=0, int y=0, int w=0, int h=0);

	virtual HTARGET		CALL	Target_Create(int width, int height, bool zbuffer);
	virtual void		CALL	Target_Free(HTARGET target);
	virtual HTEXTURE	CALL	Target_GetTexture(HTARGET target);

	virtual HTEXTURE	CALL	Texture_Create(int width, int height);
#ifdef HGE_FULL
	virtual HTEXTURE	CALL	Texture_Load(const char *filename, DWORD size=0, bool bMipmap=false);
#endif
	virtual void		CALL	Texture_Free(HTEXTURE tex);
	virtual int			CALL	Texture_GetWidth(HTEXTURE tex);
	virtual int			CALL	Texture_GetHeight(HTEXTURE tex);
	virtual DWORD*		CALL	Texture_Lock(HTEXTURE tex, bool bReadOnly=true, int left=0, int top=0, int width=0, int height=0);
	virtual void		CALL	Texture_Unlock(HTEXTURE tex);

	//////// Implementation ////////

	static HGE_Impl*	_Interface_Get();
	void				_FocusChange(bool bAct);
	void				_PostError(char *error);


	HINSTANCE			hInstance;
	HWND				hwnd;

	bool				bActive;
	char				szError[256];
	wchar_t				szAppPath[_MAX_PATH];
	wchar_t				szTmpFilename[_MAX_PATH];


	// System States
	bool				(*procFrameFunc)();
	bool				(*procRenderFunc)();
	bool				(*procFocusLostFunc)();
	bool				(*procFocusGainFunc)();
	bool				(*procGfxRestoreFunc)();
	bool				(*procExitFunc)();
	const wchar_t*			szIcon;
	wchar_t				szWinTitle[256];
	int					nScreenWidth;
	int					nScreenHeight;
	int					nScreenBPP;
	bool				bWindowed;
	bool				bZBuffer;
	bool				bTextureFilter;
	wchar_t				szLogFile[_MAX_PATH];
	int					nHGEFPS;
	bool				bHideMouse;
	bool				bDontSuspend;
	HWND				hwndParent;


	// Graphics
	D3DPRESENT_PARAMETERS*  d3dpp;

	D3DPRESENT_PARAMETERS   d3dppW;
	RECT					rectW;
	LONG					styleW;

	D3DPRESENT_PARAMETERS   d3dppFS;
	RECT					rectFS;
	LONG					styleFS;

	IDirect3D8*				pD3D;
	IDirect3DDevice8*		pD3DDevice;
	IDirect3DVertexBuffer8*	pVB;
	IDirect3DIndexBuffer8*	pIB;

	IDirect3DSurface8*	pScreenSurf;
	IDirect3DSurface8*	pScreenDepth;
	CRenderTargetList*	pTargets;
	CRenderTargetList*	pCurTarget;

	D3DXMATRIX			matView;
	D3DXMATRIX			matProj;

	hgeVertex*			VertArray;

	int					nPrim;
	int					CurPrimType;
	int					CurBlendMode;
	HTEXTURE			CurTexture;

	bool				_GfxInit();
	void				_GfxDone();
	bool				_GfxRestore();
	void				_AdjustWindow();
	void				_Resize(int width, int height);
	bool				_init_lost();
	void				_render_batch(bool bEndScene=false);
	int					_format_id(D3DFORMAT fmt);
	void				_SetBlendMode(int blend);
	void				_SetProjectionMatrix(int width, int height);
	

	// Input
	int					VKey;
	int					Char;
	int					Zpos;
	float				Xpos;
	float				Ypos;
	bool				bMouseOver;
	bool				bCaptured;
	char				keyz[256];
	CInputEventList*	queue;
	void				_UpdateMouse();
	void				_InputInit();
	void				_ClearQueue();
	void				_BuildEvent(int type, int key, int scan, int flags, int x, int y);


	// Timer
	float				fTime;
	float				fDeltaTime;
	DWORD				nFixedDelta;
	int					nFPS;
	DWORD				t0, t0fps, dt;
	int					cfps;


private:
	HGE_Impl();
};

extern HGE_Impl*		pHGE;

#endif

