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

	//���� ������Ʈ ���� �� �ʱ�ȭ
	//MonsterWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MonsterWidgetComponent"));
	//MonsterWidgetComponent->SetupAttachment(GetMesh(), FName("HealthWidgetSocket")); // ��� ���Ͽ� ���̱�
	//MonsterWidgetComponent->SetRelativeLocation(GetActorLocation());
	//MonsterWidgetComponent->SetWidgetClass(MonsterWidgetClass);

	//����ü
	MonsterStatus.MaxMonsterHP = 200.f;
	MonsterStatus.CurrentMonsterHP = MonsterStatus.MaxMonsterHP;
	MonsterStatus.Damage = 20.f;
}

void ABossMonster::BeginPlay()
{
	Super::BeginPlay();
	CheckMonsterAttackCollisionComponents();
}

void ABossMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ü���� �� ���Ϸ� ��������
	/*if (MonsterStatus.CurrentMonsterHP <= MonsterStatus.MaxMonsterHP / 2) {
		WaitForNextActionTime = 1.5;
	}*/

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

void ABossMonster::WidgetFaceToPlayer()
{
	Super::WidgetFaceToPlayer();

	//if (GetMonsterWidgetComponent()) {
	//	if (auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
	//		FVector2D ScreenPosition;
	//		//FVector WorldLocation = GetActorLocation();  // ���� ������ ���� ��ġ

	//		// ���� ��ġ�� ȭ�� �������� ��ȯ
	//		if (PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenPosition)) {
	//			// ȭ�� �ϴ��� �߾ӿ� ��ġ�ϵ��� ����
	//			int32 ScreenSizeX, ScreenSizeY;
	//			PlayerController->GetViewportSize(ScreenSizeX, ScreenSizeY);

	//			// ������ ��ũ�� ��ġ�� �ϴ� �߾����� ���߱� ���� ���
	//			float WidgetPositionY = ScreenSizeY - MonsterWidgetComponent->GetDesiredSize().Y - 50.f;  // �ణ�� ������
	//			float WidgetPositionX = ScreenSizeX * 0.5f - MonsterWidgetComponent->GetDesiredSize().X * 0.5f;

	//			// ������ ��ũ�� ��ǥ ���� (�ϴ� �߾�)
	//			FVector2D WidgetScreenPosition(WidgetPositionX, WidgetPositionY);

	//			// ������ ���ο� ��ġ�� ����
	//			MonsterWidgetComponent->SetDrawAtDesiredSize(true);
	//			MonsterWidgetComponent->SetWorldLocation(WorldLocation);  // ���� ���� ��ġ
	//			MonsterWidgetComponent->SetScreenSpaceWidgetPosition(WidgetScreenPosition);  // ��ũ�� ��ǥ
	//		}
	//	}
	//}
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
	if (CloseAttackMontage)
		return CloseAttackMontage;
	return nullptr;
}

UAnimMontage* ABossMonster::GetMidAttackMontage()
{
	if (MidAttackMontage)
		return MidAttackMontage;
	return nullptr;
}

UAnimMontage* ABossMonster::GetLongAttackMontage()
{
	if (LongAttackMontage)
		return LongAttackMontage;
	return nullptr;
}
