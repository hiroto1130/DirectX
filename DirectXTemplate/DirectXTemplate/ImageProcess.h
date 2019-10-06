#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H

#include"Main.h"
#include"DrawTexture.h"
#include <math.h>

class ImageProcess
{
private:
	int CentralX;
	int CentralY;
	float Cos;
	float Sin;


public:
	// コンストラクタ
	ImageProcess();

	// デストラクタ
	~ImageProcess();


	// 平行移動
	void ParallelDisplacement(int TextureX, int TextureY, int Tx, int Ty, Number* number);

	// 回転
	void Rotation(int Angle,Texture::TEXTUREDATAEX texturedataEx[], Texture::TEXTUREDATAEX DrawDataEx[]);
	
	// 拡縮
	void ScaleTrans(float Scale, int* TextureX, int* TextureY, int* TextureWidth, int* TextureHight);
};

#endif