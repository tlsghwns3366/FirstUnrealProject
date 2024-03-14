// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MainGameState.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EDayType : uint8
{
	E_None UMETA(DisplayName = "None"),
	E_DayTime UMETA(DisplayName = "DayTime"),
	E_Night UMETA(DisplayName = "Night"),
};

UCLASS()
class FIRSTUNREALPROJECT_API AMainGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AMainGameState();
public:
	UPROPERTY(BlueprintReadWrite)
		float GameTime;

	UPROPERTY(BlueprintReadWrite)
		float GameSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 DayCount;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		int32 Hours;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Minute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Second;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Year;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Month;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Day;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Latitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Longitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TimeZone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SunsetTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SunriseTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDayType DayType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsRun = true;;



protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void SetGameTime(float DeltaTime, float GS, float GT);
	void SetClock();
	void SetCelender();


};
