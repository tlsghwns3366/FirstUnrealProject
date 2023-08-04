// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class FIRSTUNREALPROJECT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsAttacking = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DestroyTime = 60.f;// Default 60

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UEnemyStateActorComponent* EnemyStateComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UEnemyInventoryComponent* EnemyInventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UEnemyAnimInstance* Anim;
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

	UFUNCTION()
	virtual void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:

	UFUNCTION(BlueprintImplementableEvent)
		void Attack();
	UFUNCTION(BlueprintImplementableEvent)
		void SetEnemyInventory();

public:
	void OnHit();
	void DropItem();

};
