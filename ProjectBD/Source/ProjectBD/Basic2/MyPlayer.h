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
	AMyPlayer();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	
	void DoCrouch();
	
	void StartFire();
	void StopFire();

	void DoIronsight();
	void UndoIronsight();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bIsFire : 1;

	UFUNCTION(BlueprintCallable)
	void SetWalkSpeed(float NewSpeed);

	bool bIronsight = false;
	float NormalSpeed = 600.0f;
	float IronsightSpeed = 300.0f;
	float CrouchSpeed = 300.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWeaponComponent *Weapon;

	UFUNCTION(BlueprintCallable)
	FRotator GetAimOffset() const;
};
