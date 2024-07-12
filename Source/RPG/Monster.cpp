// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidget.h"
#include "MyCharacter.h"

// Sets default values
AMonster::AMonster()
{	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	//몬스터 체력 위젯
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthValue"));
	if (WidgetComponent) {
		WidgetComponent->SetupAttachment(RootComponent);
		WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		WidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 95.f));
		static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Content/Game/UI/WBP_MonsterHealthBar"));
		if (WidgetClass.Succeeded()) {
			WidgetComponent->SetWidgetClass(WidgetClass.Class);
		}
	}

	//구조체
	MonsterStatus.MaxMonsterHP = 100.0f;
	MonsterStatus.CurrentMonsterHP = MonsterStatus.MaxMonsterHP;
	MonsterStatus.AttackDamage = 10.f;
	MonsterStatus.DropMoney = 0.f;
}

void AMonster::ConsumeHPForAction(float HPCost)
{
	MonsterStatus.UseHP(HPCost);
	OnUIUpdated.Broadcast(MonsterStatus.CurrentMonsterHP);
}

bool AMonster::bHasEnoughHP(float HPCost) const
{
	return MonsterStatus.CurrentMonsterHP >= HPCost;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	if (UHealthBarWidget* HealthWidget = Cast<UHealthBarWidget>(WidgetComponent->GetUserWidgetObject())) {
		HealthWidget->UpdateHP(MonsterStatus.CurrentMonsterHP, MonsterStatus.MaxMonsterHP);
	}
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UHealthBarWidget* HealthWidget = Cast<UHealthBarWidget>(WidgetComponent->GetUserWidgetObject())) {
		HealthWidget->UpdateHP(MonsterStatus.CurrentMonsterHP, MonsterStatus.MaxMonsterHP);
	}
}

void AMonster::MonsterAttack()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
		if (MonsterAttackMontage) {
			AnimInstance->Montage_Play(MonsterAttackMontage);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("MonsterAttackMontage"));
		}
	}
}

UBehaviorTree* AMonster::GetBehaviorTree() const
{
	return BehaviorTree;
}

APatrolPath* AMonster::GetPatrolPath() const
{
	return PatrolPath;	
}

UAnimMontage* AMonster::GetMonsterAttackMontage()
{
	return MonsterAttackMontage;
}

float AMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (EventInstigator == nullptr) {
		return -1.f;
	}
	if (DamageCauser == nullptr) {
		return -1.f;
	}

	if (!MonsterStatus.UseHP(DamageAmount)) {
		//액터 사망 처리
		OnDie(Cast<AMyCharacter>(DamageCauser));
	}
	else {
		//데미지를 입었을 때
		OnHit();
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

}

void AMonster::OnHit()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
		if (MonsterHitMontage) {
			AnimInstance->Montage_Play(MonsterHitMontage);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("MonsterHitMontage"));
		}
	}
}

void AMonster::OnDie(AMyCharacter* LastAttacker)
{
	GetController()->StopMovement();

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
		if (MonsterDieMontage) {
			AnimInstance->Montage_Play(MonsterDieMontage);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("MonsterDieMontage"));
		}
	}

	if (LastAttacker != nullptr) {
		this->OnEventDieEvent.AddDynamic(LastAttacker, &AMyCharacter::OnEnemyDie_Money);
	}
	OnEventDieEvent.Broadcast(MonsterStatus.DropMoney);
}
