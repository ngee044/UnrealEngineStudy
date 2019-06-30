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

	virtual void BeginPlay() override;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIronsight = false;
	
	float NormalSpeed = 600.0f;
	float IronsightSpeed = 300.0f;
	float CrouchSpeed = 300.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWeaponComponent *Weapon;

	UFUNCTION(BlueprintCallable)
	FRotator GetAimOffset() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector NormalSpringArmPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector CrouchedSpringArmPosition;

	UFUNCTION(BlueprintCallable)
	void SetSpringArmPosition(const FVector &NewPosition);

	UFUNCTION(BlueprintCallable)
	FVector GetSpringArmPosition() const;

	UFUNCTION(BlueprintCallable)
	void OnFire();


	FTimerHandle FireTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UMaterialInterface* BulletDecal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UMaterialInterface* BloodDecal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	float DamageAmout = 30.0f;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage *DeathMontage;
};
