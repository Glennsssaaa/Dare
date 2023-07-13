// Fill out your copyright notice in the Description page of Project Settings.


#include "AMageCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AAMageCharacter::AAMageCharacter()
{
}

// Called when the game starts or when spawned
void AAMageCharacter::BeginPlay()
{
	Super::BeginPlay();
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bReturnFaceIndex = true;
	bUsingKeyboard = true;
	AbilitySelected = 1;
}

// Called every frame
void AAMageCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bToggleInteract)
	{
		if(AbilitySelected==1)
		{

		}
	}
	else
	{
	
	}
}


void AAMageCharacter::Interact(const FInputActionValue& Value)
{
	// Interact toggle 
	if(bToggleInteract)
	{
		// Clears water ability timer
		GetWorldTimerManager().ClearTimer(lineTraceTimer);
		GravityOffset = FVector::ZeroVector;
	}
	else
	{
		// Water ability
		if(AbilitySelected == 1)
		{
			// Calculates next position and calls function by timer
			NextLocation.X = GetActorLocation().X;
			NextLocation.Y = GetActorLocation().Y;
			NextLocation.Z = GetActorLocation().Z + 200.f;
			
			bIsDrawing = true;
			GetWorldTimerManager().SetTimer(lineTraceTimer, this, &AAMageCharacter::LineTraceArc, 0.01f, true);
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(WaterEffect,PlayerMesh,NAME_None,FVector(0,0,0), FRotator(0,0,0), EAttachLocation::Type::KeepRelativeOffset,true);
			NiagaraComp->SetVectorParameter(FName("StartLocation"),PlayerMesh->GetComponentLocation());
			NiagaraComp->SetVectorParameter(FName("TargetVector"),PlayerMesh->GetComponentLocation()+FVector(100,0,0));
		}
	}
	Super::Interact(Value);
	
}

//Trace in arc for water spell
void AAMageCharacter::LineTraceArc()
{
	// Trajectory calculation to move in arc
	float MouseDist = FVector::Distance(PlayerMesh->GetComponentLocation(), MouseHit.Location);
	GravityOffset = GravityOffset + FVector(0,0,Gravity * 0.1f);
	float Offset;
	// Position modified by mouse position
	if(bUsingKeyboard)
	{
		Offset = MouseDist * 5.f;
	}
	else
	{
		Offset = 5.f;
	}
	float Next = pow((Offset * 0.01f),2.f) / (Offset / 1000.f);
	
	FVector Vec = PlayerMesh->GetForwardVector() * Next;
	
	FHitResult Hit;
	FVector TraceStart = NextLocation;
	FVector TraceEnd = (Vec + NextLocation) + GravityOffset;
	
	// Trace against the floor
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd,ECC_GameTraceChannel1 , QueryParams);
	// DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.f);
	// If hit, call the drawfunc from blueprints with the hit actor and UV locations
	if (Hit.bBlockingHit)
	{
		NextLocation.X = GetActorLocation().X;
		NextLocation.Y = GetActorLocation().Y;
		NextLocation.Z = GetActorLocation().Z + 200.f;

		GravityOffset = FVector::ZeroVector;
		FVector2D HitUV;
		UGameplayStatics::FindCollisionUV(Hit,0,HitUV);
		DrawFunc(Hit.GetActor(),HitUV);
	}
	else
	{
		NextLocation = TraceEnd;
	}

}