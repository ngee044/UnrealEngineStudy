// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicPlayerCameraManager.h"
#include "Basic2/BaiscPlayerController.h"
#include "Basic2/MyPlayer.h"
#include "GameFramework/SpringArmComponent.h"

ABasicPlayerCameraManager::ABasicPlayerCameraManager()
{

}

void ABasicPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();
}

void ABasicPlayerCameraManager::UpdateCamera(float DeltaTime)
{

	ABaiscPlayerController *pPlayerController = Cast<ABaiscPlayerController>(GetOwningPlayerController());

	if (pPlayerController)
	{
		AMyPlayer *Pawn = Cast<AMyPlayer>(pPlayerController->GetPawn());
		if(Pawn)
		{
			float TargetFOV = Pawn->bIronsight ? IronsightFOV : NormalIFOV;
			SetFOV(FMath::FInterpTo(GetFOVAngle(), TargetFOV, DeltaTime, 15.0f));
			
			FVector TargetPosition = Pawn->bIsCrouched ? Pawn->CrouchedSpringArmPosition : Pawn->NormalSpringArmPosition;
			Pawn->SetSpringArmPosition(FMath::VInterpTo(Pawn->GetSpringArmPosition(), TargetPosition, DeltaTime, 5.0f));
		}
	}

	Super::UpdateCamera(DeltaTime);
}
