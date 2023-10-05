// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "GrowingObject.generated.h"

/**
 * 
 */
UCLASS()
class DARE_API AGrowingObject : public AInteractableObject
{
	GENERATED_BODY()
	AGrowingObject();
public:
	virtual void Tick(float DeltaTime);
	virtual void Interact();
	void Grow();

	UPROPERTY(BlueprintReadOnly)
	bool hasGrown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bForTutorial = false;
	
private:
	FTimerHandle growthTimer; 
};
