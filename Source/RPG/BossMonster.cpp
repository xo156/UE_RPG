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

	//위젯 컴포넌트 생성 및 초기화
	//MonsterWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MonsterWidgetComponent"));
	//MonsterWidgetComponent->SetupAttachment(GetMesh(), FName("HealthWidgetSocket")); // 헤드 소켓에 붙이기
	//MonsterWidgetComponent->SetRelativeLocation(GetActorLocation());
	//MonsterWidgetComponent->SetWidgetClass(MonsterWidgetClass);

	//구조체
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

	//체력이 반 이하로 내려가면
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
	//		//FVector WorldLocation = GetActorLocation();  // 보스 몬스터의 월드 위치

	//		// 월드 위치를 화면 공간으로 변환
	//		if (PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenPosition)) {
	//			// 화면 하단의 중앙에 위치하도록 조정
	//			int32 ScreenSizeX, ScreenSizeY;
	//			PlayerController->GetViewportSize(ScreenSizeX, ScreenSizeY);

	//			// 위젯의 스크린 위치를 하단 중앙으로 맞추기 위한 계산
	//			float WidgetPositionY = ScreenSizeY - MonsterWidgetComponent->GetDesiredSize().Y - 50.f;  // 약간의 오프셋
	//			float WidgetPositionX = ScreenSizeX * 0.5f - MonsterWidgetComponent->GetDesiredSize().X * 0.5f;

	//			// 위젯의 스크린 좌표 설정 (하단 중앙)
	//			FVector2D WidgetScreenPosition(WidgetPositionX, WidgetPositionY);

	//			// 위젯의 새로운 위치로 적용
	//			MonsterWidgetComponent->SetDrawAtDesiredSize(true);
	//			MonsterWidgetComponent->SetWorldLocation(WorldLocation);  // 기존 월드 위치
	//			MonsterWidgetComponent->SetScreenSpaceWidgetPosition(WidgetScreenPosition);  // 스크린 좌표
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
