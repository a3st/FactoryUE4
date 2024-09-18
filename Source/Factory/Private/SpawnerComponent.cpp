// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnerComponent.h"

#include "Components/SceneComponent.h"

USpawnerComponent::USpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnPointComponent = Cast<USceneComponent>(GetOwner()->GetComponentsByTag(USceneComponent::StaticClass(), TEXT("SpawnPoint")).Last());
}

void USpawnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USpawnerComponent::Spawn(bool &Result, AActor *&NewActor)
{
	if (!IsLocked)
	{
		FVector SpawnLocation = SpawnPointComponent->GetComponentLocation();
		FRotator SpawnRotation = SpawnPointComponent->GetComponentRotation();

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		NewActor = GetWorld()->SpawnActor<AActor>(SpawnClass.Get(), SpawnLocation, SpawnRotation, SpawnParameters);

		IsLocked = true;
		Result = true;
	}
	else
	{
		Result = false;
		NewActor = nullptr;
	}
}

void USpawnerComponent::Unlock()
{
	IsLocked = false;
}