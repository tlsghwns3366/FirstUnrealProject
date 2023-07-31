// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SerchTarget.h"
#include "EnemyAIController.h"
#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"	   
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyCharacter.h"
#include "EnemyAnimInstance.h"

UBTService_SerchTarget::UBTService_SerchTarget()
{
	NodeName = TEXT("SearchTarget");
	Interval = 1.f;
}

void  UBTService_SerchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
