// Fill out your copyright notice in the Description page of Project Settings.

#include "WorkbenchComponent.h"

#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "DynBoxComponent.h"
#include "WorkbenchSlotComponent.h"
#include "RadioComponent.h"

UWorkbenchComponent::UWorkbenchComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxParts = 3;
}

void UWorkbenchComponent::BeginPlay()
{
	Super::BeginPlay();

	CameraComponent = GetOwner()->FindComponentByClass<UCameraComponent>();
	PhysicsHandleComponent = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
}

void UWorkbenchComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentGrabObject.IsValid())
	{
		if (CurrentGrabType == EWorkbenchGrabType::NonPhysics)
		{
			if (CurrentGrabObject->ActorHasTag(TEXT("Box")))
			{
				CurrentGrabObject->FindComponentByClass<UDynBoxComponent>()->UpdateGrabEdge(DeltaTime, MoveX, MoveY);
			}
			else if (CurrentGrabObject->ActorHasTag("Radio"))
			{
				CurrentGrabObject->FindComponentByClass<URadioComponent>()->UpdateGrabEdge(DeltaTime, MoveX, MoveY);
			}
		}
		else
		{
			FVector TargetForward = GetOwner()->GetActorForwardVector();
			FVector TargetRight = GetOwner()->GetActorRightVector();
			FVector TargetLocation = CurrentGrabObject->GetActorLocation();
			TargetLocation.Z = TargetZ;

			PhysicsHandleComponent->SetTargetLocation(TargetLocation - TargetForward * MoveY * 2.0f + TargetRight * MoveX * 2.0f);
		}

		APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();

		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);

		// Заблокировать курсор в центре экрана
		PlayerController->SetMouseLocation(ViewportSize.X / 2, ViewportSize.Y / 2);
	}
}

void UWorkbenchComponent::TakeControl()
{
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->SetViewTargetWithBlend(GetOwner(), 2.0f, VTBlend_Cubic, 0.0f, true);
	PlayerController->bShowMouseCursor = true;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
										   {
		APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
		PlayerController->Possess(Cast<APawn>(GetOwner())); }, 2.0f, false);
}

void UWorkbenchComponent::AddControllerLookUp(float AxisValue)
{
	MoveY = AxisValue;
}

void UWorkbenchComponent::AddControllerLookLeft(float AxisValue)
{
	MoveX = AxisValue;
}

void UWorkbenchComponent::PressClick()
{
	FHitResult HitResult;

	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController->GetHitResultUnderCursor(ECC_Camera, false, HitResult))
	{
		if (HitResult.Actor->ActorHasTag(TEXT("Box")))
		{
			auto Component = HitResult.Actor->FindComponentByClass<UDynBoxComponent>();
			if (HitResult.BoneName.Compare(TEXT("root")) == 0)
			{
				if (Component->IsOpened)
				{
					bool Result;
					AActor *Object;
					Component->TakeItem(Result, Object);

					PhysicsHandleComponent->GrabComponentAtLocationWithRotation(
						Cast<UPrimitiveComponent>(Object->FindComponentByClass<UStaticMeshComponent>()),
						FName(),
						Object->GetActorLocation(),
						Object->GetActorRotation());
					TargetZ = Object->GetActorLocation().Z;

					CurrentGrabObject = Object;
					CurrentGrabType = EWorkbenchGrabType::Physics;
					PlayerController->bShowMouseCursor = false;
				}
			}
			else
			{
				Component->StartGrabEdge(HitResult.BoneName);

				CurrentGrabObject = HitResult.Actor;
				CurrentGrabType = EWorkbenchGrabType::NonPhysics;
				PlayerController->bShowMouseCursor = false;
			}
		}
		else if (HitResult.Actor->ActorHasTag("Slot"))
		{
			auto Component = HitResult.Actor->FindComponentByClass<UWorkbenchSlotComponent>();
			if (Component->ItemClass)
			{
				bool Result;
				AActor *Object;
				Component->TakeItem(Result, Object);

				PhysicsHandleComponent->GrabComponentAtLocationWithRotation(
					Cast<UPrimitiveComponent>(Object->FindComponentByClass<UStaticMeshComponent>()),
					FName(),
					Object->GetActorLocation(),
					Object->GetActorRotation());
				TargetZ = Object->GetActorLocation().Z;

				CurrentGrabObject = Object;
				CurrentGrabType = EWorkbenchGrabType::Physics;
				PlayerController->bShowMouseCursor = false;
			}
		}
		else if (HitResult.Actor->ActorHasTag("Radio"))
		{
			auto Component = HitResult.Actor->FindComponentByClass<URadioComponent>();
			Component->StartGrabEdge(HitResult.BoneName);

			CurrentGrabObject = HitResult.Actor;
			CurrentGrabType = EWorkbenchGrabType::NonPhysics;
			PlayerController->bShowMouseCursor = false;
		}
	}
}

void UWorkbenchComponent::ReleaseClick()
{
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	if (CurrentGrabObject.IsValid())
	{
		if (CurrentGrabType == EWorkbenchGrabType::NonPhysics)
		{
			if (CurrentGrabObject->ActorHasTag("Box"))
			{
				CurrentGrabObject->FindComponentByClass<UDynBoxComponent>()->StopGrabEdge();
			}
			else if (CurrentGrabObject->ActorHasTag("Radio"))
			{
				CurrentGrabObject->FindComponentByClass<URadioComponent>()->StopGrabEdge();
			}
		}
		else
		{
			PhysicsHandleComponent->ReleaseComponent();
		}
		CurrentGrabObject = nullptr;
	}

	CurrentGrabType = EWorkbenchGrabType::None;
	PlayerController->bShowMouseCursor = true;
}