// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Move.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTTask_Move::UBTTask_Move(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Move");
}

EBTNodeResult::Type UBTTask_Move::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto const Cont = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		auto const Location = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(Cont, Location);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
