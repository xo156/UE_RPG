// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MonsterWidget.h"
#include "MyCharacter.h"
#include "AIController.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "ItemData.h"
#include "DropItem.h"
#include "BrainComponent.h"

// Sets default values
AMonster::AMonster()
{	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	//���� ������Ʈ ���� �� �ʱ�ȭ
	MonsterWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MonsterWidgetComponent"));
	MonsterWidgetComponent->SetupAttachment(GetMesh(), FName("HealthWidgetSocket")); // ��� ���Ͽ� ���̱�
	MonsterWidgetComponent->SetRelativeLocation(GetActorLocation());
	MonsterWidgetComponent->SetWidgetClass(MonsterWidgetClass); 

	//���� ���� �ݸ��� ����
	MonsterAttackCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackCollisionComponent"));
	MonsterAttackCollisionComponent->SetupAttachment(GetMesh(), FName("AttackCollision"));
	MonsterAttackCollisionComponent->SetCollisionProfileName(TEXT("Enemy"));
	MonsterAttackCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnOverlapBegin);
	
	//����ü
	MonsterStatus.MaxMonsterHP = 100.0f;
	MonsterStatus.CurrentMonsterHP = MonsterStatus.MaxMonsterHP;
	MonsterStatus.Damage = 10.f;
	MonsterStatus.DropSoul = 100;
}

void AMonster::ConsumeHPForAction(float HPCost)
{
	MonsterStatus.UseHP(HPCost);
	OnMonsterUIUpdated.Broadcast(MonsterStatus.CurrentMonsterHP);
}

bool AMonster::bHasEnoughHP(float HPCost) const
{
	return MonsterStatus.CurrentMonsterHP > HPCost;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	if (MonsterWidgetComponent) {
		if (auto* HealthWidget = Cast<UMonsterWidget>(MonsterWidgetComponent->GetUserWidgetObject())) {
			HealthWidget->SetOwnerMonster(this);
		}
	}
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WidgetFaceToPlayer();

}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMonster::WidgetFaceToPlayer()
{
	if (MonsterWidgetComponent) {
		//���� ī�޶� ��ġ�� ��������
		if (auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
			FVector CameraLocation;
			FRotator CameraRotation;
			PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

			// ������ �׻� ī�޶� ���ϵ��� ȸ�� ����
			FVector DirectionToCamera = (CameraLocation - MonsterWidgetComponent->GetComponentLocation()).GetSafeNormal();
			FRotator LookAtRotation = DirectionToCamera.Rotation();
			MonsterWidgetComponent->SetWorldRotation(LookAtRotation);
		}
	}
}

void AMonster::MonsterAttackStart()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AMonster::MonsterAttack()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
		if (MonsterAttackMontage) {
			if (!bIsMonsterAttack) {
				bIsMonsterAttack = true;
				AnimInstance->Montage_Play(MonsterAttackMontage);

				FOnMontageEnded MontageEndedDelegate;
				MontageEndedDelegate.BindUObject(this, &AMonster::OnAttackMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MonsterAttackMontage);
			}
		}
	}
}

void AMonster::MonsterAttackEnd()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bIsMonsterAttack = false;
	OverlapActors.Empty();
}

void AMonster::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == MonsterAttackMontage) {
		MonsterAttackEnd();
	}
}

void AMonster::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor->IsA(AMyCharacter::StaticClass())) {
		if (!OverlapActors.Contains(OtherActor)) {
			OverlapActors.Add(OtherActor);
			ApplyDamageToActor(OtherActor);
		}
	}
}

float AMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (EventInstigator == nullptr)
		return -1.f;
	if (DamageCauser == nullptr)
		return -1.f;

	if (bHasEnoughHP(DamageAmount)) {
		//ü���� ����ؼ� �������� ������
		ConsumeHPForAction(DamageAmount);
		UE_LOG(LogTemp, Log, TEXT("Monster Damaged, CurrentHP: %f"), MonsterStatus.CurrentMonsterHP);
	}
	else {
		//ü���� ��� ������
		UE_LOG(LogTemp, Log, TEXT("Monster Die"));
		ConsumeHPForAction(DamageAmount); //ü���� 0���� �ϱ�

		//������ ���
		if (ItemDropTable) {
			static const FString ContextString(TEXT("Item Drop Context"));
			for (const FDropRate& DropRate : MonsterStatus.DropRates) {
				FDropRate* FoundItem = ItemDropTable->FindRow<FDropRate>(FName(*FString::FromInt(DropRate.ItemID)), ContextString);
				if (FoundItem) {
					float RandomNumber = FMath::FRandRange(0.f, 100.f);
					if (RandomNumber <= FoundItem->Rate) {
						int32 DropAmount = FMath::RandRange(FoundItem->MinAmount, FoundItem->MaxAmount);

						FVector SpawnLocation = GetActorLocation();
						FRotator SpawnRotation = GetActorRotation();

						if (ADropItem* DropItemActor = GetWorld()->SpawnActor<ADropItem>(DropItemClass, SpawnLocation, SpawnRotation)) {
							FDropItemData DropItemData;
							DropItemData.ItemID = DropRate.ItemID;
							DropItemData.Amount = DropAmount;
							DropItemData.bCounterble = DropRate.bCounterble;

							DropItemActor->SetDropItem(DropItemData);
						}
					}
				}
			}
		}
	}
	
	return DamageAmount;
}

void AMonster::ApplyDamageToActor(AActor* ActorToDamage)
{
	if (ActorToDamage) {
		float Damage = MonsterStatus.Damage;
		FDamageEvent DamageEvent;
		ActorToDamage->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
	}
}

UBehaviorTree* AMonster::GetBehaviorTree() const
{
	return BehaviorTree;
}

APatrolPath* AMonster::GetPatrolPath() const
{
	return PatrolPath;	
}

UAnimMontage* AMonster::GetMonsterAttackMontage() const
{
	return MonsterAttackMontage;
}

UCapsuleComponent* AMonster::GetAttackCollision() const
{
	return MonsterAttackCollisionComponent;
}

TArray<AActor*>& AMonster::GetOverlapActors()
{
	return OverlapActors;
}
