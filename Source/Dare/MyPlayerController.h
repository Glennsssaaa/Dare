// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DARE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	// Begin Play Function
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	void UpdateMappings(FName DisplayName, FKey NewKey);

protected:
};
