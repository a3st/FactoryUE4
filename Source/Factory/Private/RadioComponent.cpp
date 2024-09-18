// Fill out your copyright notice in the Description page of Project Settings.

#include "RadioComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"

URadioComponent::URadioComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URadioComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->GetComponents<UWidgetComponent>(WidgetComponents);
}

void URadioComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!ShowHints || CurrentGrabType != ERadioGrabType::None)
	{
		for (auto Widget : WidgetComponents)
		{
			Widget->SetVisibility(false);
		}
		return;
	}

	for (auto Widget : WidgetComponents)
	{
		if ((Widget->ComponentHasTag("Frequency") && Frequency < 1.0f) || (Widget->ComponentHasTag("Antenna") && Antenna < 1.0f))
		{
			Widget->SetVisibility(true);
		}
	}
}

void URadioComponent::StartGrabEdge(FName HitBoneName)
{
	if (IsTested)
	{
		return;
	}

	if (HitBoneName.Compare(TEXT("frequency1")) == 0)
	{
		CurrentGrabType = ERadioGrabType::Frequency;
	}
	else if (HitBoneName.Compare(TEXT("antenna_1")) == 0)
	{
		CurrentGrabType = ERadioGrabType::Antenna;
	}
}

void URadioComponent::StopGrabEdge()
{
	CurrentGrabType = ERadioGrabType::None;
}

void URadioComponent::UpdateGrabEdge(float DeltaTime, float X, float Y)
{
	if (CurrentGrabType == ERadioGrabType::None)
	{
		return;
	}

	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();

	FVector RadioLocation = GetOwner()->GetActorLocation();
	FVector RadioUp = GetOwner()->GetActorUpVector();

	switch (CurrentGrabType)
	{
		case ERadioGrabType::Frequency:
		{
			Frequency = FMath::Clamp(Frequency + X * 2.0f * DeltaTime, 0.0f, 1.0f);
			break;
		}
		case ERadioGrabType::Antenna:
		{
			Antenna = FMath::Clamp(Antenna - Y * 2.0f * DeltaTime, 0.0f, 1.0f);
			break;
		}
	}

	if (Frequency == 1.0f && Antenna == 1.0f && !IsTested)
	{
		IsTested = true;
		OnRadioTested.Broadcast();
	}
}