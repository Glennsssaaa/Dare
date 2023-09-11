// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "MageCharacter.h"
#include "DareGameModeBase.h"
#include "GrowingObject.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "InputConfigData.h"
#include "Components/ArrowComponent.h"
#include "InteractableObject.h"
#include "PickupItem.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Mesh Initialization
	if(!PlayerMesh)
	{
		PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerSkeletalMesh"));
		PlayerMesh->SetupAttachment(GetRootComponent());
	}
	//Collision Initialization
	if(!InteractCollision)
	{
		InteractCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractCollision"));
		InteractCollision->SetupAttachment(RootComponent);
		InteractCollision->SetBoxExtent(FVector(100.f,100.f,100.f));
		InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
		InteractCollision->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Player Controller Initialization
	APlayerController* PC = Cast<APlayerController>(GetController());
	// Print the controller index
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	//Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping,0);

	//Enhanced Input Setup
	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	PC->SetShowMouseCursor(true);
	
	// Input Bindings
	PEI->BindAction(InputActions->InputKeyboardMove, ETriggerEvent::Triggered, this, &APlayerCharacter::KeyboardMove);
    PEI->BindAction(InputActions->InputAim, ETriggerEvent::Triggered, this, &APlayerCharacter::Aim);
	PEI->BindAction(InputActions->InputAim, ETriggerEvent::Completed, this, &APlayerCharacter::SetIsRotating);

	PEI->BindAction(InputActions->InputInteract, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
	PEI->BindAction(InputActions->InputDash, ETriggerEvent::Started, this, &APlayerCharacter::PlayerDash);
	PEI->BindAction(InputActions->InputAbility, ETriggerEvent::Started, this, &APlayerCharacter::AbilityOne);
	PEI->BindAction(InputActions->InputAbilityTwo, ETriggerEvent::Started, this, &APlayerCharacter::AbilityTwo);

}

void APlayerCharacter::UpdateMappings(FText DisplayName, FKey NewKey)
{
	// Update the mappings
	APlayerController* PC = Cast<APlayerController>(GetController());
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

//Overriden Ability Functions
void APlayerCharacter::AbilityOne()
{
	
}
void APlayerCharacter::AbilityTwo()
{
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Reduce Dash timer if needed
	if(DashCooldown > 0.f)
	{
		DashCooldown -= DeltaTime;
		
		// Clamp Dash Cooldown to 0
		if(DashCooldown < 0.f)
		{
			DashCooldown = 0.f;
		}

		// If the player is missing dash charges, recharge one charge every cooldown
		if (DashCooldown <= 0.f && DashCharges < DashChargesMax)
		{
			DashCharges++;
			// Reset Dash Cooldown if the player is still missing charges
			if(DashCharges < DashChargesMax)
			{
				DashCooldown = DashCooldownDefault;
			}
		}
	}

	//Checks if pickupable item is valid
	if(PickupableItem)
	{
		//Checks if player is holding item
		if(bIsHoldingItem)
		{
			//Checks if item has been moved yet
			if(!PickupableItem->bHasLerped)
			{
				//Move item towards front of player character
				if(!TargetLocation.Equals(PickupableItem->GetActorLocation(),50) )
				{
					PickupableItem->SetActorLocation(FMath::Lerp(PickupableItem->GetActorLocation(),TargetLocation,DeltaTime*50));
				}
				else
				{
					//Once items moved, set bHasLerped to true
					PickupableItem->bHasLerped=true;
				}
			}
			else
			{
				//If item has been moved, set item to front of player character
				PickupableItem->SetActorLocation((PlayerMesh->GetForwardVector()*600) + PlayerMesh->GetComponentLocation() + FVector(0,0,200));
			}
		}
	}
	// Print Dash Charges to screen
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("Dash Charges: %d"), DashCharges));
}

void APlayerCharacter::KeyboardMove(const FInputActionValue& Value)
{
	// Player can't move during certain actions, for example if they are aiming the tank's charge attack
	if(!bCanPlayerMove || bPlayerFrozen) { return; }
	
	MoveValue=Value.Get<FVector2D>();
	//Add movement input
	if(MoveValue.Y != 0)
	{
		AddMovementInput(GetActorForwardVector(), MoveValue.Y * MovementSpeed);
		//Checks if player is rotating, if not, rotate player mesh
		if(!bIsPlayerRotating)
		{
			PlayerMesh->SetWorldRotation(FMath::Lerp(PlayerMesh->GetComponentRotation(), UKismetMathLibrary::MakeRotFromX(PlayerDirection), GetWorld()->DeltaTimeSeconds * RotationSpeed));
		}
	}

	//Add movement input
	if(MoveValue.X!=0)
	{
		AddMovementInput(GetActorRightVector(), MoveValue.X * MovementSpeed);

		if(!bIsPlayerRotating)
		{
			PlayerMesh->SetWorldRotation(FMath::Lerp(PlayerMesh->GetComponentRotation(), UKismetMathLibrary::MakeRotFromX(PlayerDirection), GetWorld()->DeltaTimeSeconds * RotationSpeed));
		}
	}
	PlayerDirection = FVector(MoveValue.Y,MoveValue.X,0);

}
void APlayerCharacter::Aim(const FInputActionValue& Value){
	//Player look direction by controller value (Unused)
	if(bPlayerFrozen) { return; }

	if(!bIsPlayerRotating) { bIsPlayerRotating = true; }
	
	FVector2D TempLookValue=Value.Get<FVector2D>();

	if(TempLookValue.X != 0.f)
	{
		LookValue=TempLookValue;
	}

	//Player look direction by controller value
	PlayerDirection = FVector(LookValue.Y,LookValue.X,0);
	PlayerMesh->SetWorldRotation(FMath::Lerp(PlayerMesh->GetComponentRotation(), UKismetMathLibrary::MakeRotFromX(PlayerDirection), GetWorld()->DeltaTimeSeconds * (RotationSpeed * 4.f)));

}

//Function for throwing item (TO BE IMPROVED)
void APlayerCharacter::ThrowItem()
{
	//Checks if player is holding item and if item is valid
	if(PickupableItem!=nullptr)
	{
		if(bIsHoldingItem)
		{
			//Detach item from player and set physics to true
			PickupableItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			bIsHoldingItem=false;
			PickupableItem->SetActorEnableCollision(true);
			PickupableItem->Mesh->SetSimulatePhysics(true);
			//Add impulse to item
			if(GetVelocity().Length()<=5)
			{
				PickupableItem->Mesh->AddImpulse(PlayerMesh->GetForwardVector());
			}
			else
			{
				PickupableItem->Mesh->AddImpulse(PlayerMesh->GetForwardVector() * 250000);
			}
			//Reset pickupableItems variables
			PickupableItem->bHasLerped=false;
			PickupableItem=nullptr;
		}
		else
		{
			//If player is not holding item, attach item to player and set physics to false
			PickupableItem->AttachToComponent(PlayerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			PickupableItem->Mesh->SetSimulatePhysics(false);
			PickupableItem->SetActorEnableCollision(false);
			bIsHoldingItem=true;
			TargetLocation = (PlayerMesh->GetForwardVector()*600)+PlayerMesh->GetComponentLocation() + FVector(0,0,200);
		}
	}
}

void APlayerCharacter::Interact(const FInputActionValue& Value)
{
	if(bCanInteract)
	{
		//Interact set to toggle
		if(bToggleInteract)
		{
			bToggleInteract=false;
		}
		else 
		{
			bToggleInteract=true;
		}

		//If player is overlapping an object, interact with it
		if(OverlappedObject)
		{
			//If object is a growing object, call interact function and set bPlayerFrozen to true
			if(!OverlappedObject->IsA(AGrowingObject::StaticClass()))
			{
				OverlappedObject->Interact();
				bPlayerFrozen = OverlappedObject->bFreezePlayer;
			}
		}
		/*
		if(PickupableItem!=nullptr)
		{
			ThrowItem();
		}*/
	}
}

void APlayerCharacter::PlayerDash()
{
	// Disable Player Input to prevent player movement during dash
	if(!bCanPlayerDash)
	{
		return;
	}

	bCanPlayerDash = false;
	bCanPlayerMove = false;
	bIsPlayerDashing = true;
	// Find the predicted location of the player after the dash
	PredictedLocation = (PlayerMesh->GetForwardVector() * DashDistance) + GetActorLocation();
	DashCharges--;
	
	GetWorldTimerManager().SetTimer(DashTestHandle, [this]()
	{
		bCanPlayerDash = true;
		UE_LOG(LogTemp,Warning,TEXT("recharge"));
	}, 1.0f, false, 1.0f);
	
	
	// Set function to run every frame
	GetWorldTimerManager().SetTimer(DashCooldownTimerHandle, [this]()
	{
		FHitResult SweepHitResult;
		
		// Set actor location using linear interpolation and check if there is any collision in the way
		SetActorLocation(FMath::Lerp(GetActorLocation(), PredictedLocation, GetWorld()->GetDeltaSeconds() * DashSpeed), true, &SweepHitResult);

		const FVector2D ActorLocation2D = FVector2D(GetActorLocation().X, GetActorLocation().Y);
		const FVector2D PredictedLocation2D = FVector2D(PredictedLocation.X, PredictedLocation.Y);
			
		if (SweepHitResult.bBlockingHit || ActorLocation2D.Equals(PredictedLocation2D, 100.f))
		{
			// Location reached, activate dash cooldown, re-enable input and camera lag
			bCanPlayerMove = true;
			bIsPlayerDashing = false;

			// Clear Dash timer to stop function running every frame
			GetWorld()->GetTimerManager().ClearTimer(DashCooldownTimerHandle);
			
			// Only reset the cooldown if the player has used all their dashes
			if(DashCharges == DashChargesMax - 1)
			{
				DashCooldown = DashCooldownDefault;	
			}
		}
	}, GetWorld()->DeltaTimeSeconds / 3.f, true, 0.0f);
}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Casts to overlapped object
	OverlappedObject = Cast<AInteractableObject>(OtherActor);
	if(OverlappedObject!=nullptr)
	{
	}
	//If overlapped object has the respawn tag, set respawn position to players current location
	if(OtherActor->ActorHasTag("Respawn"))
	{
		RespawnPos = this->GetActorLocation();
		UE_LOG(LogTemp,Warning,TEXT("RespawnVol"));
	}
	//If overlapped object has the kill tag, set player location to respawn position
	if(OtherComp->ComponentHasTag("Kill"))
	{
		SetActorLocation(RespawnPos);
		UE_LOG(LogTemp,Warning,TEXT("Respawn"));
	}
	//If overlapped object is a pickup item, set pickupable item to overlapped object
	if(OtherActor->IsA(APickupItem::StaticClass()))
	{
		PickupableItem = Cast<APickupItem>(OtherActor);
		if(PickupableItem==nullptr)
		{
			PickupableItem=Cast<APickupItem>(OtherActor);
		}
	}
}

void APlayerCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//When overlap ends, set overlapped object to null
	if(OverlappedObject != nullptr)
	{
		OverlappedObject=nullptr;
	}
}
