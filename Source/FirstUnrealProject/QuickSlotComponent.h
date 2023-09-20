// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuickSlotComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQuickSlotUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UQuickSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuickSlotComponent();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
		TArray<class UObject*> QuickSlot;

	UPROPERTY(BlueprintAssignable)
		FQuickSlotUpdated QuickSlotUpdste;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AddObject(int32 Num,UObject* Object);

	UFUNCTION(BlueprintCallable)
	void RemoveObject(int32 Num);
};
