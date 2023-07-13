// Copyright Epic Games, Inc. All Rights Reserved.


#include "DareGameModeBase.h"

#include <string>

#include "ObjectBase.h"
#include "Kismet/GameplayStatics.h"

void ADareGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AObjectBase::StaticClass(),FoundActors);
	for (auto FoundActor : (FoundActors))
	{
		AObjectBase* ActorCast = Cast<AObjectBase>(FoundActor);
		totalPoints+=ActorCast->Points;
		UE_LOG(LogTemp,Warning,TEXT("Casted"));
	}
	
}

void ADareGameModeBase::UpdateScore(float objPoints)
{
	currentPoints += objPoints;
	
	if(totalPoints<=currentPoints)
	{
		UE_LOG(LogTemp,Warning,TEXT("You win!"));
	}
	UE_LOG(LogTemp,Warning,TEXT("Points Updated"));
	UIUpdate();
	
}