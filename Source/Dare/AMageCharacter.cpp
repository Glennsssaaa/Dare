// Fill out your copyright notice in the Description page of Project Settings.


#include "AMageCharacter.h"
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
		bEnableWaterVFX=false;
	}
	else
	{
		//Water ability
		if(AbilitySelected==1)
		{
			//Calculates next position and calls function by timer
			NextLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 200);
			isDrawing = true;
			bEnableWaterVFX=true;
			GetWorldTimerManager().SetTimer(lineTraceTimer, this, &AAMageCharacter::LineTraceArc, 0.01f, true);
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
/*	if(bUsingKeyboard)
	{
		offset = mouseDist*5;
	}
	else
	{
		offset = LookValue.Length()*5000;
	}
*/
	if(LookValue.Length()==0)
	{
		offset = 0.5*5000;
	}
	else
	{
		offset = LookValue.Length()*5000;
	}
	
	float next = pow((offset*0.01),2) / (offset / 1000);
	FVector vec = PlayerMesh->GetForwardVector() * next;
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("Look Value: %f"), LookValue.Length()));

	FHitResult Hit;
	FVector TraceStart = NextLocation;
	FVector TraceEnd = (vec + NextLocation) + GravityOffset;
	//Trace against the floor
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd,ECC_GameTraceChannel1 , QueryParams);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.f);
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
		NextLocation = TraceEnd;
	}
}