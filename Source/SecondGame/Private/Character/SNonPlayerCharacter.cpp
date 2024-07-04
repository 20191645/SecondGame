// SNonPlayerCharacter.cpp

#include "Character/SNonPlayerCharacter.h"
#include "Controller/SAIController.h"
#include "Item/SWeaponActor.h"
#include "GameFramework/CharacterMovementComponent.h"

ASNonPlayerCharacter::ASNonPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// NPC가 생성될 때마다 SAIController 빙의
	AIControllerClass = ASAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ASNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 'WeaponSocket'에 무기 액터 장착
	FName WeaponSocket(TEXT("WeaponSocket01"));
	if (GetMesh()->DoesSocketExist(WeaponSocket) == true && IsValid(WeaponInstance) == false)
	{
		// 무기 액터 스폰
		WeaponInstance = GetWorld()->SpawnActor<ASWeaponActor>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if (IsValid(WeaponInstance) == true)
		{
			WeaponInstance->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		}
	}

	// IsPlayerControllered(): 플레이어가 조종하고 있는 폰인지 확인
	if (false == IsPlayerControlled())
	{
		// bUseControllerRotation***: ControlRotation에 따라 캐릭터가 회전하지 않는다 [false]
		bUseControllerRotationYaw = false;

		// bOrientRotationToMovement: 캐릭터 이동 방향에 회전을 일치시키지 않는다 [false]
		GetCharacterMovement()->bOrientRotationToMovement = false;
		// bUseControllerDesiredRotation: ControlRotation을 목표 회전값으로 삼아서 회전한다 [true]
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		// RotationRate: 회전 속도
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

		// NPC 이동속도 너프
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
}
