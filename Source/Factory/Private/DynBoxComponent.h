// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DynBoxComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBoxOpenedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBoxSlotAddedSignature, bool, IsReturned);

UENUM()
enum class EDynBoxGrabType : uint8
{
	None,
	Left,
	Right,
	Front,
	Back
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UDynBoxComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDynBoxComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION()
	void StartGrabEdge(FName HitBoneName);

	UFUNCTION()
	void StopGrabEdge();

	UFUNCTION()
	void UpdateGrabEdge(float DeltaTime, float X, float Y);

	UFUNCTION()
	void TakeItem(bool &Result, AActor *&Object);

	UFUNCTION()
	void AddItem(AActor *Object, bool IsReturned = false);

protected:
	virtual void BeginPlay() override;

private:
	void UpdateLastSlotMesh();

	UFUNCTION()
	void OnItemDestroyEvent(
		UPrimitiveComponent *OverlappedComponent,
		AActor *OtherActor,
		UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult);

public:
	UPROPERTY(BlueprintReadOnly)
	float Left;

	UPROPERTY(BlueprintReadOnly)
	float Right;

	UPROPERTY(BlueprintReadOnly)
	float Front;

	UPROPERTY(BlueprintReadOnly)
	float Back;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShowHints;

	UPROPERTY(BlueprintReadOnly)
	bool IsOpened;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnBoxOpenedSignature OnBoxOpened;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnBoxSlotAddedSignature OnBoxSlotAdded;

private:
	UPROPERTY()
	class UWidgetComponent *WidgetComponent;

	UPROPERTY()
	class UStaticMeshComponent *SlotMeshComponent;

	EDynBoxGrabType CurrentGrabType;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> Slots;
};