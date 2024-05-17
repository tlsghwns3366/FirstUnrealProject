// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileActor.h"
#include "ProjectileActor_Arrow.generated.h"


DECLARE_DELEGATE_TwoParams(FOnArrowHit,AActor*,float);

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API AProjectileActor_Arrow : public AProjectileActor
{
	GENERATED_BODY()

public:
	AProjectileActor_Arrow();
public:


	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LastValue;

public:
	FOnArrowHit OnArrowHit;

public:

	virtual void BeginPlay() override;

	UFUNCTION()
	void HitBoxOverlap(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void SetArrow(FRotator ArrowRotator, float Value);
	void SetHitBoxIgnore(AActor* IgnoreActor);
	void StopArrow();
	void Arrowinitialization(float SpeedValue, float DestroyTimeValue, float GravityValue);
};
