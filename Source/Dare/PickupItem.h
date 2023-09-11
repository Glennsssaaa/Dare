//Class for items that can be picked up by the player
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

UCLASS()
class DARE_API APickupItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Held state
	bool bIsHeld;
	//If the item has been lerped to the player
	bool bHasLerped;

	//Object Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	//Object Collision
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Components", meta = (AllowPrivateAccess="true"))
	class UBoxComponent* InteractCollision;
	//Object Overlap Function
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//Objects respawn position (Usually where it was set in editor)
	FVector RespawnPos;
};
