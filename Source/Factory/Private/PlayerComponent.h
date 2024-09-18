// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerComponent();

	UFUNCTION(BlueprintCallable)
	void PressInteract();

	UFUNCTION(BlueprintCallable)
	void ReleaseInteract();

	UFUNCTION(BlueprintCallable)
	void TakeControl();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InteractDistance;

private:
	UPROPERTY()
	class UCameraComponent *CameraComponent;

	TWeakObjectPtr<AActor> CurrentInteractObject;
};