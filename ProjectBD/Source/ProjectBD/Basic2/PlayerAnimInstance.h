// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="State")
	float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "State")
	uint8 bIsCrouch : 1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "State")
	uint8 bIsIronsight : 1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "State")
	float Direction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "State")
	float AimYaw;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "State")
	float AimPitch;
};
