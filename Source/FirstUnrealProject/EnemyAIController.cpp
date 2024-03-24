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
#include "CustomCharacter.h"
#include "EnemyCharacter.h"
#include "CharacterStateComponent.h"

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
		AICharacter->MainStateComponent->AggroSensitivity = Value;
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
		if (ACustomCharacter* AICharacter = Cast<ACustomCharacter>(GetPawn()))
		{
			AICharacter->AggroTarget = Actor;
			AICharacter->MainStateComponent->IsAggro = true;
		}
		GetBlackboardComponent()->SetValueAsObject("SightTarget", Actor);
	}
	else
	{
		if (ACustomCharacter* AICharacter = Cast<ACustomCharacter>(GetPawn()))
		{

			AICharacter->AggroTarget = nullptr;
			AICharacter->MainStateComponent->IsAggro = false;
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
			if (ACustomCharacter* AICharacter = Cast<ACustomCharacter>(GetPawn()))
			{
				AICharacter->MainStateComponent->AddAggro(10.f);
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
		if (ACustomCharacter* AICharacter = Cast<ACustomCharacter>(GetPawn()))
		{
			AICharacter->MainStateComponent->AddAggro(100.f);
		}
		GetBlackboardComponent()->SetValueAsVector("SenseLocation", Stimulus.StimulusLocation);
		SenseLocation = Stimulus.StimulusLocation;
	}
}
