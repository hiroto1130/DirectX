#include"ImageProcess.h"

ImageProcess::ImageProcess()
{


}

ImageProcess::~ImageProcess()
{


}

// 平行移動
void ImageProcess::ParallelDisplacement(int TextureX,int TextureY,int Tx,int Ty,Number *number)
{
	number->TextureX = TextureX + Tx;
	number->TextureY = TextureY + Ty;

}

// 回転
void ImageProcess::Rotation(int Angle, Texture::TEXTUREDATAEX DrawDataEx[])
{
	
	CentralX = 250;
	CentralY = 250;

	Texture::TEXTUREDATAEX TextureDataEx[4] =
	{
	{ 0 - CentralX , 0 - CentralY , 0 , 0 , 0 },
	{ 0 + CentralX , 0 - CentralY , 0 , 1 , 0 },
	{ 0 + CentralX , 0 + CentralY , 0 , 1 , 1 },
	{ 0 - CentralX , 0 + CentralY , 0 , 0 , 1 },
	};

	Cos = cos(Angle);
	Sin = sin(Angle);

	for (int a = 0; a < 4; a++)
	{
		DrawDataEx[a].X = 400 + (TextureDataEx[a].X * Cos - TextureDataEx[a].Y * Sin);
		DrawDataEx[a].Y = 300 + (TextureDataEx[a].X * Sin + TextureDataEx[a].Y * Cos);
	}

}


// 拡縮
void ImageProcess::ScaleTrans(float Scale, int* TextureX, int* TextureY, int* TextureWidth, int* TextureHight)
{
	// 中心座標の選択
	CentralX = ( *TextureWidth / 2 ) + *TextureX ;
	CentralY = ( *TextureHight / 2 ) + *TextureY ;

	// 拡縮処理
	*TextureHight *= Scale;
	*TextureWidth *= Scale;

	// 頂点変更
	*TextureX = CentralX - ( *TextureWidth / 2 );
	*TextureY = CentralY - ( *TextureHight / 2 );
}