// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionSystem.h"

#include "Character/EnemyCharacter.h"
#include "ActorComponent/AggroComponent.h"
#include "Anim/EnemyAnimInstance.h"
#include "ActorComponent/AttackSystemComponent.h"

AEnemyAIController::AEnemyAIController()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::SenseDetected);

	SetPerceptionSystem();
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	GetBlackboardComponent()->SetValueAsVector("StartPosition", GetPawn()->GetActorLocation());
}
void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunBehaviorTree(BehaviorTree);
}

void AEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AEnemyAIController::SetPerceptionSystem()
{
	if (SightConfig)
	{
		SightConfig->SightRadius = 1500.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->PeripheralVisionAngleDegrees = 45.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.f;
		SightConfig->PointOfViewBackwardOffset = 150.f;
		SightConfig->NearClippingRadius = 150.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
		AIPerceptionComponent->ConfigureSense(*SightConfig);
	}
	if (HearingConfig)
	{
		HearingConfig->HearingRange = 1500.f;
		HearingConfig->SetMaxAge(5.0f);
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
		AIPerceptionComponent->ConfigureSense(*HearingConfig);
	}
	if (DamageConfig)
	{
		DamageConfig->SetMaxAge(5.f);
		AIPerceptionComponent->ConfigureSense(*DamageConfig);
	}
}

void AEnemyAIController::SetCharacterState()
{
	if (AEnemyCharacter* AICharacter = Cast<AEnemyCharacter>(GetPawn()))
	{
		AICharacter->SetState();
	}
}

void AEnemyAIController::SetAggroSensitivity(float Value)
{
	if (AEnemyCharacter* AICharacter = Cast<AEnemyCharacter>(GetPawn()))
	{
		AICharacter->AggroComponent->SetAggroSensitivity(Value);
	}
}

void AEnemyAIController::SetAggroStop(bool Value)
{
	if (AEnemyCharacter* AICharacter = Cast<AEnemyCharacter>(GetPawn()))
	{
		AICharacter->AggroComponent->SetAggroStop(Value);
	}
}

void AEnemyAIController::SenseDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	auto DetectedSence = UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus);
	if (DetectedSence == UAISense_Sight::StaticClass())
	{
		SightDetected(Actor, Stimulus);
	}
	else if (DetectedSence == UAISense_Hearing::StaticClass())
	{
		HearingDetected(Actor, Stimulus);
	}
	else if (DetectedSence == UAISense_Damage::StaticClass())
	{
		DamageDetected(Actor, Stimulus);
	}
}

void AEnemyAIController::SightDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		if (AEnemyCharacter* AICharacter = Cast<AEnemyCharacter>(GetPawn()))
		{
			AICharacter->AggroTarget = Actor;
			AICharacter->AggroComponent->IsAggro = true;
		}
		GetBlackboardComponent()->SetValueAsObject("SightTarget", Actor);
	}
	else
	{
		if (AEnemyCharacter* AICharacter = Cast<AEnemyCharacter>(GetPawn()))
		{

			AICharacter->AggroTarget = nullptr;
			AICharacter->AggroComponent->IsAggro = false;
		}
		GetBlackboardComponent()->SetValueAsObject("SightTarget", nullptr);
	}
	GetBlackboardComponent()->SetValueAsVector("SenseLocation", Stimulus.StimulusLocation);
	SenseLocation = Stimulus.StimulusLocation;
}

void AEnemyAIController::HearingDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		if (GetBlackboardComponent()->GetValueAsBool("HearingSenseDetect") == false)
		{
			if (AEnemyCharacter* AICharacter = Cast<AEnemyCharacter>(GetPawn()))
			{
				AICharacter->AggroComponent->AddAggro(10.f);
			}
		}
		GetBlackboardComponent()->SetValueAsBool("HearingSenseDetect", true);
		GetBlackboardComponent()->SetValueAsVector("SenseLocation", Stimulus.StimulusLocation);
		SenseLocation = Stimulus.StimulusLocation;
	}
}

void AEnemyAIController::DamageDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		if (AEnemyCharacter* AICharacter = Cast<AEnemyCharacter>(GetPawn()))
		{
			AICharacter->AggroComponent->AddAggro(100.f);
		}
		GetBlackboardComponent()->SetValueAsVector("SenseLocation", Stimulus.StimulusLocation);
		SenseLocation = Stimulus.StimulusLocation;
	}
}

void AEnemyAIController::SetGoalLocation(FVector LocationValue)
{
	GoalLocation = LocationValue;
}

FVector AEnemyAIController::GetInputVector()
{
	if (GetPawn())
	{
		FVector InputVector = (GoalLocation - GetPawn()->GetActorLocation()).GetSafeNormal();
		InputVector.Z = 0;
		return InputVector;
	}
	return FVector::ZeroVector;
}

void AEnemyAIController::SetAiming(bool Value)
{
	if (AEnemyCharacter* AICharacter = Cast<AEnemyCharacter>(GetPawn()))
	{
		AICharacter->SetAiming(Value);
	}
}

void AEnemyAIController::StartAttack()
{
	if (AEnemyCharacter* AICharacter = Cast<AEnemyCharacter>(GetPawn()))
	{
		AICharacter->AttackAiming();
	}
}

void AEnemyAIController::SetTargetRotator(FRotator Value)
{
	if (AEnemyCharacter* AICharacter = Cast<AEnemyCharacter>(GetPawn()))
	{
		Value.Pitch = 0.f;
		Value.Yaw -= 5.f;
		AICharacter->SetActorRotation(Value);
	}
}

float AEnemyAIController::GetCharacterPower()
{
	if (AEnemyCharacter* AICharacter = Cast<AEnemyCharacter>(GetPawn()))
	{
		return AICharacter->AttackSystemComponent->GetLastStrength();
	}
	return 0.f;
}
