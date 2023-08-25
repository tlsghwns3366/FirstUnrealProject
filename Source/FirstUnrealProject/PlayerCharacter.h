// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class FIRSTUNREALPROJECT_API APlayerCharacter : public ACustomCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
public:


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UPlayerAnimInstance* Anim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSourceComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USceneComponent* Scene;


	bool MouseInput = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
		virtual void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:

	virtual void Attack() override;

	void SetIsRunTrue();
	void SetIsRunFalse();

	UFUNCTION(BlueprintCallable, Category = "Item")
	void UseItem(UItemObject* Item);

};
