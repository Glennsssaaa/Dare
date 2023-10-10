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
		MaxPoints+=ActorCast->Points;
		if(ActorCast->Points>0)
		{
			count++;
		}
	}
}

void ADareGameModeBase::MageTutorialObject()
{
	MageTutorialUpdate();
}

void ADareGameModeBase::UpdateScore(float objPoints)
{
	CurrentPoints += objPoints;
	UE_LOG(LogTemp,Warning,TEXT("Current Points: %f"),CurrentPoints);
	UE_LOG(LogTemp,Warning,TEXT("Max Points: %f"),MaxPoints);
	if(MaxPoints<=CurrentPoints)
	{
		UE_LOG(LogTemp,Warning,TEXT("You win!"));
	}
	UE_LOG(LogTemp,Warning,TEXT("Points Updated"));
	UIUpdate();
	
}