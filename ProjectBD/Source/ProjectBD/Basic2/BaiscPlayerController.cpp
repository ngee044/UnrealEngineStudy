// Fill out your copyright notice in the Description page of Project Settings.


#include "BaiscPlayerController.h"
#include "Basic2/BasicPlayerCameraManager.h"

ABaiscPlayerController::ABaiscPlayerController()
{
	PlayerCameraManagerClass = ABasicPlayerCameraManager::StaticClass();
}

void ABaiscPlayerController::BeginPlay()
{
	Super::BeginPlay();
}
