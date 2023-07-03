// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "APlayerCharacter.h"
#include "AMageCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DARE_API AAMageCharacter : public AAPlayerCharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AAMageCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	virtual void Interact(const FInputActionValue& Value);

	void LineTraceArc();
	int AbilitySelected = 0;
};
