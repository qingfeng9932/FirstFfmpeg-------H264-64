// FirstFfmpegDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FirstFfmpeg.h"
#include "FirstFfmpegDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CFirstFfmpegDlg 对话框




CFirstFfmpegDlg::CFirstFfmpegDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFirstFfmpegDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	InitializeCriticalSection(&m_csFrameQueue);
}

void CFirstFfmpegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFirstFfmpegDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CFirstFfmpegDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFirstFfmpegDlg::OnBnClickedButton2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CFirstFfmpegDlg 消息处理程序

BOOL CFirstFfmpegDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFirstFfmpegDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFirstFfmpegDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CFirstFfmpegDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFirstFfmpegDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	
	unsigned int nDecoderThreadID=0;
	m_hDecoderThread=(HANDLE)_beginthreadex(NULL,0,&DecoderThread,(void*)this,0,&nDecoderThreadID);

//	unsigned int nEncoderThreadID=0;
//	m_hEncoderThread=(HANDLE)_beginthreadex(NULL,0,&EncoderThread,(void*)this,0,&nEncoderThreadID);

	unsigned int nX264EncoderThreadID=0;
	m_hEncoderThread=(HANDLE)_beginthreadex(NULL,0,&X264EncoderThread,(void*)this,0,&nX264EncoderThreadID);

	
	int n=0;
}

void CFirstFfmpegDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CFirstFfmpegDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	DeleteCriticalSection(&m_csFrameQueue);

	CDialog::OnClose();
}

BOOL g_bStop=FALSE;

unsigned __stdcall CFirstFfmpegDlg::DecoderThread(void* pParam)
{
	CFirstFfmpegDlg *pDlg=(CFirstFfmpegDlg*)pParam;

	AVFormatContext *pAVFormatCtx=NULL;
	AVInputFormat *fmt=NULL;
	AVDictionary *options=NULL;

	const char* pFilePathName="I:/码流文件/fashion.ts";

	//	pAVFormatCtx=avformat_alloc_context();

	//	AVFormatContext **ps, const char *filename, AVInputFormat *fmt, AVDictionary **options
	if(avformat_open_input(&pAVFormatCtx,pFilePathName,NULL,NULL) < 0)
	{
		ASSERT(FALSE);
	}

	if(avformat_find_stream_info(pAVFormatCtx,NULL) < 0)
	{
		ASSERT(FALSE);
	}

	int nVideoIndex=-1;

	for (int i=0;i<pAVFormatCtx->nb_streams;i++)
	{
		if (pAVFormatCtx->streams[i]->codec->codec_id == AVCodecID::AV_CODEC_ID_MPEG2VIDEO)
		{
			nVideoIndex=i;
			break;
		}
	}

	AVCodecContext* pAVCodecContext=pAVFormatCtx->streams[nVideoIndex]->codec;
	AVCodec* pAVCodec=avcodec_find_decoder(pAVCodecContext->codec_id);

	if(avcodec_open2(pAVCodecContext,pAVCodec,NULL) != 0)
	{
		ASSERT(FALSE);
	}

	AVFrame* pFrame=av_frame_alloc();
	AVFrame* pYUVFrame=av_frame_alloc();

	AVPacket *pPacket=(AVPacket *)av_malloc(sizeof(AVPacket));
	av_init_packet(pPacket);

	int nCount=0;

	while (av_read_frame(pAVFormatCtx,pPacket) >= 0)
	{
		if (pPacket->stream_index == nVideoIndex)
		{
			int nGopPtr=0;

			int nRet=avcodec_decode_video2(pAVCodecContext,pFrame,&nGopPtr,pPacket);
			if (nGopPtr != 0)
			{

				MyFrame stMyFrame;
				stMyFrame.nYLen=pAVCodecContext->width*pAVCodecContext->height;
				stMyFrame.pYData=new unsigned char[stMyFrame.nYLen];

				stMyFrame.nULen=pAVCodecContext->width*pAVCodecContext->height/4;
				stMyFrame.pUData=new unsigned char[stMyFrame.nULen];

				stMyFrame.nVLen=pAVCodecContext->width*pAVCodecContext->height/4;
				stMyFrame.pVData=new unsigned char[stMyFrame.nVLen];


				//拷贝
				int nTotalLen=0;
				for (int i=0;i<pAVCodecContext->height;i++)
				{
					memcpy(stMyFrame.pYData+nTotalLen,pFrame->data[0]+i*pFrame->linesize[0],pAVCodecContext->width);
					nTotalLen+=pAVCodecContext->width;
				}

				nTotalLen=0;
				for (int i=0;i<pAVCodecContext->height/2;i++)
				{
					memcpy(stMyFrame.pUData+nTotalLen,pFrame->data[1]+i*pFrame->linesize[1],pAVCodecContext->width/2);
					nTotalLen+=pAVCodecContext->width/2;
				}

				nTotalLen=0;
				for (int i=0;i<pAVCodecContext->height/2;i++)
				{
					memcpy(stMyFrame.pVData+nTotalLen,pFrame->data[2]+i*pFrame->linesize[2],pAVCodecContext->width/2);
					nTotalLen+=pAVCodecContext->width/2;
				}

				//向队列添加节点
				EnterCriticalSection(&pDlg->m_csFrameQueue);
				int nFramePktCount=pDlg->m_frameQueue.size();

				if (nFramePktCount > 5)
				{
		//			TRACE("队列中堆积的数据包数量超过5个\n");
				}

				nCount++;

				TRACE("已解码数据包 %d 个,队列中剩余 %d 个\n",nCount,nFramePktCount);

				pDlg->m_frameQueue.push(stMyFrame);

				LeaveCriticalSection(&pDlg->m_csFrameQueue);


			}
		}
	}

	g_bStop=TRUE;

//	fclose(pYUVFile);

	avformat_free_context(pAVFormatCtx);
	pAVFormatCtx=NULL;

	av_frame_free(&pFrame);
	pFrame=NULL;

	av_free((AVPacket *)pPacket);
	pPacket=NULL;


	return 0;
}


void  CFirstFfmpegDlg::InitX264Encoder()
{
	
}

void  CFirstFfmpegDlg::OpenX264()
{
	
}

unsigned __stdcall CFirstFfmpegDlg::X264EncoderThread(void* pParam)
{
	CFirstFfmpegDlg *pDlg=(CFirstFfmpegDlg*)pParam;
	

	int width = 720;
	int height = 576;
	int fps = 25;
	size_t yuv_size = width * height *3/2;
	x264_t *encoder;
	x264_picture_t pic_in,pic_out;
	int inf,outf;
	uint8_t *yuv_buffer;

	x264_param_t m_param;
	x264_param_default(&m_param);	
	x264_param_default_preset(&m_param,"veryfast","zerolatency");
//	x264_param_default_preset(&m_param,"slow","zerolatency");


	m_param.i_width=width;
	m_param.i_height=height;
	m_param.i_fps_num=25;
	m_param.i_fps_den=1;
	m_param.i_threads=1;		/*X264_THREADS_AUTO*/


//	m_param.i_threads = 4;
	m_param.i_bframe = 10;
	// 	m_param.i_fps_den = 1;
	m_param.i_keyint_max = 25;
	m_param.b_intra_refresh = 1;
	m_param.b_annexb = 1;


 //	x264_param_default_preset(&m_param,"ultrafast","zerolatency");
// 
// 	m_param.i_threads = 1;
// 
// 	m_param.i_bframe = 10;
// 	m_param.i_fps_den = 1;
// 	m_param.i_keyint_max = 25;
// 	m_param.b_intra_refresh = 1;
// 	m_param.b_annexb = 1;

//	m_param.i_level_idc

	x264_param_apply_profile(&m_param,"main");




	//////////////////////////////////////////////////////////////////////////
	

// 	m_param.i_threads = 1;
// 	m_param.i_width = width;
// 	m_param.i_height = height;
// 	m_param.i_fps_num = fps;
// 	m_param.i_bframe = 10;
// 	m_param.i_fps_den = 1;
// 	m_param.i_keyint_max = 25;
// 	m_param.b_intra_refresh = 1;
// 	m_param.b_annexb = 1;
// 	x264_param_apply_profile(&m_param,"high422");

	//


	encoder = x264_encoder_open(&m_param);

	x264_encoder_parameters( encoder, &m_param );

	x264_picture_alloc(&pic_in, X264_CSP_I420, width, height);

	yuv_buffer = (uint8_t *)malloc(yuv_size);

	pic_in.img.plane[0] = yuv_buffer;
	pic_in.img.plane[1] = pic_in.img.plane[0] + width * height;
	pic_in.img.plane[2] = pic_in.img.plane[1] + width * height / 4;

//	FILE* infile = fopen("ds_480x272.yuv","rb");
	FILE* outfile = fopen("d:\\out.h264","wb");
	if( !outfile)
	{
		printf("open file error\n");
		return 0;
	}
	int64_t i_pts = 0;

	x264_nal_t *nals;
	int nnal;

	DWORD dwStart=GetTickCount();

	int nCounter=0;

	while (1)
	{
		
		
		EnterCriticalSection(&pDlg->m_csFrameQueue);
		int nFramePktCount=pDlg->m_frameQueue.size();

		if (nFramePktCount > 5)
		{
			TRACE("~~~~~~~~~~~~~~~~~``````队列中堆积的数据包数量 %d 个\n",nFramePktCount);
		}

		LeaveCriticalSection(&pDlg->m_csFrameQueue);

		if (g_bStop && nFramePktCount == 0)
		{
			break;
		}

		if (nFramePktCount == 0)
		{
			if (g_bStop)
			{
				break;
			}
			
			Sleep(50);
			continue;
		}

//		TRACE("-------------已编码 %d 帧图像 -------------\n");

		MyFrame stMyFrame;

		EnterCriticalSection(&pDlg->m_csFrameQueue);

		stMyFrame=pDlg->m_frameQueue.front();
		pDlg->m_frameQueue.pop();

		LeaveCriticalSection(&pDlg->m_csFrameQueue);
		
		memcpy(yuv_buffer,stMyFrame.pYData,stMyFrame.nYLen);
		memcpy(yuv_buffer+stMyFrame.nYLen,stMyFrame.pUData,stMyFrame.nULen);
		memcpy(yuv_buffer+stMyFrame.nYLen+stMyFrame.nULen,stMyFrame.pVData,stMyFrame.nVLen);

		pic_in.i_pts = i_pts++;
		x264_encoder_encode(encoder, &nals, &nnal, &pic_in, &pic_out);
		x264_nal_t *nal;
		for (nal = nals; nal < nals + nnal; nal++)
		{
			fwrite(nal->p_payload, 1,nal->i_payload,outfile);
		}

		nCounter++;
		if (nCounter >= 1000)
		{
//			break;
		}
	
	}

	DWORD dwEnd=GetTickCount();

	DWORD dwTimeDiff=(dwEnd-dwStart)/1000;

	TRACE("编码总共耗时 %d 秒",dwTimeDiff);

	x264_encoder_close(encoder);
//	fclose(infile);
	fclose(outfile);
	free(yuv_buffer);

	return 0;
}

unsigned __stdcall CFirstFfmpegDlg::EncoderThread(void* pParam)
{
	CFirstFfmpegDlg *pDlg=(CFirstFfmpegDlg*)pParam;

	const char* pH264File="D:/264.h264";

	AVFormatContext* pEncoderFormatCtx=avformat_alloc_context();; 
	AVOutputFormat* pOutPutFormat=av_guess_format(NULL,pH264File,NULL);
	pEncoderFormatCtx->oformat=pOutPutFormat;

	if (avio_open(&(pEncoderFormatCtx->pb),pH264File,AVIO_FLAG_READ_WRITE) < 0)
	{
		return 0;
	}

	AVStream *pStream=avformat_new_stream(pEncoderFormatCtx,NULL);
	pStream->time_base.num=0;
	pStream->time_base.den=0;

	AVCodecContext *pContext=NULL;
	pContext=pStream->codec;
	pContext->codec_id=pOutPutFormat->video_codec;
	pContext->codec_type=AVMEDIA_TYPE_VIDEO;
	pContext->pix_fmt=PIX_FMT_YUV420P;
	pContext->width=480;
	pContext->height=272;
	pContext->time_base.num=1;
	pContext->time_base.den=25;
	pContext->bit_rate=400000;
	pContext->gop_size=250;	

	pContext->qmin=10;
	pContext->qmax=51;

	pContext->max_b_frames=3;


	AVDictionary *param = 0;  
	//H.264  
	if(pContext->codec_id == AV_CODEC_ID_H264) {  
		av_dict_set(&param, "preset", "slow", 0);  
		av_dict_set(&param, "tune", "zerolatency", 0);  
		//av_dict_set(¶m, "profile", "main", 0);  
	}  
	//H.265  
	if(pContext->codec_id == AV_CODEC_ID_H265){  
		av_dict_set(&param, "preset", "ultrafast", 0);  
		av_dict_set(&param, "tune", "zero-latency", 0);  
	}  

	//Show some Information  
	av_dump_format(pEncoderFormatCtx, 0, pH264File, 1);  

	AVCodec* pCodec = avcodec_find_encoder(pContext->codec_id);  

//	AVCodec* pCodec = avcodec_find_encoder(CODEC_ID_MPEG4); 
///	AVCodec* pCodec = avcodec_find_encoder(AV_CODEC_ID_AAC); 
//	AVCodec* pCodec = avcodec_find_encoder(AV_CODEC_ID_H264); 
	if (!pCodec)
	{  
		printf("Can not find encoder! \n");  
		return -1;  
	}  

	if (avcodec_open2(pContext, pCodec,&param) < 0){  
		printf("Failed to open encoder! \n");  
		return -1;  
	}

	//Write File Header  
	avformat_write_header(pEncoderFormatCtx,NULL);  
int picture_size = avpicture_get_size(pContext->pix_fmt, pContext->width, pContext->height); 
	uint8_t* picture_buf = (uint8_t *)av_malloc(picture_size);  

	AVFrame* pFrame = av_frame_alloc();  

	avpicture_fill((AVPicture *)pFrame, picture_buf, pContext->pix_fmt, pContext->width, pContext->height);

	 

	AVPacket pkt;
	av_new_packet(&pkt,picture_size); 

	int nEnFrameCount=0;

	while(1)
	{
		EnterCriticalSection(&pDlg->m_csFrameQueue);
		int nFramePktCount=pDlg->m_frameQueue.size();

		if (nFramePktCount > 5)
		{
	//		TRACE("队列中堆积的数据包数量超过5个\n");
		}

		LeaveCriticalSection(&pDlg->m_csFrameQueue);

		if (nFramePktCount == 0)
		{
			Sleep(50);
			continue;
		}

		MyFrame stMyFrame;

		EnterCriticalSection(&pDlg->m_csFrameQueue);
		
		stMyFrame=pDlg->m_frameQueue.front();
		pDlg->m_frameQueue.pop();

		LeaveCriticalSection(&pDlg->m_csFrameQueue);
		
		//
		for (int i=0; i<nFramePktCount; i++)
		{  
			
			pFrame->data[0] = stMyFrame.pYData;              // Y  
			pFrame->data[1] = stMyFrame.pUData;      // U   
			pFrame->data[2] = stMyFrame.pVData;  // V  
			//PTS  
			pFrame->pts=i;  
			int got_picture=0;  
			//Encode  
			int ret = avcodec_encode_video2(pContext, &pkt,pFrame, &got_picture);  
		
			if(ret < 0)
			{  
				TRACE("Failed to encode! \n");  
				return -1;  
			}  

			if (got_picture==1)
			{  
				TRACE("Succeed to encode frame: %5d\n",nEnFrameCount);  
	//			framecnt++;  
				pkt.stream_index = pStream->index;  
				ret = av_write_frame(pEncoderFormatCtx, &pkt);  
				av_free_packet(&pkt);  

				nEnFrameCount++;
			}  
		} 

		

		if (nEnFrameCount >= 250*6)
		{
			break;
		}

		delete[] stMyFrame.pYData;
		delete[] stMyFrame.pUData;
		delete[] stMyFrame.pVData;
		

		
	
	}


	av_write_trailer(pEncoderFormatCtx);
	
	if (pStream != NULL)
	{  
		avcodec_close(pStream->codec);  
		av_free(pFrame);  
		av_free(picture_buf);  
	} 


	avio_close(pEncoderFormatCtx->pb);  



	avformat_free_context(pEncoderFormatCtx);
	pEncoderFormatCtx=NULL;

	

	return 0;
}