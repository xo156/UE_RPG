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
#include "DataTableGameInstance.h"
#include "MyPlayerController.h"
#include "MyGameModeBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Components/BoxComponent.h"
#include "Weapon.h"

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

	//위젯 컴포넌트 생성 및 초기화
	MonsterWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MonsterWidgetComponent"));
	MonsterWidgetComponent->SetupAttachment(GetMesh(), FName("HealthWidgetSocket")); // 헤드 소켓에 붙이기
	MonsterWidgetComponent->SetRelativeLocation(GetActorLocation());
	MonsterWidgetComponent->SetWidgetClass(MonsterWidgetClass); 

	//몬스터 공격 콜리전 검출
	MonsterAttackCollisionComponent0 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackCollisionComponent0"));
	MonsterAttackCollisionComponent0->SetupAttachment(GetMesh(), FName("AttackCollision_LeftHand"));
	MonsterAttackCollisionComponent0->SetCollisionProfileName(TEXT("NoCollision"));
	MonsterAttackCollisionComponent0->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnOverlapBegin);
	SetMonsterAttackCollision(MonsterAttackCollisionComponent0);

	//구조체
	MonsterStatus.MaxMonsterHP = 100.f;
	MonsterStatus.CurrentMonsterHP = MonsterStatus.MaxMonsterHP;
	MonsterStatus.Damage = 10.f;
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

	if (auto* GameInstance = Cast<UDataTableGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))) {
		ItemDropTable = GameInstance->GetDropItemTable();
		CameraShake = GameInstance->GetCameraShake();
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
		//현재 카메라 위치를 가져오기
		if (auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
			FVector CameraLocation;
			FRotator CameraRotation;
			PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

			// 위젯이 항상 카메라를 향하도록 회전 설정
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

void AMonster::MonsterAttackExecute()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
		if (MonsterAttackMontage && !bIsMonsterAttack) {
			bIsMonsterAttack = true;
			AnimInstance->Montage_Play(MonsterAttackMontage);

			FOnMontageEnded MontageEndedDelegate;
			MontageEndedDelegate.BindUObject(this, &AMonster::OnAttackMontageEnded);
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MonsterAttackMontage);
		}
	}
}

void AMonster::MonsterAttackEnd()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bIsMonsterAttack = false;
}

void AMonster::OnAttackMontageEnded(UAnimMontage* NowPlayMontage, bool bInterrupted)
{
	if (NowPlayMontage == MonsterAttackMontage) {
		MonsterAttackEnd();
	}
}

void AMonster::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor->IsA(AMyCharacter::StaticClass())) {
		if (!OverlapActors.Contains(OtherActor)) {
			OverlapActors.Add(OtherActor);
			ApplyDamageToActor(OtherActor, OtherComponent);
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

	if (auto* PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
		PlayerController->ClientPlayCameraShake(CameraShake);
	}

	if (bHasEnoughHP(DamageAmount)) {
		//체력이 충분해서 데미지를 입을때
		ConsumeHPForAction(DamageAmount);
		UE_LOG(LogTemp, Log, TEXT("Monster Damaged, CurrentHP: %f"), MonsterStatus.CurrentMonsterHP);
	}
	else {
		//체력이 없어서 죽을때
		UE_LOG(LogTemp, Log, TEXT("Monster Die"));
		ConsumeHPForAction(DamageAmount); //체력을 0으로 하기
		bIsMonsterDead = true;
		//아이템 드랍
		DroppedItem();
		//몬스터 사라지기
		DieMonster();
	}
	
	return DamageAmount;
}

void AMonster::DroppedItem()
{
	if (ItemDropTable && MonsterStatus.DropRates.Num() > 0) {
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

void AMonster::DieMonster()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
		if (auto* MyGameModeBase = GetWorld()->GetAuthGameMode<AMyGameModeBase>()) {
			MyGameModeBase->MonsterDeadCount += 1;
		}
		if (MonsterWidgetComponent) {
			MonsterWidgetComponent->DestroyComponent();
		}
		if (auto* AIControllerInstance = GetController()) {
			if (auto* AIController = Cast<AAIController>(AIControllerInstance)) {
				AIController->StopMovement();
				AIController->UnPossess();
			}
		}
		if (MonsterDieMontage) {
			AnimInstance->Montage_Play(MonsterDieMontage);
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &AMonster::OnDieMontageEnded);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, MonsterDieMontage);
		}
	}
}

void AMonster::OnDieMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	Destroy();
}

void AMonster::ApplyDamageToActor(AActor* ActorToDamage, UPrimitiveComponent* OtherComponent)
{
	if (ActorToDamage) {
		if (auto* PlayerCharacter = Cast<AMyCharacter>(ActorToDamage)) {
			Damage = MonsterStatus.Damage;
			if (PlayerCharacter->bIsGuard) {
				if (OtherComponent == PlayerCharacter->GetGuardComponent()) {
					Damage = 0.f;
				}
			}
			FDamageEvent DamageEvent;
			ActorToDamage->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
		}
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

TArray<AActor*>& AMonster::GetOverlapActors()
{
	return OverlapActors;
}

UWidgetComponent* AMonster::GetMonsterWidgetComponent() const
{
	if (MonsterWidgetComponent)
		return MonsterWidgetComponent;
	return nullptr;
}

TSubclassOf<class UMonsterWidget> AMonster::GetMonsterWidgetClass() const
{
	if (MonsterWidgetClass)
		return MonsterWidgetClass;
	return TSubclassOf<class UMonsterWidget>();
}

UCapsuleComponent* AMonster::GetAttackCollisionComponent(FName AttackCollisionFName) const
{
	if (MonsterAttackCollisionComponents.Num() > 0) {
		for (auto& CheckAttackCollision : MonsterAttackCollisionComponents) {
			if (CheckAttackCollision->GetFName() == AttackCollisionFName) {
				return CheckAttackCollision;
			}
		}
	}

	return nullptr;
}

void AMonster::SetMonsterAttackCollision(UCapsuleComponent* AttackCollision)
{
	MonsterAttackCollisionComponents.AddUnique(AttackCollision);
}
