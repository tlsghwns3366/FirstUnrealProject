// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameState.h"
#include "Misc/DateTime.h"
#include "Components/DirectionalLightComponent.h"


AMainGameState::AMainGameState()
{
	Year = Month = Day = 1; //DefaultDate
}

void AMainGameState::BeginPlay()
{
	Super::BeginPlay();
	GameSpeed = 1; //DefaultGameSpeed = 1
	GameTime = Hours + (Minute / 60.f) + (Second / 3600.f);
}

void AMainGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetGameTime(DeltaTime, GameSpeed, GameTime);
	SetClock();
	SetCelender();

	//UE_LOG(LogTemp, Log, TEXT("%d , %d, %d"), Clock[0], Clock[1], Clock[2]);
	//UE_LOG(LogTemp, Log, TEXT("%d , %d, %d"), GameDate[0], GameDate[1], GameDate[2]);
}

void AMainGameState::SetGameTime(float DeltaTime, float GS, float GT)
{
	//GS = GameSpeed, GT = GameTime
	GameTime = ((DeltaTime * GS) + GT);
	DayCount = GameTime / 24.f;
	if (GameTime >= 24.f)
		GameTime -= 24.f;
}

void AMainGameState::SetClock()
{
	Second = (int32)(GameTime * 3600) % 60 ;
	Minute = (int32)(GameTime * 60) % 60;
	Hours = (int32)GameTime;
}

void AMainGameState::SetCelender()
{
	Day += DayCount;
	if (Day > FDateTime::DaysInMonth(Year, Month))
	{
		Day = 1;
		Month++;
	}
	if (Month > 12)
	{
		Year++;
		Month = 1;
	}
}