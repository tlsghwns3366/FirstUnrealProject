// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AggroComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UAggroComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAggroComponent();
public:
	//Aggro
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro, meta = (ClampMin = 0.0f, ClampMax = 100.0f))
	float AggroCount = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
	bool IsAggro = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
	float AggroSensitivity = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
	float AggroTime = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
	float AggroCurrentTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
	float AggroDelayTime = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
	float AggroCurrentDelayTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
	bool IsAggroStop = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
	float AggroExp = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
	bool AggroT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
	bool AggroTAdd;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	//Aggro
	virtual void TickAggroCount(float DeltaTime);
	virtual void AddAggro(float Value);
	virtual void SetAggroStop(bool Value);
	virtual void SetAggroSensitivity(float Value);
		
};
