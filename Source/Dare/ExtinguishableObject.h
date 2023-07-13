// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "ObjectBase.h"
#include "ExtinguishableObject.generated.h"

/**
 * 
 */
UCLASS()
class DARE_API AExtinguishableObject : public AObjectBase
{
	GENERATED_BODY()
protected:
	AExtinguishableObject();
	
public:
	void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void DrawOnObject();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UNiagaraSystem* FireVfx;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UNiagaraSystem* PoofVfx;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* BaseMesh;

	UNiagaraComponent* NiagaraComp;
};
