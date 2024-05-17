// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/BTTask_Aiming.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Controller/EnemyAIController.h"

UBTTask_Aiming::UBTTask_Aiming()
{
	NodeName = TEXT("Aiming");
	// accept only actors and vectors
	bNotifyTick = true;
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Aiming, BlackboardKey), AActor::StaticClass());
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Aiming, BlackboardKey));
	BlackboardKey.AddRotatorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Aiming, BlackboardKey));
}
EBTNodeResult::Type UBTTask_Aiming::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (AIController == NULL || AIController->GetPawn() == NULL)
	{
		return EBTNodeResult::Failed;
	}
	EBTNodeResult::Type Result = EBTNodeResult::InProgress;
	AIController->SetAiming(true);
	return Result;
}

void UBTTask_Aiming::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (AIController == NULL || AIController->GetPawn() == NULL)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	else
	{
		AIController->StartAttack();
		APawn* Pawn = AIController->GetPawn();
		const FVector PawnLocation = Pawn->GetActorLocation();
		const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();

		if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
		{
			UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
			AActor* ActorValue = Cast<AActor>(KeyValue);
			if(ActorValue != NULL)
				AIController->SetTargetRotator((ActorValue->GetActorLocation() - PawnLocation).Rotation());
			else
			{
				AIController->SetAiming(false);
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}

		}
		else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
		{
			const FVector KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
			AIController->SetTargetRotator((KeyValue - PawnLocation).Rotation());
		}
		else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
		{
			const FRotator KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Rotator>(BlackboardKey.GetSelectedKeyID());
			AIController->SetTargetRotator(KeyValue);
		}
		if (AIController->GetCharacterPower() >= 90.f)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		else
			FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	}
}

EBTNodeResult::Type UBTTask_Aiming::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (AIController == NULL || AIController->GetPawn() == NULL)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	AIController->SetAiming(false);
	return EBTNodeResult::Aborted;
}