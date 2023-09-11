// Fill out your copyright notice in the Description page of Project Settings.


#include "MageCharacter.h"

#include "GrowingObject.h"
#include "InteractableObject.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMageCharacter::AMageCharacter()
{
}

// Called when the game starts or when spawned
void AMageCharacter::BeginPlay()
{
	Super::BeginPlay();
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bReturnFaceIndex=true;
}

// Called every frame
void AMageCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Debug message to show water trace positioning 
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("Trace Start: %f"), NextLocation.Length()));
}


void AMageCharacter::Interact(const FInputActionValue& Value)
{
	//Use base class interact
	Super::Interact(Value);
}

//Water Spell
void AMageCharacter::AbilityOne()
{
	Super::AbilityOne();

	//Interact toggle
	if(bToggleWater)
	{
		//Clears water ability timer
		GetWorldTimerManager().ClearTimer(lineTraceTimer);
		GravityOffset = FVector::ZeroVector;
		bEnableWaterVFX=false;
		bToggleWater=false;
		bCanInteract=true;
		MovementSpeed=5.0f;
	}
	else if(!bToggleWater && !bToggleEarth && !bIsHoldingItem)
	{
		//Calculates next position and calls function by timer
		NextLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 200);
		isDrawing = true;
		bEnableWaterVFX=true;
		GetWorldTimerManager().SetTimer(lineTraceTimer, this, &AMageCharacter::LineTraceArc, 0.01f, true);
		bToggleWater=true;
		bCanInteract=false;
		MovementSpeed=0.5f;
	}
}

//Earth spell
void AMageCharacter::AbilityTwo()
{
	Super::AbilityTwo();

	//Toggle earth spell
	if(bToggleEarth){
		//Unfreezes player
		bToggleEarth=false;
		bPlayerFrozen=false;
	}
	//If earth spell toggled and player is not holding an object, and is overlapping an object
	else if(!bToggleEarth && !bIsHoldingItem && OverlappedObject)
	{
		//If overlapped object is a growing object
		if(OverlappedObject->IsA(AGrowingObject::StaticClass()))
		{
			//If growing object is finished, unfreeze player
			if(OverlappedObject->bFinished)
			{
				bPlayerFrozen=false;
				bToggleEarth=false;
			}
			else
			{
				//Else, toggle earth spell and interact with object
				bToggleEarth=true;
				OverlappedObject->Interact();
				bPlayerFrozen=true;
			}
		}
		//Else if overlapped object is an interactable object and is tagged as a bridge
		else if(OverlappedObject->IsA(AInteractableObject::StaticClass()) && OverlappedObject->ActorHasTag("Bridge"))
		{
				//If bridge is finished, unfreeze player
			if(OverlappedObject->bFinished)
			{
				bPlayerFrozen=false;
				bToggleEarth=false;
			}
			else
			{
				//Else, toggle earth spell and interact with object
				bToggleEarth=true;
				OverlappedObject->Interact();
				bPlayerFrozen=true;
			}
		}
	}
}

//Trace in arc for water spell
void AMageCharacter::LineTraceArc() {
	//Trajectory calculation to move in arc
	float mouseDist = FVector::Distance(PlayerMesh->GetComponentLocation(), MouseHit.Location);
	GravityOffset = GravityOffset + FVector(0,0,gravity * 0.1);
	float offset;
	
	offset = LookValue.Length()*5000;

	float next = pow((offset*0.01),2) / (offset / 1000);
	FVector vec = PlayerMesh->GetForwardVector() * next;

	FHitResult Hit;
	FVector TraceStart = NextLocation;
	FVector TraceEnd = (vec + NextLocation) + GravityOffset;

	//Trace against the floor
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd,ECC_WorldStatic , QueryParams);
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 1.0f, 0, 10.f);
	
	//If hit, call the drawfunc from blueprints with the hit actor and UV locations
	if (Hit.bBlockingHit) {
		NextLocation.X = GetActorLocation().X;
		NextLocation.Y = GetActorLocation().Y;
		NextLocation.Z = GetActorLocation().Z + 200;
		GravityOffset = FVector::ZeroVector;
		FVector2D hitUV;
		UGameplayStatics::FindCollisionUV(Hit,0,hitUV);
		DrawFunc(Hit.GetActor(),hitUV);
		WaterHitPosition = Hit.Location;
	}
	else {
		if(TraceEnd!=FVector::Zero())
		{
			NextLocation = TraceEnd;
		}
		else
		{
			NextLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 200);
		}
	}
}



//Code for using mouse position for water spell (UNUSED)
/*	if(bUsingKeyboard)
	{
		offset = mouseDist*5;
	}
	else
	{
		offset = LookValue.Length()*5000;
	}
*/
/*
	if(LookValue.Length()==0)
	{
		offset = 0.5*5000;
	}
	else
	{
		offset = LookValue.Length()*5000;
	}*/