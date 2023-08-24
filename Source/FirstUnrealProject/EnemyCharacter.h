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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UEnemyInventoryComponent* EnemyInventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UEnemyAnimInstance* Anim;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UWidgetComponent* WidgetComponent;


	UPROPERTY(EditAnywhere, Category = Enemy, meta = (ClampMin = 0.0, ClampMax = 200.0))
		float RestTime;
	UPROPERTY(EditAnywhere, Category = Enemy, meta = (ClampMin = 0.0, ClampMax = 200.0))
		float DropExp;

	UPROPERTY(VisibleAnywhere)
		class AMainGameState* MainState;
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
		virtual void Attack() override;
	UFUNCTION(BlueprintImplementableEvent)
		void SetEnemyInventory();

public:
	void DropItem();

};
