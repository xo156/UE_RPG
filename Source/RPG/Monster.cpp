// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "MonsterWidget.h"
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
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	if (WidgetComponent) {
		WidgetComponent->SetupAttachment(RootComponent);
		WidgetComponent->SetWidgetClass(UMonsterWidget::StaticClass());
		WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		WidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
		WidgetComponent->SetDrawSize(FVector2D(200.f, 50.f));
		WidgetComponent->SetVisibility(true);
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
	OnMonsterUIUpdated.Broadcast(MonsterStatus.CurrentMonsterHP);
	UE_LOG(LogTemp, Log, TEXT("Monster HP Updated: %f"), MonsterStatus.CurrentMonsterHP);
}

bool AMonster::bHasEnoughHP(float HPCost) const
{
	return MonsterStatus.CurrentMonsterHP >= HPCost;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetComponent) {
		if (auto* HealthWidget = Cast<UMonsterWidget>(WidgetComponent->GetUserWidgetObject())) {
			HealthWidget->UpdateHP(MonsterStatus.CurrentMonsterHP, MonsterStatus.MaxMonsterHP);
			UE_LOG(LogTemp, Log, TEXT("BeginPlay: HealthWidget Initialized with HP: %f / %f"), MonsterStatus.CurrentMonsterHP, MonsterStatus.MaxMonsterHP);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("BeginPlay: HealthWidget is nullptr"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("BeginPlay: WidgetComponent is nullptr"));
	}
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Log, TEXT("Current Monster HP: %f"), MonsterStatus.CurrentMonsterHP);
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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

float AMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (EventInstigator == nullptr)
		return -1.f;
	if (DamageCauser == nullptr)
		return -1.f;

	if (bHasEnoughHP(DamageAmount)) {
		ConsumeHPForAction(DamageAmount);
		UE_LOG(LogTemp, Log, TEXT("Monster Damaged, CurrentHP: %f"), MonsterStatus.CurrentMonsterHP);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Monster Die"));
	}

	return DamageAmount;
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