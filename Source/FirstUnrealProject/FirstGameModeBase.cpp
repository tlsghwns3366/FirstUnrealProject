// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstGameModeBase.h"
#include "PlayerCharacter.h"
#include "MainGameState.h"

AFirstGameModeBase::AFirstGameModeBase()
{
	ConstructorHelpers::FClassFinder<APlayerCharacter> Player(TEXT("/Script/Engine.Blueprint'/Game/BluePrints/BP_Player.BP_Player_C'"));
	ConstructorHelpers::FClassFinder< AMainGameState> GS(TEXT("/Script/Engine.Blueprint'/Game/BluePrints/BP_MainGameState.BP_MainGameState_C'"));
	if (Player.Succeeded())
	{
		DefaultPawnClass = Player.Class;
		GameStateClass = GS.Class;
	}
}
