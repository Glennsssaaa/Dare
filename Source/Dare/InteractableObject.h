// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectBase.h"
#include "InteractableObject.generated.h"

UENUM(BlueprintType)
enum class EobjectType : uint8
{
	E_Bridge = 0,
	E_Tree = 1
};

UCLASS()
class DARE_API AInteractableObject : public AObjectBase
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ObjectMesh;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components", meta=(AllowPrivateAccess="true"))
	class UBoxComponent* CollisionMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	virtual void Interact();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Components", meta=(AllowPrivateAccess="true"))
	EobjectType ObjectType;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bInteracted;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bFreezePlayer;
	
};
