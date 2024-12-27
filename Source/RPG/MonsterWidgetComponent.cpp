// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterWidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterWidget.h"
#include "Monster.h"
#include "BossMonster.h"

UMonsterWidgetComponent::UMonsterWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMonsterWidgetComponent::FaceToPlayer()
{
	if (auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		FVector DirectionToCamera = (CameraLocation - GetComponentLocation()).GetSafeNormal();
		FRotator LookAtRotation = DirectionToCamera.Rotation();
		SetWorldRotation(LookAtRotation);
	}
}

void UMonsterWidgetComponent::SetOwnerMonsterWidget()
{
	HealthWidget = Cast<UMonsterWidget>(GetUserWidgetObject());
	if (HealthWidget && OwnerMonster) {
		if (Cast<ABossMonster>(OwnerMonster)) {
			HealthWidget->AddToViewport();
		}
		HealthWidget->SetOwnerMonster(OwnerMonster);
	}
}

UMonsterWidget* UMonsterWidgetComponent::GetHealthWidget()
{
	return HealthWidget ? HealthWidget : nullptr;
}

void UMonsterWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!OwnerMonster) {
		OwnerMonster = Cast<AMonster>(GetOwner());
	}
}
