// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/Damage/DamageTypeBase.h"
#include "ActorComponent/Damage/DamageComponent.h"

bool UDamageTypeBase::SetAttackType(UDamageComponent* DamageComponent, float Damage, AActor* DamageCauser)
{
	if (bUseCritical && DamageComponent->CheckCritical(DamageCauser))
	{
		
		float TempDamage = DamageComponent->OnCriticalDamaged(Damage, DamageCauser);
		DamageComponent->DamageTaken(TempDamage, CriticalDamageColor);
	}
	else
	{
		DamageComponent->OnDamaged(Damage);
		DamageComponent->DamageTaken(Damage, DamageColor);
	}


	if (bUseDamageOverTime)
	{
		DamageComponent->SetDamageOverTime(this, Damage, DamageOverTime);
		TargetDamageComponent = DamageComponent;
		OneTimeDot = Damage / DotDamage;
	}

	return false;
}

void UDamageTypeBase::StartCooldown_Implementation(float CooldownTime)
{
	CurrentDamageOverTime = CooldownTime;
}

void UDamageTypeBase::EndCooldown_Implementation()
{
}

void UDamageTypeBase::OneTimeCooldown_Implementation()
{
	if (TargetDamageComponent)
	{
		TargetDamageComponent->OnDamaged(OneTimeDot);
		TargetDamageComponent->DamageTaken(OneTimeDot, DamageColor);
	}
}
