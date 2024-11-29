// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Components/CapsuleComponent.h"
#include "DialogueComponent.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.AddUnique(FName("NPC"));

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	DialogueComponent = CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComponent"));
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPC::SetupWidget()
{
	if (DialogueComponent) {
		DialogueComponent->SetOwnerCharacter(this);
		DialogueComponent->CreateDialogueWidget(DialogueTable);
	}
}

void ANPC::ShowDialogues()
{
	SetupWidget();
}

UDialogueComponent* ANPC::GetDialogueComponent()
{
	return DialogueComponent ? DialogueComponent : nullptr;
}

