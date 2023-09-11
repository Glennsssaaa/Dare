//Improved camera class
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DedicatedCamera.generated.h"

UCLASS()
class DARE_API ADedicatedCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADedicatedCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Spring arm component
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USpringArmComponent* SpringArm;
	//Camera component
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UCameraComponent* Camera;
	//Box component
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBoxComponent* Box;
	//Mage Character
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ACharacter* MageCharacter;
	//Tank Character
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ACharacter* TankCharacter;

};
