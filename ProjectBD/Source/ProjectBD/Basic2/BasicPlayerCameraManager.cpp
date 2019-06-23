// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicPlayerCameraManager.h"
#include "Basic2/BaiscPlayerController.h"
#include "Basic2/MyPlayer.h"

ABasicPlayerCameraManager::ABasicPlayerCameraManager()
{

}

void ABasicPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();
}

void ABasicPlayerCameraManager::UpdateCamera(float DeltaTime)
{

	ABaiscPlayerController *pPlayerCnt = Cast<ABaiscPlayerController>(GetOwningPlayerController());

	if (pPlayerCnt)
	{
		AMyPlayer *Pawn = Cast<AMyPlayer>(pPlayerCnt->GetPawn());
		if(Pawn)
		{
			float TargetFOV = Pawn->bIronsight ? IronsightFOV : NormalIFOV;
			
			SetFOV(FMath::FInterpTo(GetFOVAngle(), TargetFOV, DeltaTime, 15.0f));
		}
	}

	Super::UpdateCamera(DeltaTime);
}
