#pragma once
#include "stdafx.h"
/*#include "..\MyStatic.h"*/
#include <ddraw.h>


#pragma  comment(lib, "ddraw.lib")
#pragma  comment(lib, "dxguid.lib")


DEFINE_GUID (IID_IDirectDraw2, 0xB3A6F3E0,0x2B43,0x11CF,0xA2,0xDE,0x00,0xAA,0x00,0xB9,0x33,0x56);
DEFINE_GUID (IID_IDirectDrawSurface2, 0x57805885,0x6eec,0x11cf,0x94,0x41,0xa8,0x23,0x03,0xc1,0x0e,0x27);

#define   FOURCC_YV12 0x32315659   //   MAKEFOURCC('Y','V','1','2')   
#define   FOURCC_Y411 0x31313459   //   MAKEFOURCC('Y','4','1','1')   
#define   FOURCC_YUY2 0x32595559   //   MAKEFOURCC('Y','U','Y','2')   
#define   FOURCC_UYVY 0x59565955   //   MAKEFOURCC('U','Y','V','Y') 

#define   AUDIO_BAR_PERCENT  60

typedef struct 
{
	LPDIRECTDRAW7 ddraw;
	LPDIRECTDRAWSURFACE7 PrimarySurface;
	LPDIRECTDRAWCLIPPER clipper;
	LPDIRECTDRAWSURFACE7 BackSurface;
} DirectxInstance;


class CVideoPlayer
{
public:
	int             m_nBarWidth;//�������Ŀ��
	int             m_nPicWidth;//ͼ����
	int             m_nPicHeight;//ͼ��߶�
	CRect			m_clientRect;//��ʾ���ڵĴ�С
	CWnd*           m_pWindow;//������ָ��
	BOOL            m_bOriginalScale; //���ͼ���Ƿ�Զ������ʾ

	BOOL            m_bIsBroadcast;  //����Ƿ�����Ƶ�㲥


	int             m_nScreenOffset;//��Ļƫ��

	

public:
	CVideoPlayer(void);
	~CVideoPlayer(void);
	BOOL			InitialVideo(CWnd* pWnd, GUID* lpGuid,int nWidth, int nHeigh, int nTextHeigh, int nOffset,BOOL bOriginalScale,BOOL bIsBroadcast);
	BOOL 			FinitialVideo();//����ʼ��
	void			DrawPicture(unsigned char *buffer[3], int nLeftAudioBar, int nRightAudioBar);//��ʾͼ��֡
//	void	        DrawBlackPicture(unsigned char *buffer[3], int nLeftVolume, int nRightVolume);//������

private:
	DirectxInstance m_dxInstance;//directDraw����
//	CAudioBar       m_AudioBar;//��ɫ������

	int             m_nTextHeigh;//����߶�

	GUID*			m_lpGuid;

private:
	BOOL			ResetVideo(void);
	void			FfmpegDeinterlace(unsigned char* pImg[3], int nWidth, int nHeight);//����֯	
};
