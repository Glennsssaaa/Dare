// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "APlayerCharacter.h"
#include "NiagaraComponent.h"
#include "ATankCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DARE_API AATankCharacter : public AAPlayerCharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AATankCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Input functions
	UFUNCTION()
	void TankCharge();

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// UFUNCTION()
	// void PlayerAbility() override;

	void AbilityOne() override;

	void AbilityTwo() override;


	UFUNCTION()
	void AimCharge();

	UFUNCTION()
	void Charge();

	UFUNCTION()
	void Rebuild();
	
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

protected:
	// Tank Charge Collision Hitbox
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBoxComponent* ChargeHitBox;

	// Used for player to aim the dash
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* DashAimArrowComponent;

	UPROPERTY()
	float ChargeDistance = 1500.f;

	UPROPERTY()
	class ARebuildableBase* Rebuildable;

	bool bIsInRebuildZone;

};
