// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIPerceptionComponent.generated.h"

// Sets default values
AMonsterBase::AMonsterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; //�̵��ϴ� �������� ĳ���͸� ȸ��
}

// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
	MonsterStartPosition = GetActorLocation(); //���� ���۵Ǹ� ������ ���� ����������
}

void AMonsterBase::Die()
{
	if (MonsterCurrentHealth <= 0.f)
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("MonsterDie"));
}

// Called every frame
void AMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MonsterPosition();

	Die();
}

FVector AMonsterBase::MonsterPosition()
{
	return MonsterCurrentPosition = GetActorLocation();
}

