// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Character/CustomCharacter.h"
#include "Character/EnemyCharacter.h"
#include "ActorComponent/CharacterStateComponent.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Character = Cast<ACustomCharacter>(Pawn);
		if (IsValid(Character))
		{
			CharacterMovement = Character->GetCharacterMovement();
			SkeletalMeshComponent = Character->GetMesh();
		}
	}
}

void UCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(Character))
	{
		DeltaTimeX = DeltaSeconds;
		Velocity = CharacterMovement->Velocity;
		Speed = Velocity.Length();
		IsMoving = Speed > 3.0f;
		IsFalling = CharacterMovement->IsFalling();
		if (IsFalling || !IsZoom)
			IsPull = false;
		SetLocomotionState();
	}
}

void UCharacterAnimInstance::NativePostEvaluateAnimation()
{
	ResetTransitions();
}

void UCharacterAnimInstance::AnimNotify_Hit()
{
	OnAttackHit.Broadcast();
}

void UCharacterAnimInstance::AnimNotify_AttackStart()
{
	AttackStartTrace.Broadcast();
}

void UCharacterAnimInstance::AnimNotify_DrawArrow()
{
	OnDrawArrow.Broadcast();
}

void UCharacterAnimInstance::AnimNotify_Pull()
{
	IsPull = true;
}

void UCharacterAnimInstance::AnimNotify_PullOut()
{
	IsPull = false;
}

void UCharacterAnimInstance::SetLocomotionState()
{
	if (IsFalling)
		return;

	FVector Acceleration = CharacterMovement->GetCurrentAcceleration();
	FVector VelocityNormalize = Velocity;
	Acceleration.Normalize(0.0001f);
	VelocityNormalize.Normalize(0.0001f);
	if (FVector::DotProduct(Acceleration, VelocityNormalize) < -0.5f)
		CharacterLocoState = ECharacterState::E_Idle;
	else
	{
		if (Character->MainStateComponent)
		{
			if (IsZoom)
			{
				CharacterLocoState = ECharacterState::E_Zoom;
			}
			else if (IsMovementWithinThresholds(1.0f, Character->MainStateComponent->FinalState.RunSpeed, 0.5f))
			{
				CharacterLocoState = ECharacterState::E_Jog;
			}
			else if (IsMovementWithinThresholds(1.0f, 0.0f, 0.01f))
			{
				CharacterLocoState = ECharacterState::E_Walk;
			}
			else
			{
				CharacterLocoState = ECharacterState::E_Idle;
			}
		}
	}
}

bool UCharacterAnimInstance::IsMovementWithinThresholds(float MinCurrentSpeed, float MinMaxSpeed, float MinInputAcceleration)
{
	if (MinCurrentSpeed <= Speed && MinMaxSpeed <= CharacterMovement->GetMaxSpeed() && MinInputAcceleration <= InputVector.Length())
		return true;
	else
		return false;
}

void UCharacterAnimInstance::SetBowPower(float Value)
{
	BowPower = Value;
}

void UCharacterAnimInstance::ResetTargetRotation()
{
	if (Character != nullptr)
	{
		PrimaryTargetRotation = Character->GetActorRotation();
		SecondaryTargetRotation = Character->GetActorRotation();
	}
}
void UCharacterAnimInstance::ResetTransitions()
{
	PlayWalkStart = false;
	PlayJogStart = false;
}
FRotator UCharacterAnimInstance::GetTargetRotation()
{
	if (DoInputVectorRotation)
	{
		return FRotator(0.f, InputVector.Rotation().Yaw, 0.f);
	}
	else
		return FRotator(0.f, Velocity.Rotation().Yaw, 0.f);
}

void UCharacterAnimInstance::UpdateOnWalkEntry()
{
	if (Speed < 50.f)
	{
		if (Character)
		{
			StartRotation = Character->GetActorRotation();
			PrimaryTargetRotation = UKismetMathLibrary::MakeRotFromX(InputVector);
			SecondaryTargetRotation = PrimaryTargetRotation;
			WalkStartAngle = UKismetMathLibrary::NormalizedDeltaRotator(PrimaryTargetRotation, StartRotation).Yaw;
			PlayWalkStart = true;
		}
	}
	else
	{
		ResetTargetRotation();
	}
}

void UCharacterAnimInstance::UpdateOnJogEntry()
{
	if (Speed < 200.f)
	{
		if (Character)
		{
			StartRotation = Character->GetActorRotation();
			PrimaryTargetRotation = UKismetMathLibrary::MakeRotFromX(InputVector);
			SecondaryTargetRotation = PrimaryTargetRotation;
			JogStartAngle = UKismetMathLibrary::NormalizedDeltaRotator(PrimaryTargetRotation, StartRotation).Yaw;
			PlayJogStart = true;
		}
	}
	else
	{
		ResetTargetRotation();
	}
}

void UCharacterAnimInstance::UpdateOnIdleEntry()
{
	if (Character == nullptr)
		return;
	StartRotation = Character->GetActorRotation();
	PrimaryTargetRotation = StartRotation;
	SecondaryTargetRotation = PrimaryTargetRotation;
}

void UCharacterAnimInstance::UpdateOnZoomLoop()
{
	if (IsZoom)
	{
		StartRotation = Character->GetActorRotation();
		PrimaryTargetRotation = Character->GetController()->GetControlRotation();
		float InYaw = UKismetMathLibrary::RInterpTo(StartRotation, PrimaryTargetRotation, DeltaTimeX, 100.f).Yaw;
		Character->SetActorRotation(FRotator(0.f, InYaw, 0.f));
	}
	else
	{
		ResetTargetRotation();
	}
}

void UCharacterAnimInstance::UpdateLocomotionValue()
{
	PlayRate = UKismetMathLibrary::SafeDivide(Speed, FMath::Clamp(GetCurveValue("MoveData_Speed"), 50.f, 1000.f));
}

void UCharacterAnimInstance::UpdateDeltaValue()
{
	if (Character != nullptr)
	{
		FRotator Rot = Character->GetControlRotation();
		DeltaValue = UKismetMathLibrary::NormalizedDeltaRotator(SecondaryTargetRotation, Rot).Yaw;
		//UE_LOG(LogTemp, Log, TEXT("%f"), DeltaValue);
	}
}