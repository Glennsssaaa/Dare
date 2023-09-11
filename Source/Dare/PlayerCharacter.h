//Base player character class, handles player movement, rotation, and abilities
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class DARE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//Input mapping context
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMapping;
	//Input config data
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputConfigData* InputActions;

	//Player ability functions
	UFUNCTION()
	virtual void AbilityOne();
	UFUNCTION()
	virtual void AbilityTwo();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Called to bind functionality to input
	void UpdateMappings(FText DisplayName, FKey NewKey);
	
	// Player Rotation speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	float RotationSpeed = 5.0f;
	// Player Movement speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	float MovementSpeed = 5.0f;
	
	//If Player is using keyboard (Unused)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	bool bUsingKeyboard;

	//Players look value (From controller)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	FVector2D LookValue;

	//Object hit by mouse cast (Unused)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	TArray<TEnumAsByte<EObjectTypeQuery>> mousehitObjs;

	//Interactable object the player is overlapping
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* InteractableActor;

	//Player movement direction
    FVector PlayerDirection;
	//Player movement value
	FVector2D MoveValue;
	//Player rotation value
	FRotator playerDirection;
	//Player respawn position
	FVector RespawnPos;
	
	//MouseHit results (Unused)
	FHitResult MouseHit;
	//Mouse position (Unused)
	FVector MousePosition;
	//Mouse direction (Unused)
	FVector MouseDirection;

	//Currently selected ability
	int SelectedAbility=1;

	//Whether player is interacting with an object
	bool bToggleInteract = false;
	//Whether the player is able to interact with an object
	bool bCanInteract=true;
	
	// Player Dash
	UFUNCTION()
	void PlayerDash();

	// Player Keyboard movement
	void KeyboardMove(const FInputActionValue& Value);
	//Interact Function
	virtual void Interact(const FInputActionValue& Value);
	// Player aim function
	void Aim(const FInputActionValue& Value);
	
	//Throw Item (TO BE IMPLEMENTED)
	void ThrowItem();

	//Overlap functions
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Setter for rotation
	void SetIsRotating(){bIsPlayerRotating = false;}

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* PlayerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Components", meta = (AllowPrivateAccess="true"))
	class UBoxComponent* InteractCollision;

	//Player frozen
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool bPlayerFrozen;
	
protected:
	
	// Dash Cooldown Timer
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashCooldown = 0.f;

	// Default Dash Cooldown
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashCooldownDefault = 3.f;

	// Starting amount of charges
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	int DashCharges = 3;

	// Maximum amount of charges that can be stored
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	int DashChargesMax = 3;

	// Maximum distance a player will dash before they stop
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashDistance = 1000.0f;

	// Speed at which a player will dash
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashSpeed = 1.75f;

	// Dash predicted location if there is no collision in the way
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	FVector PredictedLocation;

	//Checks if player is dashing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	bool bIsPlayerDashing = false;

	//Cooldown timer handle for dash
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	FTimerHandle DashCooldownTimerHandle;

	//Dash timer handle (FOR TESTING)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTimerHandle DashTestHandle;

	//Checks if player is able to dash
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanPlayerDash = true;

	//Checks if player is able to move
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanPlayerMove = true;

	//Checks if player is using an ability that requires aiming
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHasAimedAbility = false;

	//Checks if player is holding an item
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsHoldingItem = false;

	//Checks if player is rotating
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsPlayerRotating = false;

	//Target location for throwing item
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	FVector TargetLocation;

	//Overlapped object
	UPROPERTY()
	class AInteractableObject* OverlappedObject;

	//Pickupable item
	UPROPERTY()
	class APickupItem* PickupableItem;
};
