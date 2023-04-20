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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
		float RotationSpeed = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
		float MovementSpeed = 5.0f;
    
    FVector PlayerDirection;
	FVector2D MoveValue;
private:
	void Move(const FInputActionValue& Value);
	
	//cube component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* CubeMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* DirectionArrowComponent;
};
