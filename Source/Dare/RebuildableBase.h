// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "RebuildableBase.generated.h"

UCLASS()
class DARE_API ARebuildableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARebuildableBase();

	UFUNCTION(BlueprintCallable)
	void ToggleHouseDestruction();

	UFUNCTION(BlueprintCallable)
	void ToggleFire();

	UFUNCTION(BlueprintCallable)
	void ToggleClean();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Destroyed static mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UStaticMeshComponent* DestroyedMesh;

	// Rebuilt static mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UStaticMeshComponent* RebuiltMesh;

	// Rebuilding Collision Area
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UBoxComponent* RebuildCollision;

	// House Collision Box
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UBoxComponent* HouseCollision;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UNiagaraSystem* FireVfx;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "House State")
	bool bIsOnFire;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "House State")
	bool bIsDestroyed;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "House State")
	bool bIsClean;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
