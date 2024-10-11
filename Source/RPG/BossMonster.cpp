// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonster.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MonsterWidget.h"
#include "DataTableGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyPlayerController.h"

ABossMonster::ABossMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	//bUseControllerRotationPitch = false;
	//bUseControllerRotationYaw = false;
	//bUseControllerRotationRoll = false;

	//GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->MaxWalkSpeed = 500.f;
	//GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
	//GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	
	////위젯 컴포넌트 생성 및 초기화
	BossMonsterWidgetClass = UMonsterWidget::StaticClass();

	////몬스터 공격 콜리전 검출
	//MonsterAttackCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackCollisionComponent"));
	//MonsterAttackCollisionComponent->SetupAttachment(GetMesh(), FName("AttackCollision"));
	//MonsterAttackCollisionComponent->SetCollisionProfileName(TEXT("NoCollision"));
	//MonsterAttackCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnOverlapBegin);

	//구조체
	MonsterStatus.MaxMonsterHP = 300.f;
	MonsterStatus.CurrentMonsterHP = MonsterStatus.MaxMonsterHP;
	MonsterStatus.Damage = 15.f;

}

void ABossMonster::BeginPlay()
{
	Super::BeginPlay();

	if (auto* GameInstance = Cast<UDataTableGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))) {
		BossCameraShake = GameInstance->GetBossCameraShake();
	}
}

void ABossMonster::SetUpBossWidget()
{
	if (BossMonsterWidgetClass) {
		BossMonsterWidgetInstance = CreateWidget<UMonsterWidget>(GetWorld(), BossMonsterWidgetClass);
		if (BossMonsterWidgetInstance) {
			BossMonsterWidgetInstance->AddToViewport();
			BossMonsterWidgetInstance->UpdateHP(MonsterStatus.CurrentMonsterHP, MonsterStatus.MaxMonsterHP);
		}
	}
}

void ABossMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABossMonster::MonsterAttackExecute(UAnimMontage* Montage)
{
	Super::MonsterAttackExecute();

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
		if (Montage) {
			if (!bIsMonsterAttack) {
				bIsMonsterAttack = true;
				AnimInstance->Montage_Play(Montage);

				FOnMontageEnded MontageEndedDelegate;
				MontageEndedDelegate.BindUObject(this, &AMonster::OnAttackMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, Montage);
			}
		}
	}
}

float ABossMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (EventInstigator == nullptr)
		return -1.f;
	if (DamageCauser == nullptr)
		return -1.f;

	if (auto* PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
		PlayerController->ClientPlayCameraShake(BossCameraShake);
	}

	if (bHasEnoughHP(DamageAmount)) {
		//체력이 충분해서 데미지를 입을때
		ConsumeHPForAction(DamageAmount);
		UE_LOG(LogTemp, Log, TEXT("BossMonster Damaged, CurrentHP: %f"), MonsterStatus.CurrentMonsterHP);
	}
	else {
		//체력이 없어서 죽을때
		UE_LOG(LogTemp, Log, TEXT("BossMonster Die"));
		ConsumeHPForAction(DamageAmount); //체력을 0으로 하기
		//몬스터 사라지기
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
			if (BossMonsterDieMontage) {
				AnimInstance->Montage_Play(BossMonsterDieMontage);
				FOnMontageEnded EndDelegate;
				EndDelegate.BindUObject(this, &AMonster::OnDieMontageEnded);
				AnimInstance->Montage_SetEndDelegate(EndDelegate, BossMonsterDieMontage);
			}
		}
	}

	return DamageAmount;
}

void ABossMonster::NoAction()
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), GoalLocation);

	if (WanderCount >= 2) {
		WanderCount = 0;
		bIsWander = false;
	}
}

void ABossMonster::FindPlayerAround()
{
	if (auto* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) {
		PlayerLocation = PlayerPawn->GetActorLocation();
		GoalLocation = PlayerLocation + FMath::VRand() * FMath::FRandRange(200.0f, 400.0f); //랜덤 위치 생성

		for (WanderCount; WanderCount < 2; WanderCount++) {
			bIsWander = true;
			NoAction();
		}
	}
}

UAnimMontage* ABossMonster::GetCloseAttackMontage()
{
	if (BossMonsterCloseAttackMontage) {
		NowPlayMontage = BossMonsterCloseAttackMontage;
		return BossMonsterCloseAttackMontage;
	}
	return nullptr;
}

UAnimMontage* ABossMonster::GetMidAttackMontage()
{
	if (BossMonsterMidAttackMontage) {
		NowPlayMontage = BossMonsterMidAttackMontage;
		return BossMonsterMidAttackMontage;
	}
	return nullptr;
}

UAnimMontage* ABossMonster::GetLongAttackMontage()
{
	if (BossMonsterLongAttackMontage) {
		NowPlayMontage = BossMonsterLongAttackMontage;
		return BossMonsterLongAttackMontage;
	}
	return nullptr;
}

UAnimMontage* ABossMonster::GetNowAttackMontage()
{
	if (NowPlayMontage)
		return NowPlayMontage;
	return nullptr;
}
