// Fill out your copyright notice in the Description page of Project Settings.


#include "LockonWidget.h"
#include "Components/Image.h"
#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"

void ULockonWidget::SetTarget(AActor* NewTarget)
{
	if (TargetActor != NewTarget)
		TargetActor = NewTarget;

	SetVisibility(NewTarget ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void ULockonWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!TargetActor.IsValid()) {
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	auto* PlayerController = Cast<AMyPlayerController>(GetOwningPlayer());
	if (!PlayerController) 
		return;

	FVector2D ScreenPosition;
	if (UGameplayStatics::ProjectWorldToScreen(PlayerController, TargetActor->GetActorLocation(), ScreenPosition)) {
		SetPositionInViewport(ScreenPosition);
	}
}
