// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaiscPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ABaiscPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ABaiscPlayerController();

	virtual void BeginPlay() override;


};
