// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Basic2/MyCharacter.h"
#include "MyPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API AMyPlayer : public AMyCharacter
{
	GENERATED_BODY()
public:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	
};
