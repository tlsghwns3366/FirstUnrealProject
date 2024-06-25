// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageComponent();
public:
	UPROPERTY(VisibleAnywhere)
	class UCharacterStateComponent* MainStateComponent;

	UPROPERTY(VisibleAnywhere)
	class UCoolDownComponent* CoolDownComponent;

	UPROPERTY(VisibleAnywhere)
		class ACustomCharacter* Character;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void OnDamaged(float Damage);
	virtual float OnCriticalDamaged(float Damage, AActor* CauserActor);
	virtual void DamageTaken(float Damage,FColor Color);
	
	const bool CheckCritical(AActor* CauserActor);
	const float GetCriticalDamage();
	const float GetPhysicalDamage();

	void SetDamageOverTime(UObject* DamageObject, float Damage, float DamageOverTime);
};
