// Fill out your copyright notice in the Description page of Project Settings.


#include "APlayerCharacter.h"

#include "AMageCharacter.h"
#include "DareGameModeBase.h"
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
AAPlayerCharacter::AAPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	if(!PlayerMesh)
	{
		PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
		PlayerMesh->SetupAttachment(GetRootComponent());
		PlayerMesh->OnComponentBeginOverlap.AddDynamic(this, &AAPlayerCharacter::OnMeshOverlapBegin);
	}

	if(!PlayerSkeletalMesh)
	{
		PlayerSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerSkeletalMesh"));
		PlayerSkeletalMesh->SetupAttachment(PlayerMesh);
	}

	if(!InteractCollision)
	{
		InteractCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractCollision"));
		InteractCollision->SetupAttachment(PlayerMesh);
		InteractCollision->SetBoxExtent(FVector(100.f,100.f,100.f));
		InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &AAPlayerCharacter::OnOverlapBegin);
		InteractCollision->OnComponentEndOverlap.AddDynamic(this, &AAPlayerCharacter::OnOverlapEnd);
	}
}

// Called to bind functionality to input
void AAPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PC = Cast<APlayerController>(GetController());
	// Print the controller index
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	//Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping,0);

	//Enhanced Input Setup
	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	PC->SetShowMouseCursor(true);
	
	// Input Bindings
	PEI->BindAction(InputActions->InputKeyboardMove, ETriggerEvent::Triggered, this, &AAPlayerCharacter::KeyboardMove);
    PEI->BindAction(InputActions->InputAim, ETriggerEvent::Triggered, this, &AAPlayerCharacter::Aim);
	PEI->BindAction(InputActions->InputInteract, ETriggerEvent::Started, this, &AAPlayerCharacter::Interact);
	PEI->BindAction(InputActions->InputDash, ETriggerEvent::Started, this, &AAPlayerCharacter::PlayerDash);
	PEI->BindAction(InputActions->InputAbility, ETriggerEvent::Started, this, &AAPlayerCharacter::AbilityOne);
	PEI->BindAction(InputActions->InputAbilityTwo, ETriggerEvent::Started, this, &AAPlayerCharacter::AbilityTwo);

}

void AAPlayerCharacter::UpdateMappings(FText DisplayName, FKey NewKey)
{
	// Update the mappings
	APlayerController* PC = Cast<APlayerController>(GetController());
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

	

}


// Called when the game starts or when spawned
void AAPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAPlayerCharacter::AbilityOne()
{
	
}

void AAPlayerCharacter::AbilityTwo()
{
	
}

// Called every frame
void AAPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bUsingKeyboard)
	{
		// Player Direction Mouse Controls
		GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorForObjects(mousehitObjs, false, MouseHit);
		playerDirection = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MouseHit.Location);
		PlayerMesh->SetWorldRotation(FMath::Lerp(PlayerMesh->GetComponentRotation(), FRotator(0,playerDirection.Yaw,0), DeltaTime*RotationSpeed));
	}
	// lerp the player's direction to the direction variable
	else if(LookValue.Y !=0 || LookValue.X!=0 && !bUsingKeyboard)
	{
		PlayerDirection = FVector(-LookValue.Y,LookValue.X,0);
		PlayerMesh->SetWorldRotation(FMath::Lerp(PlayerMesh->GetComponentRotation(), UKismetMathLibrary::MakeRotFromX(PlayerDirection), DeltaTime * RotationSpeed));
	}

	
	// Reduce Dash timer if needed
	if(DashCooldown > 0.f)
	{
		DashCooldown -= DeltaTime;
		
		// Clamp Dash Cooldown to 0
		if(DashCooldown < 0.f)
		{
			DashCooldown = 0.f;
		}
		
		if (DashCooldown <= 0.f && DashCharges < DashChargesMax)
		{
			DashCharges++;
			if(DashCharges < DashChargesMax)
			{
				DashCooldown = DashCooldownDefault;
			}
		}
	}

	if(PickupableItem)
	{
		if(bIsHoldingItem)
		{
			if(!PickupableItem->bHasLerped)
			{
				if(!TargetLocation.Equals(PickupableItem->GetActorLocation(),50) )
				{
					PickupableItem->SetActorLocation(FMath::Lerp(PickupableItem->GetActorLocation(),TargetLocation,DeltaTime*50));
				}
				else
				{
					PickupableItem->bHasLerped=true;
				}
			}
			else
			{
				PickupableItem->SetActorLocation((PlayerMesh->GetForwardVector()*600) + PlayerMesh->GetComponentLocation() + FVector(0,0,200));
			}
		}
	}
	// Print Dash Charges to screen
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("Dash Charges: %d"), DashCharges));
}

void AAPlayerCharacter::KeyboardMove(const FInputActionValue& Value)
{
	// Player can't move during certain actions, for example if they are aiming the tank's charge attack
	if(!bCanPlayerMove || bPlayerFrozen) { return; }
	
	MoveValue=Value.Get<FVector2D>();
	//Add movement input
	if(MoveValue.Y != 0)
	{
		AddMovementInput(GetActorForwardVector(), MoveValue.Y * MovementSpeed);
	}

	if(MoveValue.X!=0)
	{
		AddMovementInput(GetActorRightVector(), MoveValue.X * MovementSpeed);
	}
	
}

void AAPlayerCharacter::Aim(const FInputActionValue& Value){
	//Player look direction by controller value (Unused)
	if(bPlayerFrozen) { return; }

	LookValue=Value.Get<FVector2D>();

	if(LookValue.X == 0)
	{
		LookValue.X = 1;    
	}

	PlayerDirection = FVector(LookValue.Y,LookValue.X,0);

}

void AAPlayerCharacter::ThrowItem()
{
	if(PickupableItem!=nullptr)
	{
		if(bIsHoldingItem)
		{
			PickupableItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			bIsHoldingItem=false;
			PickupableItem->SetActorEnableCollision(true);
			PickupableItem->Mesh->SetSimulatePhysics(true);
			if(GetVelocity().Length()<=5)
			{
				PickupableItem->Mesh->AddImpulse(PlayerMesh->GetForwardVector());
			}
			else
			{
				PickupableItem->Mesh->AddImpulse(PlayerMesh->GetForwardVector()*250000);
			}
			PickupableItem->bHasLerped=false;
		}
		else
		{
			PickupableItem->AttachToComponent(PlayerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			PickupableItem->Mesh->SetSimulatePhysics(false);
			PickupableItem->SetActorEnableCollision(false);
			bIsHoldingItem=true;
			TargetLocation = (PlayerMesh->GetForwardVector()*600)+PlayerMesh->GetComponentLocation() + FVector(0,0,200);
		}
	}
}

void AAPlayerCharacter::Interact(const FInputActionValue& Value)
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
	
	if(OverlappedObject)
	{
		OverlappedObject->Interact();
		bPlayerFrozen = OverlappedObject->bFreezePlayer;
	}

	if(PickupableItem!=nullptr)
	{
		ThrowItem();
	}
}

void AAPlayerCharacter::PlayerDash()
{
	// Check if the player is already dashing or if the dash is on cooldown
	if(bIsPlayerDashing || DashCharges == 0) return;

	// Disable Player Input to prevent player movement during dash
	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UE_LOG(LogTemp, Warning, TEXT("Dash"));
	
	bIsPlayerDashing = true;
	// Find the predicted location of the player after the dash
	PredictedLocation = (PlayerMesh->GetForwardVector() * DashDistance) + GetActorLocation();

	DashCharges--;
	
	GetWorldTimerManager().SetTimer(DashCooldownTimerHandle, [this]()
	{
		FHitResult SweepHitResult;
			
		// Set actor location using interpolation and check if there is any collision in the way
		SetActorLocation(FMath::Lerp(GetActorLocation(), PredictedLocation, GetWorld()->GetDeltaSeconds() * DashSpeed), true, &SweepHitResult);

		const FVector2D ActorLocation2D		= FVector2D(GetActorLocation().X, GetActorLocation().Y);
		const FVector2D PredictedLocation2D = FVector2D(PredictedLocation.X, PredictedLocation.Y);
			
		if (SweepHitResult.bBlockingHit || ActorLocation2D.Equals(PredictedLocation2D, 100.f))
		{
			// Location reached, activate dash cooldown, re-enable input and camera lag
			EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

			// Clear Dash timer to stop function running every frame
			GetWorld()->GetTimerManager().ClearTimer(DashCooldownTimerHandle);
				
			bIsPlayerDashing = false;

			// Only reset the cooldown if the player has 1 dash charge left, otherwise cooldown stays as it is 
			if(DashCharges == DashChargesMax - 1)
			{
				DashCooldown = DashCooldownDefault;
			}
		}
	}, GetWorld()->DeltaTimeSeconds / 2.75, true, 0.0f);
	
}

void AAPlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	OverlappedObject = Cast<AInteractableObject>(OtherActor);
	if(OverlappedObject!=nullptr)
	{
	}
	if(OtherActor->ActorHasTag("Respawn"))
	{
		RespawnPos = this->GetActorLocation();
	}
	if(OtherActor->ActorHasTag("Kill"))
	{
		SetActorLocation(RespawnPos);
	}
	if(OtherActor->IsA(APickupItem::StaticClass()))
	{
		PickupableItem = Cast<APickupItem>(OtherActor);
		if(PickupableItem==nullptr)
		{
			PickupableItem=Cast<APickupItem>(OtherActor);
		}
	}
}

void AAPlayerCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OverlappedObject != nullptr)
	{
		OverlappedObject=nullptr;
	}
}

void AAPlayerCharacter::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}




	