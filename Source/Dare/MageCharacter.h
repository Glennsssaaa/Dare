// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "NiagaraSystem.h"
#include "MageCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DARE_API AMageCharacter : public APlayerCharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AMageCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category="Water")
	UNiagaraSystem* WaterEffect;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Interact(const FInputActionValue& Value) override;
	virtual void AbilityOne() override;
	virtual void AbilityTwo() override;
	
	
	UFUNCTION(BlueprintImplementableEvent)
		void DrawFunc(AActor* hitActor, FVector2D hitUV);

	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector WaterHitPosition;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bEnableWaterVFX;
	
	bool bHasLerped;
private:

	//Line Trace Variables
	void LineTraceArc();
	FVector GravityOffset;
	FVector NextLocation;
	float gravity = -980;
	bool isDrawing = false;
	FTimerHandle lineTraceTimer; 
	FCollisionQueryParams QueryParams;

	bool bToggleWater;
	bool bToggleEarth;
	int AbilitySelected = 0;
};
