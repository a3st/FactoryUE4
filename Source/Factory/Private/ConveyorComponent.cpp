// Fill out your copyright notice in the Description page of Project Settings.

#include "ConveyorComponent.h"

#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"

UConveyorComponent::UConveyorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Filter = ConstructorHelpers::FClassFinder<AActor>(TEXT("/Game/Blueprints/Objects/BP_Box_Static")).Class;
	Speed = 60.0f;

	IsEnabled = false;
}

void UConveyorComponent::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent = GetOwner()->FindComponentByClass<UBoxComponent>();
	ArrowComponent = GetOwner()->FindComponentByClass<UArrowComponent>();
}

void UConveyorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsEnabled)
	{
		return;
	}

	TSet<AActor *> OverlappedActors;

	CollisionComponent->GetOverlappingActors(OverlappedActors, Filter);

	FVector MoveDirection = ArrowComponent->GetForwardVector();
	FVector DeltaLocation = DeltaTime * Speed * MoveDirection;

	for (auto Actor : OverlappedActors)
	{
		Actor->AddActorWorldOffset(DeltaLocation, false);
	}
}

void UConveyorComponent::Toggle()
{
	IsEnabled = !IsEnabled;
}