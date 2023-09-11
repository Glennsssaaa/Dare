//Character class for the Mage, inherits from PlayerCharacter
#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "NiagaraSystem.h"
#include "MageCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DARE_API AMageCharacter : public APlayerCharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AMageCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Water VFX
	UPROPERTY(EditAnywhere,Category="Water")
	UNiagaraSystem* WaterEffect;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Interact Function 
	void Interact(const FInputActionValue& Value) override;
	//Overriden Ability Functions
	virtual void AbilityOne() override;
	virtual void AbilityTwo() override;
	
	//Draw Function for water spell (Implemented in blueprints)
	UFUNCTION(BlueprintImplementableEvent)
		void DrawFunc(AActor* hitActor, FVector2D hitUV);

	//Position the arc line trace hits
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector WaterHitPosition;

	//Water VFX Toggle
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bEnableWaterVFX;

	//For being picked up
	bool bHasLerped;

	//Toggles for spells
	bool bToggleWater;
	bool bToggleEarth;
private:

	//Line Trace Variables
	void LineTraceArc();
	
	//Line trace arc variables
	FVector GravityOffset;
	FVector NextLocation;
	float gravity = -980;
	bool isDrawing = false;
	FTimerHandle lineTraceTimer; 
	FCollisionQueryParams QueryParams;

	//Ability Selected
	int AbilitySelected = 0;
};
