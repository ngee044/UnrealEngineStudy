// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Basic2/PlayerAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"

AMyPlayer::AMyPlayer()
{
	Weapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(),TEXT("WeaponHandR"));

	//재 설정하기 싫어서 하는거
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	SpringArm->SetRelativeLocation(FVector(0, -30, 40));
	SpringArm->TargetArmLength = 150.0f;
	SpringArm->bUsePawnControlRotation = true;
}

void AMyPlayer::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyPlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyPlayer::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyPlayer::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyPlayer::Turn);

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &AMyPlayer::DoCrouch);
	//PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &AMyPlayer::DoCrouch);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMyPlayer::StartFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AMyPlayer::StopFire);

	PlayerInputComponent->BindAction(TEXT("lronsight"), IE_Pressed, this, &AMyPlayer::DoIronsight);
	PlayerInputComponent->BindAction(TEXT("lronsight"), IE_Released, this, &AMyPlayer::UndoIronsight);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AMyPlayer::Jump);
}

void AMyPlayer::MoveForward(float Value)
{
	if (Value != 0)
	{
		AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
}

void AMyPlayer::MoveRight(float Value)
{
	if (Value != 0)
	{
		AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
}

void AMyPlayer::LookUp(float Value)
{
	if (Value != 0)
	{
		AddControllerPitchInput(Value);
	}
}

void AMyPlayer::Turn(float Value)
{
	if (Value != 0)
	{
		AddControllerYawInput(Value);
	}
}

void AMyPlayer::DoCrouch()
{
	CanCrouch() ? Crouch() : UnCrouch();
}

void AMyPlayer::StartFire()
{
}

void AMyPlayer::StopFire()
{
}

void AMyPlayer::DoIronsight()
{
	bIronsight = true;
	SetWalkSpeed(IronsightSpeed);
}

void AMyPlayer::UndoIronsight()
{
	bIronsight = false;
	SetWalkSpeed(NormalSpeed);
}

void AMyPlayer::SetWalkSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

FRotator AMyPlayer::GetAimOffset() const
{
	//const FVector AimDirWS = GetBaseAimRotation().Vector();
	//const FVector AimDirLS = ActorToWorld().InverseTransformPositionNoScale(AimDirWS);
	//const FRotator Result = AimDirLS.Rotation();

	return ActorToWorld().InverseTransformVectorNoScale(GetBaseAimRotation().Vector()).Rotation();
}
