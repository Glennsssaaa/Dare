// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DareGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DARE_API ADareGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	float points;
public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxPoints=0;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float CurrentPoints=0;

	UFUNCTION(BlueprintCallable)
	void UpdateScore(float objPoints);
	UFUNCTION(BlueprintCallable)
	void MageTutorialObject();
	
	UFUNCTION(BlueprintImplementableEvent)
	void UIUpdate();

	UFUNCTION(BlueprintImplementableEvent)
	void MageTutorialUpdate();
	UFUNCTION(BlueprintImplementableEvent)
	void TankTutorialUpdate();
	
	int count;
};
