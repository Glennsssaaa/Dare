//Tank Character Class, inherits from PlayerCharacter
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

	// Tank Charge Ability
	UFUNCTION()
	void Charge();

	//Overlap Functions
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	//Interact Function
	virtual void Interact(const FInputActionValue& Value);

protected:
	// Tank Charge Collision Hitbox
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBoxComponent* ChargeHitBox;

	// Used for player to aim the dash
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* DashAimArrowComponent;

	//Distance of the charge ability
	UPROPERTY()
	float ChargeDistance = 1500.f;

	//Speed of the charge ability
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ChargeSpeed = 1.f;

	//Rebuildable Object
	UPROPERTY()
	class ARebuildableBase* Rebuildable;

	//Timer Handle for charge ability (Unused)
	UPROPERTY()
	FTimerHandle ChargeTestTimer;

	//If player is within the rebuild zone of the rebuildable object
	UPROPERTY()
	bool bIsInRebuildZone;

	//If player is charging
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCharging = false;

	//If player can charge
	UPROPERTY()
	bool bCanPlayerCharge = true;

	
private:
	//Mage character for throwing
	class AMageCharacter* PickupablePlayer;
	//Throwing function
	void ThrowItem();
};