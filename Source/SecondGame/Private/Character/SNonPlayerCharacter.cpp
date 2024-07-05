// SNonPlayerCharacter.cpp

#include "Character/SNonPlayerCharacter.h"
#include "Controller/SAIController.h"
#include "Item/SWeaponActor.h"

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
}
