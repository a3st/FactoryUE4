// Fill out your copyright notice in the Description page of Project Settings.

#include "WorkbenchSlotComponent.h"

#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"

#include "WorkbenchComponent.h"

UWorkbenchSlotComponent::UWorkbenchSlotComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWorkbenchSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent = GetOwner()->FindComponentByClass<UBoxComponent>();
	WidgetComponent = GetOwner()->FindComponentByClass<UWidgetComponent>();
	MeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &UWorkbenchSlotComponent::OnItemAttachEvent);
}

void UWorkbenchSlotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ShowHints && !ItemClass)
	{
		WidgetComponent->SetVisibility(true);
	}
	else
	{
		WidgetComponent->SetVisibility(false);
	}

	if (ItemClass)
	{
		MeshComponent->SetVisibility(true);
	}
	else
	{
		MeshComponent->SetVisibility(false);
	}
}

void UWorkbenchSlotComponent::OnItemAttachEvent(
	UPrimitiveComponent *OverlappedComponent,
	AActor *OtherActor,
	UPrimitiveComponent *OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult &SweepResult)
{
	if (!OtherActor->ActorHasTag("Item"))
	{
		return;
	}

	if (ItemClass)
	{
		return;
	}

	AttachItem(OtherActor);
	OtherActor->Destroy();

	auto WorkbenchComponent = GetOwner()->GetParentActor()->FindComponentByClass<UWorkbenchComponent>();
	WorkbenchComponent->CurrentParts++;

	if (WorkbenchComponent->MaxParts == WorkbenchComponent->CurrentParts)
	{
		WorkbenchComponent->OnWorkbenchSlotsFilled.Broadcast();
	}
}

void UWorkbenchSlotComponent::TakeItem(bool &Result, AActor *&Object)
{
	if (!ItemClass)
	{
		Result = false;
		return;
	}

	FVector SpawnLocation = MeshComponent->GetComponentLocation() + FVector(0.0f, 0.0f, 40.0f);
	FRotator SpawnRotation = MeshComponent->GetComponentRotation();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	Object = GetWorld()->SpawnActor<AActor>(ItemClass, SpawnLocation, SpawnRotation, SpawnParameters);

	auto ItemMeshComponent = Object->FindComponentByClass<UStaticMeshComponent>();
	if (ItemMeshComponent)
	{
		ItemMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &UWorkbenchSlotComponent::OnItemDestroyEvent);
	}

	ItemClass = nullptr;
	Result = true;
}

void UWorkbenchSlotComponent::OnItemDestroyEvent(
	UPrimitiveComponent *OverlappedComponent,
	AActor *OtherActor,
	UPrimitiveComponent *OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult &SweepResult)
{
	if (!OtherActor->ActorHasTag("Link"))
	{
		AttachItem(OverlappedComponent->GetOwner(), true);
		OverlappedComponent->GetOwner()->Destroy();
	}
}

void UWorkbenchSlotComponent::AttachItem(AActor *Object, bool IsReturned)
{
	MeshComponent->SetStaticMesh(Object->FindComponentByClass<UStaticMeshComponent>()->GetStaticMesh());
	ItemClass = Object->GetClass();

	OnSlotAttach.Broadcast(IsReturned);
}