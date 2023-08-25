// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CustomCharacter.generated.h"

UCLASS()
class FIRSTUNREALPROJECT_API ACustomCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACustomCharacter();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UDamageComponent* DamageComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UAttackSystemComponent* AttackSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UCharacterStateComponent* MainStateComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction");
		float TraceDistance;
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

	virtual void Attack();
	virtual void OnHitActor();
	virtual void Interaction();
};
