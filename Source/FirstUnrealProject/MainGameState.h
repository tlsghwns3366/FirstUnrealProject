// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MainGameState.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API AMainGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AMainGameState();
public:
	UPROPERTY(VisibleAnywhere)
		float GameTime;
	UPROPERTY(VisibleAnywhere)
		float GameSpeed;

	UPROPERTY(VisibleAnywhere)
		float InitialGameTime;
	UPROPERTY(VisibleAnywhere)
		int32 DayCount;

	UPROPERTY(BlueprintReadWrite)
		int32 Hours;
	UPROPERTY(BlueprintReadWrite)
		int32 Minute;
	UPROPERTY(BlueprintReadWrite)
		int32 Second;

	UPROPERTY(BlueprintReadWrite)
		int32 Year;
	UPROPERTY(BlueprintReadWrite)
		int32 Month;
	UPROPERTY(BlueprintReadWrite)
		int32 Day;


	UPROPERTY(VisibleAnywhere)
	class UDirectionalLightComponent* DirectionalLightComponent;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void SetGameTime(float DeltaTime, float GS, float GT);
	void SetClock();
	void SetCelender();
};
