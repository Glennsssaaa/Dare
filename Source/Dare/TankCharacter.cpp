// Fill out your copyright notice in the Description page of Project Settings.

#include "TankCharacter.h"
#include "DestructableObject.h"
#include "EnhancedInputComponent.h"
#include "InputConfigData.h"
#include "RebuildableBase.h"
#include "MageCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATankCharacter::ATankCharacter()
{
	if(!ChargeHitBox)
	{
		ChargeHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ChargeHitBox"));
		ChargeHitBox->SetupAttachment(PlayerMesh);
	}

	if(!DashAimArrowComponent)
	{
		DashAimArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("DashAimArrowComponent"));
		if(PlayerMesh)
		{
			DashAimArrowComponent->SetupAttachment(PlayerMesh);
		}
		DashAimArrowComponent->SetHiddenInGame(false);
	}

}



// Called when the game starts or when spawned
void ATankCharacter::BeginPlay()
{
	Super::BeginPlay();
	DashAimArrowComponent->SetVisibility(false);
	ChargeHitBox->OnComponentBeginOverlap.AddDynamic(this, &ATankCharacter::OnOverlapBegin);
	DashDistance = 1350.f;

}

// Called every frame
void ATankCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(PickupablePlayer)
	{
		if(bIsHoldingItem)
		{
			if(!PickupablePlayer->bHasLerped)
			{
				if(!TargetLocation.Equals(PickupablePlayer->GetActorLocation(),50) )
				{
					PickupablePlayer->SetActorLocation(FMath::Lerp(PickupablePlayer->GetActorLocation(),TargetLocation,DeltaTime*50));
				}
				else
				{
					PickupablePlayer->bHasLerped=true;
				}
			}
			else
			{
				PickupablePlayer->SetActorLocation((PlayerMesh->GetForwardVector()*600) + PlayerMesh->GetComponentLocation() + FVector(0,0,200));
			}
		}
	}
}

void ATankCharacter::AbilityOne()
{
	Super::AbilityOne();

	// Make sure tank isn't currently holding an object
	if(!bIsHoldingItem)
	{
		if(!bHasAimedAbility)
		{
			// Step 1: First time input has been pressed, player now aims the charge
			
			// Player can't move 
			bCanPlayerMove = false;

			DashAimArrowComponent->SetVisibility(true);
			bHasAimedAbility = true;
			GetWorldTimerManager().SetTimer(DashCooldownTimerHandle, [this]()
			{
				bHasAimedAbility = false;
			}, 5.f, false);
		}
		else
		{
			// Step 2: If player presses input again within 5 seconds, they will
			// charge forward in the direction they are aiming
			
			Charge();
			
			bHasAimedAbility = false;
			bCanPlayerMove = true;
		}
	}
	
}

void ATankCharacter::AbilityTwo()
{
	Super::AbilityTwo();
	
	if(!bIsHoldingItem)
	{
		// If building destroyed, rebuild it
		if(!Rebuildable) return;

		if(bIsInRebuildZone && Rebuildable->GetIsDestroyed() && !bIsHoldingItem)
		{
			Rebuildable->SetIsRebuilding(true);	
		}
	}
	
}

void ATankCharacter::Charge()
{
	// Disable Player Input to prevent player movement during dash
	bCanPlayerMove = false;
	
	bIsPlayerDashing = true;
	
	// Find the predicted location of the player after the dash
	PredictedLocation = (DashAimArrowComponent->GetForwardVector() * ChargeDistance) + GetActorLocation();

	// Set function to run every frame
	GetWorldTimerManager().SetTimer(DashCooldownTimerHandle, [this]()
	{
		DashAimArrowComponent->SetVisibility(false);
		
		FHitResult SweepHitResult;
		
		// Set actor location using linear interpolation and check if there is any collision in the way
		SetActorLocation(FMath::Lerp(GetActorLocation(), PredictedLocation, GetWorld()->GetDeltaSeconds() * ChargeSpeed), true, &SweepHitResult);

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




void ATankCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor->ActorHasTag("Destruct") && bIsPlayerDashing)
	{
		// Smoke fog function at some point
		//dynamic_cast<ADestructableObject>(OtherActor).DoSomethingReallyCoolLater;

		// Instead do this for now
		OtherActor->Destroy();
	}

	if(OtherComp->ComponentHasTag("Rebuild"))
	{

		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Big Rebuild")));
		// ARebuildableBase* OverlappedCharacter = Cast<ARebuildableBase>(OtherActor);
		// OverlappedCharacter->ToggleHouseDestruction();
		Rebuildable = Cast<ARebuildableBase>(OtherActor);
		bIsInRebuildZone = true;
		
		
	}

	if(OtherComp->ComponentHasTag("HouseCollision"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Big Destory")));
		ARebuildableBase* OverlappedCharacter = Cast<ARebuildableBase>(OtherActor);

		if(OverlappedCharacter->GetIsDestroyed() == false && bIsPlayerDashing)
		{
			OverlappedCharacter->ToggleHouseDestruction();
		}
	}

	if(OtherActor->IsA(AMageCharacter::StaticClass()))
	{
		PickupablePlayer= Cast<AMageCharacter>(OtherActor);
	}
	
}

void ATankCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

	// Player left build zone, stop rebuilding
	if(OtherComp->ComponentHasTag("Rebuild"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Left rebuild area")));
		Rebuildable->SetIsRebuilding(false);
		Rebuildable = nullptr;
		bIsInRebuildZone = false;
	}
}

void ATankCharacter::Interact(const FInputActionValue& Value)
{
	Super::Interact(Value);
	if(PickupablePlayer!=nullptr)
	{
		ThrowItem();
	}
}

void ATankCharacter::ThrowItem()
{
	Super::ThrowItem();
	
	if(bIsHoldingItem)
	{
		PickupablePlayer->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		bIsHoldingItem=false;
		PickupablePlayer->SetActorEnableCollision(true);
	//	PickupablePlayer->GetCapsuleComponent()->SetSimulatePhysics(true);
		//PickupablePlayer->GetCapsuleComponent()->AddImpulse(PlayerMesh->GetForwardVector()*250000);
		PickupablePlayer->bHasLerped=false;
	}
	else
	{
		PickupablePlayer->AttachToComponent(PlayerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		PickupablePlayer->GetCapsuleComponent()->SetSimulatePhysics(false);
		PickupablePlayer->SetActorEnableCollision(false);
		bIsHoldingItem=true;
		TargetLocation = (PlayerMesh->GetForwardVector()*6000)+PlayerMesh->GetComponentLocation() + FVector(0,0,200);
	}
}
