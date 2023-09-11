//GameModeBase class for Dare project
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DareGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DARE_API ADareGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	//Point float for tracking level progress
	float points;
public:
	//Max points, calculated by casting to all objectbase actors and adding their points
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxPoints=0;
	//Current points, updated by objectbase actors
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float currentPoints=0;
	//Update score function, called by objectbase actors
	UFUNCTION(BlueprintCallable)
	void UpdateScore(float objPoints);
	//UI update function, called by objectbase actors
	UFUNCTION(BlueprintImplementableEvent)
	void UIUpdate();
};
