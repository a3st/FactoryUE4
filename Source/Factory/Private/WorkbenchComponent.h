// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WorkbenchComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWorkbenchSlotsFilledSignature);

UENUM()
enum class EWorkbenchGrabType
{
	None,
	Physics,
	NonPhysics
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UWorkbenchComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWorkbenchComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void TakeControl();

	UFUNCTION(BlueprintCallable)
	void AddControllerLookUp(float AxisValue);

	UFUNCTION(BlueprintCallable)
	void AddControllerLookLeft(float AxisValue);

	UFUNCTION(BlueprintCallable)
	void PressClick();

	UFUNCTION(BlueprintCallable)
	void ReleaseClick();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxParts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentParts;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnWorkbenchSlotsFilledSignature OnWorkbenchSlotsFilled;

private:
	UPROPERTY()
	class UPhysicsHandleComponent *PhysicsHandleComponent;

	UPROPERTY()
	class UCameraComponent *CameraComponent;

	TWeakObjectPtr<AActor> CurrentGrabObject;
	EWorkbenchGrabType CurrentGrabType;

	float MoveX;
	float MoveY;
	float TargetZ;
};