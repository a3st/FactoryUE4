// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ButtonComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonActionSignature);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UButtonComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UButtonComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Press();

	UFUNCTION(BlueprintCallable)
	void Release();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly)
	bool IsPressed;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnButtonActionSignature OnButtonAction;
};