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
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%lld"), count));
		}
	}
	MaxPoints-=100;
}

void ADareGameModeBase::UpdateScore(float objPoints)
{
	CurrentPoints += objPoints;
	
	if(MaxPoints<=CurrentPoints)
	{
		UE_LOG(LogTemp,Warning,TEXT("You win!"));
	}
	UE_LOG(LogTemp,Warning,TEXT("Points Updated"));
	UIUpdate();
	
}