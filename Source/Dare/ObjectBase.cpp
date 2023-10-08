// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AObjectBase::AObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObjectBase::UpdateGameMode()
{
	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ADareGameModeBase::StaticClass(),FoundActors);

	if(FoundActors.IsEmpty()){return;}
	if(FoundActors[0]!=nullptr)
	{
		ADareGameModeBase* GameMode = Cast<ADareGameModeBase>(FoundActors[0]);
		if(mageTutorialObject)
		{
			GameMode->MageTutorialUpdate();
		}
		else if(tankTutorialObject)
		{
			GameMode->TankTutorialUpdate();
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Updating Score"));
			GameMode->UpdateScore(Points);
		}
	}
}

