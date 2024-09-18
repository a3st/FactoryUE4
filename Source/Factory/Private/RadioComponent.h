// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RadioComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRadioTestedSignature);

UENUM()
enum class ERadioGrabType
{
	None,
	Frequency,
	Antenna
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class URadioComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URadioComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION()
	void StartGrabEdge(FName HitBoneName);

	UFUNCTION()
	void StopGrabEdge();

	UFUNCTION()
	void UpdateGrabEdge(float DeltaTime, float X, float Y);

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly)
	float Antenna;

	UPROPERTY(BlueprintReadOnly)
	float Frequency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShowHints;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnRadioTestedSignature OnRadioTested;

	UPROPERTY(BlueprintReadOnly)
	bool IsTested;

private:
	ERadioGrabType CurrentGrabType;

	UPROPERTY()
	TArray<class UWidgetComponent*> WidgetComponents;
};