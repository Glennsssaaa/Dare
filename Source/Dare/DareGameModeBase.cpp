// Copyright Epic Games, Inc. All Rights Reserved.


#include "DareGameModeBase.h"

#include <string>

#include "ObjectBase.h"
#include "Kismet/GameplayStatics.h"

void ADareGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//Get all actors of class ObjectBase and add their points to MaxPoints
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AObjectBase::StaticClass(),FoundActors);
	for (auto FoundActor : (FoundActors))
	{
		AObjectBase* ActorCast = Cast<AObjectBase>(FoundActor);
		MaxPoints+=ActorCast->Points;
	}
	//USED FOR TESTING, TO BE DELETED
	MaxPoints-=100;
}

void ADareGameModeBase::UpdateScore(float objPoints)
{
	//Add objects points to the players current points
	currentPoints += objPoints;
	//If the player has more points than the max points, they win
	if(MaxPoints<=currentPoints)
	{
		UE_LOG(LogTemp,Warning,TEXT("You win!"));
		//Implement ending transition here
	}
	//Update points on UI
	UE_LOG(LogTemp,Warning,TEXT("Points Updated"));
	UIUpdate();
}