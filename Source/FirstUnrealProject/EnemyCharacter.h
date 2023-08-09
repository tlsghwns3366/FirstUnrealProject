// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomCharacter.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class FIRSTUNREALPROJECT_API AEnemyCharacter : public ACustomCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DestroyTime = 60.f;// Default 60

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		class UEnemyStateActorComponent* EnemyStateComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		class UEnemyInventoryComponent* EnemyInventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UEnemyAnimInstance* Anim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UWidgetComponent* WidgetComponent;
public:
	FTimerHandle TimerHandle;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
public:

	UFUNCTION(BlueprintImplementableEvent)
		void Attack();
	UFUNCTION(BlueprintImplementableEvent)
		void SetEnemyInventory();

public:
	virtual void OnHitActor() override;
	void DropItem();

};
