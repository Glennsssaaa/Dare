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
}

// Called every frame
void AAMageCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AAMageCharacter::Interact(const FInputActionValue& Value)
{
	NextLocation.X = GetActorLocation().X;
	NextLocation.Y = GetActorLocation().Y;
	NextLocation.Z = GetActorLocation().Z + 200;
	LineTraceArc();
	isDrawing = true;
	GetWorldTimerManager().SetTimer(lineTraceTimer, this, &AAMageCharacter::LineTraceArc, 0.01f, true);
}

void AAMageCharacter::InteractEnd(const FInputActionValue& Value) {
	GetWorldTimerManager().ClearTimer(lineTraceTimer);
	GravityOffset = FVector::ZeroVector;
}

void AAMageCharacter::LineTraceArc() {
	GravityOffset = GravityOffset + FVector(0,0,gravity * 0.1);
	float offset = (abs(MoveValue.X + MoveValue.Y) + 1) * 1000;
	float next = pow((offset*0.01),2) / (offset / 1000);
	FVector vec = PlayerMesh->GetForwardVector() * next;
	
	FHitResult Hit;
	FVector TraceStart = NextLocation;
	FVector TraceEnd = (vec + NextLocation) + GravityOffset;
	
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_GameTraceChannel1, QueryParams);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.f);
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
	}
	
}