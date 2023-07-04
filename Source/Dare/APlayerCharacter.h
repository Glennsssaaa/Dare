// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "APlayerCharacter.generated.h"

UCLASS()
class DARE_API AAPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
		class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Input")
		class UInputConfigData* InputActions;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void PlayerAbility();

	UFUNCTION()
	virtual void AbilityOne();

		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	float RotationSpeed = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	float MovementSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	FVector2D LookValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AActor* InteractableActor;
	
    FVector PlayerDirection;
	FVector2D MoveValue;

	// Player Dash
	void PlayerDash();

	// Getters and Setters
	bool GetIsDashing() { return bIsPlayerDashing; };

protected:

	//Dash Variables
	// Dash Timer
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
	float DashDistance = 650.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashSpeed = 1.35f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	FVector PredictedLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	bool bIsPlayerDashing = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	FTimerHandle DashCooldownTimerHandle;

	bool isAbility = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bCanPlayerMove = true;
	
protected:
	void KeyboardMove(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void Aim(const FInputActionValue& Value);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PlayerMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* DirectionArrowComponent;



};
