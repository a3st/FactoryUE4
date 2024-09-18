// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WorkbenchSlotComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWorkbenchSlotAttachSignature, bool, IsReturned);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UWorkbenchSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWorkbenchSlotComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION()
	void TakeItem(bool &Result, AActor *&Object);

	UFUNCTION()
	void AttachItem(AActor* Object, bool IsReturned = false);

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnItemAttachEvent(
		UPrimitiveComponent *OverlappedComponent,
		AActor *OtherActor,
		UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult);

	UFUNCTION()
	void OnItemDestroyEvent(
		UPrimitiveComponent *OverlappedComponent,
		AActor *OtherActor,
		UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShowHints;

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnWorkbenchSlotAttachSignature OnSlotAttach;

private:
	UPROPERTY()
	class UBoxComponent *CollisionComponent;

	UPROPERTY()
	class UWidgetComponent *WidgetComponent;

	UPROPERTY()
	class UStaticMeshComponent *MeshComponent;
};