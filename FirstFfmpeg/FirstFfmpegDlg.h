// FirstFfmpegDlg.h : 头文件
//

#pragma once

#include "global.h"


// CFirstFfmpegDlg 对话框
class CFirstFfmpegDlg : public CDialog
{
// 构造
public:
	CFirstFfmpegDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FIRSTFFMPEG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

private:

public:


	afx_msg void OnBnClickedButton2();


private:

	CRITICAL_SECTION m_csFrameQueue;
	MyFrameQueue m_frameQueue;


	static unsigned __stdcall DecoderThread(void* pParam);
	HANDLE m_hDecoderThread;

	static unsigned __stdcall EncoderThread(void* pParam);
	HANDLE m_hEncoderThread;

	static unsigned __stdcall X264EncoderThread(void* pParam);
	HANDLE m_hX264EncoderThread;

//	CX264Encoder m_264Encoder;

private:

	void  InitX264Encoder();
	void  OpenX264();


public:
	afx_msg void OnClose();
};
