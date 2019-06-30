// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Basic2/PlayerAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Basic2/BaiscPlayerController.h"
#include "Engine/World.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "BulletType.h"
#include "TimerManager.h"
#include "Basic2/FireCameraShake.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetSystemLibrary.h"

AMyPlayer::AMyPlayer()
{
	Weapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(),TEXT("WeaponHandR"));

	//재 설정하기 싫어서 하는거
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	SpringArm->SetRelativeLocation(FVector(0, 40, 80));
	SpringArm->TargetArmLength = 150.0f;
	SpringArm->bUsePawnControlRotation = true;

	GetCharacterMovement()->CrouchedHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	NormalSpringArmPosition = GetSpringArmPosition();
	CrouchedSpringArmPosition = NormalSpringArmPosition - FVector(0, 0, 44.0f);
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

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMyPlayer::Jump);
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
	bIsFire = true;
	OnFire();
}

void AMyPlayer::StopFire()
{
	bIsFire = false;
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
	//const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	//const FRotator Result = AimDirLS.Rotation();

	return ActorToWorld().InverseTransformVectorNoScale(GetBaseAimRotation().Vector()).Rotation();
}

void AMyPlayer::SetSpringArmPosition(const FVector & NewPosition)
{
	SpringArm->SetRelativeLocation(NewPosition);
}

FVector AMyPlayer::GetSpringArmPosition() const
{
	return SpringArm->GetRelativeTransform().GetLocation();
}

void AMyPlayer::OnFire()
{
	if (!bIsFire) return;

	FVector CameraLocation;
	FRotator CameraRotation;
	int32 SizeX;
	int32 SizeY;
	FVector CrosshairWorldLocation;
	FVector CrosshairWorldDirection;

	ABaiscPlayerController *pPC = Cast<ABaiscPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	if (pPC)
	{
		pPC->GetPlayerViewPoint(CameraLocation, CameraRotation);
		pPC->GetViewportSize(SizeX, SizeY);
		pPC->DeprojectScreenPositionToWorld(SizeX / 2 + FMath::RandRange(-10, 10), SizeY / 2,
			CrosshairWorldLocation, CrosshairWorldDirection);

		FVector TraceStart = CameraLocation;
		FVector TraceEnd = TraceStart + (CrosshairWorldDirection *90000.f); 

		//충돌 체크
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
		
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(this);
		FHitResult OutHit;

		//Camera로부터 화면 크로스헤어를 지나는 선을 가지고 충돌 체크
		bool bIsResult = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
			TraceStart,
			TraceEnd,
			ObjectTypes,
			false,
			IgnoreActors,
			EDrawDebugTrace::None,
			OutHit,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			5.0f);

		//Effect MuzzleFlash
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			MuzzleFlash, Weapon->GetSocketTransform(TEXT("MuzzleFlash")));
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound,
			Weapon->GetSocketLocation(TEXT("MuzzleFlash")));

		if (bIsResult) //Decal, HitEffect
		{
			TraceStart = Weapon->GetSocketLocation(TEXT("MuzzleFlash"));
			TraceEnd = TraceStart + ((OutHit.ImpactPoint - TraceStart) * 1.1f); //float 오차

			//총구 끝부터 충돌위치 재 체크
			bIsResult = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
				TraceStart,
				TraceEnd,
				ObjectTypes,
				false,
				IgnoreActors,
				EDrawDebugTrace::None,
				OutHit,
				true,
				FLinearColor::Red,
				FLinearColor::Green,
				5.0f);
			if (bIsResult)
			{
				if (OutHit.GetActor()->ActorHasTag(TEXT("Player")))
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
						BloodEffect, OutHit.ImpactPoint + (OutHit.ImpactNormal * 10.0f));
					UDecalComponent* MakedDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
						BloodDecal,
						FVector(5, 5, 5),
						OutHit.ImpactPoint,
						OutHit.ImpactNormal.Rotation(),
						10.0f);
					MakedDecal->SetFadeScreenSize(0.005f);
				}
				else
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
						HitEffect, OutHit.ImpactPoint + (OutHit.ImpactNormal * 10.0f));
					UDecalComponent* MakedDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
						BulletDecal,
						FVector(5, 5, 5),
						OutHit.ImpactPoint,
						OutHit.ImpactNormal.Rotation(),
						10.0f);
					MakedDecal->SetFadeScreenSize(0.005f);
				}
			}
			UGameplayStatics::ApplyPointDamage(OutHit.GetActor(), DamageAmout, -OutHit.ImpactNormal, OutHit, GetController(), this, UBulletType::StaticClass());
		}
	}

	FRotator CurrentRotation = GetControlRotation();
	CurrentRotation.Pitch += FMath::RandRange(0.5f, 1.0f);
	GetController()->SetControlRotation(CurrentRotation);

	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->PlayCameraShake(UFireCameraShake::StaticClass());

	GetWorldTimerManager().SetTimer(
		FireTimer,
		this,
		&AMyPlayer::OnFire,
		0.12f
	);
}

float AMyPlayer::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* RadialDamageEvent = (FRadialDamageEvent*)(&DamageEvent);
#if 0
		FRadialDamageEvent *DamageEvent = Cast< FRadialDamageEvent>(&DamageEvent);
		UBulletType * BulletType = Cast<UBulletType>(DamageEvent->DamageTypeClass);
		if (BulletType)
		{
			UE_LOG(LogClass, Warning, TEXT("%s by Attack 11"), *DamageCauser->GetName());
		}
#endif
	}
	else if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent *PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);
		if (PointDamageEvent->HitInfo.BoneName.Compare(TEXT("head")) == 0)
		{
			CurrentHP = 0;
			UE_LOG(LogClass, Warning, TEXT("head shot"));

		}
		else {
			CurrentHP -= DamageAmount;
			UE_LOG(LogClass, Warning, TEXT("nomal dmg"));

		}
		CurrentHP = FMath::Clamp<float>(CurrentHP, 0, 100);
		if (CurrentHP == 0)
		{
			//UAnimInstance *AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;
			//if (!AnimInstance->Montage_IsPlaying(DeathMontage))
			//{
			//	FString SectionName = FString::Printf(TEXT("Death%d"), FMath::RandRange(1, 3));
			//	PlayAnimMontage(DeathMontage, 1.0f, FName(*SectionName));
			//}

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			GetMesh()->SetSimulatePhysics(true);
			GetMesh()->AddImpulse(FVector(-PointDamageEvent->HitInfo.ImpactNormal), PointDamageEvent->HitInfo.BoneName);
		}
	}

	return DamageAmount;
}



