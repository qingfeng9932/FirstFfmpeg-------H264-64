#pragma once


class CX264Encoder
{
public:
	CX264Encoder(void);
	~CX264Encoder(void);

public:
	
	void InitX264Encoder();
	void Open();
	void Encoder();
	void Close();

private:


private:



};
