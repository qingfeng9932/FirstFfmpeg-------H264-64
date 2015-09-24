// FirstFfmpegDlg.h : ͷ�ļ�
//

#pragma once

#include "global.h"


// CFirstFfmpegDlg �Ի���
class CFirstFfmpegDlg : public CDialog
{
// ����
public:
	CFirstFfmpegDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FIRSTFFMPEG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
