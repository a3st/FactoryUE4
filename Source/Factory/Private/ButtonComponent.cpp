// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonComponent.h"

UButtonComponent::UButtonComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UButtonComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UButtonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UButtonComponent::Press()
{
	IsPressed = true;
}

void UButtonComponent::Release()
{
	IsPressed = false;
}