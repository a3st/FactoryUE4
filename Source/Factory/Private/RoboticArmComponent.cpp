// Fill out your copyright notice in the Description page of Project Settings.

#include "RoboticArmComponent.h"

#include "Components/SkeletalMeshComponent.h"

URoboticArmComponent::URoboticArmComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Filter = ConstructorHelpers::FClassFinder<AActor>(TEXT("/Game/Blueprints/Objects/BP_Box_Static")).Class;
}

void URoboticArmComponent::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	OnRoboticArmMoveEnd.AddDynamic(this, &URoboticArmComponent::OnRoboticMoveEndEvent_Internal);
}

void URoboticArmComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsMoved || IsDropped)
	{
		return;
	}

	FHitResult HitResult;

	FVector StartLocation = MeshComponent->GetSocketLocation(TEXT("arm_socket"));
	FVector TargetLocation = MeshComponent->GetUpVector() * 50.0f + StartLocation;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, TargetLocation, ECC_Visibility, QueryParams))
	{
		if ((Filter && Filter == HitResult.Actor->GetClass()) && HitResult.Actor->ActorHasTag(TEXT("Pickup")))
		{
			IsMoved = true;
			CurrentPickupObject = HitResult.Actor;

			HitResult.Component->SetSimulatePhysics(false);
			HitResult.Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			HitResult.Component->AttachToComponent(MeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("pickup_socket"));
		}
	}
}

bool URoboticArmComponent::Toggle()
{
	if (IsEnabled)
	{
		if (IsPickup && IsMoved)
		{
			return false;
		}

		IsPickup = false;
		IsDropped = false;
		IsMoved = false;
	}
	else
	{
		IsPickup = true;
		IsDropped = false;
		IsMoved = false;
	}

	IsEnabled = !IsEnabled;
	return true;
}

void URoboticArmComponent::OnRoboticMoveEndEvent_Internal()
{
	IsDropped = true;
	IsMoved = false;

	if (CurrentPickupObject.IsValid())
	{
		CurrentPickupObject->Destroy();
		CurrentPickupObject = nullptr;
	}
}