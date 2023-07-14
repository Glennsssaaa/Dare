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
	QueryParams.bReturnFaceIndex=true;
	bUsingKeyboard=true;
	AbilitySelected=1;
	NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(WaterEffect,PlayerMesh,NAME_None,FVector(0,0,0), FRotator(0,0,0), EAttachLocation::Type::KeepRelativeOffset,true);

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
	//Interact toggle 
	if(bToggleInteract)
	{
		//Clears water ability timer
		GetWorldTimerManager().ClearTimer(lineTraceTimer);
		GravityOffset = FVector::ZeroVector;
	}
	else
	{
		//Water ability
		if(AbilitySelected==1)
		{
			//Calculates next position and calls function by timer
			NextLocation.X = GetActorLocation().X;
			NextLocation.Y = GetActorLocation().Y;
			NextLocation.Z = GetActorLocation().Z + 200;
			isDrawing = true;
			GetWorldTimerManager().SetTimer(lineTraceTimer, this, &AAMageCharacter::LineTraceArc, 0.01f, true);
			NiagaraComp->SetVectorParameter(FName("StartLocation"),PlayerMesh->GetComponentLocation());

		}
	}
	Super::Interact(Value);
	
}

//Trace in arc for water spell
void AAMageCharacter::LineTraceArc() {
	//Trajectory calculation to move in arc
	float mouseDist = FVector::Distance(PlayerMesh->GetComponentLocation(), MouseHit.Location);
	GravityOffset = GravityOffset + FVector(0,0,gravity * 0.1);
	float offset;
	//Position modified by mouse position
	if(bUsingKeyboard)
	{
		offset = mouseDist*5;
	}
	else
	{
		offset = 5;
	}
	float next = pow((offset*0.01),2) / (offset / 1000);
	FVector vec = PlayerMesh->GetForwardVector() * next;
	
	FHitResult Hit;
	FVector TraceStart = NextLocation;
	FVector TraceEnd = (vec + NextLocation) + GravityOffset;
	//Trace against the floor
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd,ECC_GameTraceChannel1 , QueryParams);
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.f);
	//If hit, call the drawfunc from blueprints with the hit actor and UV locations
	if (Hit.bBlockingHit) {
		NextLocation.X = GetActorLocation().X;
		NextLocation.Y = GetActorLocation().Y;
		NextLocation.Z = GetActorLocation().Z + 200;
		GravityOffset = FVector::ZeroVector;
		FVector2D hitUV;
		UGameplayStatics::FindCollisionUV(Hit,0,hitUV);
		DrawFunc(Hit.GetActor(),hitUV);
	}
	else {
		NextLocation = TraceEnd;
		VFXLocation=NextLocation;
		NiagaraComp->SetVectorParameter(FName("TargetVector"),VFXLocation);
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandle working %s"), *VFXLocation.ToString());

	}
	
	//NiagaraComp->SetVectorParameter(FName("TargetVector"),PlayerMesh->GetComponentLocation() + PlayerMesh->GetForwardVector()*500);
}