// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackSystemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UAttackSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackSystemComponent();
	UPROPERTY(EditAnywhere)
		class ACustomCharacter* Character;


	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* HitReactMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* CharacterAttackMontage = nullptr;


	UPROPERTY(EditAnywhere, Category = "Animation")
		class UPlayerAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 AttackIndex;

	TArray<AActor*> ActorsToIgnore;




protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:

	UFUNCTION()
		void OnNotifyBeginRecieved(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	virtual void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	bool PlayAttackMontage(UAnimMontage* AttackMontage);
	bool PlayHitReactMontage();
	void Attack();
	void Trace();
};
