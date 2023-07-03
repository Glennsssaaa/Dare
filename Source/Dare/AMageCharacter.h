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
	void Interact(const FInputActionValue& Value) override;
	void InteractEnd(const FInputActionValue& Value) override;

	UFUNCTION(BlueprintImplementableEvent)
		void DrawFunc(FHitResult Hit, FVector2D hitUV);

private:

	//Line Trace Variables
	void LineTraceArc();
	FVector GravityOffset;
	FVector NextLocation;
	float gravity = -980;
	bool isDrawing = false;
	FTimerHandle lineTraceTimer; 
	FCollisionQueryParams QueryParams;

	int AbilitySelected = 0;
};
