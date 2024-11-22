// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonster.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MonsterWidget.h"
#include "Components/WidgetComponent.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"

ABossMonster::ABossMonster() : AMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(60.f, 130.f);

	GetCharacterMovement()->MaxWalkSpeed = 700.f;

	//추가 몬스터 공격 콜리전 검출
	MonsterAttackCollisionComponent1 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackCollisionComponent1"));
	MonsterAttackCollisionComponent1->SetupAttachment(GetMesh(), FName("AttackCollision_RightHand"));
	MonsterAttackCollisionComponent1->SetCollisionProfileName(TEXT("NoCollision"));
	MonsterAttackCollisionComponent1->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnOverlapBegin);
	//SetMonsterAttackCollision(MonsterAttackCollisionComponent1);

	MonsterAttackCollisionComponent2 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackCollisionComponent2"));
	MonsterAttackCollisionComponent2->SetupAttachment(GetMesh(), FName("AttackCollision_LeftLeg"));
	MonsterAttackCollisionComponent2->SetCollisionProfileName(TEXT("NoCollision"));
	MonsterAttackCollisionComponent2->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnOverlapBegin);
	//SetMonsterAttackCollision(MonsterAttackCollisionComponent2);

	MonsterAttackCollisionComponent3 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackCollisionComponent3"));
	MonsterAttackCollisionComponent3->SetupAttachment(GetMesh(), FName("AttackCollision_RightLeg"));
	MonsterAttackCollisionComponent3->SetCollisionProfileName(TEXT("NoCollision"));
	MonsterAttackCollisionComponent3->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnOverlapBegin);
	//SetMonsterAttackCollision(MonsterAttackCollisionComponent3);

	MonsterAttackCollisionComponent4 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackCollisionComponent4"));
	MonsterAttackCollisionComponent4->SetupAttachment(GetMesh(), FName("AttackCollision_Jump"));
	MonsterAttackCollisionComponent4->SetCollisionProfileName(TEXT("NoCollision"));
	MonsterAttackCollisionComponent4->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnOverlapBegin);
	//SetMonsterAttackCollision(MonsterAttackCollisionComponent4);

	//구조체
	MaxMonsterHP = MonsterData.MaxMonsterHP;
	CurrentMonsterHP = MaxMonsterHP;
	MonsterDamage = MonsterData.Damage;
}

void ABossMonster::BeginPlay()
{
	Super::BeginPlay();

}

void ABossMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//항상 플레이어 바라보기
	if (!bIsMonsterDead) {
		if (auto* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController())) {
			if (auto* PlayerCharacter = Cast<AMyCharacter>(PlayerController->GetPawn())) {
				FVector PlayerLocation = PlayerCharacter->GetActorLocation();
				FRotator LookAtRotation = (PlayerLocation - GetActorLocation()).Rotation();
				SetActorRotation(FRotator(0.0f, LookAtRotation.Yaw, 0.0f));
			}
		}
	}
	else {
		GetMonsterWidgetComponent()->SetVisibility(false);
	}
}

void ABossMonster::MonsterAttackExecute(int32 PatternNumber)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
		if (!bIsMonsterAttack) {
			bIsMonsterAttack = true;

			FOnMontageEnded MontageEndedDelegate;
			MontageEndedDelegate.BindUObject(this, &AMonster::OnAttackMontageEnded);

			switch (PatternNumber) {
			case 1:
				AnimInstance->Montage_Play(CloseAttackMontage);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, CloseAttackMontage);
				break;
			case 2:
				AnimInstance->Montage_Play(MidAttackMontage);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MidAttackMontage);
				break;
			case 3:
				AnimInstance->Montage_Play(LongAttackMontage);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, LongAttackMontage);
				break;
			default:
				break;
			}
		}
	}
}

float ABossMonster::GetCloseRange()
{
	return CloseRange;
}

float ABossMonster::GetMidRange()
{
	return MidRange;
}

UAnimMontage* ABossMonster::GetCloseAttackMontage()
{
	return CloseAttackMontage ? CloseAttackMontage : nullptr;
}

UAnimMontage* ABossMonster::GetMidAttackMontage()
{
	return MidAttackMontage ? MidAttackMontage : nullptr;
}

UAnimMontage* ABossMonster::GetLongAttackMontage()
{
	return LongAttackMontage ? LongAttackMontage : nullptr;
}

float ABossMonster::GetWaitForNextActionTime()
{
	return WaitForNextActionTime;
}
