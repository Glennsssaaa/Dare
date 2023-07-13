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
	float totalPoints=0;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float currentPoints=0;
	
	void UpdateScore(float objPoints);

	UFUNCTION(BlueprintImplementableEvent)
	void UIUpdate();
};
