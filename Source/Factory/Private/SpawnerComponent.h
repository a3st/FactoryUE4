// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnerComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class USpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USpawnerComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Spawn(bool &Result, AActor *&NewActor);

	UFUNCTION(BlueprintCallable)
	void Unlock();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly)
	bool IsLocked;

private:
	UPROPERTY()
	class USceneComponent *SpawnPointComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SpawnClass;
};