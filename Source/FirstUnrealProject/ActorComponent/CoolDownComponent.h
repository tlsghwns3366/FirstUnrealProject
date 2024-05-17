// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "CoolDownComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UCoolDownComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCoolDownComponent();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
		TArray<class UObject*> CoolDownTimeObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
		TArray<class UConsumableItemObject*>CoolDownTimeConsumableItem;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddCoolDownObject(UObject* Object);
	void RemoveCoolDownObject(UObject* Object);
	void RemoveBuffCoolDownObject(UConsumableItemObject* Object);


	void TickCoolTime(UObject* Object, float DeltaTime);
	void TickCoolTimeConsumbleItem(UConsumableItemObject* Object, float DeltaTime);
};
