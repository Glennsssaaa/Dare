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
	bool bToggleWater;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bToggleEarth;
	bool bHasLerped;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector WaterHitPosition;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bEnableWaterVfx;
	


private:

	//Line Trace Variables
	void LineTraceArc();
	FVector GravityOffset;
	FVector NextLocation;
	
	float Gravity = -980;
	bool bIsDrawing = false;
	FTimerHandle LineTraceTimer; 
	FCollisionQueryParams QueryParams;

	int AbilitySelected = 0;
};
