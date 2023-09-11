//Base object for interactables in level
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DareGameModeBase.h"
#include "ObjectBase.generated.h"

UCLASS()
class DARE_API AObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Points assigned to object, used for tracking level score
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float Points;
	//Updates game score
	void UpdateGameMode();
};
