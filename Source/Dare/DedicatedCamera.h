// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DedicatedCamera.generated.h"

UCLASS()
class DARE_API ADedicatedCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADedicatedCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBoxComponent* Box;


protected:
	// Player Character pointers
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ACharacter* MageCharacter;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ACharacter* TankCharacter;

	// Minimum and Maximum zoom distance
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MinZoomDistance = 4000.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxZoomDistance = 15000.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ZoomAddition = 500.f;
	
	FTimerHandle StartTimer;

};
