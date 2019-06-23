// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true; //Tick 함수 활성 이거 true 안하면 절대 틱함수 호출 안함
}

float UWeaponComponent::GetDamge() const
{
	return Damage;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UWeaponComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType,ThisTickFunction);
}
