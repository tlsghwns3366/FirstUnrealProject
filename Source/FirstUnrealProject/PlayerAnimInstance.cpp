// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> AnimMontage(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Player/AM_ComboPunch.AM_ComboPunch'"));
	if (AnimMontage.Succeeded())
	{
		AttackMontage = AnimMontage.Object;
	}
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Player = Cast<APlayerCharacter>(Pawn);
		if (IsValid(Player))
		{
			CharacterMovement = Player->GetCharacterMovement();
		}
	}
}

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(Player))
	{
		//UE_LOG(LogTemp, Log, TEXT("%f"), Speed);
		Velocity = CharacterMovement->Velocity;
		Speed = Velocity.Size();
		IsMoving = Speed > 3.0f;
		IsFalling = CharacterMovement->IsFalling();
	}
}

void UPlayerAnimInstance::PlayMontage()
{
}

void UPlayerAnimInstance::PlayHitReactMontage()
{
}

void UPlayerAnimInstance::AnimNotify_Hit()
{
	OnAttackHit.Broadcast();
}
