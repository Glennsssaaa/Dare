//Base class for rebuildable objects
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectBase.h"
#include "RebuildableBase.generated.h"

UCLASS()
class DARE_API ARebuildableBase : public AObjectBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARebuildableBase();

	//Function to destroy or rebuild object
	UFUNCTION(BlueprintCallable)
	void ToggleHouseDestruction();
	//Function to set object on fire or put out fire
	UFUNCTION(BlueprintCallable)
	void ToggleFire();
	//Function to clean or dirty object
	UFUNCTION(BlueprintCallable)
	void ToggleClean();

	// Getters and Setters
	UFUNCTION(BlueprintCallable)
	bool GetIsDestroyed(){ return bIsDestroyed; };

	UFUNCTION(BlueprintCallable)
	bool GetIsOnFire(){ return bIsOnFire; };

	UFUNCTION(BlueprintCallable)
	bool GetIsClean(){ return bIsClean; };

	UFUNCTION(BlueprintCallable)
	void SetIsRebuilding(bool bTemp) {  bIsRebuilding = bTemp; };
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Destroyed static mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* DestroyedMesh;

	// Rebuilt static mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* RebuiltMesh;

	// Rebuilding Collision Area
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UBoxComponent* RebuildCollision;

	// House Collision Box
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UBoxComponent* HouseCollision;

	// Fire VFX
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UNiagaraSystem* FireVfx;

	//Object on fire state
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "House State")
	bool bIsOnFire = false;
	//Object destroyed state
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "House State")
	bool bIsDestroyed = true;
	//Object clean state
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "House State")
	bool bIsClean = false;
	//Object rebuilding state
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "House State")
	bool bIsRebuilding = false;
	//Object rebuilding progress
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "House State")
	float RebuildProgress = 0.0f;
	//Object rebuilding speed
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "House State")
	float RebuildSpeed = 10.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
