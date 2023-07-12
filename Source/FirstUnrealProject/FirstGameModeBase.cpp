// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstGameModeBase.h"
#include "PlayerCharacter.h"

AFirstGameModeBase::AFirstGameModeBase()
{
	ConstructorHelpers::FClassFinder<APlayerCharacter> Player(TEXT("/Script/Engine.Blueprint'/Game/BluePrints/BP_Player.BP_Player_C'"));
	if (Player.Succeeded())
	{
		DefaultPawnClass = Player.Class;
	}
}
