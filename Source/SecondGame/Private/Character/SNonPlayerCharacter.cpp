// SNonPlayerCharacter.cpp

#include "Character/SNonPlayerCharacter.h"
#include "Controller/SAIController.h"
#include "Item/SWeaponActor.h"
#include "GameFramework/CharacterMovementComponent.h"

ASNonPlayerCharacter::ASNonPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// NPC�� ������ ������ SAIController ����
	AIControllerClass = ASAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ASNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ���� ���� �� 'WeaponSocket'�� ���� ���� ����
	FName WeaponSocket(TEXT("WeaponSocket01"));
	if (GetMesh()->DoesSocketExist(WeaponSocket) == true && IsValid(WeaponInstance) == false)
	{
		// ���� ���� ����
		WeaponInstance = GetWorld()->SpawnActor<ASWeaponActor>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if (IsValid(WeaponInstance) == true)
		{
			WeaponInstance->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		}
	}

	// IsPlayerControllered(): �÷��̾ �����ϰ� �ִ� ������ Ȯ��
	if (false == IsPlayerControlled())
	{
		// bUseControllerRotation***: ControlRotation�� ���� ĳ���Ͱ� ȸ������ �ʴ´� [false]
		bUseControllerRotationYaw = false;

		// bOrientRotationToMovement: ĳ���� �̵� ���⿡ ȸ���� ��ġ��Ű�� �ʴ´� [false]
		GetCharacterMovement()->bOrientRotationToMovement = false;
		// bUseControllerDesiredRotation: ControlRotation�� ��ǥ ȸ�������� ��Ƽ� ȸ���Ѵ� [true]
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		// RotationRate: ȸ�� �ӵ�
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

		// NPC �̵��ӵ� ����
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
}
