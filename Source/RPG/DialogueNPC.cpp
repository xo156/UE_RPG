// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueNPC.h"
#include "Components/CapsuleComponent.h"
#include "DialogueComponent.h"

// Sets default values
ADialogueNPC::ADialogueNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.AddUnique(FName("NPC"));

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	DialogueComponent = CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComponent"));
}

// Called when the game starts or when spawned
void ADialogueNPC::BeginPlay()
{
	Super::BeginPlay();

	if (DialogueComponent && DialogueTable) {
		DialogueComponent->SetDialogueCache(DialogueTable);
	}
}

// Called every frame
void ADialogueNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADialogueNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADialogueNPC::ShowDialogues()
{
	if (DialogueComponent) {
		DialogueComponent->CreateDialogueWidget();
	}
}

UDialogueComponent* ADialogueNPC::GetDialogueComponent()
{
	return DialogueComponent ? DialogueComponent : nullptr;
}

