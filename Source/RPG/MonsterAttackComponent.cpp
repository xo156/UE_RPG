// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAttackComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacter.h"
#include "Engine/DamageEvents.h"
#include "Components/BoxComponent.h"

// Sets default values for this component's properties
UMonsterAttackComponent::UMonsterAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UMonsterAttackComponent::MonsterStartAttack()
{
	if (!bIsAttack ) {
		bIsAttack = true;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void UMonsterAttackComponent::MonsterExecuteAttack()
{
	if (auto* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance()) {
		if (AttackMontage && !bIsAttack) {
			bIsAttack = true;
			AnimInstance->Montage_Play(AttackMontage);

			FOnMontageEnded MontageEndedDelegate;
			MontageEndedDelegate.BindUObject(this, &UMonsterAttackComponent::OnAttackMontageEnded);
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackMontage);
		}
	}
}

void UMonsterAttackComponent::MonsterEndAttack()
{
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	bIsAttack = false;
}

void UMonsterAttackComponent::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	MonsterEndAttack();
}

void UMonsterAttackComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != OwnerCharacter && OtherActor->IsA(AMyCharacter::StaticClass())) {
		if (!OverlapActors.Contains(OtherActor)) {
			OverlapActors.Add(OtherActor);
			ApplyDamageToActor(OtherActor, OtherComponent);
		}
	}
}

void UMonsterAttackComponent::ApplyDamageToActor(AActor* ActorToDamage, UPrimitiveComponent* OtherComponent)
{
	if (ActorToDamage) {
		if (auto* PlayerCharacter = Cast<AMyCharacter>(ActorToDamage)) {
			if (PlayerCharacter->bIsGuard) { //방어 중이면
				if (OtherComponent == PlayerCharacter->GetGuardComponent()) {
					return; // 피해 없음
				}
			}
			FDamageEvent DamageEvent;
			ActorToDamage->TakeDamage(Damage, DamageEvent, OwnerCharacter ? OwnerCharacter->GetInstigatorController() : nullptr, OwnerCharacter);
		}
	}
}


// Called when the game starts
void UMonsterAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (OwnerCharacter)
		OwnerCharacter = Cast<ACharacter>(GetOwner());
}


// Called every frame
void UMonsterAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

