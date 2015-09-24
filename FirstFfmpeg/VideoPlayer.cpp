#include "StdAfx.h"
#include "FirstFfmpeg.h"
#include "VideoPlayer.h"

CVideoPlayer::CVideoPlayer(void)
{
	m_dxInstance.ddraw = NULL;
	m_dxInstance.PrimarySurface = NULL;
	m_dxInstance.BackSurface = NULL;
	m_dxInstance.clipper = NULL;

	m_pWindow = NULL;
	m_nBarWidth = 12;

	m_nTextHeigh = 20;

	m_bOriginalScale=FALSE;
	m_bIsBroadcast=FALSE;
}

CVideoPlayer::~CVideoPlayer(void)
{
	this->FinitialVideo();
}

BOOL  CVideoPlayer::InitialVideo(CWnd* pWnd, GUID* lpGuid,int nWidth, int nHeigh, int nTextHeigh, int nOffset,BOOL bOriginalScale,BOOL bIsBroadcast)
{
	DDSURFACEDESC2 ddsd;
	m_nPicWidth = nWidth;
	m_nPicHeight = nHeigh;
	m_nTextHeigh = nTextHeigh;
	m_pWindow = pWnd;
	m_lpGuid = lpGuid;
	m_nScreenOffset = nOffset;
	m_bOriginalScale=bOriginalScale;

	m_bIsBroadcast=bIsBroadcast;

	if (DirectDrawCreateEx(m_lpGuid, (LPVOID*)&m_dxInstance.ddraw,IID_IDirectDraw7, NULL) != DD_OK
		||m_dxInstance.ddraw->SetCooperativeLevel(m_pWindow->GetSafeHwnd(), DDSCL_NORMAL) != DD_OK)
	{
		return FALSE;
	}
/////////////////
	ZeroMemory(&ddsd, sizeof (DDSURFACEDESC2));
	ddsd.dwSize = sizeof (DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	if(DD_OK != m_dxInstance.ddraw->CreateSurface(&ddsd, &m_dxInstance.PrimarySurface, NULL))
	{
		return FALSE;
	}
	if (DD_OK != m_dxInstance.ddraw->CreateClipper(0,&m_dxInstance.clipper, NULL) 
		||DD_OK != m_dxInstance.clipper->SetHWnd(0, m_pWindow->GetSafeHwnd()) 
		||DD_OK != m_dxInstance.PrimarySurface->SetClipper(m_dxInstance.clipper))
	{
		return FALSE;//error initialize directDraw clipper
	}

	CRect wndRect;
	GetWindowRect(m_pWindow->GetSafeHwnd(), &wndRect);

	if (bIsBroadcast)
	{
		m_nPicWidth=wndRect.Width()/13;
		m_nBarWidth = wndRect.Width()*6/13;
	}
	else
	{
		m_nBarWidth = m_nPicWidth/AUDIO_BAR_PERCENT;
		if(m_nPicWidth%AUDIO_BAR_PERCENT >= AUDIO_BAR_PERCENT/2)
		{
			m_nBarWidth++;
		}
	}

////////////////////
	m_nBarWidth += m_nBarWidth%2;

	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
	ddsd.dwSize = sizeof (DDSURFACEDESC2);
	ddsd.ddpfPixelFormat.dwSize = sizeof (DDPIXELFORMAT);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_CAPS;
	ddsd.dwHeight = m_nPicHeight;
	ddsd.dwWidth = m_nPicWidth + m_nBarWidth * 2;
	ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC;
	ddsd.ddpfPixelFormat.dwFourCC = FOURCC_YV12;
	ddsd.dwFlags |= DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	if (DD_OK != m_dxInstance.ddraw->CreateSurface(&ddsd, &m_dxInstance.BackSurface, NULL) )
	{
		return  FALSE;
	}
	

	return  TRUE;
}

BOOL CVideoPlayer::ResetVideo(void)
{
	if(m_dxInstance.ddraw != NULL)
	{
		if(m_dxInstance.PrimarySurface != NULL)
		{
			m_dxInstance.PrimarySurface->Release();
			m_dxInstance.PrimarySurface = NULL;
		}
		if(m_dxInstance.BackSurface != NULL)
		{
			m_dxInstance.BackSurface->Release();
			m_dxInstance.BackSurface = NULL;
		}
		if(m_dxInstance.clipper != NULL)
		{
			m_dxInstance.clipper->Release();
			m_dxInstance.clipper = NULL;
		}

		m_dxInstance.ddraw->Release();
		m_dxInstance.ddraw = NULL;
	}

	if (DirectDrawCreateEx(m_lpGuid, (LPVOID*)&m_dxInstance.ddraw,IID_IDirectDraw7, NULL) != DD_OK
		||m_dxInstance.ddraw->SetCooperativeLevel(m_pWindow->GetSafeHwnd(), DDSCL_NORMAL) != DD_OK)
	{
		return FALSE;
	}
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof (DDSURFACEDESC2));
	ddsd.dwSize = sizeof (DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	if(DD_OK != m_dxInstance.ddraw->CreateSurface(&ddsd, &m_dxInstance.PrimarySurface, NULL))
	{
		return FALSE;//Error Create PrimarySurface
	}
	if (DD_OK != m_dxInstance.ddraw->CreateClipper(0,&m_dxInstance.clipper, NULL) 
		||DD_OK != m_dxInstance.clipper->SetHWnd(0, m_pWindow->GetSafeHwnd()) 
		||DD_OK != m_dxInstance.PrimarySurface->SetClipper(m_dxInstance.clipper))
	{
		return FALSE;//error initialize directDraw clipper
	}

	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
	ddsd.dwSize = sizeof (DDSURFACEDESC2);
	ddsd.ddpfPixelFormat.dwSize = sizeof (DDPIXELFORMAT);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_CAPS;
	ddsd.dwHeight = m_nPicHeight;
	ddsd.dwWidth = m_nPicWidth + m_nBarWidth * 2;
	ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC;
	ddsd.ddpfPixelFormat.dwFourCC = FOURCC_YV12;
	ddsd.dwFlags |= DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	if (DD_OK != m_dxInstance.ddraw->CreateSurface(&ddsd, &m_dxInstance.BackSurface, NULL) )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL  CVideoPlayer::FinitialVideo()
{
	if(m_dxInstance.PrimarySurface != NULL)
	{
		m_dxInstance.PrimarySurface->Release();
		m_dxInstance.PrimarySurface = NULL;
	}
	if(m_dxInstance.BackSurface != NULL)
	{
		m_dxInstance.BackSurface->Release();
		m_dxInstance.BackSurface = NULL;
	}
	if(m_dxInstance.clipper != NULL)
	{
		m_dxInstance.clipper->Release();
		m_dxInstance.clipper = NULL;
	}
	if(m_dxInstance.ddraw)
	{
		m_dxInstance.ddraw->Release();
		m_dxInstance.ddraw = NULL;
	}
	if(m_pWindow)
	{
		m_pWindow->Invalidate(TRUE);
	}

	m_bIsBroadcast=FALSE;

	return TRUE;
}

void CVideoPlayer::DrawPicture(unsigned char *buffer[3], int nLeftVolume, int nRightVolume)
{
//	CMultiplexPlayerApp* pMyApp=(CMultiplexPlayerApp*)AfxGetApp();

	GetWindowRect(m_pWindow->GetSafeHwnd(), &m_clientRect);

	int nMargin = (m_clientRect.right - m_clientRect.left)/50;
	int nBottomMargin = 20+2;

	m_clientRect.left += nMargin;
	m_clientRect.top += nMargin;
	m_clientRect.right -= nMargin;
	m_clientRect.bottom -= nBottomMargin;


	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
	ddsd.dwSize = sizeof (DDSURFACEDESC2);
	if(m_dxInstance.BackSurface)
	{
		if(m_dxInstance.BackSurface->Lock(NULL, &ddsd, DDLOCK_NOSYSLOCK | DDLOCK_WAIT, NULL) != DD_OK)
		{
			if(ResetVideo() > 0x0)
			{
				return;
			}
			m_dxInstance.BackSurface->Lock(NULL, &ddsd, DDLOCK_NOSYSLOCK | DDLOCK_WAIT, NULL);
			if(ddsd.lpSurface == NULL)
			{
				return;
			}
		}
	}
	else
	{
		if(ResetVideo() > 0x0)
		{
			return;
		}
		m_dxInstance.BackSurface->Lock(NULL, &ddsd, DDLOCK_NOSYSLOCK | DDLOCK_WAIT, NULL);
		if(ddsd.lpSurface == NULL)
		{
			return;
		}
	}

	unsigned char* pYbuffer = buffer[0];
//	unsigned char* pUbuffer = buffer[2];
//	unsigned char* pVbuffer = buffer[1];

	unsigned char* pUbuffer = buffer[1];
	unsigned char* pVbuffer = buffer[2];

	unsigned char* pYdest = (unsigned char*)ddsd.lpSurface;
	unsigned char* pUdest = (unsigned char*)(pYdest + ddsd.lPitch * m_nPicHeight);
	unsigned char* pVdest = (unsigned char*)(pUdest + (ddsd.lPitch * m_nPicHeight >> 2));

	for (int i = 0; i < m_nPicHeight/2; i++)
	{
		memcpy (pYdest + m_nBarWidth, pYbuffer +2*i*m_nPicWidth, m_nPicWidth);
		pYdest += ddsd.lPitch;
		memcpy (pYdest + m_nBarWidth, pYbuffer+(2*i+1)*m_nPicWidth, m_nPicWidth);
		pYdest += ddsd.lPitch;
		memcpy (pUdest + m_nBarWidth / 2, pUbuffer+(i*m_nPicWidth/2), m_nPicWidth/2);
		pUdest += ddsd.lPitch/2;
		memcpy (pVdest + m_nBarWidth / 2, pVbuffer+(i*m_nPicWidth/2), m_nPicWidth/2);
		pVdest += ddsd.lPitch/2;
	}

	
	m_dxInstance.BackSurface->Unlock(NULL);

	//¼ÆËã±ÈÀý
	if (m_bOriginalScale)
	{
		int nWidth = m_clientRect.Width();
		int nHeight = m_clientRect.Height();

		int nMarginWidth = m_nPicWidth * m_clientRect.Height()/m_nPicHeight;
		int nMarginHeight = m_nPicHeight * m_clientRect.Width()/m_nPicWidth;

		int nXMargin = (nWidth-(nWidth>nMarginWidth ? nMarginWidth : nWidth))/2;
		int nYMargin = (nHeight-(nHeight>nMarginHeight ? nMarginHeight : nHeight))/2;

		m_clientRect.left += nXMargin;
		m_clientRect.right -= nXMargin;
		m_clientRect.top += nYMargin;
		m_clientRect.bottom -= nYMargin;
	}
	
	DDBLTFX ddbltfx;
	ZeroMemory(&ddbltfx, sizeof (DDBLTFX));
	ddbltfx.dwSize = sizeof (DDBLTFX);
	ddbltfx.dwDDFX = DDBLTFX_NOTEARING;
	if(m_dxInstance.PrimarySurface != NULL)
	{
		m_clientRect.left -= m_nScreenOffset;
		m_clientRect.right -= m_nScreenOffset;
		if (DDERR_SURFACELOST == m_dxInstance.PrimarySurface->Blt(&m_clientRect,m_dxInstance.BackSurface, NULL, DDBLT_WAIT, &ddbltfx)) 
		{
			// restore surface and try again 
			m_dxInstance.PrimarySurface->Restore();
			m_dxInstance.PrimarySurface->Blt(&m_clientRect,m_dxInstance.BackSurface, NULL, DDBLT_WAIT, &ddbltfx);
		}
	}
}
