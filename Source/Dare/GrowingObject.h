//Object that can be grown by the mage, inherits from InteractableObject

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "GrowingObject.generated.h"

/**
 * 
 */
UCLASS()
class DARE_API AGrowingObject : public AInteractableObject
{
	GENERATED_BODY()
	AGrowingObject();
public:
	// Called every frame
	virtual void Tick(float DeltaTime);
	// Called when the object is interacted with
	virtual void Interact();
	// Called while the object is being grown
	void Grow();
	// Whether the object has been grown
	UPROPERTY(BlueprintReadOnly)
	bool hasGrown;
	
private:
	//Timer for growing the object
	FTimerHandle growthTimer; 
};
