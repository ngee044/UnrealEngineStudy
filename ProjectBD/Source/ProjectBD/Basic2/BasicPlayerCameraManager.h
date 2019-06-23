// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "BasicPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ABasicPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	ABasicPlayerCameraManager();

	virtual void BeginPlay() override;
	virtual void UpdateCamera(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float NormalIFOV = 90.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float IronsightFOV = 60.0f;

};