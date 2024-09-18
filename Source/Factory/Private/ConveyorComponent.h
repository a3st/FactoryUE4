// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ConveyorComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UConveyorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UConveyorComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Toggle();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly)
	bool IsEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Speed;

private:
	UPROPERTY()
	class UBoxComponent *CollisionComponent;

	UPROPERTY()
	class UArrowComponent *ArrowComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Filter;
};