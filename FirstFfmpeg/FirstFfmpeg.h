// FirstFfmpeg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFirstFfmpegApp:
// �йش����ʵ�֣������ FirstFfmpeg.cpp
//

class CFirstFfmpegApp : public CWinApp
{
public:
	CFirstFfmpegApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()


private:

	void  InitFfmpegLib();
};

extern CFirstFfmpegApp theApp;