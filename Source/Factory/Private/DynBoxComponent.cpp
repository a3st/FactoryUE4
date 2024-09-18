// Fill out your copyright notice in the Description page of Project Settings.

#include "DynBoxComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"

UDynBoxComponent::UDynBoxComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Slots.Add(ConstructorHelpers::FClassFinder<AActor>(TEXT("/Game/Blueprints/Objects/BP_Radio_Front")).Class);
	Slots.Add(ConstructorHelpers::FClassFinder<AActor>(TEXT("/Game/Blueprints/Objects/BP_Radio_Back")).Class);
	Slots.Add(ConstructorHelpers::FClassFinder<AActor>(TEXT("/Game/Blueprints/Objects/BP_Radio_Chip")).Class);
}

void UDynBoxComponent::BeginPlay()
{
	Super::BeginPlay();

	SlotMeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	WidgetComponent = GetOwner()->FindComponentByClass<UWidgetComponent>();
}

void UDynBoxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!ShowHints || CurrentGrabType != EDynBoxGrabType::None || IsOpened)
	{
		WidgetComponent->SetVisibility(false);
		return;
	}

	if (Left < 1.0f || Right < 1.0f)
	{
		WidgetComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	}

	if (Front < 1.0f || Back < 1.0f)
	{
		WidgetComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 90.0f));
	}

	WidgetComponent->SetVisibility(true);
}

void UDynBoxComponent::StartGrabEdge(FName HitBoneName)
{
	if (HitBoneName.Compare(TEXT("left")) == 0)
	{
		if (Front < 1.0f || Back < 1.0f)
		{
			return;
		}
		CurrentGrabType = EDynBoxGrabType::Left;
	}
	else if (HitBoneName.Compare(TEXT("right")) == 0)
	{
		if (Front < 1.0f || Back < 1.0f)
		{
			return;
		}
		CurrentGrabType = EDynBoxGrabType::Right;
	}
	else if (HitBoneName.Compare(TEXT("front")) == 0)
	{
		CurrentGrabType = EDynBoxGrabType::Front;
	}
	else if (HitBoneName.Compare(TEXT("back")) == 0)
	{
		CurrentGrabType = EDynBoxGrabType::Back;
	}
}

void UDynBoxComponent::StopGrabEdge()
{
	CurrentGrabType = EDynBoxGrabType::None;
}

void UDynBoxComponent::UpdateGrabEdge(float DeltaTime, float X, float Y)
{
	if (CurrentGrabType == EDynBoxGrabType::None)
	{
		return;
	}

	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();

	FVector BoxLocation = GetOwner()->GetActorLocation();

	FVector2D StartScreenLocation;
	PlayerController->ProjectWorldLocationToScreen(BoxLocation, StartScreenLocation);

	FVector2D MouseOffset(X, Y);
	MouseOffset.Normalize(1.0f);

	switch (CurrentGrabType)
	{
		case EDynBoxGrabType::Left:
		case EDynBoxGrabType::Right:
		{
			FVector BoxForward = GetOwner()->GetActorForwardVector();

			FVector2D EndScreenLocation;
			PlayerController->ProjectWorldLocationToScreen(BoxLocation + BoxForward, EndScreenLocation);

			FVector2D NormScreenOffset = EndScreenLocation - StartScreenLocation;
			NormScreenOffset.Normalize();

			float Value = FMath::Abs((MouseOffset * NormScreenOffset).Size());
			if (Value > 0.7f)
			{
				if (CurrentGrabType == EDynBoxGrabType::Left)
				{
					Left = FMath::Clamp(Left + Value * 2.0f * DeltaTime, 0.0f, 1.0f);
				}
				else
				{
					Right = FMath::Clamp(Right + Value * 2.0f * DeltaTime, 0.0f, 1.0f);
				}
			}
			break;
		}
		case EDynBoxGrabType::Front:
		case EDynBoxGrabType::Back:
		{
			FVector BoxRight = GetOwner()->GetActorRightVector();

			FVector2D EndScreenLocation;
			PlayerController->ProjectWorldLocationToScreen(BoxLocation + BoxRight, EndScreenLocation);

			FVector2D NormScreenOffset = EndScreenLocation - StartScreenLocation;
			NormScreenOffset.Normalize();

			float Value = FMath::Abs((MouseOffset * NormScreenOffset).Size());
			if (Value > 0.7f)
			{
				if (CurrentGrabType == EDynBoxGrabType::Front)
				{
					Front = FMath::Clamp(Front + Value * 2.0f * DeltaTime, 0.0f, 1.0f);
				}
				else
				{
					Back = FMath::Clamp(Back + Value * 2.0f * DeltaTime, 0.0f, 1.0f);
				}
			}
			break;
		}
	}

	if (Left == 1.0f && Right == 1.0f && Front == 1.0f && Back == 1.0f && !IsOpened)
	{
		IsOpened = true;
		OnBoxOpened.Broadcast();

		UpdateLastSlotMesh();
	}
}

void UDynBoxComponent::UpdateLastSlotMesh()
{
	if (Slots.Num() == 0)
	{
		SlotMeshComponent->SetVisibility(false);
	}
	else
	{
		FVector SpawnLocation = SlotMeshComponent->GetComponentLocation() + FVector(0.0f, 0.0f, 30.0f);
		FRotator SpawnRotation = SlotMeshComponent->GetComponentRotation();

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		auto ItemObject = GetWorld()->SpawnActor<AActor>(Slots.Top().Get(), SpawnLocation, SpawnRotation, SpawnParameters);

		auto ItemMeshComponent = ItemObject->FindComponentByClass<UStaticMeshComponent>();
		if (ItemMeshComponent)
		{
			UStaticMesh *StaticMesh = ItemMeshComponent->GetStaticMesh();
			SlotMeshComponent->SetStaticMesh(StaticMesh);
			SlotMeshComponent->SetVisibility(true);
		}

		ItemObject->Destroy();
	}
}

void UDynBoxComponent::TakeItem(bool &Result, AActor *&Object)
{
	if (Slots.Num() == 0)
	{
		Result = false;
		return;
	}

	FVector SpawnLocation = SlotMeshComponent->GetComponentLocation() + FVector(0.0f, 0.0f, 40.0f);
	FRotator SpawnRotation = SlotMeshComponent->GetComponentRotation();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	Object = GetWorld()->SpawnActor<AActor>(Slots.Top().Get(), SpawnLocation, SpawnRotation, SpawnParameters);

	auto ItemMeshComponent = Object->FindComponentByClass<UStaticMeshComponent>();
	if (ItemMeshComponent)
	{
		ItemMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &UDynBoxComponent::OnItemDestroyEvent);
	}

	Slots.Pop();

	UpdateLastSlotMesh();
	Result = true;
}

void UDynBoxComponent::AddItem(AActor *Object, bool IsReturned)
{
	Slots.Push(Object->GetClass());

	UpdateLastSlotMesh();

	OnBoxSlotAdded.Broadcast(IsReturned);
}

void UDynBoxComponent::OnItemDestroyEvent(
	UPrimitiveComponent *OverlappedComponent,
	AActor *OtherActor,
	UPrimitiveComponent *OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult &SweepResult)
{
	if (!OtherActor->ActorHasTag("Slot"))
	{
		AddItem(OverlappedComponent->GetOwner(), true);
		OverlappedComponent->GetOwner()->Destroy();
	}
}