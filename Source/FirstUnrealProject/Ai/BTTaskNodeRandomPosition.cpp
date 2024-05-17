// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNodeRandomPosition.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Controller/EnemyAIController.h"

UBTTaskNodeRandomPosition::UBTTaskNodeRandomPosition()
{
	NodeName = TEXT("RandomPosition");
}

EBTNodeResult::Type UBTTaskNodeRandomPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn != nullptr)
	{
		auto NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		if (NavSystem != nullptr)
		{
			FNavLocation RandomLocation; 
			auto Location = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName(TEXT("StartPosition")));
			if (NavSystem->GetRandomPointInNavigableRadius(Location, SearchRadius, RandomLocation))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("RandomPosition")), RandomLocation.Location);
				FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
