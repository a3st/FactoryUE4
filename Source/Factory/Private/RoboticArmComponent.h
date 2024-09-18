// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RoboticArmComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoboticArmMoveEndSignature);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class URoboticArmComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URoboticArmComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool Toggle();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnRoboticMoveEndEvent_Internal();

public:
	UPROPERTY(BlueprintReadOnly)
	bool IsPickup;

	UPROPERTY(BlueprintReadOnly)
	bool IsDropped;

	UPROPERTY(BlueprintReadOnly)
	bool IsMoved;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnRoboticArmMoveEndSignature OnRoboticArmMoveEnd;

	UPROPERTY(BlueprintReadOnly)
	bool IsEnabled;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Filter;

	UPROPERTY()
	class USkeletalMeshComponent *MeshComponent;

	TWeakObjectPtr<AActor> CurrentPickupObject;
};