// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonster.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MonsterWidget.h"
#include "DataTableGameInstance.h"
#include "Kismet/GameplayStatics.h"

ABossMonster::ABossMonster()
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

	/*
	//위젯 컴포넌트 생성 및 초기화
	MonsterWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MonsterWidgetComponent"));
	MonsterWidgetComponent->SetupAttachment(GetMesh(), FName("HealthWidgetSocket")); // 헤드 소켓에 붙이기
	MonsterWidgetComponent->SetRelativeLocation(GetActorLocation());
	MonsterWidgetComponent->SetWidgetClass(MonsterWidgetClass);

	//몬스터 공격 콜리전 검출
	MonsterAttackCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackCollisionComponent"));
	MonsterAttackCollisionComponent->SetupAttachment(GetMesh(), FName("AttackCollision"));
	MonsterAttackCollisionComponent->SetCollisionProfileName(TEXT("NoCollision"));
	MonsterAttackCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnOverlapBegin);
	*/

	//구조체
	MonsterStatus.MaxMonsterHP = 300.f;
	MonsterStatus.CurrentMonsterHP = MonsterStatus.MaxMonsterHP;
	MonsterStatus.Damage = 15.f;
}

void ABossMonster::BeginPlay()
{
	Super::BeginPlay();

	/*
	if (MonsterWidgetComponent) {
		if (auto* HealthWidget = Cast<UMonsterWidget>(MonsterWidgetComponent->GetUserWidgetObject())) {
			HealthWidget->SetOwnerMonster(this);
		}
	}

	if (auto* GameInstance = Cast<UDataTableGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))) {
		CameraShake = GameInstance->GetCameraShake();
	}
	*/
}

void ABossMonster::Tick(float DeltaTime)
{
	WidgetFaceToPlayer();
}

void ABossMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
