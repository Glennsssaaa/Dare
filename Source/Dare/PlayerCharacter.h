// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class DARE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputConfigData* InputActions;

	UFUNCTION()
	virtual void AbilityOne();
	
	UFUNCTION()
	virtual void AbilityTwo();

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	void UpdateMappings(FText DisplayName, FKey NewKey);
	
	// Control Variables 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	float RotationSpeed = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	float MovementSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	bool bUsingKeyboard;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	FVector2D LookValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	TArray<TEnumAsByte<EObjectTypeQuery>> mousehitObjs;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* InteractableActor;
	
    FVector PlayerDirection;
	FVector2D MoveValue;
	FVector MousePosition;
	FVector MouseDirection;
	FRotator playerDirection;
	FHitResult MouseHit;

	FVector RespawnPos;
	
	int SelectedAbility=1;
	
	bool bToggleInteract = false;
	bool bCanInteract=true;
	
	// Player Dash
	UFUNCTION()
	void PlayerDash();

	// Player Keyboard movement
	void KeyboardMove(const FInputActionValue& Value);
	
	virtual void Interact(const FInputActionValue& Value);
	
	void Aim(const FInputActionValue& Value);
	
	void ThrowItem();
	
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlayerMesh;

	void SetIsRotating(){bIsPlayerRotating = false;}
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool bPlayerFrozen;
	
protected:
	
	// Dash Cooldown Timer
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashCooldown = 0.f;

	// Default Dash Cooldown
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashCooldownDefault = 3.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	int DashCharges = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	int DashChargesMax = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashDistance = 1000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashSpeed = 1.75f;

	// Dash predicted location if there is no collision in the way
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	FVector PredictedLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	bool bIsPlayerDashing = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	FTimerHandle DashCooldownTimerHandle;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanPlayerMove = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsAbility = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* PlayerSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Components", meta = (AllowPrivateAccess="true"))
	class UBoxComponent* InteractCollision;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsHoldingItem = false;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsPlayerRotating = false;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	FVector TargetLocation;
	

	UPROPERTY()
	class AInteractableObject* OverlappedObject;

	class APickupItem* PickupableItem;

};
