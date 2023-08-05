// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageActor.generated.h"

UCLASS()
class FIRSTUNREALPROJECT_API ADamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageActor();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UDamageTextWidget> Widget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDamageTextWidget* DamageWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* WidgetComponent;

	FTimerHandle TimerHandle;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	void SetDamage(float Damage);

};
