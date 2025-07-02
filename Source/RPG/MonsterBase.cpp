// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacter.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "DropItem.h"
#include "DataTableGameInstance.h"
#include "MyPlayerController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Components/BoxComponent.h"
#include "Weapon.h"
#include "MyGameModeBase.h"
#include "MonsterData.h"
#include "DropRate.h"
#include "HPActorComponent.h"
#include "MonsterStateMachineComponent.h"
#include "MonsterAttackPatternDataAsset.h"

// Sets default values
AMonsterBase::AMonsterBase()
{	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	//자원
	HPActorComponent = CreateDefaultSubobject<UHPActorComponent>(TEXT("Resource"));

	//상태
	MonsterStateMachineComponent = CreateDefaultSubobject<UMonsterStateMachineComponent>(TEXT("StateMachine"));

}

// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	//TODO: 체력바 바인딩할것

}

// Called every frame
void AMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMonsterBase::InitMonsterInfo(const FMonsterData& MonsterData)
{
	if (!HPActorComponent)
		return;

	HPActorComponent->InitHP(MonsterData.MaxMonsterHP);

	MonsterID = MonsterData.MonsterID;
	MonsterName = MonsterData.MonsterName;
	MonsterDamage = MonsterData.Damage;
	MonsterDropItemIDs = MonsterData.DropItemIDS;
	MonsterAttackPatterns = MonsterData.MonsterAttackPatterns;

	UE_LOG(LogTemp, Log, TEXT("Monster [%s] Initialized | HP: %f | Damage: %f"), *MonsterName.ToString(), MonsterData.MaxMonsterHP, MonsterData.Damage);

	CacheAttackBodies();
}

void AMonsterBase::CacheAttackBodies()
{
	AttackBodies.Empty();
	TArray<FName> BodyCast;
	GetMesh()->GetBoneNames(BodyCast);
	for (FName Bone : BodyCast) {
		if (Bone.ToString().EndsWith(TEXT("_Attack"))) {
			FBodyInstance* BodyInstance = GetMesh()->GetBodyInstance(Bone);
			if (BodyInstance && BodyInstance->OwnerComponent.IsValid()) {
				BodyInstance->SetInstanceSimulatePhysics(false); //혹시 모를 물리 연산 차단
				BodyInstance->SetResponseToAllChannels(ECR_Ignore);
				BodyInstance->SetResponseToChannel(ECC_Pawn, ECR_Overlap); //명시적으로 필요한 채널만
				AttackBodies.Add(Bone, BodyInstance->OwnerComponent.Get());
			}
		}
	}
}

void AMonsterBase::MonsterAttackStart(int32 AttackIndex)
{
	if (!bIsValidAttackPatternIndex(AttackIndex))
		return;

	if (!MonsterStateMachineComponent)
		return;

	MonsterStateMachineComponent->ChangeState(EMonsterState::Attack);
	MonsterAttackExecute(AttackIndex);
}

void AMonsterBase::MonsterAttackExecute(int32 AttackIndex)
{
	if (!GetMesh() || !GetMesh()->GetAnimInstance())
		return;

	auto* AnimInstance = GetMesh()->GetAnimInstance();
	auto* Pattern = MonsterAttackPatterns[AttackIndex];

	if (Pattern && Pattern->AttackMontage) {
		OverlapActors.Empty(); //데미지를 입을 대상 초기화
		AnimInstance->Montage_Play(Pattern->AttackMontage);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AMonsterBase::OnAttackMontageEnded);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, Pattern->AttackMontage);	
	}
}

void AMonsterBase::UpdateValidPatternIndexes(float Distance, float HPRatio)
{
	ValidAttackPatternIndexes.Empty();

	for (int32 i = 0; i < MonsterAttackPatterns.Num(); i++) {
		if (MonsterAttackPatterns[i] && MonsterAttackPatterns[i]->bIsValidCondition(Distance, HPRatio)) {
			ValidAttackPatternIndexes.Add(i);
		}
	}
}

bool AMonsterBase::bIsValidAttackPatternIndex(int32 Index) const
{
	return MonsterAttackPatterns.IsValidIndex(Index) && MonsterAttackPatterns[Index] != nullptr;
}

void AMonsterBase::EnableAttackBody(FName TargetBodyName, bool bEnable)
{
	if (UPrimitiveComponent** CompPtr = AttackBodies.Find(TargetBodyName)) {
		(*CompPtr)->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
}

void AMonsterBase::OnAttackMontageEnded(UAnimMontage* NowPlayMontage, bool bInterrupted)
{
	MonsterAttackEnd();
}

void AMonsterBase::MonsterAttackEnd()
{
	if (!MonsterStateMachineComponent)
		return;

	MonsterStateMachineComponent->ChangeState(EMonsterState::Idle);
}

void AMonsterBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!MonsterStateMachineComponent)
		return;

	if (MonsterStateMachineComponent->IsInAnyState({EMonsterState::Attack}))
		return;

	if (OtherActor && OtherActor != this && OtherActor->IsA(AMyCharacter::StaticClass())) {
		if (!OverlapActors.Contains(OtherActor)) {
			OverlapActors.Add(OtherActor);
			ApplyDamageToActor(OtherActor, OtherComponent);
		}
	}
}

float AMonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!EventInstigator)
		return -1.f;
	if (!DamageCauser)
		return -1.f;
	if (!HPActorComponent)
		return -1.f;
	if (!MonsterStateMachineComponent)
		return -1.f;

	if (auto* PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
		PlayerController->ClientStartCameraShake(CameraShake);
	}

	if (HPActorComponent->bCanConsumeHP(DamageAmount)) { //생존
		HPActorComponent->ConsumeHP(DamageAmount);
	}
	else { //사망
		HPActorComponent->ConsumeHP(DamageAmount);
		DroppedItem();
		DieMonster();
	}
	return DamageAmount;
}

void AMonsterBase::DroppedItem()
{
	for (const int32& DropItemID : MonsterDropItemIDs) {
		FDropRate* FoundItem = ItemCache.FindRef(DropItemID);
		if (FoundItem) {
			float RandomNumber = FMath::FRandRange(0.f, 100.f);
			if (RandomNumber <= FoundItem->Rate) {
				int32 DropAmount = FMath::RandRange(FoundItem->MinAmount, FoundItem->MaxAmount);
				FVector SpawnLocation = GetActorLocation();
				FRotator SpawnRotation = GetActorRotation();
				if (auto* DropItemActor = GetWorld()->SpawnActor<ADropItem>(DropItemClass, SpawnLocation, SpawnRotation)) {
					FDropItemData DropItemData;
					DropItemData.ItemID = FoundItem->ItemID;
					DropItemData.Amount = DropAmount;
					DropItemData.bCounterble = FoundItem->bCounterble;
					DropItemActor->SetDropItem(DropItemData);
				}
			}
		}
	}
}

void AMonsterBase::DieMonster()
{
	if (!MonsterStateMachineComponent)
		return;

	MonsterStateMachineComponent->ChangeState(EMonsterState::Dead);

	if (auto* AIController = Cast<AAIController>(GetController())) {
		AIController->StopMovement();
		AIController->UnPossess();
	}

	if (auto* AnimInstance = GetMesh()->GetAnimInstance()) {
		if (MonsterDieMontage) {
			AnimInstance->Montage_Play(MonsterDieMontage);
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &AMonsterBase::OnDieMontageEnded);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, MonsterDieMontage);
		}
		else {
			Destroy();
		}
	}
	else {
		Destroy();
	}
}

void AMonsterBase::OnDieMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	Destroy();
}

void AMonsterBase::ApplyDamageToActor(AActor* ActorToDamage, UPrimitiveComponent* OtherComponent)
{
	if (!ActorToDamage)
		return;

	auto* PlayerCharacter = Cast<AMyCharacter>(ActorToDamage);
	if (PlayerCharacter) {
		float FinalDamage = MonsterDamage;
		//TODO: 피격시 데미지를 증감하게 하는 로직 추가해볼것(방어력 계산, 가드 성공, 패링 실패등)
		//TODO: 스테미나 소모나 카메라 흔들림 추가해볼것
		FDamageEvent DamageEvent;
		ActorToDamage->TakeDamage(MonsterDamage, DamageEvent, GetInstigatorController(), this);
	}
}

UBehaviorTree* AMonsterBase::GetBehaviorTree() const
{
	return BehaviorTree;
}

APatrolPath* AMonsterBase::GetPatrolPath() const
{
	return PatrolPath;	
}

TArray<UMonsterAttackPatternDataAsset*>& AMonsterBase::GetAttackPatterns()
{
	return MonsterAttackPatterns;
}

TArray<int32> AMonsterBase::GetValidPatternIndexes() const
{
	return ValidAttackPatternIndexes;
}

TArray<AActor*>& AMonsterBase::GetOverlapActors()
{
	return OverlapActors;
}

UHPActorComponent* AMonsterBase::GetHPActorComponent() const
{
	return HPActorComponent ? HPActorComponent : nullptr;
}

UMonsterStateMachineComponent* AMonsterBase::GetMonsterStateMachineComponent() const
{
	return MonsterStateMachineComponent ? MonsterStateMachineComponent : nullptr;
}
