// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNodeRandomPosition.h"
#include "NavigationSystem.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

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
			if (NavSystem->GetRandomPointInNavigableRadius(CurrentPawn->GetActorLocation(), 500.f, RandomLocation))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("RandomPosition")), RandomLocation.Location);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Type();
}
