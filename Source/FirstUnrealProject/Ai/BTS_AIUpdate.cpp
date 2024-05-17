// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_AIUpdate.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Character/EnemyCharacter.h"
#include "ActorComponent/AggroComponent.h"
#include "Controller/EnemyAIController.h"
#include "ActorComponent/CharacterStateComponent.h"
#include "Item/Weapon.h"

UBTS_AIUpdate::UBTS_AIUpdate()
{
	NodeName = TEXT("AIUpdate");
	Interval = 0.1f;
}

void UBTS_AIUpdate::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Character = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	float CharacterAggro = Character->AggroComponent->AggroCount;
	Character->SetState();

	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	MyBlackboard->SetValueAsFloat("AggroCount", CharacterAggro);
	MyBlackboard->SetValueAsEnum("CharacterState", (uint8)Character->MyCharacterState);
	if (Character->MainStateComponent->AttachedWeapon)
	{
		MyBlackboard->SetValueAsEnum("WeaponEnum", (uint8)Character->MainStateComponent->AttachedWeapon->WeaponEnum);
	}
	if (Character->MyCharacterState == ECustomCharacterState::E_Attack)
	{
		auto Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject("SightTarget");
		if (Target != nullptr)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject("AttackTarget", Target);
		}
	}
	else if (Character->MyCharacterState == ECustomCharacterState::E_Peace)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject("AttackTarget", nullptr);
	}
}
