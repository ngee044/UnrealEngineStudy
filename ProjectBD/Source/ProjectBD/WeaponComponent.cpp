// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true; //Tick �Լ� Ȱ�� �̰� true ���ϸ� ���� ƽ�Լ� ȣ�� ����
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
