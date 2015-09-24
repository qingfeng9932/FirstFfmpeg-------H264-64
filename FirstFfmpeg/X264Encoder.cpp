#include "StdAfx.h"
#include "X264Encoder.h"

CX264Encoder::CX264Encoder(void)
{
}

CX264Encoder::~CX264Encoder(void)
{
}


void CX264Encoder::InitX264Encoder()
{
/*	x264_param_t st264Param;

	x264_param_default(&st264Param);

	st264Param.i_width=640;
	st264Param.i_height=480;
*/

	//
	int width = 480;
	int height = 272;
	int fps = 25;
	size_t yuv_size = width * height *3/2;
	x264_t *encoder;
	x264_picture_t pic_in,pic_out;
	int inf,outf;
	uint8_t *yuv_buffer;

	x264_param_t m_param;
	x264_param_default_preset(&m_param,"veryfast","zerolatency");
	m_param.i_threads = 1;
	m_param.i_width = width;
	m_param.i_height = height;
	m_param.i_fps_num = fps;
	m_param.i_bframe = 10;
	m_param.i_fps_den = 1;
	m_param.i_keyint_max = 25;
	m_param.b_intra_refresh = 1;
	m_param.b_annexb = 1;
	x264_param_apply_profile(&m_param,"high422");
	encoder = x264_encoder_open(&m_param);

	x264_encoder_parameters( encoder, &m_param );

	x264_picture_alloc(&pic_in, X264_CSP_I420, width, height);

	yuv_buffer = (uint8_t *)malloc(yuv_size);

	pic_in.img.plane[0] = yuv_buffer;
	pic_in.img.plane[1] = pic_in.img.plane[0] + width * height;
	pic_in.img.plane[2] = pic_in.img.plane[1] + width * height / 4;

	FILE* infile = fopen("ds_480x272.yuv","rb");
	FILE* outfile = fopen("out.h264","ab");
	if(!infile || !outfile)
	{
		printf("open file error\n");
		return ;
	}
	int64_t i_pts = 0;

	x264_nal_t *nals;
	int nnal;
	while (fread(yuv_buffer, 1,yuv_size, infile) > 0)
	{
		pic_in.i_pts = i_pts++;
		x264_encoder_encode(encoder, &nals, &nnal, &pic_in, &pic_out);
		x264_nal_t *nal;
		for (nal = nals; nal < nals + nnal; nal++) {
			fwrite(nal->p_payload, 1,nal->i_payload,outfile);
		}
	}
	x264_encoder_close(encoder);
	fclose(infile);
	fclose(outfile);
	free(yuv_buffer);


}	

void CX264Encoder::Open()
{
	
}


void CX264Encoder::Encoder()
{
	
}	

void CX264Encoder::Close()
{
	
}