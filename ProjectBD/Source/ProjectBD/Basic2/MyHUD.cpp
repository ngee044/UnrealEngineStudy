// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Engine/StreamableManager.h"
#include "Engine/Texture2D.h"
#include "Engine/Canvas.h"

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	FStreamableManager Loader;
	FStringAssetReference CrosshairPath(TEXT("Texture2D'/Game/Ui/crosshair.crosshair'"));
	Crosshair = Loader.LoadSynchronous<UTexture2D>(CrosshairPath);
}

void AMyHUD::DrawHUD()
{
	Super::DrawHUD();

	if (Crosshair)
	{
		DrawTextureSimple(Crosshair,
			Canvas->SizeX / 2 - Crosshair->GetSizeX() / 2,
			Canvas->SizeY / 2 - Crosshair->GetSizeY() / 2);
	}

	//DrawTexture�� ��� �̹��� ©�� ����Ҽ� �ְ� ���� �Լ�
	//DrawTextureSimple �̹��� ��ü�� ����ϰڴٴ� �Լ�
	//Draw2DLine(100,100,600,100,FColor::Red);
}
