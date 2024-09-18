// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerComponent.h"

#include "Camera/CameraComponent.h"

#include "ButtonComponent.h"

UPlayerComponent::UPlayerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	InteractDistance = 200.0f;
}

void UPlayerComponent::BeginPlay()
{
	Super::BeginPlay();

	CameraComponent = GetOwner()->FindComponentByClass<UCameraComponent>();
}

void UPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerComponent::PressInteract()
{
	FHitResult HitResult;

	FVector CameraLocation = CameraComponent->GetComponentLocation();
	FVector TargetLocation = CameraComponent->GetForwardVector() * InteractDistance + CameraLocation;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	if (GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TargetLocation, ECC_Camera, QueryParams))
	{
		if (HitResult.Actor->ActorHasTag(TEXT("Button")))
		{
			CurrentInteractObject = HitResult.Actor;

			auto ButtonComponent = Cast<UButtonComponent>(CurrentInteractObject->GetComponentByClass(UButtonComponent::StaticClass()));
			ButtonComponent->Press();
		}
	}
}

void UPlayerComponent::ReleaseInteract()
{
	if (CurrentInteractObject.IsValid())
	{
		auto ButtonComponent = Cast<UButtonComponent>(CurrentInteractObject->GetComponentByClass(UButtonComponent::StaticClass()));
		ButtonComponent->Release();

		CurrentInteractObject = nullptr;
	}
}

void UPlayerComponent::TakeControl()
{
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->SetViewTargetWithBlend(GetOwner(), 2.0f, VTBlend_Cubic, 0.0f, true);
	PlayerController->bShowMouseCursor = false;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
										   {
		APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
		PlayerController->Possess(Cast<APawn>(GetOwner())); }, 2.0f, false);
}