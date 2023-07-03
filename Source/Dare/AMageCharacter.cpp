// Fill out your copyright notice in the Description page of Project Settings.


#include "AMageCharacter.h"

// Sets default values
AAMageCharacter::AAMageCharacter()
{

}



// Called when the game starts or when spawned
void AAMageCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAMageCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAMageCharacter::Interact(const FInputActionValue& Value)
{
	//print interact
	UE_LOG(LogTemp, Warning, TEXT("Interact"));
	APlayerController* PC = Cast<APlayerController>(GetController());
}


void AAMageCharacter::LineTraceArc() {
	FHitResult Hit;
	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = GetActorLocation() + GetActorForwardVector() * 1000.f;
	FCollisionQueryParams QueryParams;
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_GameTraceChannel1, QueryParams);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.f);
	
}