// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStateComponent.h"
#include "EnemyStateActorComponent.generated.h"

/**
 * 
 */

UCLASS()
class FIRSTUNREALPROJECT_API UEnemyStateActorComponent : public UCharacterStateComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Enemy, meta = (ClampMin = 0.0, ClampMax = 100.0))
		float RestTime;
	UPROPERTY(EditAnywhere, Category = Enemy,meta = (ClampMin = 0.0,ClampMax = 100.0))
		float SleepTime;;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
		FVector StartPosition;
};
