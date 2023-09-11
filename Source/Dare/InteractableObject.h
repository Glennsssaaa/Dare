//Object that player can interact with, inherits from ObjectBase for point tracking
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectBase.h"
#include "InteractableObject.generated.h"

//Enum for object type, used to determine which object method to call
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
	//Mesh for object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ObjectMesh;
	//Collision for object
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components", meta=(AllowPrivateAccess="true"))
	class UBoxComponent* CollisionMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Overlap Function
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//End overlap function
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//Called when player interacts with object
	UFUNCTION(BlueprintCallable)
	virtual void Interact();
	//Enum for object type
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Components", meta=(AllowPrivateAccess="true"))
	EobjectType ObjectType;
	//If object has been interacted with
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bInteracted;
	//Used to hold player in place if interacting with object
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bFreezePlayer;
	//Checks to see if objects interaction is complete
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bFinished;
};
