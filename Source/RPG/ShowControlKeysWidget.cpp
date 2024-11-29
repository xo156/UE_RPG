// Fill out your copyright notice in the Description page of Project Settings.


#include "ShowControlKeysWidget.h"

void UShowControlKeysWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CollisionEnabledHasPhysics(ECollisionEnabled::NoCollision);
}
