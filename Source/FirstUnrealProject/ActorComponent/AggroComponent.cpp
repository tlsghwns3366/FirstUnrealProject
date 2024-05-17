// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/AggroComponent.h"

#include "Character/EnemyCharacter.h"

// Sets default values for this component's properties
UAggroComponent::UAggroComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAggroComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void UAggroComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		TickAggroCount(DeltaTime);
	// ...
}

void UAggroComponent::TickAggroCount(float DeltaTime)
{
	if (!IsAggroStop)
	{
		if (IsAggro)
		{
			if (AggroCount <= 100.f)
			{
				if (AggroT || AggroTAdd)
				{
					float Alpha = AggroCount / 100.f;
					AggroCurrentTime = AggroTime * powf(Alpha, 1.f / AggroExp);
					AggroT = false;
					AggroTAdd = false;
				}
				float Ratio = FMath::InterpEaseIn(0.f, 1.0f, FMath::Clamp(AggroCurrentTime / AggroTime, 0.f, 1.0f), AggroExp);
				AggroCount = FMath::Lerp(0.f, 100.f, Ratio);
				AggroCurrentTime = FMath::Clamp(AggroCurrentTime + DeltaTime * AggroSensitivity, 0.f, AggroTime);
				AggroCurrentDelayTime = AggroDelayTime;
			}
		}
		else
		{
			if (AggroCount >= 0 && AggroCurrentDelayTime <= 0)
			{
				if (!AggroT || AggroTAdd)
				{
					float Alpha = AggroCount / 100.f;
					AggroCurrentTime = AggroTime * (1.f - powf(1.f - Alpha, 1.f / AggroExp));
					AggroT = true;
					AggroTAdd = false;
				}
				float Ratio = FMath::InterpEaseOut(0.f, 1.0f, FMath::Clamp(AggroCurrentTime / AggroTime, 0.f, 1.0f), AggroExp);
				AggroCount = FMath::Lerp(0.f, 100.f, Ratio);
				AggroCurrentTime = FMath::Clamp(AggroCurrentTime - DeltaTime, 0.f, AggroTime);
			}
			else
			{
				AggroCurrentDelayTime = FMath::Clamp(AggroCurrentDelayTime - DeltaTime, 0.f, AggroDelayTime);
			}
		}
	}
}

void UAggroComponent::AddAggro(float Value)
{
	AggroCurrentDelayTime = AggroDelayTime;
	AggroCount += Value * AggroSensitivity;
	AggroTAdd = true;
	if (AggroCount > 100.f)
		AggroCount = 100.f;
}

void UAggroComponent::SetAggroStop(bool Value)
{
	IsAggroStop = Value;
}

void UAggroComponent::SetAggroSensitivity(float Value)
{
	AggroSensitivity = Value;
}

