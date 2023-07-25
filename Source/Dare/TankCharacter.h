// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "NiagaraComponent.h"
#include "TankCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DARE_API ATankCharacter : public APlayerCharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	ATankCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Tank Charge Ability
	virtual void AbilityOne() override;

	// Tank Rebuild Ability
	virtual void AbilityTwo() override;
	
	UFUNCTION()
	void Charge();

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	virtual void Interact(const FInputActionValue& Value);

protected:
	// Tank Charge Collision Hitbox
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBoxComponent* ChargeHitBox;

	// Used for player to aim the dash
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* DashAimArrowComponent;

	UPROPERTY()
	float ChargeDistance = 1500.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ChargeSpeed = 1.f;
	
	UPROPERTY()
	class ARebuildableBase* Rebuildable;

	UPROPERTY()
	bool bIsInRebuildZone;


private:
	class AMageCharacter* PickupablePlayer;

	void ThrowItem();
};