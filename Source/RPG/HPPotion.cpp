
// Fill out your copyright notice in the Description page of Project Settings.


#include "HPPotion.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHPPotion::AHPPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AHPPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHPPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
