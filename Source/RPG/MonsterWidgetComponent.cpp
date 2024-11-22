// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterWidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterWidgetComponent.h"
#include "Monster.h"

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

void UMonsterWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	if (OwnerMonster)
		OwnerMonster = Cast<AMonster>(GetOwner());

	if (auto* HealthWidget = Cast<UMonsterWidget>(GetUserWidgetObject())) {
		HealthWidget->SetOwnerMonster(OwnerMonster);
	}
}
