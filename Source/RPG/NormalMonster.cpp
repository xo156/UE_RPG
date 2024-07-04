// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalMonster.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidget.h"

// Sets default values
ANormalMonster::ANormalMonster()
{	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	NormalMonsterHealth = MaxNormalMonsterHealth;

	//몬스터 체력 용도
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthValue"));
	if (WidgetComponent) {
		WidgetComponent->SetupAttachment(RootComponent);
		WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		WidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 95.f));
		static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/Game/UI/WBP_HealthBar"));
		if (WidgetClass.Succeeded()) {
			WidgetComponent->SetWidgetClass(WidgetClass.Class);
		}
	}

	//구조체
	NormalMonsterStatus.CurrentNormalMonsterHP = 100.0f;
	NormalMonsterStatus.MaxNormalMonsterHP = 100.0f;
}

// Called when the game starts or when spawned
void ANormalMonster::BeginPlay()
{
	Super::BeginPlay();
	
	if (UHealthBarWidget* HealthWidget = Cast<UHealthBarWidget>(WidgetComponent->GetUserWidgetObject())) {
		HealthWidget->SetBarValuePercent(NormalMonsterHealth / MaxNormalMonsterHealth);
	}
}

// Called every frame
void ANormalMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANormalMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UHealthBarWidget* HealthWidget = Cast<UHealthBarWidget>(WidgetComponent->GetUserWidgetObject())) {
		HealthWidget->SetBarValuePercent(NormalMonsterHealth / MaxNormalMonsterHealth);
	}
}

void ANormalMonster::MonsterAttack()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
		if (NormalMonsterAttackMontage) {
			//AnimInstance->Montage_Play(NormalMonsterAttackMontage);
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("NormalMonsterAttackMontage"));
		}
	}
}

UBehaviorTree* ANormalMonster::GetBehaviorTree() const
{
	return BehaviorTree;
}

APatrolPath* ANormalMonster::GetPatrolPath() const
{
	return PatrolPath;	
}

float ANormalMonster::GetPatrolSpeed() const
{
	return PatrolSpeed;
}

float ANormalMonster::GetChaseSpeed() const
{
	return ChaseSpeed;
}

float ANormalMonster::GetHealth() const
{
	return NormalMonsterHealth;
}

float ANormalMonster::GetMaxHealth() const
{
	return MaxNormalMonsterHealth;
}

void ANormalMonster::SetHealth(float NewHealth)
{
	NormalMonsterHealth = NewHealth;
}

UAnimMontage* ANormalMonster::GetNormalMonsterAttackMontage()
{
	return NormalMonsterAttackMontage;
}

