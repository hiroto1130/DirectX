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
void ImageProcess::Rotation(int Angle, Texture::TEXTUREDATAEX TextureDataEx[], Texture::TEXTUREDATAEX DrawDatraEx[])
{
	CentralX = DrawDatraEx[0].X + ((DrawDatraEx[1].X - DrawDatraEx[0].X) / 2);
	CentralY = DrawDatraEx[0].Y + ((DrawDatraEx[2].Y - DrawDatraEx[0].Y) / 2);
	
	Cos = cos(Angle);
	Sin = sin(Angle);

	for (int a = 0; a < 4; a++)
	{
		TextureDataEx[a].X = TextureDataEx[a].X * Cos - TextureDataEx[a].Y * Sin;
		TextureDataEx[a].Y = TextureDataEx[a].X * Sin + TextureDataEx[a].Y * Cos;

		TextureDataEx[a].X = TextureDataEx[a].X + CentralX;
		TextureDataEx[a].Y = TextureDataEx[a].Y + CentralY;
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