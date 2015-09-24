
typedef struct _MyFrame
{
	unsigned char* pYData;
	unsigned int nYLen;

	unsigned char* pUData;
	unsigned int nULen;

	unsigned char* pVData;
	unsigned int nVLen;

}MyFrame,*PMyFrame;

#include <queue>
using namespace std;

typedef queue<MyFrame>MyFrameQueue;

#include <process.h>
//#include "X264Encoder.h"



