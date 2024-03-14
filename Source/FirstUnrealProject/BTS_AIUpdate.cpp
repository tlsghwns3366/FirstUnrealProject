// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_AIUpdate.h"
#include "CharacterStateComponent.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyCharacter.h"

UBTS_AIUpdate::UBTS_AIUpdate()
{
	NodeName = TEXT("AIUpdate");
	Interval = 0.1f;
}

void UBTS_AIUpdate::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Character = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	float CharacterAggro = Character->MainStateComponent->AggroCount;
	Character->SetState();

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat("AggroCount", CharacterAggro);
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum("CharacterState", (uint8)Character->MyCharacterState);
	/*
	if(CharacterAggro <= 0)
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("SightSenseDetect", false);
		*/
}
